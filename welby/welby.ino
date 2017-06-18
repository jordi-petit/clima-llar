#include <SimpleDHT.h>
#include <IRrecv.h>
//#include "queue.hh"


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

// The DHT11 sensor device
SimpleDHT11 dht11;

// The IR sensor device
IRrecv irrecv(Pin_IR);

decode_results results;

// The event queue and its capacity
//Queue<10> q;

// The variables with the readings
int temperature;
int humidity;
int light;



void translateIR() // takes action based on IR code received

// describing Remote IR codes

{

  switch(results.value)

  {
  case 0xFFA25D: Serial.println("POWER"); break;
  case 0xFFE21D: Serial.println("FUNC/STOP"); break;
  case 0xFF629D: Serial.println("VOL+"); break;
  case 0xFF22DD: Serial.println("FAST BACK");    break;
  case 0xFF02FD: Serial.println("PAUSE");    break;
  case 0xFFC23D: Serial.println("FAST FORWARD");   break;
  case 0xFFE01F: Serial.println("DOWN");    break;
  case 0xFFA857: Serial.println("VOL-");    break;
  case 0xFF906F: Serial.println("UP");    break;
  case 0xFF9867: Serial.println("EQ");    break;
  case 0xFFB04F: Serial.println("ST/REPT");    break;
  case 0xFF6897: Serial.println("0");    break;
  case 0xFF30CF: Serial.println("1");    analogWrite(Pin_LedR, 255); break;
  case 0xFF18E7: Serial.println("2");    analogWrite(Pin_LedR, 128);break;
  case 0xFF7A85: Serial.println("3");    analogWrite(Pin_LedR, 0);break;
  case 0xFF10EF: Serial.println("4");    analogWrite(Pin_LedG, 255); break;
  case 0xFF38C7: Serial.println("5");    analogWrite(Pin_LedG, 128); break;
  case 0xFF5AA5: Serial.println("6");    analogWrite(Pin_LedG, 0); break;
  case 0xFF42BD: Serial.println("7");    analogWrite(Pin_LedB, 255); break;
  case 0xFF4AB5: Serial.println("8");    analogWrite(Pin_LedB, 128); break;
  case 0xFF52AD: Serial.println("9");    analogWrite(Pin_LedB, 0); break;
  case 0xFFFFFFFF: Serial.println(" REPEAT");break;

  default:
    Serial.println(" other button   ");

  }// End Case

  delay(500); // Do not get immediate repeat


}



// Read temperature and humidity from DHT11 and light from photoresistor

void get_readinds() {
    byte temp = 0;
    byte humi = 0;
    if (dht11.read(Pin_DHT11, &temp, &humi, NULL)) {
        Serial.println("Read DHT11 failed");
    }
    temperature = temp;
    humidity = humi;
    light = analogRead(Pin_PhRes);
}



void setup() {
    // Setup Serial
    Serial.begin(9600);
    Serial.println(F("\n\nclima-llar-sensor (welby-edition)\n\n"));

    // Setup Photoresistor
    pinMode(Pin_PhRes, INPUT);

    // Setup Leds
    pinMode(Pin_LedR, OUTPUT);
    pinMode(Pin_LedG, OUTPUT);
    pinMode(Pin_LedB, OUTPUT);

    // Setup IR
    irrecv.enableIRIn();
}


void loop() {
    if (irrecv.decode(&results))
      {
        translateIR();
        irrecv.resume();
      }

}
