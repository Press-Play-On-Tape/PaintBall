#pragma once

#include <Arduboy2.h>
#include "../utils/Enums.h"
#include "../utils/Enums.h"
#include "../../fxdata/fxdata.h"

class Player {

    private:

        // Direction direction = Direction::East;

        PlayerMode playerMode = PlayerMode::Rotate;
        PlayerMovement playerMovement = PlayerMovement::Up_Middle;

        uint8_t direction = 6;
        int8_t directionMovement = 1;
        int16_t x = 40 * 16;
        int16_t y = Constants::Player_YMax - Constants::Player_Offset;
        

    public:

        PlayerMovement getMovement()                    { return this->playerMovement; }
        PlayerMode getPlayerMode()                      { return this->playerMode; }
        uint8_t getDirection()                          { return this->direction; }
        int8_t getDirectionMovement()                   { return this->directionMovement; }
        int8_t getX_Screen()                            { return this->x / 16; }        
        int8_t getY_Screen()                            { return this->y / 16; }        
        int16_t getX()                                  { return this->x; }        
        int16_t getY()                                  { return this->y; }        

        void setMovement(PlayerMovement val)            { this->playerMovement = val; }
        void setDirectionMovement(int8_t val)           { this->directionMovement = val; }
        void setPlayerMode(PlayerMode val)              { this->playerMode = val; }
        void setDirection(uint8_t val)                  { this->direction = val; }
        void setX(int16_t val)                          { this->x = val; }
        void setY(int16_t val)                          { this->y = val; }

    public:

        void shoot() {

            this->playerMode = PlayerMode::Moving;


            this->playerMovement = static_cast<PlayerMovement>(this->direction + (this->y <= 8 ? 12 : 0));

            if (static_cast<int16_t>(this->y) >= static_cast<int16_t>(Constants::Player_YMax - Constants::Player_Offset)) {
                this->y = this->y + Constants::Player_Offset;

            }

        }

//         void update(uint8_t frameCount, uint8_t gridPosition, uint8_t grid[Constants::MapTileHeight][Constants::MapTileWidth]) {

//             switch (this->playerMode) {

//                 case PlayerMode::Rotate:

//                     if (frameCount % 6 != 0) return;

//                     switch (this->directionMovement) {

//                         case -1:

//                             this->direction--;

//                             if (this->direction == 0) {

//                                 this->directionMovement = 1;

//                             }

//                             break;

//                         case 1:

//                             this->direction++;

//                             if (this->direction == 12) {

//                                 this->directionMovement = -1;

//                             }

//                             break;

//                     }
// // Serial.print("movement ");
// // Serial.println(this->direction);
//                     break;





//                 case PlayerMode::Moving:

//                     this->x = this->x + Constants::Player_Movement_X[static_cast<uint8_t>(this->playerMovement)];
//                     this->y = this->y + Constants::Player_Movement_Y[static_cast<uint8_t>(this->playerMovement)];


//                     if (this->x <= Constants::Player_XMin || this->x > Constants::Player_XMax) {
                        
//                         if (this->x <= Constants::Player_XMin) {
//                             this->x = Constants::Player_XMin;
//                         }
                        
//                         if (this->x > Constants::Player_XMax) {
//                             this->x = Constants::Player_XMax;
//                         }

//                         this->playerMovement = static_cast<PlayerMovement>(Constants::Player_AltDirection[static_cast<uint8_t>(this->playerMovement)]);
                       
//                     }


//                     if (this->y <= Constants::Player_YMin) {

//                         this->playerMode = PlayerMode::Rotate;
//                         this->direction = 6;
//                         y = Constants::Player_YMin;

//                     }

//                     else if (this->y >= Constants::Player_YMax) {

//                         this->y = Constants::Player_YMax;
//                         this->playerMode = PlayerMode::Rotate;
//                         this->direction = 6;
//                         this->y = this->y - Constants::Player_Offset;

//                     }
                   
//                     if (this->getX_Screen() >= -4) {
                            
//                         if (gridPosition == 0) {

//                             int8_t x = (this->getX_Screen() - gridPosition + 1) / 6;
//                             int8_t y = (this->getY_Screen() - 1 + 1) / 6;
//                             setGrid_Player(grid, x + 1, y);

//                             x = (this->getX_Screen() - gridPosition + Constants::Player_Width + 1) / 6;
//                             y = (this->getY_Screen() - 1 + 1) / 6;
//                             setGrid_Player(grid, x + 1, y);

//                             x = (this->getX_Screen() - gridPosition + 1) / 6;
//                             y = (this->getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
//                             setGrid_Player(grid, x + 1, y);

//                             x = (this->getX_Screen() - gridPosition + Constants::Player_Width + 1) / 6;
//                             y = (this->getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
//                             setGrid_Player(grid, x + 1, y);

//                         }
//                         else {

//                             int8_t x = (this->getX_Screen() - gridPosition + 6 + 1) / 6;
//                             int8_t y = (this->getY_Screen() - 1 + 1) / 6;
//                             setGrid_Player(grid, x, y);

//                             x = (this->getX_Screen() - gridPosition + 6 + Constants::Player_Width + 1) / 6;
//                             y = (this->getY_Screen() - 1 + 1) / 6;
//                             setGrid_Player(grid, x, y);

//                             x = (this->getX_Screen() - gridPosition + 6 + 1) / 6;
//                             y = (this->getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
//                             setGrid_Player(grid, x, y);

//                             x = (this->getX_Screen() - gridPosition + 6 + Constants::Player_Width + 1) / 6;
//                             y = (this->getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
//                             setGrid_Player(grid, x, y);

//                         }

//                     }                    

//                     break;

//             }

//         }


//         void setGrid_Player(uint8_t grid[Constants::MapTileHeight][Constants::MapTileWidth], int8_t x, int8_t y) {

//             if (x < 0 || x >= Constants::MapTileWidth)  return;
//             if (y < 0 || y >= Constants::MapTileHeight) return;

//             if (grid[y][x] >= 2) {
//                 grid[y][x] = 3;
// // Serial.println("here");                
//             }
//             else {
//                 grid[y][x] = 1;
//             }
            
//         }

};