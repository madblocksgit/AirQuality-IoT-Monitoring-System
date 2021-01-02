# AirQuality-IoT-Monitoring-System
A project to demonstrate the IoT and where the sensory data is uploaded to Thingspeak IoT Cloud and the same data is displayed on LED Display.

# Ingredients
1. ESP32
2. MQ135
3. MQ8
4. MQ9
5. Thingspeak IoT Cloud
6. LED Display Board

# Procedure
1. Download Arduino IDE from arduino.cc
2. Try adding ESP32 to the boards manager using https://dl.espressif.com/dl/package_esp32_index.json <br/>
2a. File -> Preferences -> Additional Boards Manager URL and paste the above link <br/>
2b. Tools -> Boards -> Board Manager, Install ESP32 family to Arduino IDE 
3. Configure the board to Arduino IDE <br/>
3a. Tools -> Boards -> ESP32 -> NodeMCU32-S <br/>
3b. Tools -> Port -> COM3 (It can be varied)
4. Install the Libraries (Adafruit Sensor, DHT11 Sensor Library and Thingspeak)
6. Change the WiFi Credentials
7. Upload the Program
8. Connect LED Display, and Switch on the Power

# Helpdesk
For questions, write us at parvathanenimadhu@gmail.com

