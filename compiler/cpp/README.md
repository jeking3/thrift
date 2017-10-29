# Building the Thrift Compiler using CMake #

You can build the compiler separately from the rest of the project.  The compiler
option `WITH_PLUGIN` does not work in this mode, however this will not be an issue
for most.

## Unix ##

Ensure the bison and flex packages are available on your system.

Example build script:
```
$ mkdir /tmp/build
$ cd /tmp/build
$ cmake ~/thrift/compiler/cpp
$ cmake --build .
```

## Windows ##

On Windows, obtain the WinFlexBison package and make sure the binaries win_bison and win_flex can be executed from your command line prompt.

Open the Visual Studio command-line prompt you want build within.

Example build script:
```
> mkdir C:\Temp\Build
> cd C:\Temp\Build
> cmake c:\workspace\thrift\compiler\cpp
> cmake --build .
```

If you run into problems where bison and flex are not found, try updating cmake, and if that doesn't work then you can tell cmake where to find them - for example:

1. Download winflexbison from http://sourceforge.net/projects/winflexbison/
2. Extract the winflexbison files to a directory, for example C:\winflexbison
3. Set `BISON_EXECUTABLE` and `FLEX_EXECUTABLE` for cmake: to the cmake build generation command.
```
> cmake -DBISON_EXECUTABLE=C:\winflexbison\win_bison.exe ^
           -DFLEX_EXECUTABLE=C:\winflexbison\win_flex.exe ^
           c:\workspace\thrift\compiler\cpp
```
