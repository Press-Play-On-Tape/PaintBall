#include <ArduboyFX.h>  
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "fxdata/fxdata.h"
#include "src/entities/Entities.h"
#include "src/utils/SpritesU.hpp"

bool d = false;
void play_Init() {

    level.initMap();
    level.setGameState(GameState::Play);
    level.launchEnemy();

}

void render(uint8_t currentPlane) {

    //SpritesU::drawOverwrite(0, 0, Images::Grid, (level.getGridPosition() * 3) + currentPlane);

    for (uint8_t y = 0; y < Constants::MapTileHeight; y++) {

        for (uint8_t x = 0; x < Constants::MapTileWidth; x++) {

            uint8_t c = 0;

            if (level.grid[y][x] <= 2) { // normal tiles

                for (uint8_t z = x; z < Constants::MapTileWidth; z++) {

                    if (level.grid[y][x] == level.grid[y][z]) {

                        c++;

                    }
                    else {

                        break;

                    }

                    if (c == 8) break;

                }


                

                uint24_t imageIdx = Images::Tiles[(level.grid[y][x] * 8) + c - 1];
            
                SpritesU::drawOverwrite(x * 6 + level.getGridPosition() - 6, (y * 6) + 1, imageIdx, currentPlane);

                x = x + c - 1;

            }
            else {

                uint8_t imageIdx = (level.grid[y][x] - 3) / 2;
// Serial.println(imageIdx)            ;
                SpritesU::drawPlusMask(x * 6 + level.getGridPosition() - 6 - 4, (y * 6) + 1 - 4, Images::Tile_Exploding, (imageIdx * 3) + currentPlane);

                level.grid[y][x] = level.grid[y][x] + 1;

                if (level.grid[y][x] == 2 + (7 * 2)) { 
                    level.grid[y][x] = 1;
                }

            }

        }

    }

    SpritesU::fillRect_i8(0, 62, 128, 8, BLACK);

    SpritesU::drawOverwriteFX(128- 25, 0, Images::HUD, currentPlane);


    //SpritesU::drawOverwrite(0 - 1 + level.getGridPosition(), 1, Images::Tile_01_08, currentPlane);


    switch (level.getPlayer().getPlayerMode()) {

        case PlayerMode::Rotate:
            {
                uint8_t offset = level.getPlayer().getY_Screen() > 40 ? 13 : 0;
                uint24_t imageIdx = FX::readIndexedUInt24(Images::Player_Rotate, level.getPlayer().getDirection() + offset);
                int8_t playerOffset_X = Constants::PlayerOffset_X[level.getPlayer().getDirection()];
                SpritesU::drawPlusMaskFX(level.getPlayer().getX_Screen() + playerOffset_X, level.getPlayer().getY_Screen(), imageIdx, currentPlane);
            }
            
            break;

        case PlayerMode::Moving:
            {
                SpritesU::drawPlusMaskFX(level.getPlayer().getX_Screen(), level.getPlayer().getY_Screen(), Images::Player, currentPlane);
            }
            
            break;

    }



    for (Enemy &enemy : level.enemies) {

        if (enemy.isActive()) {

            SpritesU::drawPlusMask(enemy.getX_Screen(), enemy.getY_Screen(), Images::Enemy, currentPlane);
        }
    }

    updateAndRenderParticles();


}


void play_Update() {

    frameCount++;

    uint8_t justPressed = getJustPressedButtons();

    if (justPressed & B_BUTTON) { 
        level.launchEnemy();
    }

    if (justPressed & A_BUTTON) { 
        level.getPlayer().shoot();
    }

    level.incGridPosition(frameCount);
    level.updatePlayer(frameCount);
    level.updateEnemies(frameCount);

}            




void play(ArduboyGBase_Config<ABG_Mode::L4_Triplane> &a) {
    
    if (a.needsUpdate()) play_Update();

    uint8_t currentPlane = a.currentPlane();
    
    switch (level.getGameState()) {

        case GameState::Play:

            render(currentPlane);
            break;

        default:
            break;

    }

    //level.updateRotators();

}


void updateAndRenderParticles() {

    for ( int i = 0; i < Constants::ParticlesMax; i++) {

        level.particles[i].update();

        if ( level.particles[i].render() ) { // the dot should be rendered

            // uint8_t pSize = level.particles[i].getSize();

Serial.println(level.particles[i].getX());

            // if (pSize == 1) {
                ArduboyG::drawPixel(level.particles[i].getX(), level.particles[i].getY(), WHITE);
            // } 
            // else {
                // arduboy.drawRect( particles[i].getX(), particles[i].getY(), pSize, pSize, 1);
            // }

        }

    }

}
