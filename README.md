# Rockometer

## Description
The Rockometer is a safety watch designed for rock climbers. It uses a pulse oximeter to monitor blood oxygen saturation (SpO2) and help identify potential signs of altitude-related hypoxia, HAPE, or HACE. If SpO2 levels persist below 90%, it alerts the climber with a gradually increasing, repeating buzzer and a visual warning on its LCD display.

## Features
Real-time blood oxygen saturation (SpO2) monitoring

Visual alerts on a 1.5-inch RGB OLED display for readings and warnings

Audible alerts for sustained low SpO2 levels (specially designed not to frighten climber by slowly increasing in volume)

Sensor connection monitoring 

Invalid reading detection

Filtered readings to reduce display fluctuations

Automatic return to normal monitoring when valid readings recover

## How it Works
1. Turn on the Rockometer to initialize the display and pulse oximeter

2. The MAX30100 sensor measures blood oxygen saturation
 
3. The Arduino Nano checks sensor readings for invalid values and connection errors

4. Multiple readings are processed to reduce fluctuations and improve stability

5. If SpO2 remains below 90% for the configured duration, the device displays a warning and activates the buzzer

6. When valid readings return to the configured safe range, the device resumes normal monitoring

<img width="831" height="540" alt="Rockometer Logic DIagram" src="https://github.com/user-attachments/assets/2ffb5803-252b-47f1-999f-1e1e06c17fc4" />


## Technologies Used
MAX30100 pulse oximeter sensor

Adafruit SSD1351 RGB OLED display

Piezoelectric buzzer

Arduino Software coded in C++

Multiple libraries are required for Arduino to hardware compatibility (zip file is supplied)

## Installation & Setup
Solder the Arduino Nano, OLED display, MAX30100 sensor, and buzzer as shown in the diagram:

<img width="603" height="296" alt="Rockometer Circuit Diagram" src="https://github.com/user-attachments/assets/f9e0c9e0-1c0f-489f-8d83-f162036e34ae" />

<br>
<br>
Download the Rockometer.ino and RockometerLibraries.zip file, and upload it to your Arduino.

<br>
<br>
The following is how I wore the device, but can be heavily improved upon:

I 3D printed a protective housing for the electronics with an SpO2 sensor cutout fitted to my wrist for proper readings. I cut a hole in a Nike arm sleeve for the sensor to be compressed to my wrist. I then sewed a pocket onto the arm sleeve to hold the circuit housing, with a cutout for the LCD screen.


## Future Improvements
Get an altimeter to work in unison with the SpO2 sensor to print the current altitude and temperature to the main screen

Attach a SIM800L GPRS GSM to be able to notify a chosen contact (potentially 911) if the altitude suddenly drops and the users heart rate spikes or slows for a long enough period to indicate an injury from a fall

Improve the circuitry housing and arm sleeve design to be more sleek and tight, allowing the sensor to be more consistently accurate

## Safety & Limitations
The Rockometer is an experimental electronics project and is not a certified medical device or a substitute for appropriate altitude-safety equipment, medical evaluation, or established climbing safety procedures.

A pulse oximeter can help identify low blood oxygen saturation, but SpO₂ alone cannot diagnose or rule out HAPE, HACE, or altitude-related illness. Consumer sensors can also produce inaccurate readings due to movement, poor circulation, temperature, sensor placement, and other factors.

The 90% threshold is a project-defined alert setting, not a universal diagnostic cutoff. Climbers should not rely on this device to determine whether it is safe to continue ascending. Suspected altitude illness requires prompt action and appropriate medical guidance.

## Liscence
This project is released under the MIT License. See the LICENSE file for details.

## Author
Harrison Perlstein
