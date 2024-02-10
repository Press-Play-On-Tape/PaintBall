#pragma once


enum class GameState : uint8_t {

    Title_Init,
    Title_Start,
        Title_SelectFloor_1 = Title_Start,
    Title_End,

    Play_Init,
    Play_Start,
        Play,
    Play_End,

};

enum class Direction : uint8_t {
    North,
    NorthEast,
    East,
    SouthEast,
    South,
    SouthWest,
    West,
    NorthWest,
};

enum class Movement : uint8_t {
    Up_02_06,    // right, up
    Up_03_05,
    Up_04_04, 
    Up_05_03,
    Up_06_02,
    Up_07_01,
    Middle,
    Down_07_01,
    Down_06_02,
    Down_05_03,
    Down_04_04,
    Down_03_05,
    Down_02_06,
};


enum class PlayerMovement : uint8_t {
    Up_Left_12_12,
    Up_Left_10_14,
    Up_Left_08_16,
    Up_Left_06_18, 
    Up_Left_04_20,
    Up_Left_02_22,    // right, up
    Up_Middle,
    Up_Right_02_22,    // right, up
    Up_Right_04_20,
    Up_Right_06_18, 
    Up_Right_08_16,
    Up_Right_10_14,
    Up_Right_12_12,
    Down_Left_12_12,
    Down_Left_10_14,
    Down_Left_08_16,
    Down_Left_06_18, 
    Down_Left_04_20,
    Down_Left_02_22,    // right, up
    Down_Middle,
    Down_Right_02_22,    // right, up
    Down_Right_04_20,
    Down_Right_06_18, 
    Down_Right_08_16,
    Down_Right_10_14,
    Down_Right_12_12,

};


enum class Speed : uint8_t {
    Slow,
    Medium,
    Fast,
};

enum class PlayerMode : uint8_t {
    Rotate,
    Moving,
};
