#pragma once

#include <Arduboy2.h>
#include "Entities.h"
#include "Player.h"
#include "../utils/Enums.h"
#include "../../fxdata/fxdata.h"
#include "../utils/Random.h"


class Level {

    private:

        GameState gameState = GameState::Play_Init;
        Player player;

        uint8_t gridPosition = 0;

    public:

        uint8_t grid[Constants::MapTileHeight][Constants::MapTileWidth];
        Enemy enemies[Constants::EnemyCount];
        Particle particles[Constants::ParticlesMax];

    public:

        GameState getGameState()                                        { return this->gameState; }
        Player &getPlayer()                                             { return this->player; }
        uint8_t getGridPosition()                                       { return this->gridPosition; }

        void setGameState(GameState val)                                { this->gameState = val; }
        void setGridPosition(uint8_t val)                               { this->gridPosition = val; }


    public:

        void initMap() {

            for (uint8_t y = 0; y < Constants::MapTileHeight; y++) {

                for (uint8_t x = 0; x < Constants::MapTileWidth; x++) {

                    grid[y][x] = 0;

                }

            }

        }

        void incGridPosition(uint8_t frameCount) {

            if (frameCount % 24 == 0) {

                this->gridPosition++;

                if (this->gridPosition == 6) {

                    this->gridPosition = 0;

                    for (uint8_t y = 0; y < Constants::MapTileHeight; y++) {

                        for (int8_t x = Constants::MapTileWidth - 2; x >= 0; x--) {

                            grid[y][x + 1] = grid[y][x];

                        }

                    }

                    for (uint8_t y = 0; y < Constants::MapTileHeight; y++) {

                        grid[y][0] = 0;

                    }
//                             for (Enemy &enemy : this->enemies) {
                                
//                                 if (enemy.isActive()) {
//                                     enemy.setX(enemy.getX() + 8);
// Serial.print("L1 ");
// Serial.println(enemy.getX());
//                                 }

//                             }
                }


                for (Enemy &enemy : this->enemies) {
                    
                    if (enemy.isActive()) {
                        enemy.setX(enemy.getX() + 16);
                    }

                }

            }

        }



        void launchEnemy() {

            uint8_t movement = random(0, static_cast<uint8_t>(Movement::Down_02_06) + 1);
            uint8_t speed = random(0, static_cast<uint8_t>(Speed::Fast) + 1);

            this->enemies[0].setActive(true);
            this->enemies[0].setX(-120);
            this->enemies[0].setY(random(40, 900));
            this->enemies[0].setY(40);
            this->enemies[0].setMovement(static_cast<Movement>(movement));
            this->enemies[0].setSpeed(static_cast<Speed>(speed));
            // Serial.println(speed);
            
            for (uint8_t y = 0; y < Constants::MapTileHeight; y++) {

                for (uint8_t x = 0; x < Constants::MapTileWidth; x++) {

                    //grid[y][x] = random(0, 3);
                    // grid[y][x] = 1;//random(0, 3);
                    grid[y][x] = 0;

                }

            }


        }

        void updateEnemies(uint8_t frameCount) {

            for (Enemy &enemy : this->enemies) {

                if (enemy.isActive()) {

                    enemy.update(frameCount);
                    
                   
                    if (enemy.getX_Screen() >= -4) {
                            
                        if (gridPosition == 0) {

                            int8_t x = (enemy.getX_Screen() - gridPosition) / 6;
                            int8_t y = (enemy.getY_Screen() - 1) / 6;
                            setGrid(x + 1, y, 2);
                            // this->grid[y][x + 1] = 2;

                            x = (enemy.getX_Screen() - gridPosition + Constants::EnemyWidth) / 6;
                            y = (enemy.getY_Screen() - 1) / 6;
                            setGrid(x + 1, y, 2);
                            // this->grid[y][x + 1] = 2;

                            x = (enemy.getX_Screen() - gridPosition) / 6;
                            y = (enemy.getY_Screen() - 1 + Constants::EnemyWidth) / 6;
                            setGrid(x + 1, y, 2);
                            // this->grid[y][x + 1] = 2;

                            x = (enemy.getX_Screen() - gridPosition + Constants::EnemyWidth) / 6;
                            y = (enemy.getY_Screen() - 1 + Constants::EnemyWidth) / 6;
                            setGrid(x + 1, y, 2);
                            // this->grid[y][x + 1] = 2;

                        }
                        else {

                            int8_t x = (enemy.getX_Screen() - gridPosition + 6) / 6;
                            int8_t y = (enemy.getY_Screen() - 1) / 6;

                            setGrid(x, y, 2);
                            // this->grid[y][x] = 2;

                            x = (enemy.getX_Screen() - gridPosition + 6 + Constants::EnemyWidth) / 6;
                            y = (enemy.getY_Screen() - 1) / 6;
                            setGrid(x, y, 2);
                            // this->grid[y][x] = 2;

                            x = (enemy.getX_Screen() - gridPosition + 6) / 6;
                            y = (enemy.getY_Screen() - 1 + Constants::EnemyWidth) / 6;
                            setGrid(x, y, 2);
                            // this->grid[y][x] = 2;

                            x = (enemy.getX_Screen() - gridPosition + 6 + Constants::EnemyWidth) / 6;
                            y = (enemy.getY_Screen() - 1 + Constants::EnemyWidth) / 6;
                            setGrid(x, y, 2);
                            // this->grid[y][x] = 2;

                        }

                    }


                }

            }
            
        }

        void printGrid() {

            DEBUG_PRINTLN("------------------");

            for (uint8_t y = 0; y < Constants::MapTileHeight; y++) {

                for (uint8_t x = 0; x < Constants::MapTileWidth; x++) {

                    DEBUG_PRINT(grid[y][x]);

                }

                DEBUG_PRINTLN("");

            }

        }

        void setGrid(int8_t x, int8_t y, uint8_t val) {

            if (x < 0 || x >= Constants::MapTileWidth)  return;
            if (y < 0 || y >= Constants::MapTileHeight) return;

            this->grid[y][x] = val;
            
        }


        void updatePlayer(uint8_t frameCount) {

            switch (this->getPlayer().getPlayerMode()) {

                case PlayerMode::Rotate:

                    if (frameCount % 6 != 0) return;

                    switch (this->getPlayer().getDirectionMovement()) {

                        case -1:

                            this->getPlayer().setDirection(this->getPlayer().getDirection() - 1);

                            if (this->getPlayer().getDirection() == 0) {

                                this->getPlayer().setDirectionMovement(1);

                            }

                            break;

                        case 1:

                            this->getPlayer().setDirection(this->getPlayer().getDirection() + 1);

                            if (this->getPlayer().getDirection() == 12) {

                                this->getPlayer().setDirectionMovement(-1);

                            }

                            break;

                    }
// Serial.print("movement ");
// Serial.println(this->direction);
                    break;


                case PlayerMode::Moving:

                    this->getPlayer().setX(this->getPlayer().getX() + Constants::Player_Movement_X[static_cast<uint8_t>(this->getPlayer().getMovement())]);
                    this->getPlayer().setY(this->getPlayer().getY() + Constants::Player_Movement_Y[static_cast<uint8_t>(this->getPlayer().getMovement())]);


                    if (this->getPlayer().getX() <= Constants::Player_XMin || this->getPlayer().getX()> Constants::Player_XMax) {
                        
                        if (this->getPlayer().getX() <= Constants::Player_XMin) {
                            this->getPlayer().setX(Constants::Player_XMin);
                        }
                        
                        if (this->getPlayer().getX() > Constants::Player_XMax) {
                            this->getPlayer().setX(Constants::Player_XMax);
                        }

                        this->getPlayer().setMovement(static_cast<PlayerMovement>(Constants::Player_AltDirection[static_cast<uint8_t>(this->getPlayer().getMovement())]));
                       
                    }


                    if (this->getPlayer().getY() <= Constants::Player_YMin) {

                        this->getPlayer().setPlayerMode(PlayerMode::Rotate);
                        this->getPlayer().setDirection(6);
                        this->getPlayer().setY(Constants::Player_YMin);

                    }

                    else if (this->getPlayer().getY() >= Constants::Player_YMax) {

                        this->getPlayer().setY(Constants::Player_YMax);
                        this->getPlayer().setPlayerMode(PlayerMode::Rotate);
                        this->getPlayer().setDirection(6);
                        this->getPlayer().setY(this->getPlayer().getY() - Constants::Player_Offset);

                    }
                   
                    if (this->getPlayer().getX_Screen() >= -4) {
                            
                        if (gridPosition == 0) {

                            int8_t x = (this->getPlayer().getX_Screen() - this->gridPosition + 1) / 6;
                            int8_t y = (this->getPlayer().getY_Screen() - 1 + 1) / 6;
                            setGrid_Player(x + 1, y);

                            x = (this->getPlayer().getX_Screen() - this->gridPosition + Constants::Player_Width + 1) / 6;
                            y = (this->getPlayer().getY_Screen() - 1 + 1) / 6;
                            setGrid_Player(x + 1, y);

                            x = (this->getPlayer().getX_Screen() - this->gridPosition + 1) / 6;
                            y = (this->getPlayer().getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
                            setGrid_Player(x + 1, y);

                            x = (this->getPlayer().getX_Screen() - this->gridPosition + Constants::Player_Width + 1) / 6;
                            y = (this->getPlayer().getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
                            setGrid_Player(x + 1, y);

                        }
                        else {

                            int8_t x = (this->getPlayer().getX_Screen() - this->gridPosition + 6 + 1) / 6;
                            int8_t y = (this->getPlayer().getY_Screen() - 1 + 1) / 6;
                            setGrid_Player(x, y);

                            x = (this->getPlayer().getX_Screen() - this->gridPosition + 6 + Constants::Player_Width + 1) / 6;
                            y = (this->getPlayer().getY_Screen() - 1 + 1) / 6;
                            setGrid_Player(x, y);

                            x = (this->getPlayer().getX_Screen() - this->gridPosition + 6 + 1) / 6;
                            y = (this->getPlayer().getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
                            setGrid_Player(x, y);

                            x = (this->getPlayer().getX_Screen() - this->gridPosition + 6 + Constants::Player_Width + 1) / 6;
                            y = (this->getPlayer().getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
                            setGrid_Player(x, y);

                        }

                    }      


                    // Collission with an enemy?

                    Rect recPlayer;
                    recPlayer.x = (this->getPlayer().getX_Screen() - this->gridPosition);
                    recPlayer.y = (this->getPlayer().getY_Screen() - 1);
                    recPlayer.width = Constants::Player_Width_Full;
                    recPlayer.height = Constants::Player_Width_Full;

                    for (Enemy &enemy : this->enemies) {

                        if (enemy.isActive()) {

                            Rect recEnemy;
                            recEnemy.x = (enemy.getX_Screen() - this->gridPosition);
                            recEnemy.y = (enemy.getY_Screen() - 1);
                            recEnemy.width = Constants::EnemyWidth;
                            recEnemy.height = Constants::EnemyWidth;

                            if (Arduboy2::collide(recPlayer, recEnemy)) {

Serial.print(recPlayer.x);
Serial.print(",");
Serial.print(recPlayer.y);
Serial.print(",");
Serial.print(recPlayer.width);
Serial.print(",");
Serial.print(recPlayer.height);

Serial.print(" - ");

Serial.print(recEnemy.x);
Serial.print(",");
Serial.print(recEnemy.y);
Serial.print(",");
Serial.print(recEnemy.width);
Serial.print(",");
Serial.println(recEnemy.height);

                                launchParticles(recEnemy.x, recEnemy.y);
                                enemy.setActive(false);
                            }
                        }
                    }

                    break;

            }

        }


        void setGrid_Player(int8_t x, int8_t y) {

            if (x < 0 || x >= Constants::MapTileWidth)  return;
            if (y < 0 || y >= Constants::MapTileHeight) return;

            if (this->grid[y][x] >= 2) {
                this->grid[y][x] = 3;
            }
            else {
                this->grid[y][x] = 1;
            }
            
        }

        void launchParticles(int16_t x, int16_t y) {
Serial.println("launch");

            for (int i = 0; i < Constants::ParticlesMax; i++) {

                particles[i].setX(x);
                particles[i].setY(y);
                particles[i].setVelX(random(-4, 5));
                particles[i].setVelY(random(-1, 6));
                particles[i].setCounter(random(10, 46));
                particles[i].setSize(random(1, 3));
            
            }

        }

};