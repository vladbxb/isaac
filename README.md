# Binding of Isaac clone made with raylib
This is a simple fan recreation of The Binding of Isaac made for fun, in C!
## Compiling
[Build and install raylib on your system](https://github.com/raysan5/raylib?tab=readme-ov-file#build-and-installation). Additional OS-specific instructions are below.
### On GNU/Linux
[Install the dependencies. Then install the raylib package on your package manager or build from source.](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).
In the project's directory, run this command to compile the game with gcc:
```sh
gcc main.c -o bin/main -lraylib -lm -ldl -lpthread -lGL
```
This works on both Xorg and Wayland from our testing.
### On macOS
[Follow installation with Homebrew](https://github.com/raysan5/raylib/wiki/Working-on-macOS#with-homebrew). Then, in the project's directory, run this command to compile the game with Xcode's cc. **For this, you might need to have Xcode developer tools installed**.
```sh
eval cc main.c $(pkg-config --libs --cflags raylib) -o bin/main
```
**Execute `bin/main` in order to play the game.**
