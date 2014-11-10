/*
 Programmer:  Will Baden
 Date:        11/3/14
 Name:        Sensor_Testing
 Description: Used for testing the scaling of the voltage and current
 sensor inputs.   This program will output to the serial port the
 values using the same format as the battery test programs
 
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
  BatteryCurrent = map(BatteryCurrent, 101, 922, -500, 500); //Full scale remapping(0,1023,-625,625)
                                                             //Sensor full scale remapping(101, 922, -500, 500)  
}





