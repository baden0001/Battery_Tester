/*
 Programmer:  Will Baden
 Date:        10/16/14
 Name:        Current Tester
 Description: Used for testing 12V batteries for electric vehicles. 
 Batteries will maintain a 38 - 40 Amp current from fully charged
 (roughly 12.5V) to fully drained (10.5V).  By switching linear loads 
 into and out of the circuit this current will be maintianed.
 
 Inputs monitored:
 Current Sensor        Analog input 0  0-5V
 Voltage Sensor        Analog input 1  0-5V
 
 Outputs controlled:
 Power Bank 1          Pin 2
   Two 1 ohm resistors
 Power Bank 2          Pin 3
   One 1 ohm resistor
 Power Bank 3          Pin 4
   One 2 ohm resistor
 Power Bank 4          Pin 5
   One 3 Ohm resistor
 Power Bank 5          Pin 6
   One 10 Ohm resistor
 Power Bank 6          Pin 7
   One 10 Ohm resistor
 Power Bank 7          Pin 8
   One 10 Ohm resistor
 
 The voltage will be monitored to every tenth of a volt at an interval
 of one second defined by SamplePeriod (described in milliseconds).  From there, 
 the resistors will be brought into and out of circuit
 to control the current.  
 
 The data will be output the RX/TX ports to a computer that will log
 the information.  The following protocol will be used:
 
 Arduino Status, TimeStamp, Voltage, Current, Resistor Banks active
 
 On initial startup, the Arduino will await for an acknowledgement from the
 computer.  Once the Arduino receives this, it will switch over to a
 Wait for Start state.  When it receives the start command, it will run
 the battery test until 10.5 V is reached or the Arduino receives an exit
 command. 
 
 Revisions:
 
 V0.01      Initial release.
 
 */
#define CurrentSensor 0    //Current sensor input
#define VoltageSensor 1    //Voltage sensor input
#define HalfOhm 2    //Resistor banks 
#define OneOhm 3
#define TwoOhm 4
#define ThreeOhm 5
#define TenOhm1 6
#define TenOhm2 7
#define TenOhm3 8
#define SamplePeriod 1000  //Period of time between voltage checks

int TestState = 0;  //Create state machine, set to wait for key press
                    // 0 = waiting for 'Acknowledgement' to be sent from outside
                    // 1 = Send out 'Send Start to begin'
                    // 2 = waiting for 'Start' command to start test
                    // 3 = Battery test in progress
int BatteryVoltage = 0;  //Store battery voltage
int BatteryCurrent = 0;  //Store battery current
unsigned long StartTime;  //Store the initial timestamp
unsigned long CurrentTime;  //Store current battery readings timestamp
unsigned long StopTime;  //Store the final time that the test finished
String ResistorBank = "";  //Keep track of resistor banks
char buffer[15];  //buffer to store data from serial port, array size of 15

void setup() {
  // initialize serial communication:
  Serial.begin(38400);
  Serial.setTimeout(100);  //only allow 100 mS between character input
  
  //intialize resistor bank control outputs
  pinMode(HalfOhm,OUTPUT);
  pinMode(OneOhm,OUTPUT);
  pinMode(TwoOhm,OUTPUT);
  pinMode(ThreeOhm,OUTPUT);
  pinMode(TenOhm1,OUTPUT);
  pinMode(TenOhm2,OUTPUT);
  pinMode(TenOhm3,OUTPUT);
  
  TestState = 0;
}

void loop() {
  
  switch (TestState) {
  case 0:  //Waiting for computer acknowledge connection
    Serial.println("System Idle");
    delay(5000); //delay five seconds for response from computer
    if (Serial.available())
    {
      Serial.readBytes(buffer,11);
      if (buffer[0] == 'A' && buffer[1] == 'c' && buffer[2] == 'k' && buffer[3] == 'n' && buffer[4] == 'o' && buffer[5] == 'w' && buffer [6] == 'l' && buffer[7] == 'e' && buffer[8] == 'd' && buffer[9] == 'g' && buffer[10] == 'e')
      {
        TestState = 1;  //wait for start command
      }
      else
      {
        Serial.println("Need to Acknowledge first");
      }
    }
    break;
  
  case 1:  //Wait for Start command from computer
  
    Serial.println("Send Start to begin");
    TestState = 2;
    break; 
  
  case 2:
    if (Serial.available())
    {
      Serial.readBytes(buffer,15);
      if (buffer[0] == 'S' && buffer[1] == 't' && buffer[2] == 'a' && buffer[3] == 'r' && buffer[4] == 't')
      {
        TestState = 3; 
        StartTime = millis();
        Serial.println("Beginning," + String(StartTime));
      } 
      //Following allows for computer to ask what the status of the Arduino is
      else if (buffer[0] == 'S' && buffer[1] == 't' && buffer[2] == 'a' && buffer[3] == 't' && buffer[4] == 'u' && buffer[5] == 's')
      {
        Serial.println("Send Start to begin");
      }
      else //Catch failed commands sent with this one
      {
        Serial.println("Unknown Command");
      }  
    }  
    break;
    
  case 3:  //Running the test.  Check out the voltage and current sensors
           // output correct resistance loads. 
           
    ReadBatteryStatus();
    if (Serial.available())
    {
     Serial.readBytes(buffer,4);
     //During program run time, the test can be stopped by sending 'Exit'
     if (buffer[0] == 'E' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't')
      {
       StopTime = millis(); 
       SendEndStatus();
       Serial.println("Test Stopped");
       TestState = 1; 
       break;
      }
    }  
    if (BatteryVoltage <= 105)
    {
     StopTime = millis();     
     digitalWrite(HalfOhm, LOW); //Shut off all resistor banks
     digitalWrite(OneOhm, LOW);
     digitalWrite(TwoOhm, LOW);
     digitalWrite(ThreeOhm, LOW);
     digitalWrite(TenOhm1, LOW);
     digitalWrite(TenOhm2, LOW);
     digitalWrite(TenOhm3, LOW);
     ResistorBank = "0000000";    
     SendEndStatus();
     TestState = 1; //Test is now complete, stop running program and wait for restart
    }
    else if (BatteryVoltage >= 106 && BatteryVoltage <= 108)
    {
     digitalWrite(HalfOhm, HIGH);
     digitalWrite(OneOhm, HIGH);
     digitalWrite(TwoOhm, HIGH);
     digitalWrite(ThreeOhm, LOW);
     digitalWrite(TenOhm1, LOW);
     digitalWrite(TenOhm2, HIGH);
     digitalWrite(TenOhm3, HIGH);
     ResistorBank = "1110011";
     //delay(500);  //Delay half a second for voltage/current to stabilize
     //ReadBatteryStatus();
     SendStatus();
    }
    else if (BatteryVoltage >= 109 && BatteryVoltage <= 111)
    {
     digitalWrite(HalfOhm, HIGH);
     digitalWrite(OneOhm, HIGH);
     digitalWrite(TwoOhm, HIGH);
     digitalWrite(ThreeOhm, LOW);
     digitalWrite(TenOhm1, LOW);
     digitalWrite(TenOhm2, LOW);
     digitalWrite(TenOhm3, HIGH);
     ResistorBank = "1110001";
     //delay(500);  //Delay half a second for voltage/current to stabilize
     //ReadBatteryStatus();
     SendStatus();
    }
    else if (BatteryVoltage >= 112 && BatteryVoltage <= 113)
    {
     digitalWrite(HalfOhm, HIGH);
     digitalWrite(OneOhm, HIGH);
     digitalWrite(TwoOhm, LOW);
     digitalWrite(ThreeOhm, HIGH);
     digitalWrite(TenOhm1, LOW);
     digitalWrite(TenOhm2, HIGH);
     digitalWrite(TenOhm3, HIGH);
     ResistorBank = "1101011";
     //delay(500);  //Delay half a second for voltage/current to stabilize
     //ReadBatteryStatus();
     SendStatus();
    }
    else if (BatteryVoltage >= 114 && BatteryVoltage <= 116)
    {
     digitalWrite(HalfOhm, HIGH);
     digitalWrite(OneOhm, HIGH);
     digitalWrite(TwoOhm, LOW);
     digitalWrite(ThreeOhm, HIGH);
     digitalWrite(TenOhm1, LOW);
     digitalWrite(TenOhm2, LOW);
     digitalWrite(TenOhm3, HIGH);
     ResistorBank = "1101001";
     //delay(500);  //Delay half a second for voltage/current to stabilize
     //ReadBatteryStatus();
     SendStatus();
    }
    else if (BatteryVoltage >= 117 && BatteryVoltage <= 119)
    {
     digitalWrite(HalfOhm, HIGH);
     digitalWrite(OneOhm, HIGH);
     digitalWrite(TwoOhm, LOW);
     digitalWrite(ThreeOhm, HIGH);
     digitalWrite(TenOhm1, LOW);
     digitalWrite(TenOhm2, LOW);
     digitalWrite(TenOhm3, LOW);
     ResistorBank = "1101000";
     //delay(500);  //Delay half a second for voltage/current to stabilize
     //ReadBatteryStatus();
     SendStatus();
    }
    else if (BatteryVoltage >= 121 && BatteryVoltage <= 125)
    {
     digitalWrite(HalfOhm, HIGH);
     digitalWrite(OneOhm, HIGH);
     digitalWrite(TwoOhm, LOW);
     digitalWrite(ThreeOhm, LOW);
     digitalWrite(TenOhm1, LOW);
     digitalWrite(TenOhm2, HIGH);
     digitalWrite(TenOhm3, HIGH);
     ResistorBank = "1100011";
     //delay(500);  //Delay half a second for voltage/current to stabilize
     //ReadBatteryStatus();
     SendStatus();
    }
    break;           
  } 
  delay(SamplePeriod);        // Delay for sampleperiod
}

void SendStatus() {
  Serial.print("Program Run,");
  Serial.print(String(CurrentTime));
  Serial.print(",");
  Serial.print(String(BatteryVoltage));
  Serial.print(",");
  Serial.print(String(BatteryCurrent));
  Serial.print(",");
  Serial.println(ResistorBank);
}  

void SendEndStatus() {
  Serial.print("Program End,"); 
  Serial.print(String(StopTime));
  Serial.print(",");
  Serial.print(String(BatteryVoltage));
  Serial.print(",");
  Serial.print(String(BatteryCurrent));
  Serial.print(",");
  Serial.println(ResistorBank);
}  

void ReadBatteryStatus() {
  CurrentTime = millis();
  //Read battery voltage.  Need to rescale this according to actual sensor used
  BatteryVoltage = analogRead(VoltageSensor);
  BatteryVoltage = map(BatteryVoltage, 0, 1023, 90, 150);
    
  //Read in Current.  Need to rescale this according to actual sensor used
  BatteryCurrent = analogRead(CurrentSensor);
  BatteryCurrent = map(BatteryCurrent, 0, 1023, 0, 50);  
}





