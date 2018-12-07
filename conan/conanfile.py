from conans.model import Generator
from conans import ConanFile
from os import walk

"""
The Cast generator supports integrating Cast with Conan for external dependency
management. This generator will create dependency config files for source code
projects using Cast as its build system.

The format for cast dependency config files is a simple line-separated file of
key, value pairs separated by a colon (:). The key is the header file relative
path and the value is the path to the library installed on the machine. If a
library depends on other libraries, the keyword "depLib" is used to indicate.

An example dependency config file may contain:
curl/curl.h:/home/user/.conan/data/curl/7.48.0/conan/stable/package/58ab6a9db77483d191a9b9008621529feb6c1ead/lib/libcurl.a
depLib:/home/user/.conan/data/zlib/1.2.3/conan/stable/package/23234ksdf0923rlkjsdf2389u234lkjef9234jlk/lib/libzlib.a
"""

CAST_GENERATOR_NAME = "CastGenerator"
CAST_TOOL_NAME = "Cast"


class CastGenerator(Generator):

    @property
    def filename(self):
        pass

    @property
    def content(self):
        contents = {}
        for depname, cpp_info in self.deps_build_info.dependencies:
            if depname == CAST_GENERATOR_NAME or depname == CAST_TOOL_NAME:
                continue

            depContents = ""
            for includeDir in cpp_info.includedirs:
                includePath = cpp_info.rootpath+"/"+includeDir
                for root, dirs, files in walk(includePath):
                    for f in files:
                        relativePath = root+"/"+f
                        relativePath = relativePath.replace(includePath+"/", "")
                        depContents += ("header:"+relativePath)+"\n"

            for libDir in cpp_info.libdirs:
                for root, dirs, files in walk(cpp_info.rootpath+"/"+libDir):
                    for f in files:
                        depContents += ("lib:"+root+"/"+f)+"\n"

            for public_dep in cpp_info.public_deps:
                depContents += "dep:"+public_dep+"\n"

            contents["%s.cfg" % depname] = depContents
        return contents


class CastGeneratorPackage(ConanFile):
    name = CAST_GENERATOR_NAME
    version = "01.00.00"
    url = "https://github.com/comychitz/cast"
    license = "MIT"
    description = "Cast Generator"

    def build(self):
        pass

    def package_info(self):
        self.cpp_info.includedirs = []
        self.cpp_info.libdirs = []
        self.cpp_info.bindirs = []
