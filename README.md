# cast
A simple and lightweight C/C++ build system with minimal user configuration. It
supports unit testing and compiling with external dependencies.

# project structure 
With Cast, every directory under your `src` folder is automatically compiled
based on the `cast.cfg`. If no config exists, the default behavior is to compile
a driver. Cast supports compiling executables and libraries (static and
dynamic). Folders named `test` are targeted for unit tests. 

## cast.cfg
A simple library configuration can contain the following:
```
target:so
cflag:-fPIC 
ldflag:-ldl
```

If your project contains subdirectories that are required to be built in a
particular order, list the subdir config item. If a directory exists that isn't
referenced in a config, Cast will enter it an compile it anyway. 
```
subdir:myLib
subdir:myExe
```

