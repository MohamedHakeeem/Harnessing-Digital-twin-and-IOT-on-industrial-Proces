# Harnessing-Digital-twin-and-IOT-on-industrial-Proces
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/645fe325-17a3-4862-9a4b-3897e83b6e4c)

My graduation  Project is called “Harnessing Digital Twin & IIOT” and aims to be a system that achieves many topics that are the Future of the time such as the IIOT that makes automation part of the internet for communicating with other systems easy and efficient, the Digital Twin that is considered as one of the important items in the industry 4.0  with usual features as the Scada System, Plc and HMI …

It  passes with  four important topics  [Maintanances, PLC controling, SCADA, and communication & IIOT]
1-Maintanances: the hardware of my automation system has sensors such as industrial temperature sensors, flow meter sensors, load cell sensor, and taco sensor most of which send signals as current signals in range (4:20 mA)and has actuators such as control valves, radiator, and pumps, stirrer and heater, we had to maintenance most of these actuators  and sensors.

![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/5403775e-33a3-4e0f-95fd-2ad04167635c)

2- Controlling PLC: The available PLCS that we had them were belonging to LSIS Company and type (DR30SU)  where we had a very important challenge on how to control the system through three plc communicate between them and transfer any data from and to the cloud.

3- Scada System: we built a SCADA system for this hardware using XP_Builder Program and then monitoring the system by the Scada and taking any action from it  to be done in the system

![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/a5e52b7c-9f3d-4f70-b41b-d91810c4c5cc)

4- the Communication & IIOT : 
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/df418296-8f6c-4711-8ae9-762aebe0993e)

the greatest achievement is that we can make communication between three PLCS  each other and an esp32 microcontroller that can connect  with  wifi and then transfer data  to the cloud and take any action from the cloud to the esp32,  this communication splits into two strong missions, 
the First Mission:  we make communication between the PLCs with each other  between esp32,  we do it through using Modbus RTU protocol  via serial communication RS 485 where the esp32 is master Modbus RTU by using a converter called "RS485 TTL CONVERTER "
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/3845b5ca-12f1-494a-98a7-d263041db882)


that can request from any Slaves as PLCS  and take responses from them, and then we can take any data from PLC or give any the data or required action to it.
the Second Mission: we use esp32 as the bridge between the PLCs and the cloud that we handle called Upidots, in this Mission we use MQTT protocol for publishing and subscribing with the cloud is achieved by the "UpidotsEsp32Mqtt" library. 

![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/bf5a3ef9-0abb-40bc-b330-ad71db400a4a)  
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/b4db6b95-3255-4d21-85cc-13b940447597)



5- Another Future feature that will be achieved on the system in the second term is called "Digital Twin" 







