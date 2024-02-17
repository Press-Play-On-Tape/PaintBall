#pragma once

#include <ArduboyFX.h>  

struct Settings {

    uint8_t music;
    uint8_t sfx;

};

struct Cookie {

    bool hasSavedGame;
    SoundSettings soundSettings;
    uint16_t score = 0;
    uint16_t highScore = 0;

};
