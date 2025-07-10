# What's this?
It's a project copying the game Pong, made entirely in C and using Wayland, Vulkan and XKB

# How to build?
## Dependencies
- wayland-client
- meson
- python
- gcc(I haven't tested it with other compiler)
- glslc
- vulkan
- xkbcommon

## Command line
- First, we need to clone the repository :
```
~ $ git clone https://github.com/ItaloYt/pong
~ $ cd pong
```

- Then, we use meson to setup and compile the project
```
~/pong $ meson setup -Dbuildtype=release build/
~/pong $ meson compile -C build/
```

- To run, you just need to use:
```
~/pong $ ./build/pong
```
