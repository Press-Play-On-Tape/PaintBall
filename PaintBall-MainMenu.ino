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

                    soundSettings.setMusic(!soundSettings.getMusic());
                    if (soundSettings.getMusic()) {
                        playMusic();
                    }
                    else {
                        SynthU::stop();
                    }

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

                    soundSettings.setSFX(!soundSettings.getSFX());

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

                    if (soundSettings.getVolume() > 0) {

                        soundSettings.setVolume(soundSettings.getVolume() - 1);
                        SynthU::setVolume(soundSettings.getVolume());

                    }

                }
                
                if (justPressed & RIGHT_BUTTON) {

                    if (soundSettings.getVolume() < 7) {

                        soundSettings.setVolume(soundSettings.getVolume() + 1);
                        SynthU::setVolume(soundSettings.getVolume());

                    }

                }

                if (justPressed & B_BUTTON) {

                    gameState = GameState::Title_OptSound;
                    saveCookie(true);

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
        
        case GameState::Title_OptSound_Music ... GameState::Title_OptSound_Volume:
            {

                if (soundSettings.getMusic())    SpritesU::drawPlusMaskFX(25, 40, Images::Sound_Checkbox, currentPlane);
                if (soundSettings.getSFX())      SpritesU::drawPlusMaskFX(25, 49, Images::Sound_Checkbox, currentPlane);

                uint8_t volume = (soundSettings.getMusic() || soundSettings.getSFX()) ? soundSettings.getVolume() : 0;

                if (soundSettings.getMusic() || soundSettings.getSFX()) {
                    SpritesU::drawOverwriteFX(84, 39, Images::Sound_Volume, (soundSettings.getVolume() * 3) + currentPlane);
                }
                else {
                    SpritesU::drawOverwriteFX(84, 39, Images::Sound_Volume, currentPlane);
                }

            }
            break;        

        default:
            break;    

    }

}
