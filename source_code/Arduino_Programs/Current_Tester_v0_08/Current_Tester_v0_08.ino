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
 of five seconds defined by SamplePeriod (described in milliseconds).  From there, 
 the resistors will be brought into and out of circuit
 to control the current.  
 
 The data will be output the RX/TX ports to a computer that will log
 the information.  The following protocol will be used:
 
 Arduino Status, TimeStamp, Voltage, Current, Resistor Banks active, ActualVoltage (mV)
 
 On initial startup, the Arduino will await for an acknowledgement from the
 computer.  Once the Arduino receives this, it will switch over to a
 Wait for Start state.  When it receives the start command, it will run
 the battery test until 10.5 V is reached or the Arduino receives an exit
 command. 
 
 System Setup:
 Arduino powers up
 Arduino prints: System Idle
   Expected Response: Acknowledge
   Failed Response: Need to Acknowledge First
   Successful Response: Send Start to Begin
 Arduino can then receive in commands:
   Command: Start
     Starts the battery test
     Command: Exit
       Stop battery test
       Response: Test Stopped
   Command: Status
     Arduino prints: Send Start to Begin
   Command: Ohms
     Arduino prints the set ohm value of each resistor bank
   Command: Offset
     Response: Set or Read?
     User: Set,<voltage offset>,<amp offset>
       Values are then set into 
     
       OR:
       
     User: Read
     Response: Read,<voltage offset>,<amp offset> 

     Response: Send Start to Begin
   Failed Command response: Unknown Command
 
 
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
            Over Current Shutdown was Removed.
 V0.04      Added ResistorBank string identification for use in serial string
            Added Command that would send the resistance values of the Banks
             This can be accessed by sending an "Ohms" command after there
             has been an Acknowledgement
 V0.05      Added deadband
            Variables added:
             DeadBand
             LowerCurrent
             UpperCurrent
 V0.06      Added secretvoltmeter as described at:
             https://code.google.com/p/tinkerit/wiki/SecretVoltmeter
             by Tinkerit
            This will allow the program to use the 1.1V reference to backtrack what
             the actual input voltage is.  This should be ran before each check of
             the analog input.
            Added ActualVoltage to serial string of data from Arduino.
            Remeasured Resistance with simple current/voltage test 
             with Resistor_Bank_Testing program.  This showed different
             values for the bank resistances.  Reflected below.  Also
             noticed that the current on the RobotShop ammeter is no 
             longer the same as the ammeter used.  Differential is 1.3Amps below 7 amps.
             DONE Need to compare upper current differential with capable meter.
             Compared with inline shunt meter, result:
               RobotShop Ammeter = 36.9 Amps
               Shunt Ammeter     = 42.1 Amps
             This is a 6 amp difference.  
             Need to add offset calibration for robot shop ammeter.
 V0.07       ADDED offset for voltage and current.  This value can be set via the
              serial interface.
               Current offset will correct the current by adding/subtracting an offset
               Voltage offset will correct the voltage by adding/subtracting an offset
             Current and voltage offset can be adjusted from the PC.  While the Arduino
               is waiting for a 'Start' command, send a 'Offset' command instead. 
                   Command: Offset
                   Response: Set or Read?
                   User: Set,<voltage offset>,<amp offset>
                     Values are then set and saved into EEPROM
     
                     OR:
       
                   User: Read
                   Response: Read,<voltage offset>,<amp offset> 

                   Response: Send Start to Begin
             Current offset and voltage offset are read from EEPROM on startup.
             ADDED end of command character recognition.  Prior to parsing the
               command, verify that the command has been completely input into Arduino.
               Arduino will look for CR (carriage return)
             MODIFIED Visual Basic program to send Carriage Return to Arduino.
             ADDED end of response character to send back to PC program.  This is done already
               with println.  
             Visual Basic program still does not look for Carriage Return.
             Modified Serial port read function to happen once per time around the loop.  
               If there is data available, then it will read in data until the CR 
               character is seen or the timeout has processed.  Then the rest of the
               program will run. 
             Requires EEPROMex.h header file to allow for long integer saving
               to the EEPROM
             Fluctuating Voltage and Current sense were still seen.  Along with a fluctuating
               Arduino 5V voltage.  This caused poor consistency with the external volt meter and
               amp meter.  To rectify this problem, changing the relay board power to a different
               5V power supply cause more consistent readings with the external volt meter and amp
               meter. 
 V0.08       Changed the voltage calculations to double data type
              Purpose was to give more consistant amp and volt
              measurements using the offsets 
            
 */
 
#include <stdlib.h>  //strtol
#include <EEPROMex.h>  //long read and write
 
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
                    // 4 = Waiting for response from PC to switch between Read and Set.  Once decided
                    //      Send offset value for Read
                    //      Set offset value from PC
long BatteryVoltage = 1250;  //Store battery voltage (1250/100 = 12.5V)
long BatteryCurrent = 0;  //Store battery current (500/10 = 50 Amps)
unsigned long StartTime;  //Store the initial timestamp
unsigned long CurrentTime;  //Store current battery readings timestamp
unsigned long StopTime;  //Store the final time that the test finished
String strResistorBank = "";  //Keep track of resistor banks
char buffer[16];  //buffer to store data from serial port, array size of 16
int SamplePeriod = 5000;  //Period of time between voltage checks
long ResistorBank[8];  //Array to contain the 7 different resistor banks values
                      // (355/1000 = .355)
long CalculatedCurrent = 0;  //General purpose calculation variable for the
                            // Resistor banks (CalculatedCurrent = Voltage/Resistance)
long SummedCurrent = 0;  //Contains the current calculations (SummedCurrent = SummedCurrent - CalculatedCurrent)
long DesiredCurrent = 390;  //Store Desired test current here (400/10 = 40.0 amps)
long DeadBand = 10;  //Dead band width that will be centered up on the
                     // DesiredCurrent
long LowerCurrent;  //Lower limit of possible current, use DesiredCurrent and Deadband to 
                    // calculate
long UpperCurrent;  //Upper limit of possible current, use DesiredCurrent and Deadband to 
                    // calculate
double ActualVoltage;  //This is the actual supply voltage when measured against
                    // the 1.1V reference
long AmpOffset;  //Current offset, set by user
long VoltageOffset;  //Voltage offset, set by user
byte ArraySize;  //Size of the array that was received from the serial port
boolean SystemIdleSent;  //Allow "System Idle" to be sent only once
int EEPROMVoltageOffsetAddress;  //Address of EEPROM for Voltage Offset
int EEPROMAmpOffsetAddress;  //Address of EEPROM for Amp Offset

void setup() {
  // initialize serial communication:
  Serial.begin(38400);
  Serial.setTimeout(100);  //only allow 100 mS between character input
  
  //Calculate upper and lower current limits
  LowerCurrent = DesiredCurrent - DeadBand/2;
  UpperCurrent = DesiredCurrent + DeadBand/2;
  
  //Map out EEPROM addresses
  EEPROMVoltageOffsetAddress = EEPROM.getAddress(sizeof(long));
  EEPROMAmpOffsetAddress = EEPROM.getAddress(sizeof(long));
  
  /* //Save voltage offset to EEPROM to be used first time the eeprom was used
  EEPROM.writeLong(EEPROMVoltageOffsetAddress, 0);
  //Save amp offset to EEPROM to be used first time the eeprom was used
  EEPROM.writeLong(EEPROMVoltageOffsetAddress, 0);
  */
  
  VoltageOffset = EEPROM.readLong(EEPROMVoltageOffsetAddress);
  AmpOffset = EEPROM.readLong(EEPROMAmpOffsetAddress);
  
  SystemIdleSent = false;
  
  //intialize resistor bank control outputs
  pinMode(Bank1,OUTPUT);
  pinMode(Bank2,OUTPUT);
  pinMode(Bank3,OUTPUT);
  pinMode(Bank4,OUTPUT);
  pinMode(Bank5,OUTPUT);
  pinMode(Bank6,OUTPUT);
  pinMode(Bank7,OUTPUT);
  
  //11/10/14 test results for the ohm readings
  //ResistorBank[1] = 355;  //Holds resistance value to third decimal (355/1000 = .355 ohms)
  //ResistorBank[2] = 807;  
  //ResistorBank[3] = 1527;
  //ResistorBank[4] = 2396;
  //ResistorBank[5] = 8093;
  //ResistorBank[6] = 8100;
  //ResistorBank[7] = 8670;
  
  //11/20/14 test results for the ohm readings
  ResistorBank[1] = 368;  //Holds resistance value to third decimal (355/1000 = .355 ohms)
  ResistorBank[2] = 906;  
  ResistorBank[3] = 1790;
  ResistorBank[4] = 2685;
  ResistorBank[5] = 10130;
  ResistorBank[6] = 10130;
  ResistorBank[7] = 10690;
    
  TestState = 0;
}

void loop() {
  
  //Read in Serial Data once per cycle through the program.  Use the buffer through out
  //  the rest of the program.
  
  //The end of string character within the array will be '\0' or null.  Null out
  //  the first character in the array, showing that the data has been processed.
  buffer[0] = '\0';
    
  if (Serial.available())
  {
    //Read in bytes to an array until the Carriage Return is seen.
    ArraySize = Serial.readBytesUntil('\r',buffer,16);
    buffer[ArraySize]='\0';  // terminate string with NULL
  }
  
  switch (TestState) {
  case 0:  //Waiting for computer to acknowledge connection
    if (SystemIdleSent = false)
    {
      Serial.println("System Idle"); 
      SystemIdleSent = true;     
    }  
    
    if (buffer[0] == 'A' && buffer[1] == 'c' && buffer[2] == 'k' && buffer[3] == 'n' && buffer[4] == 'o' && buffer[5] == 'w' && buffer [6] == 'l' && buffer[7] == 'e' && buffer[8] == 'd' && buffer[9] == 'g' && buffer[10] == 'e')
    {
      TestState = 1;  //wait for start command
    }
    else if (buffer[0] != '\0')  //check if there is data in the serial buffer
    {
      Serial.println("Need to Acknowledge first");
    }
    
    break;
  
  case 1:  //Wait for Start command from computer
  
    Serial.println("Send Start to begin");
    TestState = 2;
    //Serial.println(String(LowerCurrent));
    //Serial.println(String(UpperCurrent));
    break;     
  
  case 2:

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
    //Following will send the values for the resistance values used in calculations.
    else if (buffer[0] == 'O' && buffer[1] == 'h' && buffer[2] == 'm' && buffer[3] == 's')
    {
      for (int Value = 1; Value < 8; Value++) {
        Serial.println("Resistance Bank " + String(Value) + " = " + String(ResistorBank[Value]));          
      }
    }
    else if (buffer[0] == 'O' && buffer[1] == 'f' && buffer[2] == 'f' && buffer[3] == 's' && buffer[4] == 'e' && buffer[5] == 't')
    {
      TestState = 4;
      Serial.println("Set or Read?");
    }
    else if (buffer[0] == 'E' && buffer[1] == 'x' && buffer[2] == 'i' && buffer[3] == 't')
    {
      Serial.println("Battery Test not Active");
      Serial.println("Send Start to begin");    
    }  
    else if (buffer[0] != '\0') //Catch failed commands
    {
      Serial.println("Unknown Command");
    }   
    break;
    
  case 3:  //Running the test.  Check out the voltage and current sensors
           // output correct resistance loads. 
           
    ReadBatteryStatus();
    
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
    else if (buffer[0] != '\0') 
    {
      Serial.println("Test Running");  //Try to catch bad commands when running
    }
    
    if (BatteryVoltage <= 1050)  //If battery voltage is below
                                 // 10.5V, stop the test.
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
      //If the current is outside of the current range then recalculate
      // Resistor banks
      
      //Serial.println(String(LowerCurrent));
      //Serial.println(String(UpperCurrent));
      if (BatteryCurrent > UpperCurrent || BatteryCurrent < LowerCurrent)
      {        
        //Current reading = 400/10 = 40.0 Amps
        //Ohm Value       = 355/1000 = .355 ohms
        //Voltage         = 1250/100 = 12.50 Volts
        //SummedCurrent = 400
        SummedCurrent = DesiredCurrent;  //Load desired current. 
        strResistorBank = "";  //Clear out Resistor Bank status 
        //Cycle through calculated resistances and decide if load needs to be
        // turned on.   
        for (int Bank = 1; Bank < 8; Bank++) {
          //CalculatedCurrent = 1250/355 * 10 = 35.21 = 35        
          CalculatedCurrent = BatteryVoltage * 10 ;
          CalculatedCurrent = CalculatedCurrent / ResistorBank[Bank];
          CalculatedCurrent = CalculatedCurrent * 10;
          int BankReAddress = Bank + 1;
          if (SummedCurrent > CalculatedCurrent)
          {
            SummedCurrent = SummedCurrent - CalculatedCurrent;
            digitalWrite(BankReAddress, HIGH); 
            strResistorBank = "1" + strResistorBank;         
          }
          else
          {
            digitalWrite(BankReAddress, LOW); 
            strResistorBank = "0" + strResistorBank;       
          }        
        }
      }
      SendStatus();
    }
  
    delay(SamplePeriod);  //Delay for sampleperiod
    break;           
  
  case 4:  
    //Test whether the PC program is setting or reading.
    if (buffer[0] == 'S' && buffer[1] == 'e' && buffer[2] == 't' && buffer[3] == ',')
    {
      int conversionPointer = 0;
      char integerConversion[11];     
      char * pEnd;
      
      TestState = 1;
      
      for (int i=4; i<=15; i++){
        if (buffer[i] != '\0' && buffer[i] != ',' && i != 15)
        {
          integerConversion[conversionPointer] = buffer[i];
        }
        else if (buffer[i] != '\0' && i == 15)
        {
          //If there is no null detected throughout the buffer, then place one
          integerConversion[conversionPointer] = '\0';
        }  
        else if (buffer[i] == '\0')
        {
          integerConversion[conversionPointer] = '\0';
          break;
        }
        else if (buffer[i] == ',')
        {
          integerConversion[conversionPointer] = ' ';
        }  
        conversionPointer += 1;
      }
      
      //Pull integers from string ignoring whitespace characters.
      VoltageOffset = strtol(integerConversion, &pEnd, 10);     
      AmpOffset = strtol(pEnd, NULL, 10);
      
      /*
      Serial.println(String(AmpOffset));
      Serial.println(String(VoltageOffset));
      */
      
      //Save offsets to EEPROM to be used after a power cycle
      EEPROM.writeLong(EEPROMVoltageOffsetAddress, VoltageOffset);  
      EEPROM.writeLong(EEPROMAmpOffsetAddress, AmpOffset);   
      
    }  
    else if (buffer[0] == 'R' && buffer[1] == 'e' && buffer[2] == 'a' && buffer[3] == 'd')
    {
      TestState = 1;
      Serial.println("Read," + String(VoltageOffset) + "," + String(AmpOffset));
    }
    else if (buffer[0] != '\0')
    {
      Serial.println("Set or Read?");      
    }  
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
  Serial.print(strResistorBank);
  Serial.print(",");
  Serial.println(String(ActualVoltage));
}  

void SendEndStatus() {
  Serial.print("Program End,"); 
  Serial.print(String(StopTime));
  Serial.print(",");
  Serial.print(String(BatteryVoltage));
  Serial.print(",");
  Serial.print(String(BatteryCurrent));
  Serial.print(",");
  Serial.print(strResistorBank);
  Serial.print(",");
  Serial.println(String(ActualVoltage));
} 

void ReadBatteryStatus() {
  //Calculate the shift that can be applied to the current and voltage 
  // analog inputs
  // 1023 bits in entire range of analog input
  // 5V/1023 = .00489 volts/bit
  // 5.15V/1023 = .00503 volts/bit
  // 4.96V/1023 = .00485 volts/bit
  // 4.85V/1023 = .00474 volts/bit
  // Use double data type to do calculations and then convert to integer to be used by
  //  main program
  double tempCalculate ;
  double tempAnalogValue ;
  
  //Save actual Vcc read when compared to on board 1.1V reference
  ActualVoltage = (double)readVcc() / 1000;
    
  CurrentTime = millis();
  //Read battery voltage.  Need to rescale this according to actual sensor used
  tempAnalogValue = analogRead(VoltageSensor);
  
  //calculate the voltage from the pin
  tempCalculate = (tempAnalogValue / 1023) * ActualVoltage;
  
  //calculate the voltage of the 12V source through the voltage divider
  // and prepare to convert back to integer (*100)
  tempCalculate = tempCalculate * 27.25 / 5 * 100;
  
  BatteryVoltage = (int)tempCalculate;
  
  //BatteryVoltage = map(BatteryVoltage, 0, 1023, 0, ActualVoltage);
  //remap 0-5V range to calculated voltage divider range.
  //BatteryVoltage = map(BatteryVoltage, 0, 5000, 0, 2725);
   
  //Read in Current.  Need to rescale this according to actual sensor used
  //Up to 6.5 Amps the current sensed on the RobotShop was consistent
  //  with an ammeter (test 11/10/14. Testing on 11/20/14 showed a differential
  //  of 1.5 amps @ 6.5 amps and lower.  Did not measure higher amperage differentials)
  tempAnalogValue = analogRead(CurrentSensor);
  
  //calculate the voltage from the pin
  tempCalculate = (tempAnalogValue / 1023) * ActualVoltage;
  
  //calculate amperage from 40mV/A with a 2.5V center
  // and prepare to convert back to integer (/10)
  tempCalculate = (tempCalculate - 2.5) / .04 * 10;
  
  BatteryCurrent = (int)tempCalculate;
  //BatteryCurrent = analogRead(CurrentSensor);
  //remap to have the actual voltage of the analog input
  //BatteryCurrent = map(BatteryCurrent, 0, 1023, 0, ActualVoltage);
  //remap to current values
  // 2.5V + 50*.04 = 4.5V
  // 2.5V - 50*.04 = .5V
  //BatteryCurrent = map(BatteryCurrent, 500, 4500, -500, 500); //Full scale remapping(0,1023,-625,625)
                                                             //Sensor full scale remapping(101, 922, -500, 500)  

  //Offset current and voltage value according to user definition
  BatteryVoltage = BatteryVoltage + VoltageOffset;
  BatteryCurrent = BatteryCurrent + AmpOffset;

}

//This returns the calculated Vcc voltage that is supplied to the UNO
// and should not affect the reference when a pin is checked
// via analogRead() due to the Arduino software writing the 
// requested reference (default in this case).
// Following Code from Tinkerit:
long readVcc() {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}
// End Tinkerit code





