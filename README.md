# Binding of Isaac clone made with raylib
## How to compile on GNU/Linux using gcc
[Build and install raylib](https://github.com/raysan5/raylib?tab=readme-ov-file#build-and-installation). Then, in the project's directory, run this command to compile the game:
```sh
gcc main.c tear.c -o bin/main -lraylib -lm -ldl -lpthread -lGL -lX11
```
Execute `bin/main` in order to run the game.

**Tested on GNU/Linux, running on Xorg and Wayland.**
