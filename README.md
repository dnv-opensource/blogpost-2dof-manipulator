2DoF manipulator example
========

This is a repository of 2Dof manipulator example written in plain C++ using the
[cpp-fmus] framework.

Prerequisites
--------------

1. [git]
2. [conan]>=2.0
3. [cmake]>= 3.26.3
4. gcc>=11 (Linux) or [Build Tools for Visual Studio 2019] (Windows)


How to build
------------

### Linux
Run `buildfmu`

### Windows
Run `buildfmu.cmd`


`manipulator.fmu` will be created in `cmake-build-release-(linux64|win64)` directory.

[cpp-fmus]: https://github.com/open-simulation-platform/cpp-fmus
[git]: https://git-scm.com/
[conan]: https://conan.io/
[cmake]: https://cmake.org
[Build Tools for Visual Studio 2019]: https://visualstudio.microsoft.com/vs/older-downloads/
