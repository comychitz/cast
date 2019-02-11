# example2
An example project where we have two subfolders within the `src` folder. There
is a `common` folder, which builds a static library that is used by the
executable built within the `myExe` folder. Also, within the `common` folder
there is a unit test for the library.

```
|--src
| |--cast.cfg
| |--myExe
| | |--main.cpp
| |--common
| | |--cast.cfg
| | |--test
| | | |--IncrementerTest.cpp
| | |--Incrementer.h
| | |--Incrementer.cpp
```
