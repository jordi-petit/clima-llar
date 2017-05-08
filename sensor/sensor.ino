
// Macros with constants (defines SSID, PSWD, SERVER_IP, PORT, KEY)
// This file is not included in the GIT repo to hide sensitive data.
#include "sensor.h"


// Standard headers
#include <stdio.h>
#include <string.h>


// Libraries for devices
#include <SimpleDHT.h>
#include <SoftwareSerial.h>


// Polling period
const int period = 10 * 1000;

// Name of the wireless network
const char* ssid = SSID;

// Password of the wireless network
const char* pswd = PSWD;

// IP of the server
const char* server_ip = SERVER_IP;

// Port of the server
const int port = PORT;

// Clima-Llar key
const char* key = KEY;

// Pin for RX
const int Pin_RX = 3;

// Pin for TX
const int Pin_TX = 2;

// Pin for DHT11 sensor
const int Pin_DHT11 = 4;

// Pin for the photoresistor (analog!)
const int Pin_PhRes = A0;

// Pin for the onboard LED
const int Pin_Led = 13;

// Lengths of the string buffers
const int MaxLen = 128;


// The DHT11 sensor device
SimpleDHT11 dht11;

// The ESP01 wifi device
SoftwareSerial wifi(Pin_RX, Pin_TX);


// Read temperature and humidity from DHT11

void read_DHT11(byte& temperature, byte &humidity) {
    if (dht11.read(Pin_DHT11, &temperature, &humidity, NULL)) {
        Serial.print("Read DHT11 failed");
    }
}


// Read light intensity from photoresistor

int read_light() {
    // Reminder: ints in Arduino are 16 bits long,
    //           and analogRead() returns 10 bits.
    const int n = 10;   // number of samples
    int s = 0;
    for (int i = 0; i < n; ++i) {
        s += analogRead(Pin_PhRes);
        delay(10);
    }
    return s / n;
}


void wifi_readline(char* data) {
    int p = 0;
    while (true) {
        while (wifi.available()) {
            char c = wifi.read();
            if (c == 10) return;
            if (c == 13) continue;
            data[p++] = c;
        }
    }
}


bool wifi_command(const char* command) {
    wifi.print(command);
    char line[MaxLen];
    memset(line, 0, MaxLen);
    while (true) {
        wifi_readline(line);
        Serial.println(line);
        if (strcmp(line, "OK") == 0) return true;
        if (strcmp(line, "ERROR") == 0) return false;
    }
}


// Send data over wifi

void send_wifi(const char* data) {
    char cmd[MaxLen];
    snprintf(cmd, MaxLen, "AT+CIPSTART=\"TCP\",\"%s\",%d\r\n", server_ip, port);
    wifi_command(cmd);
    int length = strlen(data);
    char str[MaxLen];
    sprintf(str, "AT+CIPSEND=%d\r\n", length);
    wifi_command(str); delay(100);
    wifi.print(data);
    wifi.print("\r\n"); Serial.println(data); delay(100);
    wifi_command("AT+CIPCLOSE\r\n");delay(100);
}



// Arduino setup code

void setup() {
    // Setup Serial (console)
    Serial.begin(9600);
    Serial.println("# ClimaLlar sensor starting");

    // Setup LED
    pinMode(Pin_Led, OUTPUT);
    digitalWrite(Pin_Led, LOW);

    // Setup Photoresistor
    pinMode(Pin_PhRes, INPUT);

    // Setup Wifi
    Serial.println("# Wifi init\n");
    wifi.begin(9600);

    Serial.println("# Wifi test");
    wifi_command("AT\r\n");

    Serial.println("# Wifi info:");
    wifi_command("AT+GMR\r\n");

    Serial.println("# Wifi mode:");
    wifi_command("AT+CWMODE?\r\n");

    if (true) {
        Serial.println("# Wifi list of access points:");
        wifi_command("AT+CWLAP\r\n");

        Serial.println("# Wifi join access point:");
        char str[100];
        sprintf(str, "AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pswd);
        wifi_command(str);
    }

    Serial.println("# Wifi info:");
    wifi_command("AT+CWJAP?\r\n");

    Serial.println("# Wifi info:");
    wifi_command("AT+CIPSTATUS\r\n");

    Serial.println("# Wifi info:");
    wifi_command("AT+CIFSR\r\n");

    Serial.println("# Wifi info:");
    wifi_command("AT+CIPSTA\r\n");

    Serial.println("# Wifi done");

    // This LED blinking signals end of setup.
    for (int i = 0; i < 20; ++i) {
        digitalWrite(Pin_Led, HIGH);
        delay(250);
        digitalWrite(Pin_Led, LOW);
        delay(250);
    }
}



// Arduino loop code

void loop() {
    digitalWrite(Pin_Led, HIGH);
    byte temperature = 255;
    byte humidity = 255;
    read_DHT11(temperature, humidity);
    int light = read_light();
    char data[MaxLen];
    snprintf(data, MaxLen, "%d:%d:%d:%d", key, temperature, humidity, light);
    send_wifi(data);
    digitalWrite(Pin_Led, LOW);
    delay(period);
}
