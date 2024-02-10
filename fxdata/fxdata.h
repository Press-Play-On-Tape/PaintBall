#pragma once

/**** FX data header generated by fxdata-build.py tool version 1.13 ****/

using uint24_t = __uint24;

// Initialize FX hardware using  FX::begin(FX_DATA_PAGE); in the setup() function.

constexpr uint16_t FX_DATA_PAGE  = 0xffe1;
constexpr uint24_t FX_DATA_BYTES = 3812;

constexpr uint16_t FX_SAVE_PAGE  = 0xfff0;
constexpr uint24_t FX_SAVE_BYTES = 2;

namespace Images
{
  constexpr uint24_t Player_01_00 = 0x000000;
  constexpr uint24_t Player_01_01 = 0x0000C2;
  constexpr uint24_t Player_01_02 = 0x00016C;
  constexpr uint24_t Player_01_03 = 0x0001F2;
  constexpr uint24_t Player_01_04 = 0x000260;
  constexpr uint24_t Player_01_05 = 0x0002AA;
  constexpr uint24_t Player_01_06 = 0x0002E8;
  constexpr uint24_t Player_01_07 = 0x000326;
  constexpr uint24_t Player_01_08 = 0x000364;
  constexpr uint24_t Player_01_09 = 0x0003AE;
  constexpr uint24_t Player_01_10 = 0x00041C;
  constexpr uint24_t Player_01_11 = 0x0004A2;
  constexpr uint24_t Player_01_12 = 0x00054C;
  constexpr uint24_t Player_00_00 = 0x00060E;
  constexpr uint24_t Player_00_01 = 0x0006D0;
  constexpr uint24_t Player_00_02 = 0x00077A;
  constexpr uint24_t Player_00_03 = 0x000800;
  constexpr uint24_t Player_00_04 = 0x00086E;
  constexpr uint24_t Player_00_05 = 0x0008B8;
  constexpr uint24_t Player_00_06 = 0x0008F6;
  constexpr uint24_t Player_00_07 = 0x000934;
  constexpr uint24_t Player_00_08 = 0x000972;
  constexpr uint24_t Player_00_09 = 0x0009BC;
  constexpr uint24_t Player_00_10 = 0x000A2A;
  constexpr uint24_t Player_00_11 = 0x000AB0;
  constexpr uint24_t Player_00_12 = 0x000B5A;
  constexpr uint24_t Player = 0x000C1C;
  constexpr uint24_t HUD = 0x000C3C;
  constexpr uint24_t Player_Rotate = 0x000E96;
}

