/* This sketch was created by WeMos.cc */

#include "WEMOS_Motor.h"

// Libraries
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// WiFi parameters
#define WLAN_SSID       "test"
#define WLAN_PASS       "abcd1234"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "jakshob"
#define AIO_KEY         "7826ecb55cc44b85b28e1309e567c832"  // Obtained from account info on io.adafruit.com


// Functions
void connect();

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Store the MQTT server, client ID, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[]    = AIO_SERVER;
// Set a unique MQTT client ID using the AIO key + the date and time the sketch
// was compiled (so this should be unique across multiple devices for a user,
// alternatively you can manually set this to a GUID or other random value).
const char MQTT_CLIENTID[]  = AIO_KEY __DATE__ __TIME__;
const char MQTT_USERNAME[]  = AIO_USERNAME;
const char MQTT_PASSWORD[]  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

// Setup a feed called 'lamp' for subscribing to changes.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char VALUE_FEED[] = AIO_USERNAME "/feeds/lol";
Adafruit_MQTT_Subscribe valueStream = Adafruit_MQTT_Subscribe(&mqtt, VALUE_FEED);

/*************************** Sketch Code ************************************/



int pwm = 0;
unsigned long pingTimer = 0;

//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30, _MOTOR_A, 1000); //Motor A
//Motor M2(0x30,_MOTOR_B, 1000);//Motor B


void setup() {


  Serial.begin(115200);

  Serial.println(F("Adafruit IO Example"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  mqtt.subscribe(&valueStream);

  // connect to adafruit io
  connect();

}

void loop() {

  M1.setmotor( _CW, pwm);
  Serial.println(pwm);


  Serial.println("In the loop");
  Adafruit_MQTT_Subscribe *subscription;

  // ping adafruit io a few times to make sure we remain connected
  if (millis() - pingTimer < 1000) {
    pingTimer = millis();
    if (! mqtt.ping(3)) {
      // reconnect to adafruit io
      if (! mqtt.connected())
        connect();
    }
  }
  // this is our 'wait for incoming subscription packets' busy subloop
  while (subscription = mqtt.readSubscription(100)) {
    M1.setmotor( _CW, pwm);
    Serial.println("In the while");
    yield();
    // we only care about the lamp events
    if (subscription == &valueStream) {

      // convert mqtt ascii payload to int
      int value = atoi((char *)valueStream.lastread);
      pwm = value;
      Serial.print(F("Received: "));
      Serial.println(value);
      if (value == 0) {

        Serial.println("under 10");
      }
      else {

        Serial.println("Value is set");
      }

      // Apply message to lamp
      //   String message = String(value);
      //  message.trim();
      // if (message == "ON") {digitalWrite(lamp_pin, HIGH);}
      // if (message == "OFF") {digitalWrite(lamp_pin, LOW);}

    }

  }

}

// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {
    yield();
    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if (ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connected!"));

}
