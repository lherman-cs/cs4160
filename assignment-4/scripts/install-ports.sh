#!/bin/bash

/emsdk_portable/sdk/embuilder.py build sdl2 sdl2-image sdl2-image-png sdl2-ttf sdl2-mixer zlib libpng

# Prepare include
mkdir -p /vendor/include/SDL/SDL2
cp /emsdk_portable/data/.ports/sdl2_mixer/*/**.h /vendor/include/SDL/SDL2/