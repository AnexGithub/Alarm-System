#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <base64.h> // Required for Twilio Authentication
#include <Wire.h>
#include <MPU6050.h>

// 🔹 WiFi Credentials
const char* ssid = "*************";
const char* password = "***********";

// 🔹 Twilio API Credentials
const char* account_sid = "################"; // twilio account sis
const char* auth_token = "#################"; //twilio auth token
const char* twilio_number = "***********"; //twilio_number
const char* recipient_number = "*************"; //your phone number

// 🔹 Secure WiFi Client for HTTPS
WiFiClientSecure client;

// 🔹 Motion Sensor Pin
int sensor = D3;              // The pin that the sensor is attached to
int state = LOW;             // Default state: No motion detected
int val = 0;                 // Variable to store the sensor status (value)

// 🔹 MPU6050 Tamper Detection
#define BUZZER_PIN D4         // Buzzer pin
MPU6050 mpu;

bool tamperDetected = false;
unsigned long tamperTime = 0;

// Thresholds for tilt detection (adjust based on sensitivity)
const int THRESHOLD_X = 16000;  
const int THRESHOLD_Y = 16000;  
const int THRESHOLD_Z = 18000;  

// Function to Send SMS via Twilio
void sendSMS(String message) {
  Serial.println("Connecting to Twilio...");

  client.setInsecure(); // Ignore SSL verification (required for ESP8266)
  if (!client.connect("api.twilio.com", 443)) {
    Serial.println("Connection to Twilio Failed!");
    return;
  }

  // Twilio API Endpoint
  String url = "/2010-04-01/Accounts/" + String(account_sid) + "/Messages.json";
  String postData = "To=" + String(recipient_number) + "&From=" + String(twilio_number) + "&Body=" + message;
  String auth = "Basic " + base64::encode(String(account_sid) + ":" + String(auth_token));

  Serial.println("Sending HTTP POST request...");
  
  client.println("POST " + url + " HTTP/1.1");
  client.println("Host: api.twilio.com");
  client.println("Authorization: " + auth);
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.print("Content-Length: ");
  client.println(postData.length());
  client.println();
  client.println(postData);

  // Read Twilio Response
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") break;
  }

  Serial.println("Message Sent Successfully!");
}

// 🔹 Setup Function
void setup() {
  Serial.begin(115200);
  
  // WiFi setup
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n WiFi Connected!");

  // Motion sensor setup
  pinMode(sensor, INPUT);

  // MPU6050 setup
  Wire.begin();
  mpu.initialize();
  
  if (mpu.testConnection()) {
    Serial.println("MPU6050 Connected!");
  } else {
    Serial.println("MPU6050 Connection Failed!");
    while (1);
  }

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

// 🔹 Loop Function
void loop() {
  // Motion detection
  val = digitalRead(sensor);

  if (val == HIGH) {
    if (state == LOW) {
      Serial.println("Motion detected!");
      sendSMS("🚨 Motion Detected near your sensor!");
      state = HIGH;
    }
  } else {
    if (state == HIGH) {
      Serial.println("Motion stopped!");
      state = LOW;
    }
  }

  // MPU6050 Tamper Detection
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  if (abs(ax) > THRESHOLD_X || abs(ay) > THRESHOLD_Y || abs(az) > THRESHOLD_Z) {
    Serial.println("Tamper detected! Buzzer ON...");
    triggerTamper();
    sendSMS("🚨 Tampering detected! Immediate action required.");
  }

  // Deactivate the buzzer after 5 seconds
  if (tamperDetected && (millis() - tamperTime > 5000)) {
    tamperDetected = false;
    Serial.println("Buzzer OFF.");
    digitalWrite(BUZZER_PIN, LOW);
  }

  delay(500);
}

void triggerTamper() {
  tamperDetected = true;
  tamperTime = millis();
  digitalWrite(BUZZER_PIN, HIGH);
}
