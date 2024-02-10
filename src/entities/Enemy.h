#pragma once

#include <Arduboy2.h>
#include "../utils/Enums.h"
#include "../../fxdata/fxdata.h"

class Enemy {

    private:

        // Direction direction = Direction::East;

        Movement movement = Movement::Middle;
        Speed speed = Speed::Slow;
        int8_t directionMovement = 1;
        int16_t x = 40;
        int16_t y = 47;
        bool active = false;
        

    public:

        Movement getMovement()                          { return this->movement; }
        Speed getSpeed()                                { return this->speed; }
        int8_t getX_Screen()                            { return this->x / 16; }        
        int8_t getY_Screen()                            { return this->y / 16; }        
        int16_t getX()                                  { return this->x; }        
        int16_t getY()                                  { return this->y; }        
        bool isActive()                                 { return this->active; }        

        void setMovement(Movement val)                  { this->movement = val; }
        void setSpeed(Speed val)                        { this->speed = val; }
        void setX(int16_t val)                          { this->x = val; }
        void setY(int16_t val)                          { this->y = val; }
        void setActive(bool val)                        { this->active = val; }

    public:

        void update(uint8_t frameCount) {

            switch (this->speed) {

                case Speed::Slow:
                    if (frameCount % 4 != 0) return;

                case Speed::Medium:
                    if (frameCount % 3 != 0) return;

                case Speed::Fast:
                    if (frameCount % 2 != 0) return;

            }

            if (!this->active) return;


            this->x = this->x + Constants::Enemy_Movement_X[static_cast<uint8_t>(this->movement)];
            this->y = this->y + Constants::Enemy_Movement_Y[static_cast<uint8_t>(this->movement)];

            if (this->y < 24 || this->y > 924) {

                if (this->y < 24) {

                    this->y = 24;

                }

                if (this->y > 924) {

                    this->y = 924;

                }


Serial.print(this->y);
Serial.print(" ");
Serial.print((uint8_t)this->movement);

                switch (this->movement) {

                    case Movement::Up_02_06:
                        this->movement = Movement::Down_02_06;
                        break;

                    case Movement::Up_03_05:
                        this->movement = Movement::Down_03_05;
                        break;

                    case Movement::Up_04_04:
                        this->movement = Movement::Down_04_04;
                        break;

                    case Movement::Up_05_03:
                        this->movement = Movement::Down_05_03;
                        break;

                    case Movement::Up_06_02:
                        this->movement = Movement::Down_06_02;
                        break;

                    case Movement::Up_07_01:
                        this->movement = Movement::Down_07_01;
                        break;

                    case Movement::Middle:
                        break;

                    case Movement::Down_07_01:
                        this->movement = Movement::Up_07_01;
                        break;

                    case Movement::Down_06_02:
                        this->movement = Movement::Up_06_02;
                        break;

                    case Movement::Down_05_03:
                        this->movement = Movement::Up_05_03;
                        break;

                    case Movement::Down_04_04:
                        this->movement = Movement::Up_04_04;
                        break;

                    case Movement::Down_03_05:
                        this->movement = Movement::Up_03_05;
                        break;

                    case Movement::Down_02_06:
                        this->movement = Movement::Up_02_06;
                        break;

                }
Serial.print(" ");
Serial.println((uint8_t)this->movement);
            }

        }

        
};