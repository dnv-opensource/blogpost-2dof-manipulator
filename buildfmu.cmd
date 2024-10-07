@echo off
git submodule init
git submodule update
conan install . -of cmake-build-release -b missing
mkdir xtensor-blas/build
cmake -B xtensor-blas/build -DCMAKE_TOOLCHAIN_FILE=../../cmake-build-release/conan_toolchain.cmake ^
                            -DCMAKE_POLICY_DEFAULT_CMP0091=NEW ^
                            -DCMAKE_INSTALL_PREFIX=cmake-build-release ^
                            --fresh ^
                            xtensor-blas
cmake --build xtensor-blas/build --config Release --target install
cmake --preset conan-default
cmake --build --preset conan-release