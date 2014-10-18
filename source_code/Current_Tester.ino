/*
 Programmer:  Will Baden
 Date:        10/16/14
 Name:        Current Tester
 Description: Used for testing 12V batteries for electric vehicles. 
 Batteries will maintain a 38 - 40 Amp current from fully charged
 (roughly 13V) to fully drained (10.5V).  By switching linear loads 
 into and out of the circuit this current will be maintianed.
 
 Inputs monitored:
 Current Sensor        Analog input 0
 Votlage Sensor        Analog input 1
 
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
 
 Power Calculations (these are not current vales used):
 12.5V:
  12.5Amp x 3 = 37.5 Amps = 1 ohm
  1.25 x 2 = 2.5 = 10ohm
  40 Amps
 
 12V:
  12Amp x 3 = 36 Amps = 1 ohm
  4  = 3ohm
  40 Amps
  
 11.5V:
  11.5Amp x 3 = 34.5 Amps = 1 ohm
  3.83  = 3ohm
  1.15  = 10 ohm
  39.48 Amps
  
 11V:
  11Amp x 3 = 33 Amps = 1 ohm
  5.5 = 2 ohm
  1.1 = 10 ohm
  39.6 Amps
  
 10.5V:
  10.5Amp x 3 = 31.5 Amps= 1 ohm
  5.25 = 2 ohm
  1.05Amp x 3 = 3.15 = 10 ohm
  39.9 Amps
 
 The voltage will be monitored to every tenth of a volt at an interval
 of one second defined by SamplePeriod (described in milliseconds).  There
 will be an additional half second delay to allow for current/voltage settling. 
 From there, the resistors will be brought into and out of circuit
 to control the current.  There should be a monitor for the current
 to prevent any drifting from affecting the experiment.
 
 The data will be output the RX/TX ports to a computer that will log
 the information.  The following protocol will be used:
 
 TimeStamp, Voltage, Current, Resistor Banks active
 
 On initial startup, the program will request that any button be pressed.
 Once a button is pushed on the computer, then the testing will begin.
 
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
#define SamplePeriod 500  //Period of time between voltage checks

int TestState;  //Create state machine, set to wait for key press
                    // 0 = waiting for key press
                    // 1 = Test Running
unsigned long StartTime;  //Store the initial timestamp
unsigned long StopTime;  //Store the final time that the test finished
String ResistorBank = "";  //Keep track of resistor banks

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  
  //intialize resistor bank contorl outputs
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
  //Read in battery voltage, remap to useable voltage values
  //Voltage values will be be multiplied by 10 to view the
  // tenth of a volt.
  int BatteryVoltage = analogRead(VoltageSensor);
  BatteryVoltage = map(BatteryVoltage, 0, 1023, 0, 150);
  
  switch (TestState) {
  case 0:  //Wait for Keypress to start up test procedure
  
    Serial.println("Press any key to begin");
    //Wait for any key to be pressed inside the setup code
    while(!Serial.available());
    StartTime = millis();
    TestState = 1;
    Serial.println("Testing Started at:  " + StartTime);
    break; 
    
  case 1:  //Running the test.  Check out the voltage and current sensors
           // output correct resistance loads. 
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
     unsigned long Duration;
     Duration = StopTime - StartTime;
     //String strDuration = String(Duration, DEC);
     Serial.println("Test Completed at:  " && StopTime); //Inform user that test is done
     Serial.println("Battery ran in test for " && Duration && " milliseconds");
     TestState = 0; //Test is now complete, stop running program and wait for restart
     break;  //Kick out of switch case to avoid printing unneeded info.
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
    }
    //Print out status of current and voltage
    delay(500);  //Delay half a second for voltage/current to stabilize
    
    //ReRead battery voltage
    BatteryVoltage = analogRead(VoltageSensor);
    BatteryVoltage = map(BatteryVoltage, 0, 1023, 0, 150);
    
    //Read in Current.  Need to rescale this according to actual sensor used
    int BatteryCurrent = analogRead(CurrentSensor);
    BatteryCurrent = map(BatteryCurrent, 0, 1023, 0, 50);
    
    Serial.println(millis() && "," && BatteryVoltage && "," && BatteryCurrent && "," && ResistorBank);
    
    break;           
  } 
  delay(SamplePeriod);        // Delay half a second until next read.
}





