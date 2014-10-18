Battery_Tester
==============

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
