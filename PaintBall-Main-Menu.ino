#include <ArduboyFX.h>  
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "fxdata/fxdata.h"
#include "src/entities/Entities.h"
#include "src/utils/SpritesU.hpp"

void title_Init() {

    gameState = GameState::Title_OptPlay;

}

void title_Update() {

    frameCount++;

    uint8_t justPressed = getJustPressedButtons();
    uint8_t pressed = getPressedButtons();
    // uint8_t activePlayer = level.getActivePlayer();

    switch (gameState) {

        #ifndef DEBUG        

            case GameState::Title_ShowCredits:

                if (justPressed & B_BUTTON || justPressed & A_BUTTON) {

                    gameState = GameState::Title_OptCredits;

                }

                break;

        #endif
        
        case GameState::Title_OptPlay:

            if (justPressed & RIGHT_BUTTON) {
                gameState = GameState::Title_OptSound;
            }

            if (justPressed & A_BUTTON) {

                gameState = GameState::Play_Init;

            }

            break;

        #ifndef DEBUG
            
            case GameState::Title_OptSound:

                if (justPressed & LEFT_BUTTON) {
                    gameState = GameState::Title_OptPlay;
                }

                if (justPressed & RIGHT_BUTTON) {
                    gameState = GameState::Title_OptCredits;
                }

                if (justPressed & A_BUTTON) {

                    gameState = GameState::Title_OptSound_Music;

                }

                break;
            
            case GameState::Title_OptCredits:

                if (justPressed & LEFT_BUTTON) {
                    gameState = GameState::Title_OptSound;
                }

                if (justPressed & A_BUTTON) {
                    gameState = GameState::Title_ShowCredits;
                }

                break;
            
            case GameState::Title_OptSound_Music:
                
                if (justPressed & A_BUTTON) {

                    #ifdef SOUNDS_SYNTHU

                        // level.getSoundSettings().setMusic(!level.getSoundSettings().getMusic());

                        // if (level.getSoundSettings().getMusic()) {
                        //     playMusic();
                        // }
                        // else {
                        //     SynthU::stop();
                        // }

                    #endif

                }
                
                if (justPressed & B_BUTTON) {

                    gameState = GameState::Title_OptSound;

                }
                
                if (justPressed & DOWN_BUTTON) {

                    gameState = static_cast<GameState>(static_cast<uint8_t>(gameState) + 1);

                }

                break;
            
            case GameState::Title_OptSound_SFX:
                
                if (justPressed & A_BUTTON) {

                    #ifdef SOUNDS_SYNTHU

                        // level.getSoundSettings().setSFX(!level.getSoundSettings().getSFX());

                    #endif

                }
                
                if (justPressed & B_BUTTON) {

                    gameState = GameState::Title_OptSound;

                }
                
                if (justPressed & UP_BUTTON) {

                    gameState = static_cast<GameState>(static_cast<uint8_t>(gameState) - 1);

                }
                
                if (justPressed & DOWN_BUTTON) {

                    gameState = static_cast<GameState>(static_cast<uint8_t>(gameState) + 1);

                }

                break;

            case GameState::Title_OptSound_Volume:
                
                if (justPressed & LEFT_BUTTON) {

                    #ifdef SOUNDS_SYNTHU

                        // if (level.getSoundSettings().getVolume() > 0) {

                        //     level.getSoundSettings().setVolume(level.getSoundSettings().getVolume() - 1);

                        //     #ifdef SOUNDS_SYNTHU
                        //         SynthU::setVolume(level.getSoundSettings().getVolume());
                        //     #endif

                        // }

                    #endif

                }
                
                if (justPressed & RIGHT_BUTTON) {

                    #ifdef SOUNDS_SYNTHU

                    //     if (level.getSoundSettings().getVolume() < 7) {

                    //         level.getSoundSettings().setVolume(level.getSoundSettings().getVolume() + 1);

                    //         #ifdef SOUNDS_SYNTHU
                    //             SynthU::setVolume(level.getSoundSettings().getVolume());
                    //         #endif

                    //     }

                    #endif

                }

                if (justPressed & B_BUTTON) {

                    gameState = GameState::Title_OptSound;

                }
                
                if (justPressed & UP_BUTTON) {

                    gameState = static_cast<GameState>(static_cast<uint8_t>(gameState) - 1);

                }

                break;

        #endif
        

    }
       

}

void title(ArduboyGBase_Config<ABG_Mode::L4_Triplane> &a) {
    
    if (a.needsUpdate()) title_Update();

    uint8_t currentPlane = a.currentPlane();
    uint8_t idx = static_cast<uint8_t>(gameState) - static_cast<uint8_t>(GameState::Title_Start);

    SpritesU::drawOverwriteFX(0, 0, Images::Title, (3 * idx) + currentPlane);

    switch (gameState) {
        
        case GameState::Title_OptPlay ... GameState::Title_OptSound_Volume:
        // case GameState::Title_SelectSkill_3_1 ... GameState::Title_SelectSkill_2_2:
        // case GameState::Title_FloorsComplete ... GameState::Title_FloorsComplete2:
            break;

        // case GameState::Title_SelectFloor_1 ... GameState::Title_SelectFloor_10:
        //     {
        //         uint8_t levelTickStart = Constants::Level_Tick_Start[idx];

        //         for (uint8_t i = 0; i < 4; i++) {

        //             if (level.getLevelComplete((static_cast<uint8_t>(level.getSkillLevel()) * 10) + levelTickStart + i)) {

        //                 SpritesU::drawPlusMaskFX(94, 30 + (i * 7), Images::Tick, currentPlane);

        //             }

        //         }

        //     }

        //     break;


        // case GameState::Title_OptSound_Music ... GameState::Title_OptSound_Volume:
        //     {

        //         if (level.getSoundSettings().getMusic())    SpritesU::drawPlusMaskFX(25, 40, Images::Sound_Checkbox, currentPlane);
        //         if (level.getSoundSettings().getSFX())      SpritesU::drawPlusMaskFX(25, 49, Images::Sound_Checkbox, currentPlane);

        //         uint8_t volume = (level.getSoundSettings().getMusic() || level.getSoundSettings().getSFX()) ? level.getSoundSettings().getVolume() : 0;

        //         if (level.getSoundSettings().getMusic() || level.getSoundSettings().getSFX()) {
        //             SpritesU::drawOverwriteFX(84, 39, Images::Sound_Volume, (level.getSoundSettings().getVolume() * 3) + currentPlane);
        //         }
        //         else {
        //             SpritesU::drawOverwriteFX(84, 39, Images::Sound_Volume, currentPlane);
        //         }

        //     }
        //     break;            

    }

}
