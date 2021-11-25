git submodule update --init
cd raylib
mkdir build
cd build
cmake ..
cmake --build . --config Release
cd ..\..
mkdir build
cd build
cmake ..
cmake --build . --config Release
