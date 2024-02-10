#define ABG_IMPLEMENTATION
#define ABG_TIMER1
#define ABG_SYNC_PARK_ROW
#define SPRITESU_IMPLEMENTATION

#include <ArduboyFX.h>  
#include "src/utils/ArduboyG.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "fxdata/fxdata.h"
//#include "fxdata/images/Images.h"
#include "fxdata/images/Tiles.h"
#include "src/entities/Entities.h"
#include "src/utils/Random.h"

#define SPRITESU_OVERWRITE
#define SPRITESU_PLUSMASK
#define SPRITESU_RECT
#define SPRITESU_FX

#include "src/utils/SpritesU.hpp"

#ifndef DEBUG
ARDUBOY_NO_USB
#endif

extern ArduboyGBase_Config<ABG_Mode::L4_Triplane> a;
decltype(a) a;


#ifdef SOUNDS_SYNTHU
    #define SYNTHU_IMPLEMENTATION
    #define SYNTHU_NUM_CHANNELS 4
    #define SYNTHU_UPDATE_EVERY_N_FRAMES 3
    #define SYNTHU_ENABLE_SFX 1
    #define SYNTHU_FX_READDATABYTES_FUNC FX::readDataBytes
    #include "src/utils/SynthU.hpp"
    #define ABG_TIMER1
#endif

Cookie cookie;
uint8_t frameCount = 0;
Level &level = cookie.level;
// bool rotorsStillTurning = false;

void setup() {

    a.boot();
    abg_detail::send_cmds_prog<0xDB, 0x20>();
    a.startGray();
    
    FX::begin(FX_DATA_PAGE, FX_SAVE_PAGE);
    FX::loadGameState((uint8_t*)&cookie, sizeof(cookie));

    #ifdef SOUNDS_SYNTHU
        audioInit();
        setAudioOn();
    #endif

}

void loop() {

    FX::enableOLED();
    a.waitForNextPlane(BLACK);
    FX::disableOLED();

    #if defined(DEBUG) && defined(DEBUG_GAMESTATE)
        DEBUG_PRINT("GS: ");
        DEBUG_PRINTLN((uint8_t)level.getGameState());
    #endif

    switch (level.getGameState()) {

        // case GameState::Title_Init:
        //     title_Init();
        //     [[fallthrough]];

        // case GameState::Title_Start ... GameState::Title_End:
        //     title(a);
        //     break;

        case GameState::Play_Init:
            play_Init();
            [[fallthrough]];

        case GameState::Play_Start ... GameState::Play_End:
            play(a);
            break;

    }

}
