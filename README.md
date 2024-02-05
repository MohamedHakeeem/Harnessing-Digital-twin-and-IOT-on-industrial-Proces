# Harnessing-Digital-twin-and-IOT-on-industrial-Proces
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/645fe325-17a3-4862-9a4b-3897e83b6e4c)

My Project is called “Harnessing Digital Twin & IIOT” and aims to be a system that achieves many topics that are the Future of the time such as the IIOT that makes automation part of the internet for communicating it with other systems easy and efficient, the Digital Twin is considered as one of the important items in the industry 4.0  with usual features as the Scada System, Plc and HMI …

My Project passes with  four important topics  [Maintanances , PLC controling ,Scada and Commmunication & IIOT]
1-Maintanances: the hardware of my automation system has sensors such as  industrial temperature sensore ,flow meter sensor,load cell sensor and taco sensor  that most of them send signals  as current in range (4:20 mA)and has  actuators such as  control valves , radiatar   and pumps ,stirrer and heater  ,we need to maintanances  most of these actuators  and  sensors .
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/5403775e-33a3-4e0f-95fd-2ad04167635c)


2-PLC controlling: Available PLCS that we have them were belonging to LSIS Company and type (DR30SU)  where we had very important challange how to can control system through three  plc and communicate between them and transfer any data from and to the web cloud.

3- Scada System: we builded  Scada system for this hardware by XP_Builder Program and then monitoring the system by the scada and take any action from it  to be done in the system![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/d0953681-4786-435a-99b3-7897a901abd5)

4- the Communication & IIOT :![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/d0c38c3a-bafa-414f-a644-ca5d39f63602)
the most acheivement  is that we can make communication between three plcs with each other and esp32 microcontroller that have ability to  connect  with  wifi and then transer data  to cloud and take any actoin from cloud to the esp32 ,  this communnication splits to two strong missions, 
the First Mission:  we make commincatin between the plcs with each other  between esp32,  we do it through using Modbus RTU protocol  via serial communicatino RS 485 where the esp32 is master modbus rtu by using converter is called "RS485 TTL CONVERTER " ![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/52b54482-fa27-4e54-9e59-e015b6fca142)
  that  can request any Slave PLCs  and take responses from them , and then we can take any data from plc or give to any of them data or required action.
the Second Mission: we use esp32 as bridge between the plcs and cloud that we handle to it is called Upidots,in this Mission we  use MQTT protocol for publishing and subscribing with cloud that is achieved by "UpidotsEsp32Mqtt" library. 

5- the Another Future phase will be achieved in the second term called "Digital Twin" 







