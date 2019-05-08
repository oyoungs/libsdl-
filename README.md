# libsdl-
C++ wrapper of  high frequency interface in SDL2

# build and install
```bash
~$ cmake -Bbuild -H. -DCMAKE_INSTALL_PREFIX=<your-install-path>
~$ cmake --build build [--target install]
```

# usage 

1. add the <your-install-path>/include directory to your project include  directories, e.g. /usr/local/include, in CMakeLists.txt, you may type 
'include_directories(/usr/local/include)'
2. add the '#include <sdl++/sdl.h>' in your source code file, e.g.

```cpp
#include <sdl++/sdl.h>

int main(int, char**)
{
  return 0;
}
```
