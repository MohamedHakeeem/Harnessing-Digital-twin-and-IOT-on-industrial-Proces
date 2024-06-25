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
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/667f2fd1-564c-4085-bb2e-4a78f1ed11f1)



5- Digital Twin:
We  Built a Digital Twin for our system and used Microsoft Azure Portal for this mission. First, we also use two software tools,  Arduino Ide for burning the suitable code on an esp32 board, and Microsoft Visual Studio for building the Azure function that is responsible for linking the Azure IOT hub with the Azure Digital twin service that they are available on the Azure portal, In this part, we could monitor and visualize my real system in parallel on azure digital twin as 3d visualize or as 2d view and by using Azure data explorer service did querying on  historical data,  mathematical analyses, plotting of data in the range of a certain time, It does not stop there, but extends to do  predictive maintenance on the data in this service, and more can be done by it.

The below diagram demonstrates how to communicate the hardware level of the system with the Azure portal down to Azure Digital Twin.
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/dddc2e32-ab91-4f5e-a1eb-68dd709833ee)

After achieving this communication. we can upload the DTDL model of our system in 2d view as twin graphs, and upload our system with extension .glb  in 3d visualization,  now I will show some of the cases from our system.
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/49fa895d-8b14-4f0a-84d1-02f4dc92df1f)
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/a8299c69-dbd8-42be-b608-bdf642257339)


Finally, I will show some of the Business queries done in Azure Data Explorer
![image](https://github.com/MohamedHakeeem/Harnessing-Digital-twin-and-IOT-on-industrial-Proces/assets/142861162/e877e2c0-e8c1-416f-936a-e637101fab93)

--------------------------------------------------------------Video of Graduation Project------------------------------------------------------------
https://engasuedu-my.sharepoint.com/:v:/r/personal/1900218_eng_asu_edu_eg/Documents/Graduation%20project%202024/Project_Video.mp4?csf=1&web=1&nav=eyJyZWZlcnJhbEluZm8iOnsicmVmZXJyYWxBcHAiOiJPbmVEcml2ZUZvckJ1c2luZXNzIiwicmVmZXJyYWxBcHBQbGF0Zm9ybSI6IldlYiIsInJlZmVycmFsTW9kZSI6InZpZXciLCJyZWZlcnJhbFZpZXciOiJNeUZpbGVzTGlua0NvcHkifX0&e=DkM9tZ


-In The End, I would like to thank my team project for their effort in doing this great Project :
|||| Eng: Ahmed Ebrahim  ||||
|||| Eng: Mohamed El Sadek ||||
|||| Eng: Omar Hamdy ||||








