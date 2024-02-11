#include <ArduboyFX.h>  
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "fxdata/fxdata.h"
#include "src/entities/Entities.h"
#include "src/utils/SpritesU.hpp"

void play_Init() {

    initMap();
    launchEnemy();

    gameState = GameState::Play;
    score = 0;
    scorePerPass = 0;

}

void render(uint8_t currentPlane) {


    // Render Map ..

    for (uint8_t y = 0; y < Constants::MapTileHeight; y++) {

        for (uint8_t x = 0; x < Constants::MapTileWidth; x++) {

            uint8_t c = 0;

            if (grid[y][x] <= 2) { // normal tiles

                for (uint8_t z = x; z < Constants::MapTileWidth; z++) {

                    if (grid[y][x] == grid[y][z]) {

                        c++;

                    }
                    else {

                        break;

                    }

                    if (c == 8) break;

                }

                uint24_t imageIdx = Images::Tiles[(grid[y][x] * 8) + c - 1];
                SpritesU::drawOverwrite(x * 6 + gridPosition - 6, (y * 6) + 1, imageIdx, currentPlane);

                x = x + c - 1;

            }
            else {


                // Exploding cell (when the player crosses an enemy cell)..

                uint8_t imageIdx = (grid[y][x] - 3) / 2;
                SpritesU::drawPlusMask(x * 6 + gridPosition - 6 - 4, (y * 6) + 1 - 4, Images::Tile_Exploding, (imageIdx * 3) + currentPlane);


                // Increase the cell value until it reaches the max ..
                grid[y][x] = grid[y][x] + 1;

                if (grid[y][x] == 2 + (7 * 2)) { 
                    grid[y][x] = 1;
                }

            }

        }

    }

    SpritesU::fillRect_i8(0, 62, 128, 8, BLACK);
    SpritesU::drawOverwriteFX(128- 25, 0, Images::HUD, currentPlane);


    // Render player ..

    switch (player.getPlayerMode()) {

        case PlayerMode::Rotate:
            {
                uint8_t offset = player.getY_Screen() > 40 ? 13 : 0;
                uint24_t imageIdx = FX::readIndexedUInt24(Images::Player_Rotate, player.getDirection() + offset);
                int8_t playerOffset_X = Constants::PlayerOffset_X[player.getDirection()];
                SpritesU::drawPlusMaskFX(player.getX_Screen() + playerOffset_X, player.getY_Screen(), imageIdx, currentPlane);
            }
            
            break;

        case PlayerMode::Moving:
            
            SpritesU::drawPlusMaskFX(player.getX_Screen(), player.getY_Screen(), Images::Player, currentPlane);
            break;

    }


    // Render enemies ..

    for (Enemy &enemy : enemies) {

        if (enemy.isActive()) {

            SpritesU::drawPlusMask(enemy.getX_Screen(), enemy.getY_Screen(), Images::Enemy, currentPlane);

        }
    }


    // Render particles ..

    updateAndRenderParticles(frameCount);


}


void play_Update() {

    frameCount++;

    uint8_t justPressed = getJustPressedButtons();

    if (justPressed & B_BUTTON) { 
        launchEnemy();
    }

    switch (player.getPlayerMode()) {

        case PlayerMode::Rotate:

            if (justPressed & A_BUTTON) { 
                scorePerPass = 0;
                player.shoot();
            }

            break;

    }

    incGridPosition(frameCount);
    updatePlayer(frameCount);
    updateEnemies(frameCount);

}            


void play(ArduboyGBase_Config<ABG_Mode::L4_Triplane> &a) {
    
    if (a.needsUpdate()) play_Update();

    uint8_t currentPlane = a.currentPlane();
    
    switch (gameState) {

        case GameState::Play:

            render(currentPlane);
            break;

        default:
            break;

    }

}


void initMap() {

    for (uint8_t y = 0; y < Constants::MapTileHeight; y++) {

        for (uint8_t x = 0; x < Constants::MapTileWidth; x++) {

            grid[y][x] = 0;

        }

    }

}

void incGridPosition(uint8_t frameCount) {

    if (frameCount % 24 == 0) {

        gridPosition++;

        if (gridPosition == 6) {

            gridPosition = 0;

            for (uint8_t y = 0; y < Constants::MapTileHeight; y++) {

                for (int8_t x = Constants::MapTileWidth - 2; x >= 0; x--) {

                    grid[y][x + 1] = grid[y][x];

                }

            }

            for (uint8_t y = 0; y < Constants::MapTileHeight; y++) {

                grid[y][0] = 0;

            }

        }


        // If the grid has moved then move the enemies ..

        for (Enemy &enemy : enemies) {
            
            if (enemy.isActive()) {
                enemy.setX(enemy.getX() + 16);
            }

        }

    }

}



void launchEnemy() {

    uint8_t movement = random(0, static_cast<uint8_t>(Movement::Down_02_06) + 1);
    uint8_t speed = random(0, static_cast<uint8_t>(Speed::Fast) + 1);

    enemies[0].setActive(true);
    enemies[0].setX(-120);
    enemies[0].setY(random(40, 900));
    enemies[0].setY(40);
    enemies[0].setMovement(static_cast<Movement>(movement));
    enemies[0].setSpeed(static_cast<Speed>(speed));
    // Serial.println(speed);
    
    for (uint8_t y = 0; y < Constants::MapTileHeight; y++) {

        for (uint8_t x = 0; x < Constants::MapTileWidth; x++) {

            grid[y][x] = 0;

        }

    }


}

void updateEnemies(uint8_t frameCount) {

    for (Enemy &enemy : enemies) {

        if (enemy.isActive()) {

            enemy.update(frameCount);
            
            
            if (enemy.getX_Screen() >= -4) {
                    
                if (gridPosition == 0) {

                    int8_t x = (enemy.getX_Screen() - gridPosition) / 6;
                    int8_t y = (enemy.getY_Screen() - 1) / 6;
                    setGrid(x + 1, y, CellOwner::Enemy);

                    x = (enemy.getX_Screen() - gridPosition + Constants::EnemyWidth) / 6;
                    y = (enemy.getY_Screen() - 1) / 6;
                    setGrid(x + 1, y, CellOwner::Enemy);

                    x = (enemy.getX_Screen() - gridPosition) / 6;
                    y = (enemy.getY_Screen() - 1 + Constants::EnemyWidth) / 6;
                    setGrid(x + 1, y, CellOwner::Enemy);

                    x = (enemy.getX_Screen() - gridPosition + Constants::EnemyWidth) / 6;
                    y = (enemy.getY_Screen() - 1 + Constants::EnemyWidth) / 6;
                    setGrid(x + 1, y, CellOwner::Enemy);

                }
                else {

                    int8_t x = (enemy.getX_Screen() - gridPosition + 6) / 6;
                    int8_t y = (enemy.getY_Screen() - 1) / 6;
                    setGrid(x, y, CellOwner::Enemy);

                    x = (enemy.getX_Screen() - gridPosition + 6 + Constants::EnemyWidth) / 6;
                    y = (enemy.getY_Screen() - 1) / 6;
                    setGrid(x, y, CellOwner::Enemy);

                    x = (enemy.getX_Screen() - gridPosition + 6) / 6;
                    y = (enemy.getY_Screen() - 1 + Constants::EnemyWidth) / 6;
                    setGrid(x, y, CellOwner::Enemy);

                    x = (enemy.getX_Screen() - gridPosition + 6 + Constants::EnemyWidth) / 6;
                    y = (enemy.getY_Screen() - 1 + Constants::EnemyWidth) / 6;
                    setGrid(x, y, CellOwner::Enemy);

                }

            }

        }

    }
    
}


void updatePlayer(uint8_t frameCount) {

    switch (player.getPlayerMode()) {

        case PlayerMode::Rotate:

            if (frameCount % 6 != 0) return;

            switch (player.getDirectionMovement()) {

                case -1:

                    player.setDirection(player.getDirection() - 1);

                    if (player.getDirection() == 0) {

                        player.setDirectionMovement(1);

                    }

                    break;

                case 1:

                    player.setDirection(player.getDirection() + 1);

                    if (player.getDirection() == 12) {

                        player.setDirectionMovement(-1);

                    }

                    break;

            }
// Serial.print("movement ");
// Serial.println(this->direction);
            break;


        case PlayerMode::Moving:

            player.setX(player.getX() + Constants::Player_Movement_X[static_cast<uint8_t>(player.getMovement())]);
            player.setY(player.getY() + Constants::Player_Movement_Y[static_cast<uint8_t>(player.getMovement())]);


            if (player.getX() <= Constants::Player_XMin || player.getX()> Constants::Player_XMax) {
                
                if (player.getX() <= Constants::Player_XMin) {
                    player.setX(Constants::Player_XMin);
                }
                
                if (player.getX() > Constants::Player_XMax) {
                    player.setX(Constants::Player_XMax);
                }

                player.setMovement(static_cast<PlayerMovement>(Constants::Player_AltDirection[static_cast<uint8_t>(player.getMovement())]));
                
            }


            if (player.getY() <= Constants::Player_YMin) {

                player.setPlayerMode(PlayerMode::Rotate);
                player.setDirection(6);
                player.setY(Constants::Player_YMin);

            }

            else if (player.getY() >= Constants::Player_YMax) {

                player.setY(Constants::Player_YMax);
                player.setPlayerMode(PlayerMode::Rotate);
                player.setDirection(6);
                player.setY(player.getY() - Constants::Player_Offset);

            }
            
            if (player.getX_Screen() >= -4) {
                    
                if (gridPosition == 0) {

                    int8_t x = (player.getX_Screen() - gridPosition + 1) / 6;
                    int8_t y = (player.getY_Screen() - 1 + 1) / 6;
                    setGrid(x + 1, y, CellOwner::Player);

                    x = (player.getX_Screen() - gridPosition + Constants::Player_Width + 1) / 6;
                    y = (player.getY_Screen() - 1 + 1) / 6;
                    setGrid(x + 1, y, CellOwner::Player);

                    x = (player.getX_Screen() - gridPosition + 1) / 6;
                    y = (player.getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
                    setGrid(x + 1, y, CellOwner::Player);

                    x = (player.getX_Screen() - gridPosition + Constants::Player_Width + 1) / 6;
                    y = (player.getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
                    setGrid(x + 1, y, CellOwner::Player);

                }
                else {

                    int8_t x = (player.getX_Screen() - gridPosition + 6 + 1) / 6;
                    int8_t y = (player.getY_Screen() - 1 + 1) / 6;
                    setGrid(x, y, CellOwner::Player);

                    x = (player.getX_Screen() - gridPosition + 6 + Constants::Player_Width + 1) / 6;
                    y = (player.getY_Screen() - 1 + 1) / 6;
                    setGrid(x, y, CellOwner::Player);

                    x = (player.getX_Screen() - gridPosition + 6 + 1) / 6;
                    y = (player.getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
                    setGrid(x, y, CellOwner::Player);

                    x = (player.getX_Screen() - gridPosition + 6 + Constants::Player_Width + 1) / 6;
                    y = (player.getY_Screen() - 1 + Constants::Player_Width + 1) / 6;
                    setGrid(x, y, CellOwner::Player);

                }

            }      


            // Collision with an enemy?

            Rect recPlayer;
            recPlayer.x = (player.getX_Screen() - gridPosition);
            recPlayer.y = (player.getY_Screen() - 1);
            recPlayer.width = Constants::Player_Width_Full;
            recPlayer.height = Constants::Player_Width_Full;

            for (Enemy &enemy : enemies) {

                if (enemy.isActive()) {

                    Rect recEnemy;
                    recEnemy.x = (enemy.getX_Screen() - gridPosition);
                    recEnemy.y = (enemy.getY_Screen() - 1);
                    recEnemy.width = Constants::EnemyWidth;
                    recEnemy.height = Constants::EnemyWidth;

                    if (Arduboy2::collide(recPlayer, recEnemy)) {

                        launchParticles(recEnemy.x + 4, recEnemy.y + 4);
                        enemy.setActive(false);

                    }
                }
            }

            break;

    }

}


void setGrid(int8_t x, int8_t y, CellOwner owner) {

    if (x < 0 || x >= Constants::MapTileWidth)  return;
    if (y < 0 || y >= Constants::MapTileHeight) return;

    switch (owner) {

        case CellOwner::Enemy:
            grid[y][x] = static_cast<uint8_t>(owner);
            break;

        default:

            if (grid[y][x] >= 2) {
                scorePerPass++;
                score = score + scorePerPass;
                grid[y][x] = 3;
            }
            else {
                grid[y][x] = static_cast<uint8_t>(CellOwner::Player);
            }

            break;

    }
    
}
