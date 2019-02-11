# cast
A simple and lightweight C++ build system with minimal user configuration.

## Features
* Minimal user configuration
* Built-in unit testing support
* Incremental Builds
* Integrates with [Conan](https://github.com/conan-io/conan) for handling external dependencies
* Support for cross compilation

## Upcoming Features
* Relative subdirectory re-building

## usage
Just run cast from the top level of your project!
```
$ cast
```
Simply clean up using:
```
$ cast clean
```
Run unit tests using:
```
$ cast check
```

## project structure 
With Cast, every directory under your `src` folder is automatically compiled
based on the `cast.cfg`. If no config exists, the default behavior is to compile
a driver. Cast supports compiling executables and libraries (static and
dynamic). Folders named `test` are targeted for unit tests. 

Folder names are used for naming the libraries and executables by default
(libraries prefixed with lib).

Include a `conanfile.txt` file at the top level of your directory and cast will
handle installing and linking against the dependencies for you. *Note: this
requires the CastGenerator package to be available.*

### example projects
See example projects [here](https://github.com/comychitz/cast/tree/master/examples)

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

### config items

| config item | possible values | notes |
|---|---|---|
| target | "exe", "so", "a" | 
| name | *string* | file name of the resulting target. libraries are prefixed with `lib` and include the extension
| subdir | *relative_path* | add a subdir to the project
| cflag | *string* | append to the list of cflags |
| ldflag | *string* | append to the list of ldflags |

### integrating cross compilers
To have cast use a cross compiler installed on your machine, add a cross tool
config file into `/usr/local/share/cast/crosstools`, with the cross tool's name
plus `.cfg` as the file's extension. The following items shall be defined in
the cross tool config (following the same format for *cast.cfg* files).

| config item | description |
|---|---|
| ar | archive tool |
| gxx | compiler |
| strip | strip tool |


## build cast
### Requires
* c++14 compiler
* [Catch2](https://github.com/catchorg/Catch2) to be installed (for running cast's unit tests)

To build cast simply run:
```
$ cd src
$ make
```
Run unit tests using:
```
$ make check
```
install cast:
```
$ make install
```

### building the Conan generator
Build and install the Conan generator package using the following:
```
$ cd conan && conan create . <user>/<channel>
```
*Specify the appropriate user and channel values*
