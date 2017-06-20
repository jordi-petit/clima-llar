
// ****************************************************************
// clima-llar-sensor (welby-edition)
// ****************************************************************


// WiFi - https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/readme.html#
#include <ESP8266WiFi.h>

// DHT11 - Elegoo
#include <SimpleDHT.h>

// IR - https://github.com/markszabo/IRremoteESP8266
#include <IRrecv.h>

// Queue - Jordi
#include "queue.hh"

// Macros with the application parameters
#include "welby.h"


// ***************************************************************************
// The event queue and its capacity
// ***************************************************************************

Queue<10> q;


#include "ambient.h"


// ***************************************************************************
// ...
// ***************************************************************************


// Pin for the leds
const int Pin_LedR = D6;
const int Pin_LedG = D7;
const int Pin_LedB = D8;

// Pin for IR receiver
const int Pin_IR = D1;

// Pin for DHT11 sensor
const int Pin_DHT11 = D0;

// Pin for the photoresistor (analog!)
const int Pin_PhRes = A0;

// The struct with the readings
struct Readings {
    int temperature;
    int humidity;
    int light;
};

// SSID of the wifi network
const char* ssid = MY_SSID;

// Password of the wifi network
const char* pswd = MY_PSWD;

// clima-llar server host name and its port
const char* server = MY_SERVER;
const int port = MY_PORT;

// clima-llar place_id
const char* place_id = MY_PLACE;

// The ambient object
Ambient ambient(Pin_LedR, Pin_LedG, Pin_LedB);

// The DHT11 sensor device
SimpleDHT11 dht11;

// The IR sensor device and its results
IRrecv irrecv(Pin_IR);
decode_results results;





void translateIR() {

    switch (results.value) {
        case 0xFFA25D:
            Serial.println("POWER");
            if (ambient.program == 0) ambient.begin_1();
            else ambient.begin_0();
            break;
        case 0xFFE21D:
            Serial.println("FUNC/STOP");
            break;
        case 0xFF629D:
            Serial.println("VOL+");
            break;
        case 0xFF22DD:
            Serial.println("FAST BACK");
            break;
        case 0xFF02FD:
            Serial.println("PAUSE");
            break;
        case 0xFFC23D:
            Serial.println("FAST FORWARD");
            break;
        case 0xFFE01F:
            Serial.println("DOWN");
            break;
        case 0xFFA857:
            Serial.println("VOL-");
            break;
        case 0xFF906F:
            Serial.println("UP");
            break;
        case 0xFF9867:
            Serial.println("EQ");
            break;
        case 0xFFB04F:
            Serial.println("ST/REPT");
            break;
        case 0xFF6897:
            Serial.println("0");
            ambient.begin_0();
            break;
        case 0xFF30CF:
            Serial.println("1");
            ambient.begin_1();
            break;
        case 0xFF18E7:
            Serial.println("2");
            ambient.begin_2();
            break;
        case 0xFF7A85:
            Serial.println("3");
            ambient.begin_3();
            break;
        case 0xFF10EF:
            Serial.println("4");
            ambient.begin_4();
            break;
        case 0xFF38C7:
            Serial.println("5");
            ambient.begin_5();
            break;
        case 0xFF5AA5:
            Serial.println("6");
            break;
        case 0xFF42BD:
            Serial.println("7");
            break;
        case 0xFF4AB5:
            Serial.println("8");
            break;
        case 0xFF52AD:
            Serial.println("9");
            break;
        case 0xFFFFFFFF:
            Serial.println(" REPEAT");
            break;

        default:
            Serial.println(" other button   ");

    }
}


// Read temperature and humidity from DHT11 and light from photoresistor

Readings get_readings() {
    byte temp = 0;
    byte humi = 0;
    if (dht11.read(Pin_DHT11, &temp, &humi, NULL)) {
        Serial.println("Read DHT11 failed");
    }
    return {temp, humi, analogRead(Pin_PhRes)};
}



void printWifiStatus() {
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

    // print diagnostics
    WiFi.printDiag(Serial);
}


void check_ir() {
    if (irrecv.decode(&results)) {
        translateIR();
        irrecv.resume();
        q.in(500, check_ir);
    } else {
        q.in(10, check_ir);
    }
}


void send_readings() {
    Readings r = get_readings();

    WiFiClient client;

    if (client.connect(server, 80)) {

        String json
            = "{\"temperature\":" + String(r.temperature)
            + ",\"light\":" + String(r.light)
            + ",\"humidity\":" + String(r.humidity)
            + "}\r\n"
        ;

        String post
            = "POST /api/" + String(place_id) + "/submit HTTP/1.1\r\n"
            + "Host: " + String(server) + "\r\n"
            + "Content-Length: " + String(json.length()) + "\r\n"
            + "Content-Type: application/json\r\n"
            + "\r\n"
            + json + "\r\n"
        ;

        Serial.println("== send ==");
        Serial.print(post);
        client.print(post);

        Serial.println("== receive ==");
        // headers
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            Serial.println(line);
            if (line == "\r") break;
        }
        // body
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            Serial.println(line);
            if (line == "\r" or line == "") break;
        }

    } else {
        Serial.println("!!! error");
    }

    client.stop();
    q.in(20000, send_readings);
}


void setup() {
    // Setup Serial
    Serial.begin(115200);
    Serial.println(F("\n\nclima-llar-sensor (welby-edition)\n\n"));

    // Setup Photoresistor
    pinMode(Pin_PhRes, INPUT);

    // Setup Wifi
    Serial.print("Connecting to wifi");
    WiFi.begin(ssid, pswd);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("!");
    printWifiStatus();

    // Setup Ambient
    ambient.begin();

    // Setup IR
    irrecv.enableIRIn();

    // Set periodic timers
    q.in(10, check_ir);
    q.in(5000, send_readings);
}


void loop() {
    q.drain();
}


/*
    if (irrecv.decode(&results)) {
        translateIR();
        irrecv.resume();
    }

        delay(1000);

    WiFiClient client;

    const char * server = "clima-llar.potipoti.org";

    if (client.connect(server, 80)) {

        Serial.println("WiFi Client connected ");

        client.print("GET /hello HTTP/1.1\r\n");
        client.print("Host: clima-llar.potipoti.org\r\n");
        client.print("\r\n");

        Serial.println("request sent");
        while (client.connected()) {
            String line = client.readStringUntil('\n');
            if (line == "\r") {
                Serial.println("headers received");
                break;
            }
        }
        String line = client.readStringUntil('\n');

        Serial.println("reply was:");
        Serial.println("==========");
        Serial.println(line);
        Serial.println("==========");
        Serial.println("closing connection");

    } else {
        Serial.println("connext no va");
    }
    client.stop();

    delay(30000);
    */
