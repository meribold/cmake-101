# CMake 101

Files for a 10-minute presentation introducing CMake.  The slides are created with
[org-reveal] and available **[here][slides]** (hit <kbd>Space</kbd> or <kbd>N</kbd> to
move to the next slide).

The main part of the presentation is a live demonstration of setting up a minimal C++
project that uses CMake.  The end result of the demonstration is the stuff in
[example](/example/) and it roughly consists of the following steps.

#### 1. Setup a minimal project

Create a directory for the example project containing `main.cpp`

```cpp
#include <iostream>

int main() {
    std::cout << "Hello, world\n";
}
```

and `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.9.4)
project(HelloWorld)
add_executable(hello main.cpp)
```
#### 2. Build it

Run `cmake .` in the new directory followed by `make`.  This should work and generate an
executable.

#### 3. Add more source files

Add two more source files, modify `main.cpp` and update `CMakeLists.txt`.  See
[`0af6fcf`][].
*   `strings.hpp`

    ```cpp
    #include <string>

    extern std::string hello_world;
    ```

*   `strings.cpp`

    ```cpp
    #include "strings.hpp"

    std::string hello_world = "Hello, world";
    ```
*   `main.cpp` diff

    ```diff
    @@ -1,5 +1,7 @@
     #include <iostream>

    +#include "strings.hpp"
    +
     int main(int argc, char* argv[]) {
    -   std::cout << "Hello, world\n";
    +   std::cout << hello_world << "\n";
     }
    ```
*   `CMakeLists.txt` diff

    ```diff
    @@ -1,3 +1,4 @@
     cmake_minimum_required(VERSION 3.9.4)
     project(HelloWorld)
    -add_executable(hello main.cpp)
    +file(GLOB SOURCES "*.cpp")
    +add_executable(hello ${SOURCES})
    ```

CMake will find new source files automatically now.  I should note that this practice is
[recommended against](https://cmake.org/cmake/help/latest/command/file.html):
>   We do not recommend using GLOB to collect a list of source files from your source
>   tree. If no CMakeLists.txt file changes when a source is added or removed then the
>   generated build system cannot know when to ask CMake to regenerate.

#### 4. Partial rebuilds

Demonstrate that the generated makefile will only perform the necessary steps to rebuild
the `hello` target by touching `main.cpp`, `strings.cpp`, and `strings.hpp` and running
`make` each time.

#### 5. Add a subdirectory for source files

Move all sources into a subdirectory (`src/`) and adjust `CMakeLists.txt`.  See
[`f462fc9`][].  Run `make`.

#### 6. Build out of place

Switch to out-of-place builds.  We need to remove all files that CMake generated in the
project root directory.

```bash
rm -rf CMakeCache.txt CmakeFiles/ cmake_install.cmake Makefile
```

Then we can create a new directory and build there.

```bash
mkdir build
cd build
cmake ..
make
```

#### 7. Enable compiler warnings

It turns out we aren't using any compiler warnings.  We can check with what options the
compiler is invoked like this:

```bash
make clean
make VERBOSE=1
```

That's bad.  Commit [`a3478ca`][] enables some compiler warnings by changing
`CMakeLists.txt` like this:

```diff
@@ -1,4 +1,13 @@
 cmake_minimum_required(VERSION 3.9.4)
 project(HelloWorld)
+
+# Enable some warnings.  See <https://stackoverflow.com/a/14235055>.
+if (CMAKE_COMPILER_IS_GNUCC)
+   set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
+endif (CMAKE_COMPILER_IS_GNUCC)
+if (MSVC)
+   set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4")
+endif (MSVC)
+
 file(GLOB SOURCES "src/*.cpp")
 add_executable(hello ${SOURCES})
```

Confirm that this worked by running `make VERBOSE=1`.

#### 8. Explicitly specify a C++ standard

It also turns out the C++ standard we are using is just the compiler's default (I think).
Explicitly specify a C++ standard by adding `set(CMAKE_CXX_STANDARD 14)` to
`CMakeLists.txt`.  See [`bf5c30a`][].

Confirm that this worked by running `make VERBOSE=1` again.

#### 9. Do a debug and a release build

Show how to do debug and release builds.  Remove all the generated files in `build/` and
create new subdirectories first:

```bash
rm -r *
mkdir debug release
```

*   To do a release build:

    ```bash
    cd release/
    cmake -DCMAKE_BUILD_TYPE=Release ../../
    make VERBOSE=1 # check the output to confirm optimization is enabled
    ```

    This added `-O3` and `-DNDEBUG` to the compiler options when I tried it.

*   To do a debug build:

    ```bash
    cd ../debug/
    cmake -DCMAKE_BUILD_TYPE=Debug ../../
    make VERBOSE=1 # check the output to confirm optimization is enabled
    ```

    This added `-g` to the compiler options.

#### 10. Use a Boost library

After performing the changes of commit [`150287f`][], rerun `make`.

[slides]: https://meribold.github.io/cmake-101/slides/slides.html
[org-reveal]: https://github.com/yjwen/org-reveal
[`0af6fcf`]: https://github.com/meribold/cmake-101/commit/0af6fcf7717153ad16d803b900c3bbc15bd81a81
[`f462fc9`]: https://github.com/meribold/cmake-101/commit/f462fc9a1179e2b63ae5e8a431ad7484872076ef
[`a3478ca`]: https://github.com/meribold/cmake-101/commit/a3478cab0e8aef17b4dce56fbb615e5c40a9052d
[`bf5c30a`]: https://github.com/meribold/cmake-101/commit/bf5c30ac4176d4d84cf9872e8b51f0f48c52d413
[`150287f`]: https://github.com/meribold/cmake-101/commit/150287f6585f8fdf2f7c64447cc239af7dbd78d2

<!-- vim: set tw=90 sts=-1 sw=4 et spell: -->
