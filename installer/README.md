# Installer codebase
This is the codebase for the installer of this software. It's going to be much more lightweight and fast with few logics and memory allocation shenanigans because they're really not needed.

> This is Windows-only

Files to include in this:
```
bin/panel.exe
bin/*.dll
bin/generic
bin/iconengines
bin/imageformats
bin/networkinformation
bin/platforms
bin/styles
bin/tls
bin/translations
/msys64/mingw64/bin/libcrypto-3-x64.dll
/msys64/mingw64/bin/libxlnt.dll
```
All files above are going to be in the root of this software's folder, otherwise known as application root in a lot of instances. Here let's call it that way.