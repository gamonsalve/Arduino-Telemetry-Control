#include <Fuzzy.h>
#include <FuzzyComposition.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzyOutput.h>
#include <FuzzyRule.h>
#include <FuzzyRuleAntecedent.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzySet.h>
//Electrical Variables
int Table=0.0;
float Curr_P = 0.0;
float Curr_L = 0.0;
float Voltage_L = 0.0;
// Instantiating an object of library
Fuzzy* fuzzy = new Fuzzy();
//Setting Voltage Membership Function (MF)
FuzzySet* VLow = new FuzzySet(0, 0, 114, 120);
FuzzySet* VOK = new FuzzySet(115, 118, 122, 125);
FuzzySet* VHigh = new FuzzySet(120, 130, 170, 170);

unsigned long Start=millis();

//Setting Current MF
FuzzySet* ILow = new FuzzySet(0, 0, 2, 4);
FuzzySet* IHigh = new FuzzySet(5, 7, 12, 12);
FuzzySet* IMedium = new FuzzySet(3.5,4.5,4.5,5.5);

//Setting LoadIN MF
FuzzySet* LLow = new FuzzySet(0, 0, 0.4, 0.9);
FuzzySet* LMedium = new FuzzySet(0.7, 1, 1, 1.3);
FuzzySet* LHigh = new FuzzySet(1.2, 1.5, 2, 2);

//Setting Load MF
FuzzySet* Low = new FuzzySet(0, 0, 0, 45);
FuzzySet* Medium = new FuzzySet(40,50,50,60);
FuzzySet* High = new FuzzySet(55, 100, 100, 100);

void setup() {
// put your setup code here, to run once:
//Pin Setup
int I1=5;
int I2=6;
int I3=7;
pinMode(I1,OUTPUT);
pinMode(I2,OUTPUT);
pinMode(I3,OUTPUT);
Serial.begin(9600);
//FuzzyInput Current
FuzzyInput* Current = new FuzzyInput(1);
  Current->addFuzzySet(ILow);
  Current->addFuzzySet(IMedium);
  Current->addFuzzySet(IHigh);
  fuzzy->addFuzzyInput(Current);
//FuzzyInput Voltage
FuzzyInput* Voltage = new FuzzyInput(2);
  Voltage->addFuzzySet(VLow);
  Voltage->addFuzzySet(VOK);
  Voltage->addFuzzySet(VHigh);
  fuzzy->addFuzzyInput(Voltage);
//FuzzyInput LoadIN
FuzzyInput* LoadIN = new FuzzyInput(3);
  LoadIN->addFuzzySet(LLow);
  LoadIN->addFuzzySet(LMedium);
  LoadIN->addFuzzySet(LHigh);
  fuzzy->addFuzzyInput(LoadIN);
//FuzzyOutput Load
FuzzyOutput* Load = new FuzzyOutput(1);
  Load->addFuzzySet(Low);
  Load->addFuzzySet(Medium);
  Load->addFuzzySet(High);
  fuzzy->addFuzzyOutput(Load);
// LoadIN Conditions
//LoadIN=High
FuzzyRuleAntecedent* LoadINHigh = new FuzzyRuleAntecedent();
LoadINHigh->joinSingle(LHigh);
//LoadIN=Medium
FuzzyRuleAntecedent* LoadINMedium = new FuzzyRuleAntecedent();
LoadINMedium->joinSingle(LMedium);
//LoadIN=Low
FuzzyRuleAntecedent* LoadINLow = new FuzzyRuleAntecedent();
LoadINLow->joinSingle(LLow);

//THEN STATMENTS  
//Consequence THEN LoadHigh
  FuzzyRuleConsequent* THENLoadHigh = new FuzzyRuleConsequent();
  THENLoadHigh->addOutput(High);
//Consequence THEN LoadMedium
  FuzzyRuleConsequent* THENLoadMedium = new FuzzyRuleConsequent();
  THENLoadMedium->addOutput(Medium);
//Consequence THEN LoadLow
  FuzzyRuleConsequent* THENLoadLow = new FuzzyRuleConsequent();
  THENLoadLow->addOutput(Low);


//Current AND Voltage Condition
//Current=High AND Voltage=High
  FuzzyRuleAntecedent* CurrentIHighANDVoltageVHigh = new FuzzyRuleAntecedent();
  CurrentIHighANDVoltageVHigh->joinWithAND(IHigh, VHigh);
//Current=High AND Voltage=OK
  FuzzyRuleAntecedent* CurrentIHighANDVoltageVOK = new FuzzyRuleAntecedent();
  CurrentIHighANDVoltageVOK->joinWithAND(IHigh, VOK);
//Current=High AND Voltage=Low  
  FuzzyRuleAntecedent* CurrentIHighANDVoltageVLow = new FuzzyRuleAntecedent();
  CurrentIHighANDVoltageVLow->joinWithAND(IHigh, VLow);
  
//Current=Medium AND Voltage=High
  FuzzyRuleAntecedent* CurrentIMediumANDVoltageVHigh = new FuzzyRuleAntecedent();
  CurrentIMediumANDVoltageVHigh->joinWithAND(IMedium, VHigh);
//Current=Medium AND Voltage=OK
  FuzzyRuleAntecedent* CurrentIMediumANDVoltageVOK = new FuzzyRuleAntecedent();
  CurrentIMediumANDVoltageVOK->joinWithAND(IMedium, VOK);
//Current=Medium AND Voltage=Low  
  FuzzyRuleAntecedent* CurrentIMediumANDVoltageVLow = new FuzzyRuleAntecedent();
  CurrentIMediumANDVoltageVLow->joinWithAND(IMedium, VLow);

//Current=Low AND Voltage=High
  FuzzyRuleAntecedent* CurrentILowANDVoltageVHigh = new FuzzyRuleAntecedent();
  CurrentILowANDVoltageVHigh->joinWithAND(ILow, VHigh);
//Current=Low AND Voltage=OK
  FuzzyRuleAntecedent* CurrentILowANDVoltageVOK = new FuzzyRuleAntecedent();
  CurrentILowANDVoltageVOK->joinWithAND(ILow, VOK);
//Current=Low AND Voltage=Low  
  FuzzyRuleAntecedent* CurrentILowANDVoltageVLow = new FuzzyRuleAntecedent();
  CurrentILowANDVoltageVLow->joinWithAND(ILow, VLow);

//1.Current=High AND Voltage=High AND LoadIN=High THEN Load=Low
FuzzyRuleAntecedent* CurrentIHighANDVoltageVHighANDLoadINHigh = new FuzzyRuleAntecedent();
CurrentIHighANDVoltageVHighANDLoadINHigh->joinWithAND(CurrentIHighANDVoltageVHigh, LoadINHigh);
//Adding Rule
FuzzyRule* fuzzyRule1 = new FuzzyRule(1, CurrentIHighANDVoltageVHighANDLoadINHigh, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule1);
//2.Current=High AND Voltage=High AND LoadIN=Medium THEN Load=Medium
FuzzyRuleAntecedent* CurrentIHighANDVoltageVHighANDLoadINMedium = new FuzzyRuleAntecedent();
CurrentIHighANDVoltageVHighANDLoadINMedium->joinWithAND(CurrentIHighANDVoltageVHigh, LoadINMedium);
//Adding Rule
FuzzyRule* fuzzyRule2 = new FuzzyRule(2, CurrentIHighANDVoltageVHighANDLoadINMedium, THENLoadMedium);
fuzzy->addFuzzyRule(fuzzyRule2);
//3.Current=High AND Voltage=High AND LoadIN=Low THEN Load=High
FuzzyRuleAntecedent* CurrentIHighANDVoltageVHighANDLoadINLow = new FuzzyRuleAntecedent();
CurrentIHighANDVoltageVHighANDLoadINLow->joinWithAND(CurrentIHighANDVoltageVHigh, LoadINLow);
//Adding Rule
FuzzyRule* fuzzyRule3 = new FuzzyRule(3, CurrentIHighANDVoltageVHighANDLoadINLow, THENLoadHigh);
fuzzy->addFuzzyRule(fuzzyRule3);
//4.Current=High AND Voltage=OK AND LoadIN=High THEN Load=Low
FuzzyRuleAntecedent* CurrentIHighANDVoltageVOKANDLoadINHigh = new FuzzyRuleAntecedent();
CurrentIHighANDVoltageVOKANDLoadINHigh->joinWithAND(CurrentIHighANDVoltageVOK, LoadINHigh);
//Adding Rule
FuzzyRule* fuzzyRule4 = new FuzzyRule(4, CurrentIHighANDVoltageVOKANDLoadINHigh, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule4);
//5.Current=High AND Voltage=OK AND LoadIN=Medium THEN Load=Medium
FuzzyRuleAntecedent* CurrentIHighANDVoltageVOKANDLoadINMedium = new FuzzyRuleAntecedent();
CurrentIHighANDVoltageVOKANDLoadINMedium->joinWithAND(CurrentIHighANDVoltageVOK, LoadINMedium);
//Adding Rule
FuzzyRule* fuzzyRule5 = new FuzzyRule(5, CurrentIHighANDVoltageVOKANDLoadINMedium, THENLoadMedium);
fuzzy->addFuzzyRule(fuzzyRule5);
//6.Current=High AND Voltage=OK AND LoadIN=Low THEN Load=High
FuzzyRuleAntecedent* CurrentIHighANDVoltageVOKANDLoadINLow = new FuzzyRuleAntecedent();
CurrentIHighANDVoltageVOKANDLoadINLow->joinWithAND(CurrentIHighANDVoltageVOK, LoadINLow);
//Adding Rule
FuzzyRule* fuzzyRule6 = new FuzzyRule(6, CurrentIHighANDVoltageVOKANDLoadINLow, THENLoadHigh);
fuzzy->addFuzzyRule(fuzzyRule6);
//7.Current=High AND Voltage=Low AND LoadIN=High THEN Load=Low
FuzzyRuleAntecedent* CurrentIHighANDVoltageVLowANDLoadINHigh = new FuzzyRuleAntecedent();
CurrentIHighANDVoltageVLowANDLoadINHigh->joinWithAND(CurrentIHighANDVoltageVLow, LoadINHigh);
//Adding Rule
FuzzyRule* fuzzyRule7 = new FuzzyRule(7, CurrentIHighANDVoltageVLowANDLoadINHigh, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule7);
//8.Current=High AND Voltage=Low AND LoadIN=Medium THEN Load=Low
FuzzyRuleAntecedent* CurrentIHighANDVoltageVLowANDLoadINMedium = new FuzzyRuleAntecedent();
CurrentIHighANDVoltageVLowANDLoadINMedium->joinWithAND(CurrentIHighANDVoltageVLow, LoadINMedium);
//Adding Rule
FuzzyRule* fuzzyRule8 = new FuzzyRule(8, CurrentIHighANDVoltageVLowANDLoadINMedium, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule8);
//9.Current=High AND Voltage=Low AND LoadIN=Low THEN Load=Low
FuzzyRuleAntecedent* CurrentIHighANDVoltageVLowANDLoadINLow = new FuzzyRuleAntecedent();
CurrentIHighANDVoltageVLowANDLoadINLow->joinWithAND(CurrentIHighANDVoltageVLow, LoadINLow);
//Adding Rule
FuzzyRule* fuzzyRule9 = new FuzzyRule(9, CurrentIHighANDVoltageVLowANDLoadINLow, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule9);
//10.Current=Medium AND Voltage=High AND LoadIN=High THEN Load=Low
FuzzyRuleAntecedent* CurrentIMediumANDVoltageVHighANDLoadINHigh = new FuzzyRuleAntecedent();
CurrentIMediumANDVoltageVHighANDLoadINHigh->joinWithAND(CurrentIMediumANDVoltageVHigh, LoadINHigh);
//Adding Rule
FuzzyRule* fuzzyRule10 = new FuzzyRule(10, CurrentIMediumANDVoltageVHighANDLoadINHigh, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule10);
//11.Current=Medium AND Voltage=High AND LoadIN=Medium THEN Load=Low
FuzzyRuleAntecedent* CurrentIMediumANDVoltageVHighANDLoadINMedium = new FuzzyRuleAntecedent();
CurrentIMediumANDVoltageVHighANDLoadINMedium->joinWithAND(CurrentIMediumANDVoltageVHigh, LoadINMedium);
//Adding Rule
FuzzyRule* fuzzyRule11 = new FuzzyRule(11, CurrentIMediumANDVoltageVHighANDLoadINMedium, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule11);
//12.Current=Medium AND Voltage=High AND LoadIN=Low THEN Load=Medium
FuzzyRuleAntecedent* CurrentIMediumANDVoltageVHighANDLoadINLow = new FuzzyRuleAntecedent();
CurrentIMediumANDVoltageVHighANDLoadINLow->joinWithAND(CurrentIMediumANDVoltageVHigh, LoadINLow);
//Adding Rule
FuzzyRule* fuzzyRule12 = new FuzzyRule(12, CurrentIMediumANDVoltageVHighANDLoadINLow, THENLoadMedium);
fuzzy->addFuzzyRule(fuzzyRule12);
//13.Current=Medium AND Voltage=OK AND LoadIN=High THEN Load=Low
FuzzyRuleAntecedent* CurrentIMediumANDVoltageVOKANDLoadINHigh = new FuzzyRuleAntecedent();
CurrentIMediumANDVoltageVOKANDLoadINHigh->joinWithAND(CurrentIMediumANDVoltageVOK, LoadINHigh);
//Adding Rule
FuzzyRule* fuzzyRule13 = new FuzzyRule(13, CurrentIMediumANDVoltageVOKANDLoadINHigh, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule13);
//14.Current=Medium AND Voltage=OK AND LoadIN=Medium THEN Load=Medium
FuzzyRuleAntecedent* CurrentIMediumANDVoltageVOKANDLoadINMedium = new FuzzyRuleAntecedent();
CurrentIMediumANDVoltageVOKANDLoadINMedium->joinWithAND(CurrentIMediumANDVoltageVOK, LoadINMedium);
//Adding Rule
FuzzyRule* fuzzyRule14 = new FuzzyRule(14, CurrentIMediumANDVoltageVOKANDLoadINMedium, THENLoadMedium);
fuzzy->addFuzzyRule(fuzzyRule14);
//15.Current=Medium AND Voltage=OK AND LoadIN=Low THEN Load=High
FuzzyRuleAntecedent* CurrentIMediumANDVoltageVOKANDLoadINLow = new FuzzyRuleAntecedent();
CurrentIMediumANDVoltageVOKANDLoadINLow->joinWithAND(CurrentIMediumANDVoltageVOK, LoadINLow);
//Adding Rule
FuzzyRule* fuzzyRule15 = new FuzzyRule(15, CurrentIMediumANDVoltageVOKANDLoadINLow, THENLoadHigh);
fuzzy->addFuzzyRule(fuzzyRule15);
//16.Current=Medium AND Voltage=Low AND LoadIN=High THEN Load=Low
FuzzyRuleAntecedent* CurrentIMediumANDVoltageVLowANDLoadINHigh = new FuzzyRuleAntecedent();
CurrentIMediumANDVoltageVLowANDLoadINHigh->joinWithAND(CurrentIMediumANDVoltageVLow, LoadINHigh);
//Adding Rule
FuzzyRule* fuzzyRule16 = new FuzzyRule(16, CurrentIMediumANDVoltageVLowANDLoadINHigh, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule16);
//17.Current=Medium AND Voltage=Low AND LoadIN=Medium THEN Load=Low
FuzzyRuleAntecedent* CurrentIMediumANDVoltageVLowANDLoadINMedium = new FuzzyRuleAntecedent();
CurrentIMediumANDVoltageVLowANDLoadINMedium->joinWithAND(CurrentIMediumANDVoltageVLow, LoadINMedium);
//Adding Rule
FuzzyRule* fuzzyRule17 = new FuzzyRule(17, CurrentIMediumANDVoltageVLowANDLoadINMedium, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule17);
//18.Current=Medium AND Voltage=Low AND LoadIN=Low THEN Load=Low
FuzzyRuleAntecedent* CurrentIMediumANDVoltageVLowANDLoadINLow = new FuzzyRuleAntecedent();
CurrentIMediumANDVoltageVLowANDLoadINLow->joinWithAND(CurrentIMediumANDVoltageVLow, LoadINLow);
//Adding Rule
FuzzyRule* fuzzyRule18 = new FuzzyRule(18, CurrentIMediumANDVoltageVLowANDLoadINLow, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule18);
//19.Current=Low AND Voltage=High AND LoadIN=High THEN Load=Low
FuzzyRuleAntecedent* CurrentILowANDVoltageVHighANDLoadINHigh = new FuzzyRuleAntecedent();
CurrentILowANDVoltageVHighANDLoadINHigh->joinWithAND(CurrentILowANDVoltageVHigh, LoadINHigh);
//Adding Rule
FuzzyRule* fuzzyRule19 = new FuzzyRule(19, CurrentILowANDVoltageVHighANDLoadINHigh, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule19);
//20.Current=Low AND Voltage=High AND LoadIN=Medium THEN Load=Low
FuzzyRuleAntecedent* CurrentILowANDVoltageVHighANDLoadINMedium = new FuzzyRuleAntecedent();
CurrentILowANDVoltageVHighANDLoadINMedium->joinWithAND(CurrentILowANDVoltageVHigh, LoadINMedium);
//Adding Rule
FuzzyRule* fuzzyRule20 = new FuzzyRule(20, CurrentILowANDVoltageVHighANDLoadINMedium, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule20);
//21.Current=Low AND Voltage=High AND LoadIN=Low THEN Load=Medium
FuzzyRuleAntecedent* CurrentILowANDVoltageVHighANDLoadINLow = new FuzzyRuleAntecedent();
CurrentILowANDVoltageVHighANDLoadINLow->joinWithAND(CurrentILowANDVoltageVHigh, LoadINLow);
//Adding Rule
FuzzyRule* fuzzyRule21 = new FuzzyRule(21, CurrentILowANDVoltageVHighANDLoadINLow, THENLoadMedium);
fuzzy->addFuzzyRule(fuzzyRule21);
//22.Current=Low AND Voltage=OK AND LoadIN=High THEN Load=Low
FuzzyRuleAntecedent* CurrentILowANDVoltageVOKANDLoadINHigh = new FuzzyRuleAntecedent();
CurrentILowANDVoltageVOKANDLoadINHigh->joinWithAND(CurrentILowANDVoltageVOK, LoadINHigh);
//Adding Rule
FuzzyRule* fuzzyRule22 = new FuzzyRule(22, CurrentILowANDVoltageVOKANDLoadINHigh, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule22);
//23.Current=Low AND Voltage=OK AND LoadIN=Medium THEN Load=Low
FuzzyRuleAntecedent* CurrentILowANDVoltageVOKANDLoadINMedium = new FuzzyRuleAntecedent();
CurrentILowANDVoltageVOKANDLoadINMedium->joinWithAND(CurrentILowANDVoltageVOK, LoadINMedium);
//Adding Rule
FuzzyRule* fuzzyRule23 = new FuzzyRule(23, CurrentILowANDVoltageVOKANDLoadINMedium, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule23);
//24.Current=Low AND Voltage=OK AND LoadIN=Low THEN Load=Medium
FuzzyRuleAntecedent* CurrentILowANDVoltageVOKANDLoadINLow = new FuzzyRuleAntecedent();
CurrentILowANDVoltageVOKANDLoadINLow->joinWithAND(CurrentILowANDVoltageVOK, LoadINLow);
//Adding Rule
FuzzyRule* fuzzyRule24 = new FuzzyRule(24, CurrentILowANDVoltageVOKANDLoadINLow, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule24);
//25.Current=Low AND Voltage=Low AND LoadIN=High THEN Load=Low
FuzzyRuleAntecedent* CurrentILowANDVoltageVLowANDLoadINHigh = new FuzzyRuleAntecedent();
CurrentILowANDVoltageVLowANDLoadINHigh->joinWithAND(CurrentILowANDVoltageVLow, LoadINHigh);
//Adding Rule
FuzzyRule* fuzzyRule25 = new FuzzyRule(25, CurrentILowANDVoltageVLowANDLoadINHigh, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule25);
//26.Current=Low AND Voltage=Low AND LoadIN=Medium THEN Load=Low
FuzzyRuleAntecedent* CurrentILowANDVoltageVLowANDLoadINMedium = new FuzzyRuleAntecedent();
CurrentILowANDVoltageVLowANDLoadINMedium->joinWithAND(CurrentILowANDVoltageVLow, LoadINMedium);
//Adding Rule
FuzzyRule* fuzzyRule26 = new FuzzyRule(26, CurrentILowANDVoltageVLowANDLoadINMedium, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule26);
//27.Current=Low AND Voltage=Low AND LoadIN=Low THEN Load=Low
FuzzyRuleAntecedent* CurrentILowANDVoltageVLowANDLoadINLow = new FuzzyRuleAntecedent();
CurrentILowANDVoltageVLowANDLoadINLow->joinWithAND(CurrentILowANDVoltageVLow, LoadINLow);
//Adding Rule
FuzzyRule* fuzzyRule27 = new FuzzyRule(27, CurrentILowANDVoltageVLowANDLoadINLow, THENLoadLow);
fuzzy->addFuzzyRule(fuzzyRule27);  
   
Serial.println("Start time");
Serial.println(millis()-Start);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  GetCurrentP(Curr_P);
  GetLoadInfo(Curr_L, Voltage_L);
  fuzzy->setInput(1,Curr_P); //Current
  fuzzy->setInput(2,Voltage_L); //Voltage
  fuzzy->setInput(3,Curr_L); //LoadIN
  fuzzy->fuzzify();
  float LoadOut = fuzzy->defuzzify(1);
  Controller(LoadOut);
  Serial.println("Load output: ");
  Serial.println(LoadOut);
  
  Serial.print("Panel=");
  Serial.println(Curr_P);

  Serial.print("Load=");
  Serial.println(Curr_L);

  Serial.print("Voltage=");
  Serial.println(Voltage_L);

  //delay(1000);
}

//Functions
void GetLoadInfo(float &Curr_L, float &Voltage_L){
  
  unsigned long startTime = millis();
  unsigned long sampleDuration = 1000.0/60.0;
  int   sampleCount = 0;
  //RMS CURRENT
  float intg = 0;
  float curr_inst = 0.0;
  float voltage_mili_c = 0.0;
  int sensorValue_c = 0;
  
  //RMS VOLTAGE
  int voltage_max = 0;
  int sensorValue_v = 0;
 
  while ((millis()-startTime) < sampleDuration){

    //CALCULATE RMS CURRENT
    sensorValue_c = analogRead(A1);
    voltage_mili_c = sensorValue_c * (5.0 / 1023.0) * 1000.0;
    curr_inst = (voltage_mili_c - 2500.0)/100.0;
    intg = intg + pow(curr_inst,2);

    //CALCULATE RMS VOLTAGE
   sensorValue_v = analogRead(A3)*(5.0/1024.0)*1000.0;
   if (sensorValue_v>voltage_max){
      voltage_max = sensorValue_v;
   }
    
   sampleCount++;
   
  } //END WHILE
  
  Curr_L = sqrt(intg / float(sampleCount));
  Voltage_L = ((voltage_max/1000.0)*34.92)+6.51;
  
}

void GetCurrentP(float &Curr_P){
    unsigned int x=0;
    float AcsValue = 0.0;
    float Samples = 0.0;
    int nSamples = 0;
    float AvgAcs = 0.0;
    float AcsValueF = 0.0;

    for (int x=0; x<100; x++){
      AcsValue = analogRead(A0);
      Samples = Samples + AcsValue;
      delay(1);
    }
    AvgAcs = Samples/100.0;
    Curr_P = (AvgAcs*(5.0/1024.0)-2.5)/0.1;
}



void Controller(float LoadOut){
  int Table=0;
  int I1=5;
  int I2=6;
  int I3=7;
  if(LoadOut>=0 && LoadOut<=15.0){
    Table=0;
    digitalWrite(I3, LOW);
    digitalWrite(I2, LOW);
    digitalWrite(I1, LOW);
    Serial.println(Table);    
    }else if(LoadOut>15.0 && LoadOut<=30.0){
      Table=1;
      digitalWrite(I3, LOW);
      digitalWrite(I2, LOW);
      digitalWrite(I1, HIGH);
      Serial.println(Table);
      }else if(LoadOut>30.0 && LoadOut<=37.5){
        Table=2;
        digitalWrite(I3, LOW);
        digitalWrite(I2, HIGH);
        digitalWrite(I1, LOW);
        Serial.println(Table);
        }else if(LoadOut>37.5 && LoadOut<=50){
          Table=3;
          digitalWrite(I3, LOW);
          digitalWrite(I2, HIGH);
          digitalWrite(I1, HIGH);
          Serial.println(Table);
          }else if(LoadOut>50 && LoadOut<=62.5){
            Table=4;
             digitalWrite(I3, HIGH);
             digitalWrite(I2, LOW);
             digitalWrite(I1, LOW);
             Serial.println(Table);
            }else if(LoadOut>62.5 && LoadOut<=75){
              Table=5;
              digitalWrite(I3, HIGH);
              digitalWrite(I2, LOW);
              digitalWrite(I1, HIGH);
              Serial.println(Table);
              }else if(LoadOut>75 && LoadOut<=87.5){
                Table=6;
                digitalWrite(I3, HIGH);
                digitalWrite(I2, HIGH);
                digitalWrite(I1, LOW);
                Serial.println(Table);
                }else if(LoadOut>87.5){
                  Table=7;
                  digitalWrite(I3, HIGH);
                  digitalWrite(I2, HIGH);
                  digitalWrite(I1, HIGH);
                  Serial.println(Table);
                  }
}
