#!/bin/bash

SRCS=`ls src/**/unity.cpp`
CXXFLAGS=(-O3 -std=c++11 -Iinclude -I/vendor/include/expat)
LDFLAGS=(-L/vendor/lib/expat -lexpat)
EMFLAGS=(
    -s TOTAL_MEMORY=1GB
    -s USE_SDL=2
    -s USE_SDL_TTF=2
    -s USE_SDL_IMAGE=2
    -s SDL2_IMAGE_FORMATS='["bmp","png"]'
    --preload-file assets
    --closure 1
)

FLAGS="${CXXFLAGS[@]} ${LDFLAGS[@]} ${EMFLAGS[@]}"

# echo "${FLAGS[@]}"
docker run --rm -v ${PWD}:/src lherman/sdl emcc -o index.html ${FLAGS[@]} ${SRCS[@]}