# cast
A simple and lightweight C/C++ build system with minimal user configuration. It
supports unit testing and compiling with external dependencies.

## usage
Just run cast from the top level of your project!
```
$ cast
```
Simply clean up using:
```
$ cast clean
```

## project structure 
With Cast, every directory under your `src` folder is automatically compiled
based on the `cast.cfg`. If no config exists, the default behavior is to compile
a driver. Cast supports compiling executables and libraries (static and
dynamic). Folders named `test` are targeted for unit tests. 

Folder names are used for naming the libraries and executables by default
(libraries prefixed with lib).

### cast.cfg
A simple library configuration can contain the following:
```
target:so
cflag:-fPIC 
ldflag:-ldl
```
or
```
target:exe
name:hello_world
```
If your project contains subdirectories that are required to be built in a
particular order, use the `subdir` config item. If a directory exists that isn't
referenced in a config, Cast will enter it an compile it anyway. 
```
subdir:myLib
subdir:myExe
```

### Config Items

| config item | possible values | notes |
|---|---|---|
| target | "exe", "so", "a" | 
| name | *string* | file name of the resulting target. libraries are prefixed with `lib` and include the extension
| subdir | *relative_path* | add a subdir to the project
| cflag | *string* | append to the list of cflags |
| ldflag | *string* | append to the list of ldflags |


## build cast
```
$ cd src && make
```
