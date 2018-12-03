from conans.model import Generator
from conans import ConanFile

"""
The Cast generator supports integrating Cast with Conan for external dependency
management. This generator will create dependency config files for source code
projects using Cast as its build system. 

The format for cast dependency config files is a simple line-separated file of
key, value pairs separated by a colon (:). The key is the header file relative
path and the value is the path to the library installed on the machine.

An example dependency config file may contain:
curl/curl.h:/home/user/.conan/data/curl/7.48.0/conan/stable/package/58ab6a9db77483d191a9b9008621529feb6c1ead/lib/libcurl.a
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

        #
        # TODO implement me
        #

        return contents;


class CastGeneratorPackage(ConanFile):
    name = CAST_GENERATOR_NAME
    version = 01.00.00
    url = "https://github.com/comychitz/cast"
    license = "MIT"
    description = "Cast Generator"

    def build(self):
        pass

    def package_info(self):
        self.cpp_info.includedirs = []
        self.cpp_info.libdirs = []
        self.cpp_info.bindirs = []

