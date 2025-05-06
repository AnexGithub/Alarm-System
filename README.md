# Alarm-System

                                       Connection details
                                       
Modules                                   ESP8266 pin               Uses
MPU-6050 Accelerometer and Gyro           D1, D2, VCC, GND          For tamper detection
Buzzer                                    D4                        For tamper alarm
PIR sensor                                D3                        For motion detection
ESP 32  CAM                               VCC, GND                  For image taking and real time monitoring

                                       Components Required

SI No:                                   Components                 Purpose

 1                                       ESP8266 (NodeMCU)          Main controller for motion and tamper detection, SMS alerting
 2                                       ESP32-CAM Module           Captures images and sends them via Telegram bot
 3                                       PIR Motion Sensor          Detects human motion
 4                                       MPU6050 (Gyroscope +       Detects tampering  of the device
                                         Accelerometer)
 5                                       5V 2A AC to DC Adapter     Provides regulated power supply to the system
 6                                       YX850 Power Failure        Automatic switching to lithium battery during power loss 
                                         Module
 7                                       5V Lithium Battery         Backup power source for uninterrupted operation
 8                                       Buzzer                     Sounds alert during tampering


                          Implementation ESP32 CAM module with Telegram

# created a Telegram bot for ESP32-CAM.
# Start a conversation with the ESP32-CAM bot.
# When we send the message /photo to the ESP32-CAM bot, the ESP32-CAM board receives the message, takes a new photo, and responds with that photo.
# We can send the message /flash to toggle the ESP32-CAM’s LED flash.
# We can send the message /video to the ESP32-CAM bot, the ESP32-CAM board receives the message and it gives the address of the web server.
# We can send the /start message to receive a welcome message with the commands to control the board.
# The ESP32-CAM will only respond to messages coming from our Telegram account ID.

##### HAVE A GREAT DAY ####




 




                                      

