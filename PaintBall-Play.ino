#include <ArduboyFX.h>  
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "fxdata/fxdata.h"
#include "src/entities/Entities.h"
#include "src/utils/SpritesU.hpp"

void play_Init() {

    for (Enemy &enemy : enemies) {

        enemy.setActive(false);
        enemy.setX(0);
        enemy.setY(0);

    }

    launchEnemyCounter = 1;
    launchEnemyCounter_Max = 255;

    initMap();
    launchEnemy();

    player.setX(16 * 40);
    player.setY(Constants::Player_YMax - Constants::Player_Offset);
    player.setPlayerMode(PlayerMode::Rotate);

    gameState = GameState::Play;
    score = 0;
    scorePerPass = 0;
    gameOverCounter = 0;

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



    // Render enemies ..

    for (Enemy &enemy : enemies) {

        if (enemy.isActive()) {

            SpritesU::drawPlusMask(enemy.getX_Screen(), enemy.getY_Screen(), Images::Enemy, currentPlane);

        }
    }


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

        case PlayerMode::Dead:
            
            if (gameOverCounter < 128) {
                gameOverCounter++;
            }
            else {
                SpritesU::drawPlusMaskFX(14, 10, Images::GameOver, currentPlane);
            }
            break;

        default:
            break;

    }


    // Render particles ..

    updateAndRenderParticles(frameCount);

    SpritesU::drawOverwriteFX(128- 25, 0, Images::HUD, ((hudCounter / 3) * 3) + currentPlane);


    uint16_t score1 = score / 10000;
    SpritesU::drawOverwriteFX(128 - 22, 10, Images::Numbers_5x3_1D_WB, (score1 * 3) + currentPlane);
    score1 = (score - (score1 * 10000)) / 100;
    SpritesU::drawOverwriteFX(128 - 18, 10, Images::Numbers_5x3_2D_WB, (score1 * 3) + currentPlane);
    score1 = score % 100;
    SpritesU::drawOverwriteFX(128 - 10, 10, Images::Numbers_5x3_2D_WB, (score1 * 3) + currentPlane);

    score1 = score / 10000;
    SpritesU::drawOverwriteFX(128 - 22, 28, Images::Numbers_5x3_1D_WB, (score1 * 3) + currentPlane);
    score1 = (score - (score1 * 10000)) / 100;
    SpritesU::drawOverwriteFX(128 - 18, 28, Images::Numbers_5x3_2D_WB, (score1 * 3) + currentPlane);
    score1 = score % 100;
    SpritesU::drawOverwriteFX(128 - 10, 28, Images::Numbers_5x3_2D_WB, (score1 * 3) + currentPlane);

}


void play_Update() {

    frameCount++;
    hudCounter = (hudCounter + 1) % (3 * 19);

    uint8_t justPressed = getJustPressedButtons();

    switch (gameState) {

        case GameState::Play:

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
            launchEnemy();

            break;

        default:

            if (justPressed & A_BUTTON) { 
                gameState = GameState::Title_Init;
            }

           break;

    }

}            


void play(ArduboyGBase_Config<ABG_Mode::L4_Triplane> &a) {
    
    if (a.needsUpdate()) play_Update();

    uint8_t currentPlane = a.currentPlane();
    render(currentPlane);
    
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

    if (launchEnemyCounter > 0) launchEnemyCounter--;

    if (launchEnemyCounter == 0) {

        for (Enemy &enemy : enemies) {

            if (!enemy.isActive()) {

                enemy.setActive(true);
                enemy.setX(-120);
                enemy.setY(random(40 + 40, 900 - 40));
                enemy.setMovement(static_cast<Movement>(movement));
                enemy.setSpeed(static_cast<Speed>(speed));

                launchEnemyCounter = launchEnemyCounter_Max;
                if (launchEnemyCounter_Max > 5) launchEnemyCounter_Max--;
                break;

            }

        }

    }
    
}


void updateEnemies(uint8_t frameCount) {

    for (Enemy &enemy : enemies) {

        if (enemy.isActive()) {

            bool endOfGame = updateEnemy(enemy, frameCount);

            if (endOfGame) {

                launchParticles(player.getX_Screen() - gridPosition + 4, player.getY_Screen() + 4);
                player.setPlayerMode(PlayerMode::Dead);
                gameState = GameState::Play_EndOfGame;

            }   
            else {
                        
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
    
}




void setGrid(int8_t x, int8_t y, CellOwner owner) {

    if (x < 0 || x >= Constants::MapTileWidth)  return;
    if (y < 0 || y >= Constants::MapTileHeight) return;

    switch (owner) {

        case CellOwner::Enemy:
            if (grid[y][x] < static_cast<uint8_t>(CellOwner::Explode)) {
                grid[y][x] = static_cast<uint8_t>(owner);
            }
            break;

        case CellOwner::Player:

            if (grid[y][x] == static_cast<uint8_t>(CellOwner::Enemy)) {

                scorePerPass++;

           
// Serial.print(x);
// Serial.print(",");
// Serial.print(y);
// Serial.print(" - ");
// Serial.print(grid[y][x]);
// Serial.print(" - ");

                score = score + scorePerPass;
                grid[y][x] = static_cast<uint8_t>(CellOwner::Explode);
// Serial.print(grid[y][x]);
// Serial.print(" - ");
// Serial.println(scorePerPass);   
// printGrid();             


            }
            else if (grid[y][x] > static_cast<uint8_t>(CellOwner::Enemy)) {
                // Do nothing
            }
            else {
                grid[y][x] = static_cast<uint8_t>(CellOwner::Player);
            }

            break;

        default:
            break;

    }
    
}
