#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "WifiCreds.h"


// Define led variables
bool ledState = false;
const uint8_t LEDPin = D2;


// MQTT broker url
const char *mqtt_server = "192.168.0.7";

WiFiClient espClient;
PubSubClient mqttClient(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

// Setup wifi connection
void setup_wifi() {
    delay(10);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }

    randomSeed(micros());
}

// MQTT callback
void onMessageRecieved(char *topic, byte *payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;

    if (!strcmp(topic, "60958e4507e68bc0d8fcbb2e/led/set")) {
        if (!strcmp(p, "ON")) {
            ledState = true;
            digitalWrite(LEDPin, HIGH);
        } else if (!strcmp(p, "OFF")) {
            ledState = false;
            digitalWrite(LEDPin, LOW);
        } else {
            mqttClient.publish("60958e4507e68bc0d8fcbb2e/error", "Invalid command");
        }
    } else {
        mqttClient.publish("60958e4507e68bc0d8fcbb2e/error", "Invalid topic");
    }
}

void reconnect() {
    // Loop until we're reconnected
    while (!mqttClient.connected()) {
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        const char* deviceID = "60958e9207e68bc0d8fcbb31";
        const char* deviceAPIKey = "1bf169f629e0d80de0259ed4582b7f16dc1141ccbccd9c1081eb8a0483cc9982";

        if (mqttClient.connect(clientId.c_str(), deviceID, deviceAPIKey)) {
            delay(500);
            // Once connected, publish an announcement...
            mqttClient.publish("60958e4507e68bc0d8fcbb2e/new/device", "NodeMCU");

            delay(500);
            // ... and resubscribe
            mqttClient.subscribe("60958e4507e68bc0d8fcbb2e/led/set", 1);

            delay(1000);
        } else {
            //      Serial.print("failed, rc=");
            //      Serial.print(mqttClient.state());
            //      Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup() {
    //  Serial.begin(115200);
    
    // GPIO config
    pinMode(LEDPin, OUTPUT);
    digitalWrite(LEDPin, LOW);

    // Wifi config
    setup_wifi();

    // MQTT config
    mqttClient.setServer(mqtt_server, 1883);
    mqttClient.setCallback(onMessageRecieved);

    delay(1000);
}

void loop() {
    if (!mqttClient.connected()) {
        reconnect();
    }
    mqttClient.loop();

    unsigned long now = millis();
    if (now - lastMsg > 500) {
        lastMsg = now;
        mqttClient.publish("60958e4507e68bc0d8fcbb2e/led/state", ledState? "ON": "OFF");
    }
}