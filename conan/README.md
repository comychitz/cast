# cast generator
To support integration with Conan, a custom generator has been provided here
that will generate the needed dependency config files that Cast needs to be able
to link against a Conan package. 

To build the Conan package locally, simply run:
```
$ conan create . <user>/<channel>
```
