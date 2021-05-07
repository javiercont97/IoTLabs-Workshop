#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "WifiCreds.h"


// MQTT broker url
const char *mqtt_server = "192.168.0.7";

WiFiClient espClient;
PubSubClient mqttClient(espClient);
// unsigned long lastMsg = 0;
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

    if (!strcmp(topic, "60958e5407e68bc0d8fcbb2f/pwm")) {
        uint16_t pwmValue = String(p).toInt();
        analogWrite(D3, map(pwmValue, 0, 100, 0, 1023) );
    } else {
        mqttClient.publish("60958e5407e68bc0d8fcbb2f/error", "Invalid topic");
    }
}

void reconnect() {
    // Loop until we're reconnected
    while (!mqttClient.connected()) {
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        const char* deviceID = "603fe7e6085a4950dc3d1903";
        const char* deviceAPIKey = "203ebd274c20215d4b9f1bb10d1db62f5b6fc0423034b490355a4cadd1ebd28d";

        if (mqttClient.connect(clientId.c_str(), deviceID, deviceAPIKey)) {
            delay(500);
            // Once connected, publish an announcement...
            mqttClient.publish("60958e5407e68bc0d8fcbb2f/new/dev", "NodeMCU");

            delay(500);
            // ... and resubscribe
            mqttClient.subscribe("60958e5407e68bc0d8fcbb2f/pwm", 1);
            

            delay(1000);
        } else {
            // Serial.print("failed, rc=");
            // Serial.print(mqttClient.state());
            // Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

void setup() {
    //  Serial.begin(115200);
    

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

    // unsigned long now = millis();
    // if (now - lastMsg > 500) {
    
    // }
}