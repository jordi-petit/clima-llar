#include <SoftwareSerial.h>


SoftwareSerial Wifi(3, 2); // RX | TX

void setup() {
  Serial.begin(9600);
  Wifi.begin(9600);
  Serial.print("hola\n");
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() {
  if (Wifi.available()) {
    char c = Wifi.read();
    Serial.print(c);
  }
  if (Serial.available()) {
    char c = Serial.read();
    Wifi.print(c);
  }
}

/*

AT+RST
AT+GMR
AT+CWMODE=?
AT+CWMODE?
AT+CWLAP
AT+CWJAP="AndroidJordi",""
AT+CIPSTATUS
AT+CIFSR
AT+CIPSTART="TCP","192.168.43.98",8080

AT+CIPSEND=4

*/
