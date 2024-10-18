from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, CMakeDeps
from conan.tools.files import mkdir


class Manipulator(ConanFile):
    name = "CppFmu example for 2DoF manipulator"
    version = "1.0"

    # Optional metadata
    author = "Hee Jong Park"
    url = ""
    description = "2DoF manipulator with a PID controller"

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}

    requires = ("xtensor/0.25.0", "openblas/0.3.27")

    default_options = {
        "shared": False,
        "fPIC": True,
        "openblas/*:build_lapack": True,
    }

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build_xtensor_blas(self):
        cmake = CMake(self)
        mkdir(self, "xtensor-blas/build")
        cmake.configure(
            build_script_folder="xtensor-blas",
            variables={
                "CMAKE_TOOLCHAIN_FILE": "cmake-build-release/conan_toolchain.cmake",
                "CMAKE_POLICY_DEFAULT_CMP0091": "NEW",
                "CMAKE_INSTALL_PREFIX": "cmake-build-release",
            },
            cli_args=[
                "-B xtensor-blas/build",
                "--fresh",
            ],
        )
        cmake.install(cli_args=["xtensor-blas/build"])

    def build(self):
        self.build_xtensor_blas()
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
