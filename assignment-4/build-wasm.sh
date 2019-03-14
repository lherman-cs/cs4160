#!/bin/bash

SRCS=`ls src/**/unity.cpp`
echo ${SRCS[@]}
docker run \
--rm \
-v ${PWD}:/src \
-u emscripten \
trzeci/emscripten emcc -Iinclude -Ilib/libexpat/expat/lib -Llib/libexpat/expat/build -std=c++11 ${SRCS[@]} -lexpat -s TOTAL_MEMORY=1GB -s USE_SDL=2 -s USE_SDL_TTF=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["bmp","png"]'  -o index.html --preload-file assets