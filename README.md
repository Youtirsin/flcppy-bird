# flcppy-bird
 or should I say flappy bird.

## required

- MSVC
- CMake
- vcpkg
- SFML 2.6.0 (installed with vcpkg)

## build

```bash
mkdir build && cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
cmake --build .
```

