from PIL import Image
import os
from pathlib import Path

def get_shade(rgba, shades, shade):
    w = (254 + shades) // shades
    b = (shade + 1) * w
    return 1 if rgba[0] >= b else 0

def get_mask(rgba):
    return 1 if rgba[3] >= 128 else 0

def convert(fname, shades, sw = None, sh = None, num = None, maskImage = False):

    if not (shades >= 2 and shades <= 4):
        print('shades argument must be 2, 3, or 4')
        return None

    im = Image.open(fname).convert('RGBA')
    pixels = list(im.getdata())
    
    masked = maskImage
    q = 0
    for i in pixels:
        q = q + 1
        # print(i[0])
        # print(i[1])
        # print(i[2])
        # print(i[3])
        if i[3] < 255:
            # print('masked!!! ')
            # print(q)
            masked = True
            # exit()
            break

    print('{}, shades {}, masked {}'.format(fname, shades, masked))


    w = im.width
    h = im.height
    if sw is None: sw = w
    if sh is None: sh = h
    nw = w // sw
    nh = h // sh
    if num is None: num = nw * nh
    sp = (sh + 7) // 8
    
    if nw * nh <= 0:
        print('%s: Invalid sprite dimensions' % fname)
        return None
        
    bytes = bytearray([sw, sh])
    
    for n in range(num):
        bx = (n % nw) * sw
        by = (n // nw) * sh
        for shade in range(shades - 1):
            for p in range(sp):
                for ix in range(sw):
                    x = bx + ix
                    byte = 0
                    mask = 0
                    for iy in range(8):
                        y = p * 8 + iy
                        if y >= sh: break
                        y += by
                        i = y * w + x
                        rgba = pixels[i]
                        byte |= (get_shade(rgba, shades, shade) << iy)
                        mask |= (get_mask(rgba) << iy)
                    bytes += bytearray([byte])
                    if masked:
                        bytes += bytearray([mask])
    
    return bytes
    
def convert_header(fname, fout, sym, shades, sw = None, sh = None, num = None, maskImage = False):
    bytes = convert(fname, shades, sw, sh, num, maskImage)
    if bytes is None: return
    with open(fout, 'a') as f:
        # f.write('#pragma once\n\n#include <stdint.h>\n#include <avr/pgmspace.h>\n\n')
        # f.write('constexpr uint8_t %s[] PROGMEM =\n{\n' % sym)
        f.write('uint8_t %s[] = {\n  ' % sym)
        for n in range(len(bytes)):
            if n % 16 == 2:
                f.write('\n  ')
            f.write('%3d,' % bytes[n])
            # f.write(' ' if n % 16 != 15 else '\n')
            # f.write(' ' if n % 18 != 2 else '\n')
            f.write(' ')
        if len(bytes) % 16 != 2:
            f.write('\n')
        f.write('};\n\n')

def deleteFile(filename):
    if os.path.isfile(filename):
        os.remove(filename)

BASE = './images/'
IMAGES = '../images/'

deleteFile(BASE + 'Images.hpp')

convert_header(IMAGES + 'Player/Player_01_00.png',                                             BASE + 'Images.hpp', 'Player_01_00', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_01.png',                                             BASE + 'Images.hpp', 'Player_01_01', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_02.png',                                             BASE + 'Images.hpp', 'Player_01_02', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_03.png',                                             BASE + 'Images.hpp', 'Player_01_03', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_04.png',                                             BASE + 'Images.hpp', 'Player_01_04', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_05.png',                                             BASE + 'Images.hpp', 'Player_01_05', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_06.png',                                             BASE + 'Images.hpp', 'Player_01_06', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_07.png',                                             BASE + 'Images.hpp', 'Player_01_07', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_08.png',                                             BASE + 'Images.hpp', 'Player_01_08', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_09.png',                                             BASE + 'Images.hpp', 'Player_01_09', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_10.png',                                             BASE + 'Images.hpp', 'Player_01_10', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_11.png',                                             BASE + 'Images.hpp', 'Player_01_11', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_01_12.png',                                             BASE + 'Images.hpp', 'Player_01_12', 4, maskImage=True)

convert_header(IMAGES + 'Player/Player_00_00.png',                                             BASE + 'Images.hpp', 'Player_00_00', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_01.png',                                             BASE + 'Images.hpp', 'Player_00_01', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_02.png',                                             BASE + 'Images.hpp', 'Player_00_02', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_03.png',                                             BASE + 'Images.hpp', 'Player_00_03', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_04.png',                                             BASE + 'Images.hpp', 'Player_00_04', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_05.png',                                             BASE + 'Images.hpp', 'Player_00_05', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_06.png',                                             BASE + 'Images.hpp', 'Player_00_06', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_07.png',                                             BASE + 'Images.hpp', 'Player_00_07', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_08.png',                                             BASE + 'Images.hpp', 'Player_00_08', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_09.png',                                             BASE + 'Images.hpp', 'Player_00_09', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_10.png',                                             BASE + 'Images.hpp', 'Player_00_10', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_11.png',                                             BASE + 'Images.hpp', 'Player_00_11', 4, maskImage=True)
convert_header(IMAGES + 'Player/Player_00_12.png',                                             BASE + 'Images.hpp', 'Player_00_12', 4, maskImage=True)

convert_header(IMAGES + 'Player/Player.png',                                                   BASE + 'Images.hpp', 'Player', 4, maskImage=True)

convert_header(IMAGES + 'HUD.png',                                                             BASE + 'Images.hpp', 'HUD', 4)

convert_header(IMAGES + 'PPOT.png',                                                            BASE + 'Images.hpp', 'PPOT', 4, 128, 64)
convert_header(IMAGES + 'Sound_Volume.png',                                                    BASE + 'Images.hpp', 'Sound_Volume', 4, 22, 16)
convert_header(IMAGES + 'Sound_Checkbox.png',                                                  BASE + 'Images.hpp', 'Sound_Checkbox', 4)
convert_header(IMAGES + '/Title.png',                                                          BASE + 'Images.hpp', 'Title', 4, 128, 64, maskImage=False)

# convert_header(IMAGES + '/Tiles/Tile_00_01.png',                                BASE + 'Images.hpp', 'Tile_00_01', 4)
# convert_header(IMAGES + '/Tiles/Tile_00_02.png',                                BASE + 'Images.hpp', 'Tile_00_02', 4)
# convert_header(IMAGES + '/Tiles/Tile_00_03.png',                                BASE + 'Images.hpp', 'Tile_00_03', 4)
# convert_header(IMAGES + '/Tiles/Tile_00_04.png',                                BASE + 'Images.hpp', 'Tile_00_04', 4)
# convert_header(IMAGES + '/Tiles/Tile_00_05.png',                                BASE + 'Images.hpp', 'Tile_00_05', 4)
# convert_header(IMAGES + '/Tiles/Tile_00_06.png',                                BASE + 'Images.hpp', 'Tile_00_06', 4)
# convert_header(IMAGES + '/Tiles/Tile_00_07.png',                                BASE + 'Images.hpp', 'Tile_00_07', 4)
# convert_header(IMAGES + '/Tiles/Tile_00_08.png',                                BASE + 'Images.hpp', 'Tile_00_08', 4)
# convert_header(IMAGES + '/Tiles/Tile_01_01.png',                                BASE + 'Images.hpp', 'Tile_01_01', 4)
# convert_header(IMAGES + '/Tiles/Tile_01_02.png',                                BASE + 'Images.hpp', 'Tile_01_02', 4)
# convert_header(IMAGES + '/Tiles/Tile_01_03.png',                                BASE + 'Images.hpp', 'Tile_01_03', 4)
# convert_header(IMAGES + '/Tiles/Tile_01_04.png',                                BASE + 'Images.hpp', 'Tile_01_04', 4)
# convert_header(IMAGES + '/Tiles/Tile_01_05.png',                                BASE + 'Images.hpp', 'Tile_01_05', 4)
# convert_header(IMAGES + '/Tiles/Tile_01_06.png',                                BASE + 'Images.hpp', 'Tile_01_06', 4)
# convert_header(IMAGES + '/Tiles/Tile_01_07.png',                                BASE + 'Images.hpp', 'Tile_01_07', 4)
# convert_header(IMAGES + '/Tiles/Tile_01_08.png',                                BASE + 'Images.hpp', 'Tile_01_08', 4)
# convert_header(IMAGES + '/Tiles/Tile_02_01.png',                                BASE + 'Images.hpp', 'Tile_02_01', 4)
# convert_header(IMAGES + '/Tiles/Tile_02_02.png',                                BASE + 'Images.hpp', 'Tile_02_02', 4)
# convert_header(IMAGES + '/Tiles/Tile_02_03.png',                                BASE + 'Images.hpp', 'Tile_02_03', 4)
# convert_header(IMAGES + '/Tiles/Tile_02_04.png',                                BASE + 'Images.hpp', 'Tile_02_04', 4)
# convert_header(IMAGES + '/Tiles/Tile_02_05.png',                                BASE + 'Images.hpp', 'Tile_02_05', 4)
# convert_header(IMAGES + '/Tiles/Tile_02_06.png',                                BASE + 'Images.hpp', 'Tile_02_06', 4)
# convert_header(IMAGES + '/Tiles/Tile_02_07.png',                                BASE + 'Images.hpp', 'Tile_02_07', 4)
# convert_header(IMAGES + '/Tiles/Tile_02_08.png',                                BASE + 'Images.hpp', 'Tile_02_08', 4)