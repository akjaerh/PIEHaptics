/* This sketch was created by WeMos.cc */

#include "WEMOS_Motor.h"

// Libraries
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
// For button-read
#include <Arduino.h>

#define USE_SERIAL Serial
#define NOT_PRESSED HIGH
#define PRESSED LOW
// Button imports

typedef struct Buttons {
  const byte pin = D6;
  bool previousState = false;
  bool currentState = false;
} Button;

#define id "1"
#define idOther "2"

// create a Button variable type
Button button;

// WiFi parameters
#define WLAN_SSID       "test"
#define WLAN_PASS       "abcd1234"

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "jakshob"
#define AIO_KEY         "7826ecb55cc44b85b28e1309e567c832"  // Obtained from account info on io.adafruit.com
// RELATIONSHIP
#define SERIAL_NUMBER   ESP.getChipId()

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
const char HEARTBEAT[] = AIO_USERNAME "/feeds/heartbeat";



Adafruit_MQTT_Subscribe valueStream = Adafruit_MQTT_Subscribe(&mqtt, VALUE_FEED);
Adafruit_MQTT_Publish heartbeat = Adafruit_MQTT_Publish(&mqtt, HEARTBEAT);


// THIS CODE IS FOR WEMOS 1 - Since we are Wemos 1, we subscribe to a "publish"-value set by Wemos 2.
Adafruit_MQTT_Subscribe buttonSubscribe = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/person-" id ".buttonpublisher");
Adafruit_MQTT_Publish buttonPublish = Adafruit_MQTT_Publish(&mqtt,  AIO_USERNAME "/feeds/person-" idOther ".buttonpublisher");


/*************************** Sketch Code ************************************/

int     pwm = 0;

long    timeWhenPressed = 0;
boolean timerActive = false;

unsigned long tempTimer = 0;
unsigned long timer = 0;
//Motor shiled I2C Address: 0x30
//PWM frequency: 1000Hz(1kHz)
Motor M1(0x30, _MOTOR_A, 1000); //Motor A
//Motor M2(0x30,_MOTOR_B, 1000);//Motor B


void setup() {

  pinMode(button.pin, INPUT_PULLUP);
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only

  }

  Serial.println(F("Adafruit IO Example"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {

    delay(300);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  mqtt.subscribe(&valueStream);



  mqtt.subscribe(&buttonSubscribe);


  // connect to adafruit io
  connect();

}

void loop() {

  // Button
  updateMotor();
  // MQTT
  Adafruit_MQTT_Subscribe *subscription;

  // PUBLISHERS DEFINED WITHIN HERE
  //
  // publishes to MQTT-broker for keeping itself alive. (Ping-issue mitigation).
  if (millis() - timer > 60000) // do something every 60 seconds)
  {
    timer = millis();
    heartbeat.publish(SERIAL_NUMBER);
  }

  button.previousState = button.currentState;
  button.currentState = !digitalRead(button.pin);
  //Serial.println(button.currentState);

  if (button.previousState == 0 && button.currentState == 1)
  {
    buttonPublish.publish("1");
    Serial.println("Button pushed");
  }
  // this is our 'wait for incoming subscription packets' busy subloop
  // we can handle multiple subscriptions within here.
  if (subscription = mqtt.readSubscription(10)) {


    Serial.println("In the while");
    yield();
    //
    if (subscription == &valueStream) {

      // convert mqtt ascii payload to int
      int value = atoi((char *)valueStream.lastread);
      pwm = value;
      Serial.print(F("Received: "));
      Serial.println(value);
    }

    if (subscription == &buttonSubscribe) {
      pwm = 20;
      // SETS TWO GLOBAL VARIABLES USED IN TIMING EVENTS.
      timeWhenPressed = millis();
      timerActive = true;

      Serial.print(F("Received wemos two button click...: "));
    }

  }
  // THIS SNIPPIT TAKES CARE OF SETTING THE PWR BACK TO 0 WHEN TWO SECONDS HAVE PASSED, AFTER THE BUTTON-CLICK OF OTHER WEMOS HAVE BEEN REGISTERED.
  if (millis() - timeWhenPressed > 2000 && timerActive == true)
  {
    // do something two seconds later
    timerActive = false;
        pwm = 0;

  }

}

unsigned long lastUpdatedMotor = 0;
void updateMotor()
{
  if (millis() - lastUpdatedMotor > 500)
  {
    Serial.println(pwm);
  //  pwm = 20;
    // Motor
    M1.setmotor( _CW, pwm);
  }
}
// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {
    // M1.setmotor( _CW, pwm);
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
    delay(200);

  }

  Serial.println(F("Adafruit IO Connected!"));

}

