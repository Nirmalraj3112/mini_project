#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>

#define GSM_RX_PIN 10
#define GSM_TX_PIN 11
SoftwareSerial gsmSerial(GSM_RX_PIN, GSM_TX_PIN);  // GSM module communication

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);  // ADXL345 accelerometer

void setup() {
  pinMode(8, INPUT);  // LDR sensor
  pinMode(9, OUTPUT);  // LED
  Serial.begin(9600);  // Serial monitor
  gsmSerial.begin(9600);  // GSM module

  // Initialize accelerometer
  if (!accel.begin()) {
    Serial.println("Failed to initialize accelerometer!");
    while (1);
  }
}

void loop() {
  int move = digitalRead(8);  // LDR sensor
  int movementX = analogRead(A0);  // Analog input

  sensors_event_t event;
  accel.getEvent(&event);
  float acceleration = event.acceleration.x;  // Accelerometer value

  Serial.println("Intensity = ");
  Serial.println(move);

  Serial.println("Movement X = ");
  Serial.println(movementX);

  Serial.println("Acceleration = ");
  Serial.println(acceleration);

  delay(500);

  if (move == HIGH || acceleration > 5.0) {  // Light blocked or excessive acceleration
    makePhoneCall("+918248676524");  // Call function with phone number to dial
    sendSMS("+918248676524", "Alarm! Someone is trying to break the accelerometer.");  // Send SMS
    digitalWrite(9, HIGH);  // Turn on LED
  } else {
    digitalWrite(9, LOW);  // Turn off LED
  }
}

void makePhoneCall(const char* phoneNumber) {
  gsmSerial.println("ATD" + String(phoneNumber) + ";");
  delay(1000);
  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }
}

void sendSMS(const char* phoneNumber, const char* message) {
  gsmSerial.println("AT+CMGF=1");  // Set SMS text mode
  delay(100);
  gsmSerial.println("AT+CMGS=\"" + String(phoneNumber) + "\"");  // Specify recipient phone number
  delay(100);
  gsmSerial.print(message);  // Message content
  delay(100);
  gsmSerial.write(26);  // End of message (Ctrl+Z)
  delay(1000);
  while (gsmSerial.available()) {
    Serial.write(gsmSerial.read());
  }
}
