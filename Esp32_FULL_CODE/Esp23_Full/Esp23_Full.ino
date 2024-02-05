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







/*||||||||||||||||||||||||||>PLCS_VARIABLE||||||||||||||||||||||||||||||||*/
uint16_t TT1_PLC1 = 0;
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
/*||||||||||||||||||||||||||PLCS_VARIABLE<||||||||||||||||||||||||||||||||*/

/*||||||||||||||||||||||||||>WEBS_VARIABLE||||||||||||||||||||||||||||||||*/
const char *UBIDOTS_TOKEN = "BBUS-olb3W0DF86neBdwjUwMH0zhtJzjpUK";            // Put here your Ubidots TOKEN
const char *WIFI_SSID = "ASUENG wi-fi";                // Put here your Wi-Fi SSID
const char *WIFI_PASS = "ASUeng_Demo11";                // Put here your Wi-Fi password
const char *PUBLISH_DEVICE_LABEL = "sensors";    // Put here your Device label to which data  will be published
const char *SUBSCRIBE_DEVICE_LABEL = "actuators";   // Replace with the device label to subscribe to
float Switch_mode=0;
float pump1_web=0;
float pump2_web=0;
float FC_web=0;     // Valve 1  to small tank 
float LC_web=0;     //Valve 2  output valve
float Fan_web=0;
float Heater_web=0;
float Stirrer_web=0;
/*||||||||||||||||||||||||||WEBS_VARIABLE<||||||||||||||||||||||||||||||||*/

/*|||||||||||||>Create an instances of the ModbusMaster class|||||||||||||||*/
ModbusMaster Slave1;
ModbusMaster Slave2;
ModbusMaster Slave3;
/*|||||||||||||Create an instances of the ModbusMaster class<|||||||||||||||*/


/*|||||||||||||>Create an instances of the Ubidots class||||||||||||||||||||*/

Ubidots ubidots(UBIDOTS_TOKEN);

/*|||||||||||||Create an instances of the Ubidots class<||||||||||||||||||||*/


/*||||||||||||||||||||||||>CALLBACK_FUNCTION|||||||||||||||||||||||||||||||*/
/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/


void callback(char *topic, byte *payload, unsigned int length) {
  // Convert the payload to a float value
  payload[length] = '\0';
  String str_payload = String((char *)payload);
  float value = str_payload.toFloat();

  if(strcmp(SUBSCRIBE_DEVICE_LABEL, "actuators") == 0 &&
      strcmp(topic, "/v2.0/devices/actuators/pump_1-g1/lv") == 0) {
      pump1_web = value;
    Serial.println(pump1_web);
  } 
  
 if(strcmp(SUBSCRIBE_DEVICE_LABEL, "actuators") == 0 &&
      strcmp(topic, "/v2.0/devices/actuators/pump_2-g2/lv") == 0) {
      pump2_web = value;
    Serial.println(pump2_web);
  } 

 if (strcmp(SUBSCRIBE_DEVICE_LABEL, "actuators") == 0 &&
      strcmp(topic, "/v2.0/devices/actuators/control_valve-d-fc-1/lv") == 0) {
    FC_web = value;
    Serial.println(FC_web);
  }

 if(strcmp(SUBSCRIBE_DEVICE_LABEL, "actuators") == 0 &&
      strcmp(topic, "/v2.0/devices/actuators/control_valve-u-lc-1/lv") == 0) {
      LC_web = value;
    Serial.println(LC_web);
  } 

 if (strcmp(SUBSCRIBE_DEVICE_LABEL, "actuators") == 0 &&
      strcmp(topic, "/v2.0/devices/actuators/fan-motor-m1/lv") == 0) {
    Fan_web = value;
    Serial.println(Fan_web );
  }

 if(strcmp(SUBSCRIBE_DEVICE_LABEL, "actuators") == 0 &&
      strcmp(topic, "/v2.0/devices/actuators/heater-j_1/lv") == 0) {
      Heater_web = value;
    Serial.println(Heater_web);
  } 

 if (strcmp(SUBSCRIBE_DEVICE_LABEL, "actuators") == 0 &&
      strcmp(topic, "/v2.0/devices/actuators/stirrer-motor-m2/lv") == 0) {
    Stirrer_web = value;
    Serial.println(Stirrer_web);
  }

 if (strcmp(SUBSCRIBE_DEVICE_LABEL, "actuators") == 0 &&
      strcmp(topic, "/v2.0/devices/actuators/switch_mode/lv") == 0) {
    Switch_mode = value;
    Serial.println(Switch_mode);
  }


}


/*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
/*||||||||||||||||||||||||CALLBACK_FUNCTION<|||||||||||||||||||||||||||||||*/











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

  /*|||||||||||||||||||||||>UBIDOTS Initializations||||||||||||||||||||||||||*/
  /*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
  
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"pump_1-g1" ); // Insert the device and variable's Labels, respectively
  ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"pump_2-g2" ); // Insert the device and variable's Labels, respectively
  ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"control_valve-d-fc-1" ); // Insert the device and variable's Labels, respectively
  ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"control_valve-u-lc-1" ); // Insert the device and variable's Labels, respectively
  ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"fan-motor-m1" ); // Insert the device and variable's Labels, respectively
  ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"heater-j_1" ); // Insert the device and variable's Labels, respectively
  ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"stirrer-motor-m2" ); // Insert the device and variable's Labels, respectively
  ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"switch_mode" ); // Insert the device and variable's Labels, respectively
  /*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
  /*|||||||||||||||||||||||>UBIDOTS Initializations||||||||||||||||||||||||||*/
}


void loop() 
{
  /*||||||||||||||||||||||||>UBIDOTS Reconnecting||||||||||||||||||||||||||||*/
  /*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

  if (!ubidots.connected())
  {
    ubidots.reconnect();
    ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"pump_1-g1" ); // Insert the device and variable's Labels, respectively
    ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"pump_2-g2" ); // Insert the device and variable's Labels, respectively
    ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"control_valve-d-fc-1" ); // Insert the device and variable's Labels, respectively
    ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"control_valve-u-lc-1" ); // Insert the device and variable's Labels, respectively
    ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"fan-motor-m1" ); // Insert the device and variable's Labels, respectively
    ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"heater-j_1" ); // Insert the device and variable's Labels, respectively
    ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"stirrer-motor-m2" ); // Insert the device and variable's Labels, respectively
    ubidots.subscribeLastValue(SUBSCRIBE_DEVICE_LABEL,"switch_mode" ); // Insert the device and variable's Labels, respectively

  }

  /*||||||||||||||||||||||||>UBIDOTS Reconnecting||||||||||||||||||||||||||||*/
  /*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

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

  
  /*______________________Write Web Action To PLC2_________________________*/
    /*
    Switch_mode=0;
    pump1_web=0;
    pump2_web=0;
    FC_web=0;     // Valve 1  to small tank 
    LC_web=0;     //Valve 2  output valve
    Fan_web=0;
    Heater_web=0;
    Stirrer_web=0;
    */

   uint8_t result_13 = Slave2.writeSingleRegister(5	,Switch_mode );
   delay(25);
   uint8_t result_14 = Slave2.writeSingleRegister(6	,pump1_web );
   delay(25);
   uint8_t result_15 = Slave2.writeSingleRegister(7	,pump2_web );
   delay(25);
   uint8_t result_16 = Slave2.writeSingleRegister(8	,Heater_web);
   delay(25);
   uint8_t result_17= Slave2.writeSingleRegister(9	,Fan_web);
   delay(25);
   uint8_t result_18 = Slave2.writeSingleRegister(10	, Stirrer_web);
   delay(25);
   uint8_t result_19 = Slave2.writeSingleRegister(11	,FC_web);
   delay(25);
   uint8_t result_20= Slave2.writeSingleRegister(12	,LC_web);
   delay(25);
   


  /*||||||||||||||||||||||Write  analog actuator  to PLC 3||||||||||||||||||||||||*/
  /*_______________________________________________*/
    //Wrtie [FC1]  to [PLC3 in M0000]:
    //Wrtie  [LC1]  to [PLC3 in M0001]:

  uint8_t result_21 = Slave3.writeSingleRegister(1	, LC1 );
  delay(25);
  uint8_t result_22 = Slave3.writeSingleRegister(0	, FC1 );
  delay(25);


  /*|||||||||||||||||||||||||>Publishing For Cloud||||||||||||||||||||||||||||||||||||*/
  /*|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/

    //float value_1 = 205.1458;
    ubidots.add("temp_sensor-1-tt-1",TT1_PLC1); 
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("temp_sensor-2-tt-2", TT2_PLC3); 
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("diff_pressure-lt-1", LT1_PLC3); 
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("flow_sensor-ft-1", FT1_PLC2); 
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("load_cell-wt-1", WT1_PLC2); 
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    //ubidots.add("pressure_sensor-pa_1",PA1_PLC2); 
    //ubidots.publish(PUBLISH_DEVICE_LABEL);
    //ubidots.add("pressure_sensor-pa_2", PA2_PLC2); 
    //ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("taco_meter-st-1", ST1_PLC1); 
    ubidots.publish(PUBLISH_DEVICE_LABEL);
  
    ubidots.loop();


  /*||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||*/
  /*|||||||||||||||||||||||||Publishing For Cloud<||||||||||||||||||||||||||||||||||||*/
  
    delay(100);

}