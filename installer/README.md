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

## External applications:

### Python
A portable python installation posing as an environment is enough
Target: `/pythonenv/`

### Docker
Docker Desktop is probably the way to go. Just have to install silently or teach the user to install the software then moving on to just use without an account and gotta be on if the software wants to start the **judging** process.