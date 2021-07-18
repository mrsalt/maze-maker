# maze-maker

## Windows Setup
You need to have the following installed:
* CMake
  If you're building with Visual Studio, CMake support is getting better all the time.  Visual Studio now has some pretty cool native integration with CMake.  (You may need to opt in to CMake through the Visual Studio installer).
  If you're building on Linux, you likely already have CMake installed.

* cairo graphics library
  I recommend using [vcpkg](https://vcpkg.io/en/getting-started.html) as an easy way to get/build/install packages, especially for Windows development.
  1. Follow the 'Get Started' link on vcpkg to install vcpkg.
  2. Once it's installed, install cairo:
     vcpkg install cairo:x64

vcpkg integrate install
Applied user-wide integration for this vcpkg root.

All MSBuild C++ projects can now #include any installed libraries.
Linking will be handled automatically.
Installing new libraries will make them instantly available.

CMake projects should use: "-DCMAKE_TOOLCHAIN_FILE=C:/Users/salis/git/vcpkg/scripts/buildsystems/vcpkg.cmake"

## Linux / MacOS
* CMake
* cairo graphics library

Linux/ MacOS have native support for installing packages like cmake and cairo, so install them using your native package manager.  After cairo is installed, it should be discoverable by CMake.

## Building
### Command Line
### Visual Studio

##FAQ
Q: When I build with Visual Studio I see this error during compilation: C:\maze-maker-lib\src\Render.cpp(1): fatal error C1083: Cannot open include file: 'cairo.h': No such file or directory
A: This means cmake was unable to find the header file for the cairo graphics library, which is a dependency for this application.  See Windows Setup above.