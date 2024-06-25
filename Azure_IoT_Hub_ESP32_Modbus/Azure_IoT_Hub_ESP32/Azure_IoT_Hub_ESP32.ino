// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

/*
 * This is an Arduino-based Azure IoT Hub sample for ESPRESSIF ESP32 boards.
 * It uses our Azure Embedded SDK for C to help interact with Azure IoT.
 * For reference, please visit https://github.com/azure/azure-sdk-for-c.
 *
 * To connect and work with Azure IoT Hub you need an MQTT client, connecting, subscribing
 * and publishing to specific topics to use the messaging features of the hub.
 * Our azure-sdk-for-c is an MQTT client support library, helping composing and parsing the
 * MQTT topic names and messages exchanged with the Azure IoT Hub.
 *
 * This sample performs the following tasks:
 * - Synchronize the device clock with a NTP server;
 * - Initialize our "az_iot_hub_client" (struct for data, part of our azure-sdk-for-c);
 * - Initialize the MQTT client (here we use ESPRESSIF's esp_mqtt_client, which also handle the tcp
 * connection and TLS);
 * - Connect the MQTT client (using server-certificate validation, SAS-tokens for client
 * authentication);
 * - Periodically send telemetry data to the Azure IoT Hub.
 *
 * To properly connect to your Azure IoT Hub, please fill the information in the `iot_configs.h`
 * file.
 */

// C99 libraries
#include <cstdlib>
#include <string.h>
#include <time.h>

// Libraries for MQTT client and WiFi connection
#include <WiFi.h>
#include <mqtt_client.h>

// Azure IoT SDK for C includes
#include <az_core.h>
#include <az_iot.h>
#include <azure_ca.h>

// Additional sample headers
#include "AzIoTSasToken.h"
#include "SerialLogger.h"
#include "iot_configs.h"
#include <ArduinoJson.h>  

//Libraries  of Modbus RTU 
#include "UbidotsEsp32Mqtt.h"
#include <ModbusMaster.h>
#include <SoftwareSerial.h>



/*||||||||||||||>Define the Modbus communication parameters|||||||||||||||*/
#define BAUD_RATE 115200
#define PARITY 0
#define DATA_BITS 8
#define STOP_BITS 1
#define SerialRS485_RX_PIN 16
#define SerialRS485_TX_PIN 17
/*||||||||||||||Define the Modbus communication parameters<|||||||||||||||*/




/*|||||||||||||>Define the slave addresses for the two devices|||||||||||*/
#define PLC_SLAVE1_Station 1
#define PLC_SLAVE2_Station 2
#define PLC_SLAVE3_Station 3

SoftwareSerial Serial_mod(SerialRS485_RX_PIN, SerialRS485_TX_PIN);    
/*|||||||||||||Define the slave addresses for the two devices<|||||||||||*/



// When developing for your own Arduino-based platform,
// please follow the format '(ard;<platform>)'.
#define AZURE_SDK_CLIENT_USER_AGENT "c%2F" AZ_SDK_VERSION_STRING "(ard;esp32)"

// Utility macros and defines
#define sizeofarray(a) (sizeof(a) / sizeof(a[0]))
#define NTP_SERVERS "pool.ntp.org", "time.nist.gov"
#define MQTT_QOS1 1
#define DO_NOT_RETAIN_MSG 0
#define SAS_TOKEN_DURATION_IN_MINUTES 60
#define UNIX_TIME_NOV_13_2017 1510592825

#define PST_TIME_ZONE -8
#define PST_TIME_ZONE_DAYLIGHT_SAVINGS_DIFF 1

#define GMT_OFFSET_SECS (PST_TIME_ZONE * 3600)
#define GMT_OFFSET_SECS_DST ((PST_TIME_ZONE + PST_TIME_ZONE_DAYLIGHT_SAVINGS_DIFF) * 3600)

// Translate iot_configs.h defines into variables used by the sample
static const char* ssid = IOT_CONFIG_WIFI_SSID;
static const char* password = IOT_CONFIG_WIFI_PASSWORD;
static const char* host = IOT_CONFIG_IOTHUB_FQDN;
static const char* mqtt_broker_uri = "mqtts://" IOT_CONFIG_IOTHUB_FQDN;
static const char* device_id = IOT_CONFIG_DEVICE_ID;
static const int mqtt_port = AZ_IOT_DEFAULT_MQTT_CONNECT_PORT;

// Memory allocated for the sample's variables and structures.
static esp_mqtt_client_handle_t mqtt_client;
static az_iot_hub_client client;

static char mqtt_client_id[128];
static char mqtt_username[128];
static char mqtt_password[200];
static uint8_t sas_signature_buffer[256];
static unsigned long next_telemetry_send_time_ms = 0;
static char telemetry_topic[128];
static uint32_t telemetry_send_count = 0;
static String telemetry_payload = "{}";

/*||||||||||||||||||||||||||>PLCS_VARIABLE||||||||||||||||||||||||||||||||*/
uint16_t TT1_PLC1 = 30;
uint16_t ST1_PLC1 = 0;
uint16_t LC1_Virtual=0;
uint16_t FT1_PLC2 = 0;
uint16_t CE1_PLC2 = 0;
uint16_t WT1_PLC2 = 0;
uint16_t PA1_PLC2 = 0;
uint16_t PA2_PLC2 = 0;
uint16_t Fan=0;
uint16_t Stirrer=0;
uint16_t FC1=0;
uint16_t LC1=0;
uint16_t J1=0;   
uint16_t TT2_PLC3 = 0;
uint16_t LT1_PLC3 = 0;
uint16_t Pump1=0;
uint16_t Pump2=0;
/*||||||||||||||||||||||||||PLCS_VARIABLE<||||||||||||||||||||||||||||||||*/

/*|||||||||||||>Create an instances of the ModbusMaster class|||||||||||||||*/
ModbusMaster Slave1;
ModbusMaster Slave2;
ModbusMaster Slave3;
/*|||||||||||||Create an instances of the ModbusMaster class<|||||||||||||||*/


#define INCOMING_DATA_BUFFER_SIZE 128
static char incoming_data[INCOMING_DATA_BUFFER_SIZE];

// Auxiliary functions
#ifndef IOT_CONFIG_USE_X509_CERT
static AzIoTSasToken sasToken(
    &client,
    AZ_SPAN_FROM_STR(IOT_CONFIG_DEVICE_KEY),
    AZ_SPAN_FROM_BUFFER(sas_signature_buffer),
    AZ_SPAN_FROM_BUFFER(mqtt_password));
#endif // IOT_CONFIG_USE_X509_CERT

static void connectToWiFi()
{
  Logger.Info("Connecting to WIFI SSID " + String(ssid));

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");

  Logger.Info("WiFi connected, IP address: " + WiFi.localIP().toString());
}

static void initializeTime()
{
  Logger.Info("Setting time using SNTP");

  configTime(GMT_OFFSET_SECS, GMT_OFFSET_SECS_DST, NTP_SERVERS);
  time_t now = time(NULL);
  while (now < UNIX_TIME_NOV_13_2017)
  {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  Logger.Info("Time initialized!");
}

void receivedCallback(char* topic, byte* payload, unsigned int length)
{
  Logger.Info("Received [");
  Logger.Info(topic);
  Logger.Info("]: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println("");
}

static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
  switch (event->event_id)
  {
    int i, r;

    case MQTT_EVENT_ERROR:
      Logger.Info("MQTT event MQTT_EVENT_ERROR");
      break;
    case MQTT_EVENT_CONNECTED:
      Logger.Info("MQTT event MQTT_EVENT_CONNECTED");

      r = esp_mqtt_client_subscribe(mqtt_client, AZ_IOT_HUB_CLIENT_C2D_SUBSCRIBE_TOPIC, 1);
      if (r == -1)
      {
        Logger.Error("Could not subscribe for cloud-to-device messages.");
      }
      else
      {
        Logger.Info("Subscribed for cloud-to-device messages; message id:" + String(r));
      }

      break;
    case MQTT_EVENT_DISCONNECTED:
      Logger.Info("MQTT event MQTT_EVENT_DISCONNECTED");
      break;
    case MQTT_EVENT_SUBSCRIBED:
      Logger.Info("MQTT event MQTT_EVENT_SUBSCRIBED");
      break;
    case MQTT_EVENT_UNSUBSCRIBED:
      Logger.Info("MQTT event MQTT_EVENT_UNSUBSCRIBED");
      break;
    case MQTT_EVENT_PUBLISHED:
      Logger.Info("MQTT event MQTT_EVENT_PUBLISHED");
      break;
    case MQTT_EVENT_DATA:
      Logger.Info("MQTT event MQTT_EVENT_DATA");

      for (i = 0; i < (INCOMING_DATA_BUFFER_SIZE - 1) && i < event->topic_len; i++)
      {
        incoming_data[i] = event->topic[i];
      }
      incoming_data[i] = '\0';
      Logger.Info("Topic: " + String(incoming_data));

      for (i = 0; i < (INCOMING_DATA_BUFFER_SIZE - 1) && i < event->data_len; i++)
      {
        incoming_data[i] = event->data[i];
      }
      incoming_data[i] = '\0';
      Logger.Info("Data: " + String(incoming_data));

      break;
    case MQTT_EVENT_BEFORE_CONNECT:
      Logger.Info("MQTT event MQTT_EVENT_BEFORE_CONNECT");
      break;
    default:
      Logger.Error("MQTT event UNKNOWN");
      break;
  }

  return ESP_OK;
}

static void initializeIoTHubClient()
{
  az_iot_hub_client_options options = az_iot_hub_client_options_default();
  options.user_agent = AZ_SPAN_FROM_STR(AZURE_SDK_CLIENT_USER_AGENT);

  if (az_result_failed(az_iot_hub_client_init(
          &client,
          az_span_create((uint8_t*)host, strlen(host)),
          az_span_create((uint8_t*)device_id, strlen(device_id)),
          &options)))
  {
    Logger.Error("Failed initializing Azure IoT Hub client");
    return;
  }

  size_t client_id_length;
  if (az_result_failed(az_iot_hub_client_get_client_id(
          &client, mqtt_client_id, sizeof(mqtt_client_id) - 1, &client_id_length)))
  {
    Logger.Error("Failed getting client id");
    return;
  }

  if (az_result_failed(az_iot_hub_client_get_user_name(
          &client, mqtt_username, sizeofarray(mqtt_username), NULL)))
  {
    Logger.Error("Failed to get MQTT clientId, return code");
    return;
  }

  Logger.Info("Client ID: " + String(mqtt_client_id));
  Logger.Info("Username: " + String(mqtt_username));
}

static int initializeMqttClient()
{
#ifndef IOT_CONFIG_USE_X509_CERT
  if (sasToken.Generate(SAS_TOKEN_DURATION_IN_MINUTES) != 0)
  {
    Logger.Error("Failed generating SAS token");
    return 1;
  }
#endif

  esp_mqtt_client_config_t mqtt_config;
  memset(&mqtt_config, 0, sizeof(mqtt_config));
  mqtt_config.uri = mqtt_broker_uri;
  mqtt_config.port = mqtt_port;
  mqtt_config.client_id = mqtt_client_id;
  mqtt_config.username = mqtt_username;

#ifdef IOT_CONFIG_USE_X509_CERT
  Logger.Info("MQTT client using X509 Certificate authentication");
  mqtt_config.client_cert_pem = IOT_CONFIG_DEVICE_CERT;
  mqtt_config.client_key_pem = IOT_CONFIG_DEVICE_CERT_PRIVATE_KEY;
#else // Using SAS key
  mqtt_config.password = (const char*)az_span_ptr(sasToken.Get());
#endif

  mqtt_config.keepalive = 30;
  mqtt_config.disable_clean_session = 0;
  mqtt_config.disable_auto_reconnect = false;
  mqtt_config.event_handle = mqtt_event_handler;
  mqtt_config.user_context = NULL;
  mqtt_config.cert_pem = (const char*)ca_pem;

  mqtt_client = esp_mqtt_client_init(&mqtt_config);

  if (mqtt_client == NULL)
  {
    Logger.Error("Failed creating mqtt client");
    return 1;
  }

  esp_err_t start_result = esp_mqtt_client_start(mqtt_client);

  if (start_result != ESP_OK)
  {
    Logger.Error("Could not start mqtt client; error code:" + start_result);
    return 1;
  }
  else
  {
    Logger.Info("MQTT client started");
    return 0;
  }
}

/*
 * @brief           Gets the number of seconds since UNIX epoch until now.
 * @return uint32_t Number of seconds.
 */
static uint32_t getEpochTimeInSecs() { return (uint32_t)time(NULL); }

static void establishConnection()
{
  connectToWiFi();
  initializeTime();
  initializeIoTHubClient();
  (void)initializeMqttClient();
}

static void generateTelemetryPayload()
{
    StaticJsonDocument<256> jsonDocument; // Adjust the size as needed

    // Clear previous data from JSON document
    jsonDocument.clear();

    // Add telemetry data to JSON document
    jsonDocument["TT1_PLC1"] = TT1_PLC1;
    jsonDocument["ST1_PLC1"] = ST1_PLC1;
    jsonDocument["FT1_PLC2"] = FT1_PLC2;
    jsonDocument["WT1_PLC2"] = WT1_PLC2;
    jsonDocument["TT2_PLC3"] = TT2_PLC3;
    jsonDocument["Fan"] = Fan;
    jsonDocument["Stirrer"] = Stirrer;
    jsonDocument["FC1"] = FC1;
    jsonDocument["LC1"] = LC1;
    jsonDocument["Pump1"] = Pump1;
    jsonDocument["Pump2"] = Pump2;
    jsonDocument["J1"] = J1;


    // Serialize the JSON document to a string
    String jsonString;
    serializeJson(jsonDocument, jsonString);

    // Convert String to char* for MQTT publish
    telemetry_payload = jsonString;


}


static void sendTelemetry()
{
  Logger.Info("Sending telemetry ...");

  // The topic could be obtained just once during setup,
  // however if properties are used the topic need to be generated again to reflect the
  // current values of the properties.
  if (az_result_failed(az_iot_hub_client_telemetry_get_publish_topic(
          &client, NULL, telemetry_topic, sizeof(telemetry_topic), NULL)))
  {
    Logger.Error("Failed az_iot_hub_client_telemetry_get_publish_topic");
    return;
  }

  generateTelemetryPayload();

  if (esp_mqtt_client_publish(
          mqtt_client,
          telemetry_topic,
          (const char*)telemetry_payload.c_str(),
          telemetry_payload.length(),
          MQTT_QOS1,
          DO_NOT_RETAIN_MSG)
      == 0)
  {
    Logger.Error("Failed publishing");
  }
  else
  {
    Logger.Info("Message published successfully");
  }
}

// Arduino setup and loop main functions.

void setup() {
  
   /*|||||||||||||||||Initialize Speed Rate Of Communication|||||||||||||||||*/
  Serial.begin(115200);
  Serial_mod.begin(BAUD_RATE);

  /*|||||||||||||||||||||||||||>Slaves BEGINS|||||||||||||||||||||||||||||||*/
  // Modbus slave1 device
  Slave1.begin(PLC_SLAVE1_Station, Serial_mod);
  //  Modbus slave2 device
  Slave2.begin(PLC_SLAVE2_Station, Serial_mod);
  //Modbus slave3 device
  Slave3.begin(PLC_SLAVE3_Station, Serial_mod);
  /*|||||||||||||||||||||||||||Slaves BEGINS<||||||||||||||||||||||||||||||||*/
   
   
  
  /////////////////AZURE IOT HUB ESP32
  establishConnection();
   

}


void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    connectToWiFi();
  }
#ifndef IOT_CONFIG_USE_X509_CERT
  else if (sasToken.IsExpired())
  {
    Logger.Info("SAS token expired; reconnecting with a new one.");
    (void)esp_mqtt_client_destroy(mqtt_client);
    initializeMqttClient();
  }
#endif
  else if (millis() > next_telemetry_send_time_ms)
  {
    sendTelemetry();
    next_telemetry_send_time_ms = millis() + TELEMETRY_FREQUENCY_MILLISECS;
  }





  /*|||||||||||||||||||||Read Data from PlCS to PLCS||||||||||||||||||||||||||*/
  /*|||||||||||| Read analog sensors signals  from PLC1|||||||||||||*/
    //Read TT1 and St1  from register D0003 & D0004

  uint8_t result_0 = Slave1.readInputRegisters(3, 3);
  if (result_0 == Slave1.ku8MBSuccess) {
    TT1_PLC1 = Slave1.getResponseBuffer(0);
    ST1_PLC1 = Slave1.getResponseBuffer(1);
    LC1_Virtual= Slave1.getResponseBuffer(2);
  }
  
  delay(25);
  /*||||||||||||||||||||| Read data from PLC2||||||||||||||||||||||||||*/
  /*|||||||||||| Read analog sensors signals from plc2||||||||||||*/ 
    //Read Wt1 from register D00014  

  uint8_t result_1 = Slave2.readInputRegisters(14, 1);
  if (result_1 == Slave2.ku8MBSuccess) {
    WT1_PLC2 = Slave2.getResponseBuffer(0);
  }
 
  // Read FT1 from register D0008 
  uint8_t result_2 = Slave2.readInputRegisters(8, 1);
  if (result_2 == Slave2.ku8MBSuccess) {
    FT1_PLC2 = Slave2.getResponseBuffer(0);
  }

  // Read CE1 from register D0002
  uint8_t result_3 = Slave2.readInputRegisters(2, 1);
  if (result_3 == Slave2.ku8MBSuccess) {
    CE1_PLC2 = Slave2.getResponseBuffer(0);
  }

  /*|||||||||||| Read analog actuators signals from plc2 ||||||||||||*/  
  // Read  Fan=0; Stirrer=0; FC1=0; LC1=0  from  (9-12)
  uint8_t result_4 = Slave2.readInputRegisters(9, 4);
  if (result_4 == Slave2.ku8MBSuccess) {
    Fan = Slave2.getResponseBuffer(0);
    Stirrer = Slave2.getResponseBuffer(1);
    FC1 = Slave2.getResponseBuffer(2);
    LC1 = Slave2.getResponseBuffer(3);
   }
  delay(25);


 uint8_t result_71 = Slave2.readInputRegisters(30, 1);
  if (result_71 == Slave2.ku8MBSuccess) {
    Pump1 = Slave2.getResponseBuffer(0);
   }
  delay(5);
   uint8_t result_72 = Slave2.readInputRegisters(35, 1);
  if (result_72 == Slave2.ku8MBSuccess) {
    Pump2 = Slave2.getResponseBuffer(0);
   }
  delay(5);
   uint8_t result_73 = Slave2.readInputRegisters(40, 1);
  if (result_73 == Slave2.ku8MBSuccess) {
    J1 = Slave2.getResponseBuffer(0);
   }

/*
  Serial.println(" monitoring for plc2 data   ^^^^^______^^^^^ " );
  Serial.print(" J1: " );Serial.print( J1 );Serial.print(" |||| "); 
  Serial.print(" Pump1: " );Serial.print( Pump1 );Serial.print(" |||| "); 
  Serial.print(" Pump2: " );Serial.print( Pump2 );Serial.print(".................."); */


  delay(25);

  /*||||||||||||||||||||| Read data from PLC3|||||||||||||||||||||||||*/
  /*|||||||||||| Read analog sensors signals from plc3 ||||||||||||*/ 
  // Read TT2 from register D0001

  uint8_t result_5 = Slave3.readInputRegisters(1, 1);
  if (result_5 == Slave3.ku8MBSuccess) {
    TT2_PLC3  = Slave3.getResponseBuffer(0);
  }

  // Read LT1 from register D0009
  uint8_t result_6 = Slave3.readInputRegisters(9, 1);
  if (result_6 == Slave3.ku8MBSuccess) {
    LT1_PLC3  = Slave3.getResponseBuffer(0);
  }
  
  delay(25);

  /*||||||||||||||||||||||Write Data from PlCS to PLCS||||||||||||||||||||||||*/
  /*|||||||||||||||Write analog actuator to PLC 1|||||||||||||||||*/
  /*_______________________________________________*/
  // Wrtie  [Fan]  to [PLC1 in M0000]:
  //Wrtie  [ Stirrer]  to [PLC1 in M0001]:
  
  uint8_t result_7 = Slave1.writeSingleRegister(1	, Stirrer);
  delay(25);
  uint8_t result_8 = Slave1.writeSingleRegister(0	, Fan);
  delay(25);


    




  /*||||||||||||||||||||||Write analog sensors data to PLC 2||||||||||||||||||||||*/
  /*_______________________________________________*/
    //Wrtie  [TT1_PLC1]  to [PLC2 in M0000]:
    //Wrtie  [ ST1_PLC1]  to [PLC2 in M0001]:
    //Wrtie  [TT2_PLC3]  to [PLC2 in M0002]:
    //Wrtie  [ LT1_PLC3]  to [PLC2 in M0003]:

  uint8_t result_9 = Slave2.writeSingleRegister(0	,TT1_PLC1 );
  delay(25);
  uint8_t result_10 = Slave2.writeSingleRegister(1	, ST1_PLC1 );
  delay(25);
  uint8_t result_11 = Slave2.writeSingleRegister(2	, TT2_PLC3 );
  delay(25);
  uint8_t result_12 = Slave2.writeSingleRegister(3	, LT1_PLC3 );
  delay(25);

  
  
  /*||||||||||||||||||||||Write  analog actuator  to PLC 3||||||||||||||||||||||||*/
  /*_______________________________________________*/
    //Wrtie [FC1]  to [PLC3 in M0000]:
    //Wrtie  [LC1]  to [PLC3 in M0001]:

  uint8_t result_21 = Slave3.writeSingleRegister(1	, LC1 );
  delay(25);
  uint8_t result_22 = Slave3.writeSingleRegister(0	, FC1 );
  delay(25);


  
  delay(100);



}
