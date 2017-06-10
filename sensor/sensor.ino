
// Macros with constants (defines MY_SSID, MY_PSWD, MY_SERVER, MY_PORT, MY_KEY)
// This file is not included in the GIT repo to hide sensitive data.
#include "sensor.h"


// Standard headers
#include <stdio.h>
#include <string.h>


// Libraries for devices
#include <SimpleDHT.h>
#include <WiFiEsp.h>
#include <SoftwareSerial.h>


// Whether to use wifi or not (help for debugging)
const bool use_wifi = true;

// Polling period
const unsigned long period = 60000L;   // (1 minute)

// Min time between push button request
const unsigned long min_push = 1000L;   // (1 second)

// Name of the wireless network
const char* ssid = MY_SSID;

// Password of the wireless network
const char* pswd = MY_PSWD;

// IP of the server
const char* server = MY_SERVER;

// Port of the server
const int port = MY_PORT;

// Clima-Llar key
const char* key = MY_KEY;

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

// Time when last reading was sent
unsigned long last;


// The ESP01 wifi device
SoftwareSerial esp01(Pin_RX, Pin_TX);

// The Ethernet client object
WiFiEspClient client;

// The DHT11 sensor device
SimpleDHT11 dht11;



// Print Wifi satus

void printWifiStatus()
{
    // print the SSID of the network you're attached to
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print IP address
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength
    long rssi = WiFi.RSSI();
    Serial.print("Signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}


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


// Send data over wifi

void post(const char* url, const char* data) {
    if (use_wifi) {
        if (not client.connected()) client.stop();
        if (client.connect(server, port)) {
            // Verbose to save memory, delays to not fill buffers
            Serial.println("Send POST to server");
            Serial.println(data);
            client.print("POST "); client.print(url); client.println(" HTTP/1.1");  delay(50);
            client.print("Host: "); client.println(server);  delay(50);
            client.print("Content-Length: "); client.println(strlen(data));  delay(50);
            client.println("Content-Type: application/json");  delay(50);
            client.println("");  delay(50);
            client.println(data);  delay(50);
        } else {
            Serial.println("Cannot connect to server");
        }
    }
}


// Make the Pin_Led flash a bit to signal phases.

void led_flash() {
    Serial.println("# leds flashing");
    for (int i = 0; i < 3; ++i) {
        digitalWrite(Pin_Led, HIGH); delay(125);
        digitalWrite(Pin_Led, LOW ); delay(125);
        digitalWrite(Pin_Led, HIGH); delay(125);
        digitalWrite(Pin_Led, LOW ); delay(500);
    }
}


// The function that does the work

void sense_and_send() {
    digitalWrite(Pin_Led, HIGH);
    printWifiStatus();

    byte temperature = 255;
    byte humidity = 255;
    read_DHT11(temperature, humidity);
    int light = read_light();

    char data[64];
    snprintf(data, 64, "{\"temperature\": %d, \"humidity\": %d, \"light\": %d}",
        temperature, humidity, light);

    Serial.println(data);

    char url[64];
    snprintf(url, 64, "/api/%s/submit", key);

    post(url, data);

    digitalWrite(Pin_Led, LOW);
}


// Arduino setup code

void setup() {
    // Setup serial of the console
    Serial.begin(9600);
    Serial.println("# clima-llar-sensor starting");

    // Setup serial of the esp01
    esp01.begin(9600);

    // Setup LED
    pinMode(Pin_Led, OUTPUT);
    digitalWrite(Pin_Led, LOW);

    // Setup Photoresistor
    pinMode(Pin_PhRes, INPUT);

    // Setup push button
    pinMode(Pin_Btn, INPUT_PULLUP);

    // Setup Wifi
    if (use_wifi) {

        // initialize ESP module
        WiFi.init(&esp01);

        // check for the presence of the shield
        if (WiFi.status() == WL_NO_SHIELD) {
            Serial.println("ESP-01 not present");
            while (true);
        }

        // attempt to connect to WiFi network
        int status = WL_IDLE_STATUS;
        while (status != WL_CONNECTED) {
            Serial.print("Attempting to connect to WPA SSID: ");
            Serial.println(ssid);
            status = WiFi.begin(ssid, pswd);
        }

        // you're connected now, so print out the data
        Serial.println("Connected to the network");
        printWifiStatus();

        // This LED dance signals end of wifi setup.
        led_flash();
    }

    // Start

    sense_and_send();
    last = millis();
}



// Arduino loop code

void loop() {
    // Read the answer from the server if needed and copy it to console
    while (client.available()) {
        Serial.print(char(client.read()));
    }

    // Check time
    unsigned long time_since_last = millis() - last;
        // see michael_x at https://forum.arduino.cc/index.php?topic=122413.0
    if (time_since_last > period
        or (digitalRead(Pin_Btn) == LOW and time_since_last > min_push))
    {
        sense_and_send();
        last = millis();
    }
}
