# smrfhck

This is a little hack that allows to show extra informations about your surrounding in your favorite Diablo II remake.

It is built using the awesome [libsmrf](https://github.com/JeanMax/libsmrf) and [imgui](https://github.com/ocornut/imgui) for the UI. Imgui will draw `smrfhck` in a new window (for safety reasons), but it could be easily modified to be draw in an overlay on your game.



## Usage

```
./smrfhck
```
or double-click on the `smrfhck.exe`.

A map of your area (and its connected areas) will be shown. It includes exits, various units (monsters, npc...) and missiles.

<img src="https://rmrf.fr/static/img/smrfhck/settings.jpg" alt="Settings" width="800">
There is a `Settings` part where you can input the name of your Diablo II window (in case multiples could be found? not judging) and customize how and what to display on your map.

<img src="https://rmrf.fr/static/img/smrfhck/debug.jpg" alt="Debug" width="800">
You can also find a debug window showing info about nearby units.



## Installation

A Makefile is included, so you can just run:
```shell
> make
```
and `smrfhck` will be generated.


## Dependencies

Virtually none, just enough to compile some C (gcc / clang).
If you use windows (shame on you), mingw32-gcc is required.

All the dependencies are included in the `extern` dir; namely:
* imgui
* inih
* libsmrf
* OpenGL
* SDL


## License

MIT
