# 2DoF manipulator example

This is a repository of 2-DOF manipulator example, written in plain C++ using the
[cpp-fmus] framework.

## Prerequisites

1. [git]
2. [conan]>=2.0
3. [cmake]>= 3.26.3
4. gcc>=11 (Linux) or [Build Tools for Visual Studio 2019] or 2022 (Windows)

## How to build

After you check out this repository, provided you have aforementioned tools, you can just build the FMU:

### Linux

Run `buildfmu`

### Windows

Run `buildfmu.cmd` (or `.\buildfmu.cmd`).

`manipulator.fmu` will be created in `cmake-build-release-(linux64|win64)` directory.

## How to use

You can now use the FMU for co-simulation in various tools.

We have tested this in the [Simulation Trust Center], which uses the [Open Simulation Platform] software.

## License & dependencies

This code is distributed under the MIT license.

It makes use of [cpp-fmus] and [xtensor-blas], which are also distributed under the MIT license.</br>
The cpp-fmus framework makes use of [cpp-fmu], which is distributed under the MPL license at https://github.com/viproma/cppfmu.

[cpp-fmus]: https://github.com/open-simulation-platform/cpp-fmus
[cpp-fmu]: https://github.com/viproma/cppfmu
[xtensor-blas]: https://github.com/xtensor-stack/xtensor-blas/
[git]: https://git-scm.com/
[conan]: https://conan.io/
[cmake]: https://cmake.org
[Build Tools for Visual Studio 2019]: https://visualstudio.microsoft.com/vs/older-downloads/
[Open Simulation Platform]: https://opensimulationplatform.com/
[Simulation Trust Center]: https://store.veracity.com/simulation-trust-center
