/******************************************
 *
 * This example works for both Industrial and STEM users.
 *
 * Developed by Jose Garcia, https://github.com/jotathebest/
 *
 * ****************************************/

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsEsp32Mqtt.h"

/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBUS-RGgo1Uiq8TAkwbQMhpxYFgC5SpRQfU";            // Put here your Ubidots TOKEN
const char *WIFI_SSID = "ASUENG wi-fi";                // Put here your Wi-Fi SSID
const char *WIFI_PASS = "ASUeng_Demo11";                // Put here your Wi-Fi password


const char *PUBLISH_DEVICE_LABEL = "sensors";    // Put here your Device label to which data  will be published
const char *SUBSCRIBE_DEVICE_LABEL = "actuators";   // Replace with the device label to subscribe to
 
float pump1_web=0;
float pump2_web=0;
float FC_web=0;
float LC_web=0;
float Fan_web=0;
float Heater_web=0;
float Stirrer_web=0;



Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

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



}




/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  //ubidots.setDebug(true);  // uncomment this to make debug messages available
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


}





void loop()
{
  
  // put your main code here, to run repeatedly:
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
   
  }




    float value_1 = 500;
    ubidots.add("temp_sensor-1-tt-1", value_1); // Insert your variable Labels and the value to be sent
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("temp_sensor-2-tt-2", value_1*8); // Insert your variable Labels and the value to be sent
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("conductivity_ind-ce_1", value_1+5); // Insert your variable Labels and the value to be sent
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("diff_pressure-lt-1", value_1*5); // Insert your variable Labels and the value to be sent
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("flow_sensor-ft-1", value_1*7); // Insert your variable Labels and the value to be sent
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("load_cell-wt-1", value_1/7); // Insert your variable Labels and the value to be sent
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("pressure_sensor-pa_1", value_1*78); // Insert your variable Labels and the value to be sent
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("pressure_sensor-pa_2", value_1*22); // Insert your variable Labels and the value to be sent
    ubidots.publish(PUBLISH_DEVICE_LABEL);
    ubidots.add("taco_meter-st-1", value_1*20); // Insert your variable Labels and the value to be sent
    ubidots.publish(PUBLISH_DEVICE_LABEL);
  



  ubidots.loop();

  
  delay(100);


}
