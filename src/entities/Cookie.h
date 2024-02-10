#pragma once

#include <ArduboyFX.h>  
#include "Level.h"

#ifdef SOUNDS_SYNTHU
    struct Settings {

        uint8_t music;
        uint8_t sfx;

    };
#endif


struct Cookie {

    bool hasSavedGame;
    Level level;
    
    #ifdef SOUNDS_SYNTHU
        Settings settings;
    #endif

};
