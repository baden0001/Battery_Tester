/*
 Programmer:  Will Baden
 Date:        10/16/14
 Name:        Current Tester
 Description: Used for testing 12V batteries for electric vehicles. 
 Batteries will maintain a 38 - 40 Amp current from fully charged
 (roughly 12.5V) to fully drained (10.5V).  By switching linear loads 
 into and out of the circuit this current will be maintianed.
 
 2.22V = 12.4V Battery
 454.212 out of 1023
 bits/volt
 454.212/12.4V = 36.63 bits/Volt
 15V*36.63 = 549.45 bit count
 
 Resistors used
 10k
 2k
 Using the following scaling works from 10.57 - 12.4V
 BatteryVoltage = map(BatteryVoltage, 0, 1023, 0, 2725);
 
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
 V0.02      Fixed SamplePeriod bug.  Switched to variable instead of a define.
            Added Over Current Shut down.  This will shut the test down,
             turn off all Resistor outputs and send "Battery Current High"
             to computer
 V0.03      Changed the load controlling calculations, instead
             of preset voltage ranges.  This will be calculated
             knowing the actual voltage and loads applied.
             The resistor banks resistance will need to be known
             per bank and should be placed with the smallest ohmic
             reading in bank 1 and increasing from there.
            Example:
             CalculatedCurrentBank1 = Voltage Reading / First resistor bank
             CurrentDesired = CurrentDesired (40 Amps) - CalculatedCurrentBank1
             Bank1 turn on
             CalculatedCurrentBank2 = Voltage Reading / Second resistor
             If CurrentDesired > CalculatedCurrentBank2 then
               CurrentDesired = CurrentDesired - CalculatedCurrentBank2
               Bank2 Turn on
             Endif
             (continue this throughout the rest of the resistor banks) 
            The Resistor Bank notification has been disabled until it can be 
             reimplemented with the new style of bank control
            Using the current setup, the voltage may have to be adjusted further.  It
             appears to shift higher when the load test is in operation by about .15V
 */
#define CurrentSensor 0    //Current sensor input
#define VoltageSensor 1    //Voltage sensor input
#define Bank1 2    //Resistor banks 
#define Bank2 3
#define Bank3 4
#define Bank4 5
#define Bank5 6
#define Bank6 7
#define Bank7 8

int TestState = 0;  //Create state machine, set to wait for key press
                    // 0 = waiting for 'Acknowledgement' to be sent from outside
                    // 1 = Send out 'Send Start to begin'
                    // 2 = waiting for 'Start' command to start test
                    // 3 = Battery test in progress
long BatteryVoltage = 1250;  //Store battery voltage (1250/100 = 12.5V)
long BatteryCurrent = 0;  //Store battery current (500/10 = 50 Amps)
unsigned long StartTime;  //Store the initial timestamp
unsigned long CurrentTime;  //Store current battery readings timestamp
unsigned long StopTime;  //Store the final time that the test finished
String strResistorBank = "";  //Keep track of resistor banks
char buffer[15];  //buffer to store data from serial port, array size of 15
int SamplePeriod = 5000;  //Period of time between voltage checks
long ResistorBank[8];  //Array to contain the 7 different resistor banks values
                      // (355/1000 = .355)
long CalculatedCurrent = 0;  //General purpose calculation variable for the
                            // Resistor banks (CalculatedCurrent = Voltage/Resistance)
long SummedCurrent = 0;  //Contains the current calculations (SummedCurrent = SummedCurrent - CalculatedCurrent)
long DesiredCurrent = 400;  //Store Desired test current here (400/10 = 40.0 amps)

void setup() {
  // initialize serial communication:
  Serial.begin(38400);
  Serial.setTimeout(100);  //only allow 100 mS between character input
  
  //intialize resistor bank control outputs
  pinMode(Bank1,OUTPUT);
  pinMode(Bank2,OUTPUT);
  pinMode(Bank3,OUTPUT);
  pinMode(Bank4,OUTPUT);
  pinMode(Bank5,OUTPUT);
  pinMode(Bank6,OUTPUT);
  pinMode(Bank7,OUTPUT);
  
  ResistorBank[1] = 355;  //Holds resistance value to third decimal (355/1000 = .355 ohms)
  ResistorBank[2] = 807;  
  ResistorBank[3] = 1527;
  ResistorBank[4] = 2396;
  ResistorBank[5] = 8093;
  ResistorBank[6] = 8100;
  ResistorBank[7] = 8670;
  
  TestState = 0;
}

void loop() {
  
  switch (TestState) {
  case 0:  //Waiting for computer acknowledge connection
    Serial.println("System Idle");
    //delay(1000); //delay second for response from computer
    //Wait for response from computer
    while(Serial.available() == 0);
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
       digitalWrite(Bank1, LOW); //Shut off all resistor banks
       digitalWrite(Bank2, LOW);
       digitalWrite(Bank3, LOW);
       digitalWrite(Bank4, LOW);
       digitalWrite(Bank5, LOW);
       digitalWrite(Bank6, LOW);
       digitalWrite(Bank7, LOW);
       strResistorBank = "0000000";
       Serial.println("Test Stopped");
       TestState = 1; 
       break;
      }
      else
      {
        Serial.println("Test Running");  //Try to catch bad commands when running
      }
    }
    if (BatteryVoltage <= 1090)
    {
      StopTime = millis();     
      digitalWrite(Bank1, LOW); //Shut off all resistor banks
      digitalWrite(Bank2, LOW);
      digitalWrite(Bank3, LOW);
      digitalWrite(Bank4, LOW);
      digitalWrite(Bank5, LOW);
      digitalWrite(Bank6, LOW);
      digitalWrite(Bank7, LOW);
      strResistorBank = "0000000";    
      SendEndStatus();
      TestState = 1; //Test is now complete, stop running program and wait for restart
    }
    else  //Voltage is within test range, calculate what banks need to be on
    {
      //Current reading = 400/10 = 40.0 Amps
      //Ohm Value       = 355/1000 = .355 ohms
      //Voltage         = 1250/100 = 12.50 Volts
      //SummedCurrent = 400
      SummedCurrent = DesiredCurrent;  //Load desired current.      
      for (int Bank = 1; Bank < 8; Bank++) {
        //CalculatedCurrent = 1250/355 * 100 = 352.1 = 352        
        CalculatedCurrent = BatteryVoltage * 10 ;
        CalculatedCurrent = CalculatedCurrent / ResistorBank[Bank];
        CalculatedCurrent = CalculatedCurrent * 10;
        int BankReAddress = Bank + 1;
        if (SummedCurrent > CalculatedCurrent)
        {
          SummedCurrent = SummedCurrent - CalculatedCurrent;
          digitalWrite(BankReAddress, HIGH);          
        }
        else
        {
          digitalWrite(BankReAddress, LOW);          
        }        
      }
      //Will need to add code to allow for Resistor Bank status being sent
      SendStatus();
    }
  
    delay(SamplePeriod);  //Delay for sampleperiod
    break;           
  } 
}

void SendStatus() {
  Serial.print("Program Run,");
  Serial.print(String(CurrentTime));
  Serial.print(",");
  Serial.print(String(BatteryVoltage));
  Serial.print(",");
  Serial.print(String(BatteryCurrent));
  Serial.print(",");
  Serial.println(strResistorBank);
}  

void SendEndStatus() {
  Serial.print("Program End,"); 
  Serial.print(String(StopTime));
  Serial.print(",");
  Serial.print(String(BatteryVoltage));
  Serial.print(",");
  Serial.print(String(BatteryCurrent));
  Serial.print(",");
  Serial.println(strResistorBank);
}  

void ReadBatteryStatus() {
  CurrentTime = millis();
  //Read battery voltage.  Need to rescale this according to actual sensor used
  //This scaling appears to be ok except when the test is under a load
  //  The voltage appears to stay higher by another .15V
  BatteryVoltage = analogRead(VoltageSensor);
  BatteryVoltage = map(BatteryVoltage, 0, 1023, 0, 2725);
    
  //Read in Current.  Need to rescale this according to actual sensor used
  //Up to 6.5 Amps the current sensed on the RobotShop was consistent
  //  with an ammeter
  BatteryCurrent = analogRead(CurrentSensor);
  BatteryCurrent = map(BatteryCurrent, 101, 922, -500, 500); //Full scale remapping(0,1023,-625,625)
                                                             //Sensor full scale remapping(101, 922, -500, 500)  
}





