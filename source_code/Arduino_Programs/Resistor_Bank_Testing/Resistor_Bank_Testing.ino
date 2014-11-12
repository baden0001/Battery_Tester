/*
 Programmer:  Will Baden
 Date:        11/9/14
 Name:        Resistor_Bank_Testing
 Description: Used for testing the loads of the resistor banks.  
 Use this to force a bank to run.  See what the current levels out
 at, at the voltage applied.  Calculate resistance from there.
 
 11/10/14 Voltage Test:
 Voltage Divider        Meter
 2.58                  2.68
 5.03                  5.1
 7.45                  7.53
 10.01                 10.09
 10.52                 10.54
 10.94                 11.01
 11.42                 11.5
 11.96                 12.0
 12.46                 12.5
 12.97                 13.01
 13.51                 13.47
 
 11/9/14 Test:
 Bank 1:
 11.56 - 11.55V @ 32.5 amps
 .3553846 ohms
 11.56 - 11.4V @ 33.1 amps
 
 Bank 2:
 11.96 - 11.79 @ 14.6 amps
 .807534 ohms
 
 Bank 3:
 12.14 - 11.91 @ 7.8 amps
 1.526923
 
 11/10/14 Test
 6.4 amps measured with VOM meter and
 6.6 amps measured with in RobotShop meter
 12.11V reading on voltage divider.
 
 
 Bank 4:
 12.22 - 11.98 @ 5.0 amps
 2.396 ohms
 
 Bank 5:
 12.41 - 12.14 @ 1.5 amps
 8.09333 ohms
 
 Bank 6:
 12.43 - 12.15 @ 1.5 amps
 8.1 ohms
 
 Bank 7:
 12.41 - 12.14 @ 1.4 amps
 8.67 ohms
 
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

int TestState = 0;  //Create state machine, set to wait for key press
                    // 0 = waiting for 'Acknowledgement' to be sent from outside
                    // 1 = Send out 'Send Start to begin'
                    // 2 = waiting for 'Start' command to start test
                    // 3 = Battery test in progress
int BatteryVoltage = 0;  //Store battery voltage
int BatteryCurrent = 0;  //Store battery current
String ResistorBank = "";  //Keep track of resistor banks
char buffer[15];  //buffer to store data from serial port, array size of 15
int SamplePeriod = 5000;  //Period of time between voltage checks

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

}

void loop() {
  
    ReadBatteryStatus();
    SendStatus();
    digitalWrite(HalfOhm, LOW);
    digitalWrite(OneOhm, LOW);
    digitalWrite(TwoOhm, HIGH);
    digitalWrite(ThreeOhm, LOW);
    digitalWrite(TenOhm1, LOW);
    digitalWrite(TenOhm2, LOW);
    digitalWrite(TenOhm3, LOW);
    delay(SamplePeriod);  //Delay for sampleperiod     
  
}

void SendStatus() {
  Serial.print("Program Run,");
  //Serial.print(String(CurrentTime));
  Serial.print(",");
  Serial.print(String(BatteryVoltage));
  Serial.print(",");
  Serial.print(String(BatteryCurrent));
  Serial.println(",");
  //Serial.println(ResistorBank);
}  

void SendEndStatus() {
  Serial.print("Program Run,"); 
  //Serial.print(String(StopTime));
  Serial.print(",");
  Serial.print(String(BatteryVoltage));
  Serial.print(",");
  Serial.print(String(BatteryCurrent));
  Serial.print(",");
  //Serial.println(ResistorBank);
}  

void ReadBatteryStatus() {
  //Read battery voltage.  Need to rescale this according to actual sensor used
  BatteryVoltage = analogRead(VoltageSensor);
  BatteryVoltage = map(BatteryVoltage, 0, 1023, 0, 2725);
    
  //Read in Current.  Need to rescale this according to actual sensor used
  BatteryCurrent = analogRead(CurrentSensor);
  //BatteryCurrent = (BatteryCurrent-510)*5/1024/.04-.04 ; white paper calculation
  BatteryCurrent = map(BatteryCurrent, 101, 922, -500, 500); //Full scale remapping(0,1023,-625,625)
                                                             //Sensor full scale remapping(101, 922, -500, 500)  
}





