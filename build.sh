git submodule update --init
cd raylib
mkdir build
cd build
cmake ..
make
cd ../../
mkdir build
cd build
cmake ..
make