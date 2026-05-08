from conans import CMake, ConanFile, tools
import os

class DatapipeConan(ConanFile):
    name = "datapipe"
    version = "1.0.0"
    license = "MIT"
    homepage = "https://github.com/y-Adrian/datapipe"
    description = "High-performance C data pipeline middleware"
    topics = ("data-pipeline", "c", "middleware", "streaming")
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": True, "fPIC": True}
    generators = "cmake"

    def source(self):
        git = Git(self)
        git.clone("https://github.com/y-Adrian/datapipe.git", "main")
        # Or download from release:
        # tools.get("https://github.com/y-Adrian/datapipe/archive/refs/tags/v1.0.0.tar.gz")

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["datapipe"]
        self.cpp_info.includedirs = ["include", "include/datapipe"]
