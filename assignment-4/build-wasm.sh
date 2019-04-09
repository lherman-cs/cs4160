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
CONFIG=./assets/config.xml

# Turn frameCap for wasm because wasm already has its own frameCap
sed -i 's\<frameCapOn>true</frameCapOn>\<frameCapOn>false</frameCapOn>\g' ${CONFIG}
if [[ -z ${CI} ]]; then
    docker run --rm -v ${PWD}:/src lherman/sdl emcc -o index.html ${FLAGS[@]} ${SRCS[@]}
else
    emcc -o index.html ${FLAGS[@]} ${SRCS[@]}
fi
# Turn the frameCap back on
sed -i 's\<frameCapOn>false</frameCapOn>\<frameCapOn>true</frameCapOn>\g' ${CONFIG}
