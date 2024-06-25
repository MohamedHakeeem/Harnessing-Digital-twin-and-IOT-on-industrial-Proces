# Harnessing-Digital-twin-and-IOT-on-industrial-Proces
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/bd085315-1a7d-4a0e-bce3-6cea91e8a40f)

My graduation  Project is called “Harnessing Digital Twin & IIOT” and aims to be a system that achieves many topics that are the Future of the time such as the IIOT that makes automation part of the internet for communicating with other systems easy and efficient, the Digital Twin that is considered as one of the important items in the industry 4.0  with usual features as the Scada System, Plc and HMI …

It  passes with  five important topics  [Maintanances, PLC controling, SCADA, communication & IIOT, Digital Twin]
1-Maintanances: the hardware of my automation system has sensors such as industrial temperature sensors, flow meter sensors, load cell sensors, and taco sensors most of which send signals as current signals in range (4:20 mA)and have actuators such as control valves, radiator, and pumps, stirrer, and heater, we had to maintenance most of these actuators and sensors.

![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/25422bae-fc36-404c-9808-6045c18e76a4)

2- Controlling PLC: The available PLCS that we had belonged to LSIS Company and type (DR30SU)  where we had a significant challenge on how to control the system through three plc communicate between them and transfer any data from and to the cloud.


3- Scada System: we built a SCADA system for this hardware using the XP_Builder Program and then monitored the system by the Scada and took any action from it  to be done in the system.

![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/a5e52b7c-9f3d-4f70-b41b-d91810c4c5cc)

4- the Communication & IIOT : 
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/df418296-8f6c-4711-8ae9-762aebe0993e)

the greatest achievement is that we can make communication between three PLCS  each other and an esp32 microcontroller that can connect  with  wifi and then transfer data  to the cloud and take any action from the cloud to the esp32,  this communication splits into two strong missions, 
the First Mission:  we make communication between the PLCs with each other  between esp32,  we do it through using Modbus RTU protocol  via serial communication RS 485 where the esp32 is master Modbus RTU by using a converter called "RS485 TTL CONVERTER "
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/3845b5ca-12f1-494a-98a7-d263041db882)


that can request from any Slaves as PLCS  and take responses from them, and then we can take any data from PLC or give any the data or required action to it.
the Second Mission: we use esp32 as the bridge between the PLCs and the cloud that we handle called Upidots, in this Mission we use MQTT protocol for publishing and subscribing with the cloud is achieved by the "UpidotsEsp32Mqtt" library. 

![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/e7858955-991f-4b44-88be-067d5608c678)
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/77c1dd18-8a06-4928-952d-a825801cf7a0)



5- Another Future feature that will be achieved on the system in the second term is called "Digital Twin" 

-Finally, I would like to thank my team for their effort in doing  this Strong Project :
Eng: Ahmed Ebrahim 
Eng: Mohamed El Sadek
Eng: Omar Hamdy
that they  study in the Faculty of Engineering Ain Shams University with me 






