2DoF manipulator example
========

This is a repository of 2Dof manipulator example written in plain C++ using the
[CPPFMU] framework.

Prerequisites
--------------

1. [git]
2. [conan]>=2.0
3. [cmake]>= 3.26.3
4. gcc>=11 (Linux) or [Build Tools for Visual Studio 2022] (Windows)


How to build
------------

### Linux
Run `buildfmu`: `./buildfmu`

### Windows
Run `buildfmu.cmd`


`manipulator.fmu` will be created in `cmake-build-release` directory.

[git]: https://git-scm.com/
[conan]: https://conan.io/
[cmake]: https://cmake.org
[Build Tools for Visual Studio 2022]: https://visualstudio.microsoft.com/downloads/