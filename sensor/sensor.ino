
// Macros with constants (defines SSID, PSWD, SERVER_IP, PORT, KEY)
// This file is not included in the GIT repo to hide sensitive data.
#include "sensor.h"


// Standard headers
#include <stdio.h>
#include <string.h>


// Libraries for devices
#include <SimpleDHT.h>
#include <SoftwareSerial.h>


// Whether to use wifi or not (help for debugging)
const bool use_wifi = true;

// Polling period
const unsigned long period = 60000L;   // (1 minute)

// Min time between push button request
const unsigned long min_push = 1000L;   // (1 second)

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

// Pin for the push button
const int Pin_Btn = 5;

// Lengths of the string buffers
const int MaxLen = 128;


//
unsigned long last;

// The DHT11 sensor device
SimpleDHT11 dht11;


// The ESP01 wifi device
SoftwareSerial wifi(Pin_RX, Pin_TX);


// Read temperature and humidity from DHT11

void read_DHT11(byte& temperature, byte &humidity) {
    if (dht11.read(Pin_DHT11, &temperature, &humidity, NULL)) {
        Serial.println("Read DHT11 failed");
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
    bool c13 = false;
    bool c10 = false;
    while (not c13) {
        if (wifi.available()) {
            char c = wifi.read();
            if (c == 10) c10 = true;
            else if (c == 13) c13 = true;
            else data[p++] = c;
        }
    }
    data[p] = 0;
}


bool wifi_command(const char* command) {
    wifi.println(command);
    char line[MaxLen];
    while (true) {
        wifi_readline(line);
        Serial.println(line);
        if (strcmp(line, "ready") == 0) return true;
        if (strcmp(line, "OK") == 0) return true;
        if (strcmp(line, "ERROR") == 0) return false;
    }
}


// Send data over wifi

void send_wifi(const char* data) {
    if (use_wifi) {
        wifi_command("AT+CIPSTA?");
        char cmd[MaxLen];
        snprintf(cmd, MaxLen, "AT+CIPSTART=\"TCP\",\"%s\",%d", server_ip, port);
        wifi_command(cmd);
        int length = strlen(data);
        char str[MaxLen];
        sprintf(str, "AT+CIPSEND=%d", length);
        wifi_command(str);
        wifi.println(data);
        delay(100);
        wifi_command("AT+CIPCLOSE");
    }
    Serial.print("time: ");
    Serial.print(millis() / 1000);
    Serial.print("data: ");
    Serial.println(data);
}


// Make the Pin_Led flash a bit to signal phases.

void led_flash() {
    Serial.println("# leds flashing");
    for (int i = 0; i < 3; ++i) {
        digitalWrite(Pin_Led, HIGH);
        delay(125);
        digitalWrite(Pin_Led, LOW);
        delay(125);
        digitalWrite(Pin_Led, HIGH);
        delay(125);
        digitalWrite(Pin_Led, LOW);
        delay(500);
    }
}


// The function that does the work

void sense_and_send() {
    digitalWrite(Pin_Led, HIGH);
    byte temperature = 255;
    byte humidity = 255;
    read_DHT11(temperature, humidity);
    int light = read_light();
    char data[MaxLen];
    snprintf(data, MaxLen, "%s:%d:%d:%d", key, temperature, humidity, light);
    send_wifi(data);
    digitalWrite(Pin_Led, LOW);
}


// Arduino setup code

void setup() {
    // Setup Serial (console)
    Serial.begin(9600);
    Serial.println("# clima-llar sensor starting");

    // Setup LED
    pinMode(Pin_Led, OUTPUT);
    digitalWrite(Pin_Led, LOW);

    // This LED dance signals start of setup.
    led_flash();

    // Setup Photoresistor
    pinMode(Pin_PhRes, INPUT);

    // Setup push button
    pinMode(Pin_Btn, INPUT_PULLUP);

    // Setup Wifi
    if (use_wifi) {

        Serial.println("# Wifi init\n");
        wifi.begin(9600);

        Serial.println("# Wifi test");
        wifi_command("AT");

        Serial.println("# Wifi reset");
        wifi_command("AT+RST");
        delay(5000);

    /*  The following command gives me trouble.
        Serial.println("# Wifi info:");
        wifi_command("AT+GMR");
    */

        Serial.println("# Wifi mode:");
        wifi_command("AT+CWMODE?");

        Serial.println("# Wifi list of access points:");
        wifi_command("AT+CWLAP");

        Serial.println("# Wifi join access point:");
        char str[MaxLen];
        sprintf(str, "AT+CWJAP=\"%s\",\"%s\"", ssid, pswd);
        wifi_command(str);

        Serial.println("# Wifi info:");
        wifi_command("AT+CWJAP?");

        Serial.println("# Wifi info:");
        wifi_command("AT+CIPSTATUS");

        Serial.println("# Wifi info:");
        wifi_command("AT+CIFSR");

        Serial.println("# Wifi info:");
        wifi_command("AT+CIPSTA?");

        Serial.println("# Wifi done");
    }

    // This LED dance signals end of setup.
    led_flash();

    // Start
    send_wifi("hello");
    sense_and_send();
    last = millis();
}



// Arduino loop code

void loop() {
    unsigned long time_since_last = millis() - last;
        // see michael_x at https://forum.arduino.cc/index.php?topic=122413.0
    if (time_since_last > period
        or (digitalRead(Pin_Btn) == LOW and time_since_last > min_push))
    {
        sense_and_send();
        last = millis();
    }
}
