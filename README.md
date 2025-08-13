# Binding of Isaac clone made with raylib
## How to compile on GNU/Linux using gcc
[Build and install raylib](https://github.com/raysan5/raylib?tab=readme-ov-file#build-and-installation). Then, in the main directory, run:
```sh
gcc main.c -o bin/main -lraylib -lm -ldl -lpthread -lGL -lX11
```
