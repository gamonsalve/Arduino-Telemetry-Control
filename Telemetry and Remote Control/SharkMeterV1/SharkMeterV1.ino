#include <DHT.h>
#define DHTPIN 7
#define DHTTYPE DHT11
DHT dhtSensor(DHTPIN,DHTTYPE);
//#include <SoftwareSerial.h>
//SoftwareSerial SIM808(10,11); // Setting SIM808 Serial pins T conected to P 10 and R to 11
//SoftwareSerial Serial1(10,11); //(Rx,Tx) Setting Sharkmeter Serial pins
/////VARIABLES FOR MEASUREMENT///////
float VBatt1=0;
float VBatt2=0;
float Temperature;
float Humidity;
float man;
float FP=0;
float Voltage=0;
float Current=0;
float Watts=0;
byte SlaveAddress;
byte Function;
byte Address_hi;
byte Address_lo;
byte No_Reg_Hi;
byte No_Reg_lo;
byte CRC_Lo;
byte CRC_Hi;
byte frame[8];
byte buffer[9];
unsigned int CRC;
unsigned int Address;
unsigned int No_reg;
int frame_sizof=0;
int bites;
int index = 0;
int sign;
int Ex;
int exponet;
float WattsHr=0;
String data;
int Refresh=0;
void setup()
{
  //Setting Control PIN
  pinMode(12,OUTPUT);
  digitalWrite(12,LOW);
  //Starting SIM808 connection
  Serial2.begin(9600);//GPRS baud rate
  //PC communication
  Serial.begin(9600);
  delay(5000);
  //Setup SIM808
  Initialize();
  delay(60000);
  //Starting Sharkmeter connection
  Serial1.begin(19200); //TXD.TX1 and RXD.RX1
}


void loop()
{
  //Retrieving data from DataBase
  Getdata();
  delay(20000);
  //Getting electrical variables
  Voltage=GetVoltage();
  Current=GetCurrent();
  Watts=GetPower();
  WattsHr=GetPowerh();
  WattsHr=WattsHr/100;
  VBatt1=GetBatt1();
  VBatt2=GetBatt2();
  FP=GetFP();
  //Getting Tempeture and Humidity
  Humidity=dhtSensor.readHumidity();
  Temperature=dhtSensor.readTemperature();
  //Sending Data to de DataBase
  SendMeasures(Voltage,Current,Watts,WattsHr,Temperature,Humidity,VBatt1,VBatt2);
  SendMeasures2(Voltage,Current,Watts,WattsHr,VBatt1,VBatt2);
  SendMeasures3(Temperature,Humidity);
  delay(208000);

  //Restart system every 12hours
  Refresh=Refresh+1;
  if (Refresh==72){
        Initialize();
        delay(60000);
        Refresh=0;
        }

  /*
  //Printing for Test
  //Getting GPS data
  //GetGPS(data);

  //Test variables
  Serial.print("Voltaje Medidor 1: ");
  Serial.println(Voltage);
  Serial.print("Corriente Medidor 1: ");
  Serial.println(Current);
  Serial.print("Potencia Medidor 1: ");
  Serial.println(Watts);
  Serial.print("Potencia por hora Medidor 1: ");
  Serial.println(WattsHr);
  Serial.print("FP Medidor 1: ");
  Serial.println(FP);
  Serial.println("___________________________");
  Serial.print("Humidity: ");
  Serial.println(Humidity);
  Serial.print("Temperature: ");
  Serial.println(Temperature);
  Serial.println(VBatt1);
  Serial.println(VBatt2);
  Serial.println("------------");
  */

}

//Functions
//Openshift website
void SendMeasures(float Voltage, float Current, float Watts, float WattsHr,float Temperature, float Humidity,float VBatt1,float VBatt2){
 //SEND VALUES TO WEBSITE
  //Currently the refresh rate is approximately 20 seconds
  //You can change it by changing the delay times below

    Serial2.println("AT+HTTPINIT"); //Method used to concatnate the string to send and update the website
    delay(2000);
    Serial2.print("AT+HTTPPARA=\"URL\",\"http://track-mypower.rhcloud.com/mysql/dataToDB.php?user=mpardo&pssd=pardo1234&");
    Serial2.print("voltage=");
    Serial2.print(Voltage,2);
    Serial2.print("&current=");
    Serial2.print(Current,2);
    Serial2.print("&energy=");
    Serial2.print(WattsHr,2);
    Serial2.print("&power=");
    Serial2.print(Watts,2);
    Serial2.print("&latitude=");
    Serial2.print("11.019818");
    Serial2.print("&longitude=");
    Serial2.print("-74.851338");
    Serial2.print("&date_time=");
    Serial2.print(0);
    Serial2.print("&temperature=");
    Serial2.print(Temperature,2);
    Serial2.print("&humidity=");
    Serial2.print(Humidity,2);
    Serial2.print("&pf=");
    Serial2.print(FP);
    Serial2.print("&vbatt1=");
    Serial2.print(VBatt1);
    Serial2.print("&vbatt2=");
    Serial2.print(VBatt2);
    Serial2.println("\"");
    delay(2000);
    // set http action type 0 = GET, 1 = POST, 2 = HEAD
   Serial2.println("AT+HTTPACTION=0");
   delay(6000);
   // read server response
   Serial2.println("AT+HTTPREAD");
   delay(1000);
   Serial2.println("");
   Serial2.println("AT+HTTPTERM");
   delay(300);
   Serial2.println("");
  delay(5000);
  }
//Jose's Website
void SendMeasures2(float Voltage, float Current, float Watts, float WattsHr,float VBatt1,float VBatt2){
 //SEND VALUES TO WEBSITE
  //Currently the refresh rate is approximately 20 seconds
  //You can change it by changing the delay times below

    Serial2.println("AT+HTTPINIT"); //Method used to concatnate the string to send and update the website
    delay(2000);
    Serial2.print("AT+HTTPPARA=\"URL\",\"http://track-mypower.tk/measurements/electrical/new?");
    Serial2.print("voltage_med1=");
    Serial2.print(Voltage,2);
    Serial2.print("&current_med1=");
    Serial2.print(Current,2);
    Serial2.print("&energy_med1=");
    Serial2.print(WattsHr,2);
    Serial2.print("&power_med1=");
    Serial2.print(Watts,2);
    Serial2.print("&pf_med1=");
    Serial2.print(FP);
    Serial2.print("&voltage_batt1=");
    Serial2.print(VBatt1);
    Serial2.print("&voltage_batt2=");
    Serial2.print(VBatt2);
    Serial2.println("\"");
    Serial2.println("AT+HTTPPARA=\"CONTENT\",\"Basic YWRtaW46dW5pbm9ydGU=\"");
    delay(2000);
    // set http action type 0 = GET, 1 = POST, 2 = HEAD
   Serial2.println("AT+HTTPACTION=0");
   delay(6000);
   // read server response
   Serial2.println("AT+HTTPREAD");
   delay(1000);
   Serial2.println("");
   Serial2.println("AT+HTTPTERM");
   delay(300);
   Serial2.println("");
  delay(5000);
  }

void SendMeasures3(float Temperature, float Humidity){
 //SEND VALUES TO WEBSITE
 //Currently the refresh rate is approximately 20 seconds
 //You can change it by changing the delay times below

    Serial2.println("AT+HTTPINIT"); //Method used to concatnate the string to send and update the website
    delay(2000);
    Serial2.print("AT+HTTPPARA=\"URL\",\"http://track-mypower.tk/measurements/internal_conditions/new?");
    Serial2.print("temperature_int=");
    Serial2.print(Temperature,2);
    Serial2.print("&humidity_int=");
    Serial2.print(Humidity,2);
    Serial2.println("\"");
    Serial2.println("AT+HTTPPARA=\"CONTENT\",\"Basic YWRtaW46dW5pbm9ydGU=\"");
    delay(2000);
    // set http action type 0 = GET, 1 = POST, 2 = HEAD
   Serial2.println("AT+HTTPACTION=0");
   delay(6000);
   // read server response
   Serial2.println("AT+HTTPREAD");
   delay(1000);
   Serial2.println("");
   Serial2.println("AT+HTTPTERM");
   delay(300);
   Serial2.println("");
  delay(5000);
  }

void Initialize(){
  //Restart SIM808 Module
  delay(10000);
  Serial2.println("AT+CPOWD=1");
  delay(60000);
  // bearer settings
  Serial2.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  delay(2000);
  // bearer settings
  Serial2.println("AT+SAPBR=3,1,\"APN\",\"moviletb.net.co\"");
  delay(2000);
  Serial2.println("AT+SAPBR=3,1,\"USER\",\"etb\"");
  delay(2000);
  Serial2.println("AT+SAPBR=3,1,\"PWD\",\"etb\"");
  delay(2000);
  //Turn ON GPS
  Serial2.println("AT+CGNSPWR=1");
  delay(2000);
  // bearer settings
  Serial2.println("AT+SAPBR=1,1");
  delay(2000);
  }

void printSerialData()
{
 while(Serial2.available()!=0)
 Serial.write(Serial2.read());
}

void Getdata(){
  //Lectura Página Web
  Serial2.println("AT+HTTPINIT"); //init the HTTP request
  delay(2000); 
  //printSerialData();
 
  Serial2.println("AT+HTTPPARA=\"URL\",\"http://track-mypower.rhcloud.com/control.php\"");// setting the httppara, 
  //the second parameter is the website from where you want to access data 
  delay(1000);
  //printSerialData();
 
  Serial2.println();
  Serial2.println("AT+HTTPACTION=0");//submit the GET request 
  delay(8000);//the delay is important if the return datas are very large, the time required longer.
  printSerialData();
  Serial2.println("AT+HTTPREAD=0,20");// read the data from the website you access
  delay(3000);
  data=Serial2.readString();
  Serial.println("Data is:");
  Serial.println(data);
  data=data.substring(33,34);
  Serial.println("Data length:");
  Serial.println(data.length());
  Serial.println("Data substring:");
  Serial.println(data);
  Serial.println("----------");
  
  if(data=="1"){
      digitalWrite(12,HIGH);
      delay(1500);
      digitalWrite(12,LOW);
    }
    
  /*
  if(olddata=="3"){
    olddata=data;
    }else if (data=="1" && olddata=="0") {
      digitalWrite(12,HIGH);
      delay(1000);
      digitalWrite(12,LOW);
      olddata=data;
      } else if (data=="0" && olddata=="1"){
      digitalWrite(12,HIGH);
      delay(1000);
      digitalWrite(12,LOW);
      olddata=data;
        }
        */
  Serial2.println("");
  delay(1000);
  Serial2.println("AT+HTTPTERM");// terminate HTTP service
  printSerialData();
  }

float GetBatt1(){
  float voltage= analogRead(A0)*(5.0/1024.0)*5.0;
  delay(500);
  return voltage;
  }

float GetBatt2(){
  float voltage= analogRead(A1)*(5.0/1024.0)*5.0;
  delay(500);
  return voltage;
  }

void GetGPS(String &data){
   //Envio datos GPS y time
  Serial2.println("AT+CGNSINF");
  data=Serial2.readString();
  Serial.println("Data is:");
  Serial.println(data);
  }
float GetVoltage(){
  ////VOLTAGE CASE////
     SlaveAddress = 0x01;                                //This block assigns values to the packet variables
     Function = 0x03;
     Address_hi = 0x03;
     Address_lo = 0xe7;
     No_Reg_Hi = 0x00;
     No_Reg_lo =0x02;
     frame_sizof=sizeof(frame);
     Address = word(Address_hi,Address_lo);
     No_reg = word(No_Reg_Hi,No_Reg_lo);
     //Serial.print("Voltage: ");

    frame[0]= SlaveAddress;                            //This block puts the variables into an arra that will send out to the shark meter
    frame[1] = Function;
    frame[2]= (byte)(Address >> 8);
    frame[3] = (byte)Address;
    frame[4] = (byte)(No_reg>>8);
    frame[5] = (byte)No_reg;
    CRC16(frame,frame_sizof);
    CRC_Lo = lowByte(CRC);
    CRC_Hi = highByte(CRC);
    frame[6] = CRC_Lo;
    frame[7] = CRC_Hi;

    Serial1.write(frame[0]);                        //Packet is being sent
    Serial1.write(frame[1]);
    Serial1.write(frame[2]);
    Serial1.write(frame[3]);
    Serial1.write(frame[4]);
    Serial1.write(frame[5]);
    Serial1.write(frame[6]);
    Serial1.write(frame[7]);

    delay(750);
    while(Serial1.available()> 0)                  //Checks for response
  {
    bites = Serial1.available();
    for( int i = bites ; i > 0; i--)
    {
      buffer[index] = Serial1.read();            //Stores the incoming bytes from the response into an array called buffer
      index++;

    }
  }
   sign= getSign(buffer);                         //Three necessary values in order to convert the float type into decimal
   Ex =getExponet(buffer);
   man = getMantissa(buffer);

   Voltage = ((sign*man)*pow(2,Ex));
   Serial1.flush();                             //Clears whatever may be left
   index = 0;
   //Serial.println(Voltage);
   return Voltage;
  }
float GetCurrent(){
  SlaveAddress = 0x01;
     Function = 0x03;
     Address_hi = 0x03;
     Address_lo = 0xf3;
     No_Reg_Hi = 0x00;
     No_Reg_lo =0x02;
     frame_sizof=sizeof(frame);
     Address = word(Address_hi,Address_lo);
     No_reg = word(No_Reg_Hi,No_Reg_lo);


    frame[0]= SlaveAddress;
    frame[1] = Function;
    frame[2]= (byte)(Address >> 8);
    frame[3] = (byte)Address;
    frame[4] = (byte)(No_reg>>8);
    frame[5] = (byte)No_reg;
    CRC16(frame,frame_sizof);
    CRC_Lo = lowByte(CRC);
    CRC_Hi = highByte(CRC);
    frame[6] = CRC_Lo;
    frame[7] = CRC_Hi;

    Serial1.write(frame[0]);                      //Send Packet
    Serial1.write(frame[1]);
    Serial1.write(frame[2]);
    Serial1.write(frame[3]);
    Serial1.write(frame[4]);
    Serial1.write(frame[5]);
    Serial1.write(frame[6]);
    Serial1.write(frame[7]);

    delay(750);

    while(Serial1.available()> 0)                //Put received info into an array
  {
    bites = Serial1.available();
    for( int i = bites ; i > 0; i--)
    {
      buffer[index] = Serial1.read();
      index++;

    }
  }
   sign= getSign(buffer);                         //conversion of float type
   Ex =getExponet(buffer);
   man = getMantissa(buffer);

   Current = ((sign*man)*pow(2,Ex));


   Serial1.flush();
   index = 0;
   return Current;
  }

  float GetPower(){
  SlaveAddress = 0x01;                        //Setup Packet
     Function = 0x03;
     Address_hi = 0x03;
     Address_lo = 0xf9;
     No_Reg_Hi = 0x00;
     No_Reg_lo =0x02;
     frame_sizof=sizeof(frame);
     Address = word(Address_hi,Address_lo);
     No_reg = word(No_Reg_Hi,No_Reg_lo);
    frame[0]= SlaveAddress;
    frame[1] = Function;
    frame[2]= (byte)(Address >> 8);
    frame[3] = (byte)Address;
    frame[4] = (byte)(No_reg>>8);
    frame[5] = (byte)No_reg;
    CRC16(frame,frame_sizof);
    CRC_Lo = lowByte(CRC);
    CRC_Hi = highByte(CRC);
    frame[6] = CRC_Lo;
    frame[7] = CRC_Hi;

    Serial1.write(frame[0]);                  //Send Packet
    Serial1.write(frame[1]);
    Serial1.write(frame[2]);
    Serial1.write(frame[3]);
    Serial1.write(frame[4]);
    Serial1.write(frame[5]);
    Serial1.write(frame[6]);
    Serial1.write(frame[7]);

    delay(750);

    while(Serial1.available()> 0)            //Put received info in a array
  {
    bites = Serial1.available();
    for( int i = bites ; i > 0; i--)
    {
      buffer[index] = Serial1.read();
      index++;

    }
  }
   sign= getSign(buffer);                       //Float conversion
   Ex =getExponet(buffer);
   man = getMantissa(buffer);

   Watts = ((sign*man)*pow(2,Ex));
   Serial1.flush();
   index = 0;
   return Watts;
  }

  float GetPowerh(){
    SlaveAddress = 0x01;                        //Setup Packet
     Function = 0x03;
     Address_hi = 0x04;
     Address_lo = 0x4B;
     No_Reg_Hi = 0x00;
     No_Reg_lo =0x02;
     frame_sizof=sizeof(frame);
     Address = word(Address_hi,Address_lo);
     No_reg = word(No_Reg_Hi,No_Reg_lo);
    frame[0]= SlaveAddress;
    frame[1] = Function;
    frame[2]= (byte)(Address >> 8);
    frame[3] = (byte)Address;
    frame[4] = (byte)(No_reg>>8);
    frame[5] = (byte)No_reg;
    CRC16(frame,frame_sizof);
    CRC_Lo = lowByte(CRC);
    CRC_Hi = highByte(CRC);
    frame[6] = CRC_Lo;
    frame[7] = CRC_Hi;

    Serial1.write(frame[0]);                  //Send packet
    Serial1.write(frame[1]);
    Serial1.write(frame[2]);
    Serial1.write(frame[3]);
    Serial1.write(frame[4]);
    Serial1.write(frame[5]);
    Serial1.write(frame[6]);
    Serial1.write(frame[7]);

    delay(750);

    while(Serial1.available()> 0)              //Put received info into a array
  {
    bites = Serial1.available();
    for( int i = bites ; i > 0; i--)
    {
      buffer[index] = Serial1.read();
      index++;

    }
  }                                           //Since WattHr is not a float type we can just extract the data bytes from the message with the getSint32 Function
   WattsHr = getSint32(buffer);              //SINT32 conversion
   Serial1.flush();
   index = 0;
   return WattsHr;
    }

float GetFP(){
  SlaveAddress = 0x01;                        //Setup Packet
     Function = 0x03;
     Address_hi = 0x03;
     Address_lo = 0xFF;
     No_Reg_Hi = 0x00;
     No_Reg_lo =0x02;
     frame_sizof=sizeof(frame);
     Address = word(Address_hi,Address_lo);
     No_reg = word(No_Reg_Hi,No_Reg_lo);
    frame[0]= SlaveAddress;
    frame[1] = Function;
    frame[2]= (byte)(Address >> 8);
    frame[3] = (byte)Address;
    frame[4] = (byte)(No_reg>>8);
    frame[5] = (byte)No_reg;
    CRC16(frame,frame_sizof);
    CRC_Lo = lowByte(CRC);
    CRC_Hi = highByte(CRC);
    frame[6] = CRC_Lo;
    frame[7] = CRC_Hi;

    Serial1.write(frame[0]);                  //Send Packet
    Serial1.write(frame[1]);
    Serial1.write(frame[2]);
    Serial1.write(frame[3]);
    Serial1.write(frame[4]);
    Serial1.write(frame[5]);
    Serial1.write(frame[6]);
    Serial1.write(frame[7]);

    delay(750);

    while(Serial1.available()> 0)            //Put received info in a array
  {
    bites = Serial1.available();
    for( int i = bites ; i > 0; i--)
    {
      buffer[index] = Serial1.read();
      index++;

    }
  }
   sign= getSign(buffer);                       //Float conversion
   Ex =getExponet(buffer);
   man = getMantissa(buffer);

   FP = ((sign*man)*pow(2,Ex));
   Serial1.flush();
   index = 0;
   return FP;
  }

// Garza's Functions
  unsigned long CRC16(byte message[], int frame_size )
{

  byte CRC_Hi = 0xFF;
  byte CRC_Lo = 0xFF;
  CRC = word(CRC_Hi,CRC_Lo);  //Load crc register with 0xFFFF (Step 1)

  for( int i = 0; i< frame_size-2;i++)            //changed to 6 for testing purposes
  {
    CRC = CRC ^ message[i];              //Step 2: XOR message byte with low order byte of crc register
   // CRC = CRC >> 1     ;                           //First shift. That is why j starts at 1 and not 0


    for ( int j = 0; j < 8; j++)            //Step 3: is a shifts. Occurs after the conditional.
    {
   //   CRC = CRC >> 1;

    if ((CRC  & 0x0001) == 0x0001)           //LSbit is one
    {
       CRC = CRC >> 1;
     CRC = CRC ^ 0xA001;
                                      //Here is step 3 repeated
    }
    else                                        //Case where Lsbit is zero
    {
    CRC = CRC >> 1;
    }
    }
  }

  return CRC;


}

int getSign(byte array[]){
  byte HiReg1 = array[3];                //To get the sign of the float type we look at the most significant bit in the 4byte data
                                         //the most significant byte is the fourth element of the response [Slave address, Function, #bytes returned, MSbyte,...]
  int sign = bitRead(HiReg1,7);

  if( sign == 0){
   return 1;
  }
  else{
    return -1;
  }
}

int getExponet(byte array[]){            //The exponet value requires the data from the first register (first two bytes)
  byte HiReg1 = array[3];
  byte LoReg1 = array[4];

  byte NewHiReg1 = HiReg1 << 1;          //We shift because the first bit is soley used to get the sign
                                         //this new 8 bit value will determine the exponet value (NewHiReg1)
  if (bitRead(LoReg1, 7) == 1){          //We check the MSbit of the second byte. If its 1, the LSbit on NewHiReg will be set to 1
     bitSet(NewHiReg1,0);
  }
  else{
   bitClear(NewHiReg1,0);                //else its zero
  }
  Ex = NewHiReg1 -127;                   //We subtract 127 from NewHiReg1 to get the exponet value

  ;
  return Ex;

}

float getMantissa(byte array[]){
  int i ;
  int j ;
  int exponet=1;
  byte LoReg1 = array[4];
  byte HiReg2 = array[5];
  byte LoReg2 = array[6];

  float Mantissa=0;




  for( i = 4; i<7;i++){

    byte indexByte = array[i];
    if( i == 4){
       for( j = 6; j>=0;j--){
       int bitValue = bitRead(indexByte,j);

       if (bitValue == 1){
           Mantissa +=  (1 / pow(2,exponet));
           exponet++;
         }
       else{

         exponet++;
       }
      }
     }

    else{
      for( j = 7; j>=0;j--){
       int bitValue = bitRead(indexByte,j);

       if (bitValue == 1){
           Mantissa +=  (1 / pow(2,exponet));
           exponet++;
         }
       else{

         exponet++;
       }
      }
     }

  }

   Mantissa += 1;
   return Mantissa;

}

long getSint32(byte array[])
{
   long tru = 0x00000000;

   long frst = array[3];
   frst = frst << 24;
   tru = (tru | frst);

   long scnd = array[4];
   scnd = scnd << 16;
   tru = (tru | scnd);

   long thrd = array[5];
   thrd = thrd << 8;
   tru = (tru| thrd);

   long fort = array[6];
   tru = (tru|fort);

  return tru;

}
