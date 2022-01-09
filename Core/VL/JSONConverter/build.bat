mklink /D VL ..
mkdir Build
cd Build
cmake .. -DBUILD_TESTS=ON
cmake --build .
cd ..
