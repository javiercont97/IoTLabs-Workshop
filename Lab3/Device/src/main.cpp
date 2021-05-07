#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include "Robot.h"
#include "WifiCreds.h"

int x = 0;
int y = 0;

// Create an instance of Robot class defined in Robot.h
Robot robot(D0, D3, D5, D8);

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

    if (!strcmp(topic, "60958e6107e68bc0d8fcbb30/joystick_X")) {
        if (!strcmp(p, "0")) {
        x = 0;
        } else if (!strcmp(p, "1")) {
        x = 1;
        } else if (!strcmp(p, "-1")) {
        x = -1;
        }
    } else if (!strcmp(topic, "60958e6107e68bc0d8fcbb30/joystick_Y")) {
        if (!strcmp(p, "0")) {
        y = 0;
        } else if (!strcmp(p, "1")) {
        y = 1;
        } else if (!strcmp(p, "-1")) {
        y = -1;
        }
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
            mqttClient.publish("60958e6107e68bc0d8fcbb30/new/dev", "NodeMCU");

            delay(500);
            // ... and resubscribe
            mqttClient.subscribe("60958e6107e68bc0d8fcbb30/joystick_X", 1);
            mqttClient.subscribe("60958e6107e68bc0d8fcbb30/joystick_Y", 1);

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
    
    // GPIO config
    pinMode(D0, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(10, INPUT);
    
    // LCD Config
    Wire.begin(D2, D1);

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
    // if (now - lastMsg > 2000) {
    if (x == 0 && y == 0) {
        // stop
        robot.stop();
    } else if (x == 0 && y == 1) {
        // foreward
        robot.moveForeward();
    } else if (x == 0 && y == -1) {
        // backward
        robot.moveBackward();
    } else if (x == 1 && y == 0) {
        // right
        robot.turnRight();
    } else if (x == -1 && y == 0) {
        // left
        robot.turnLeft();
    } else if (x == 1 && y == 1) {
        // top right
        robot.moveRight();
    } else if (x == -1 && y == 1) {
        // top left
        robot.moveLeft();
    } else if (x == 1 && y == -1) {
        // back right
        robot.moveBackRight();
    } else if (x == -1 && y == -1) {
        // back left
        robot.moveBackLeft();
    }
    // }
}