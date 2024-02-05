#include <ModbusMaster.h>
#include <SoftwareSerial.h>

// Define the Modbus communication parameters
#define BAUD_RATE 9600
#define PARITY 0
#define DATA_BITS 8
#define STOP_BITS 1
#define SerialRS485_RX_PIN 16
#define SerialRS485_TX_PIN 17

// Define the slave addresses for the two devices
#define PLC_SLAVE1_Station 1
#define PLC_SLAVE2_Station 2
#define PLC_SLAVE3_Station 3

SoftwareSerial Serial_mod(SerialRS485_RX_PIN, SerialRS485_TX_PIN);    


 
uint16_t TT1_PLC1 = 0;
uint16_t ST1_PLC1 = 0;
uint16_t LC1_Virtual=0;
/****/
uint16_t FT1_PLC2 = 0;
uint16_t CE1_PLC2 = 0;
uint16_t WT1_PLC2 = 0;
uint16_t PA1_PLC2 = 0;
uint16_t PA2_PLC2 = 0;
uint16_t Fan=0;
uint16_t Stirrer=0;
uint16_t FC1=0;
uint16_t LC1=0;
uint16_t J1=0;   //heater



/****/
uint16_t TT2_PLC3 = 0;
uint16_t LT1_PLC3 = 0;

//tt1 and st1 from plc 1 to plc2
//

// Create an instance of the ModbusMaster class
ModbusMaster Slave1;
ModbusMaster Slave2;
ModbusMaster Slave3;

void setup() {
  // Initialize the Serial port for Modbus communication
  Serial.begin(9600);
  Serial_mod.begin(BAUD_RATE);
 
  // Set up the first Modbus slave device
  Slave1.begin(PLC_SLAVE1_Station, Serial_mod);
  // Set up the second Modbus slave device
  Slave2.begin(PLC_SLAVE2_Station, Serial_mod);
  // Set up the third Modbus slave device
  Slave3.begin(PLC_SLAVE3_Station, Serial_mod);


  
}


void loop() 
{

/**********************Read Data from PlCS to PLCS*****************************/
/************* Read analog sensors signals  from PLC1***********/
// Read TT1 and St1  from register D0003 & D0004

  uint8_t result_0 = Slave1.readInputRegisters(3, 3);
  if (result_0 == Slave1.ku8MBSuccess) {
    TT1_PLC1 = Slave1.getResponseBuffer(0);
    ST1_PLC1 = Slave1.getResponseBuffer(1);
    LC1_Virtual= Slave1.getResponseBuffer(2);
  }

 Serial.println(" monitoring for plc1 data   ^^^^^______^^^^^ " );
 Serial.print(" TT1_PLC1: " );Serial.print( TT1_PLC1 );Serial.print(" |||| ");Serial.print("   ST1_PLC1 : " );Serial.print(   ST1_PLC1  );
 Serial.print(" |||| ");Serial.print(" LC1_Virtual : " );Serial.println( LC1_Virtual );
 delay(25);

  
  



/************ Read data from PLC2***************/
/************ Read analog sensors signals from plc2  *****************/ 
  //Read Wt1 from register D00014  
  uint8_t result_1 = Slave2.readInputRegisters(14, 1);
  if (result_1 == Slave2.ku8MBSuccess) {
    WT1_PLC2 = Slave2.getResponseBuffer(0);
  }

 Serial.println(" monitoring for analog sensors plc2 data   ^^^^^______^^^^^ " );
 Serial.print(" WT1_PLC2: " );Serial.print( WT1_PLC2 );Serial.print(" |||| "); 

 
  // Read FT1 from register D0008 
  uint8_t result_2 = Slave2.readInputRegisters(8, 1);
  if (result_2 == Slave2.ku8MBSuccess) {
    FT1_PLC2 = Slave2.getResponseBuffer(0);
  }

 Serial.print(" FT1_PLC2: " );Serial.print( FT1_PLC2 );Serial.print(" |||| ");   

  // Read CE1 from register D0002
  uint8_t result_3 = Slave2.readInputRegisters(2, 1);
  if (result_3 == Slave2.ku8MBSuccess) {
    CE1_PLC2 = Slave2.getResponseBuffer(0);
  }

 Serial.print("CE1_PLC2: " );Serial.println( CE1_PLC2 );

/************ Read analog actuators signals from plc2  *****************/  
// Read  Fan=0; Stirrer=0; FC1=0; LC1=0  from  (9-12)
  uint8_t result_4 = Slave2.readInputRegisters(9, 4);
  if (result_4 == Slave2.ku8MBSuccess) {
    Fan = Slave2.getResponseBuffer(0);
    Stirrer = Slave2.getResponseBuffer(1);
    FC1 = Slave2.getResponseBuffer(2);
    LC1 = Slave2.getResponseBuffer(3);
   }

 Serial.println(" monitoring for analog actuators plc2 data   ^^^^^______^^^^^ " );
 Serial.print(" Fan: " );Serial.print( Fan );Serial.print(" |||| "); 
 Serial.print(" Stirrer: " );Serial.print( Stirrer );Serial.print(" |||| ");   
 Serial.print(" FC1: " );Serial.print(  FC1 );Serial.print(" |||| "); 
 Serial.print(" LC1: " );Serial.println(  LC1 );
 delay(25);


/************ Read digital actuators signals from plc2  *****************/  
  // Read virtual heater  from plc2  
  /*
  uint8_t result_5 = Slave2.readDiscreteInputs(0,1);
  if (result_5 == Slave2.ku8MBSuccess) {
    J1 = Slave2.getResponseBuffer(0); 
   }

 Serial.println(" monitoring for plc2 data   ^^^^^______^^^^^ " );
 Serial.print(" J1: " );Serial.print( J1 );Serial.print(" |||| "); 

  */




/************ Read data from PLC3***************/
/************ Read analog sensors signals from plc3  *****************/ 
// Read TT2 from register D0001


  uint8_t result_6 = Slave3.readInputRegisters(1, 1);
  if (result_6 == Slave3.ku8MBSuccess) {
    TT2_PLC3  = Slave3.getResponseBuffer(0);
  }
  
 Serial.println(" monitoring for plc3 data   ^^^^^______^^^^^ " );
 Serial.print("TT2_PLC3 : " );Serial.print( TT2_PLC3  );Serial.print(" |||| "); 



  // Read LT1 from register D0009
  uint8_t result_7 = Slave3.readInputRegisters(9, 1);
  if (result_7 == Slave3.ku8MBSuccess) {
    LT1_PLC3  = Slave3.getResponseBuffer(0);
  }
  

 Serial.print("LT1_PLC3: " );Serial.println( LT1_PLC3 );
 delay(25);




/*****************************Write Data from PlCS to PLCS*****************************/
/************************Write analog actuator to PLC 1********************/

/*
Wrtie  [Fan]  to [PLC1 in M0000]:
Wrtie  [ Stirrer]  to [PLC1 in M0001]:
*/

  uint8_t result_11 = Slave1.writeSingleRegister(1	, Stirrer);
  delay(25);
  uint8_t result_10 = Slave1.writeSingleRegister(0	, Fan);
  delay(25);



/***************Write analog sensors data to PLC 2**********************/
/*
Wrtie  [TT1_PLC1]  to [PLC2 in M0000]:
Wrtie  [ ST1_PLC1]  to [PLC2 in M0001]:
Wrtie  [TT2_PLC3]  to [PLC2 in M0002]:
Wrtie  [ LT1_PLC3]  to [PLC2 in M0003]:
*/


  uint8_t result_20 = Slave2.writeSingleRegister(0	,TT1_PLC1 );
  delay(25);
  uint8_t result_21 = Slave2.writeSingleRegister(1	, ST1_PLC1 );
  delay(25);
  uint8_t result_22 = Slave2.writeSingleRegister(2	, TT2_PLC3 );
  delay(25);
  uint8_t result_23 = Slave2.writeSingleRegister(3	, LT1_PLC3 );
  delay(25);
 







/*********************Write  analog actuator  to PLC 3************************/
/*
Wrtie [FC1]  to [PLC3 in M0000]:
Wrtie  [LC1]  to [PLC3 in M0001]:
*/

  uint8_t result_31 = Slave3.writeSingleRegister(1	, LC1 );
  delay(25);
  uint8_t result_30 = Slave3.writeSingleRegister(0	, FC1 );
  delay(25);



/*****Write  Digital actuator  to PLC 3*********/
/*Wrtie  [J1]  to [PLC3 in k0002]:*/
  
  //uint8_t result_32 = Slave1.writeSingleCoil(3,1);
  //uint8_t result_32 = Slave3.writeSingleCoil(0,0); 





}