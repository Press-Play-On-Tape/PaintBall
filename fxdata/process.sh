python3 ../scripts/convert_sprites.py
python3 ../scripts/convert_sprites_PROGMEM.py
# python3 ../scripts/convert_midi.py

python3 ./Arduboy-Python-Utilities-master/fxdata-build.py fxdata.txt


cp fxdata.bin ../../build/fxdata.bin