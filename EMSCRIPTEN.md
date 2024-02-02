# Emscripten

## Compile

```
emcmake cmake -DCMAKE_BUILD_TYPE=Release -DRENDERER=GL2 -DFBO=OFF -DOPENMPT=OFF -DFAST_RANDOM=ON -DFAST_TRIG=ON -DDISABLE_EXCEPTIONS=ON -DUSE_PRECOMPILED_HEADERS=OFF ..
```

## Link

```
emcc -flto -O3 -fno-exceptions -fno-rtti */*.o *.o libGL.a -o index.html -sFULL_ES2=1 -lGL -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sSDL2_IMAGE_FORMATS='["png"]' -sUSE_SDL_MIXER=2 -sSDL2_MIXER_FORMATS='["mod","wav"]' -sTOTAL_MEMORY=512mb -sASYNCIFY -sASYNCIFY_STACK_SIZE=81920 -sENVIRONMENT=web --preload-file data/ --preload-file lang/ --closure 1 -Wl,-u,fileno
```