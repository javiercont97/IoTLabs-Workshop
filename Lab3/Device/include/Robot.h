#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>

class Robot {
private:
    int m1F;
    int m1B;
    int m2F;
    int m2B;

public:
    Robot(int m1F, int m1B, int m2F, int m2B) {
        this->m1F = m1F;
        this->m1B = m1B;
        this->m2F = m2F;
        this->m2B = m2B;
        pinMode(m1F, OUTPUT);
        pinMode(m1B, OUTPUT);
        pinMode(m2F, OUTPUT);
        pinMode(m2B, OUTPUT);

        digitalWrite(m1F, LOW);
        digitalWrite(m1B, LOW);
        digitalWrite(m2F, LOW);
        digitalWrite(m2B, LOW);
    }

    void stop() {
        digitalWrite(m1F, LOW);
        digitalWrite(m1B, LOW);
        digitalWrite(m2F, LOW);
        digitalWrite(m2B, LOW);
    }

    void moveForeward() {
        digitalWrite(m1F, HIGH);
        digitalWrite(m1B, LOW);
        digitalWrite(m2F, HIGH);
        digitalWrite(m2B, LOW);
    }

  void moveBackward() {
        digitalWrite(m1F, LOW);
        digitalWrite(m1B, HIGH);
        digitalWrite(m2F, LOW);
        digitalWrite(m2B, HIGH);
    }

    void moveLeft() {
        digitalWrite(m1B, LOW);
        digitalWrite(m2B, LOW);

        analogWrite(m1F, 180);
        digitalWrite(m2F, HIGH);
    }

    void turnLeft() {
        digitalWrite(m1F, LOW);
        digitalWrite(m1B, HIGH);
        digitalWrite(m2F, HIGH);
        digitalWrite(m2B, LOW);
    }

    void moveRight() {
        digitalWrite(m1B, LOW);
        digitalWrite(m2B, LOW);

        digitalWrite(m1F, HIGH);
        analogWrite(m2F, 180);
    }

    void turnRight() {
        digitalWrite(m1B, LOW);
        digitalWrite(m1F, HIGH);
        digitalWrite(m2B, HIGH);
        digitalWrite(m2F, LOW);
    }

    void moveBackRight() {
        digitalWrite(m1F, LOW);
        digitalWrite(m2F, LOW);

        digitalWrite(m1B, HIGH);
        analogWrite(m2B, 180);
    }

    void moveBackLeft() {
        digitalWrite(m1F, LOW);
        digitalWrite(m2F, LOW);

        digitalWrite(m2B, HIGH);
        analogWrite(m1B, 180);
    }
};

#endif