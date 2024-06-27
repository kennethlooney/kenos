# set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
# set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

 
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -m64 -nostdlib -ffreestanding")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")
file(GLOB SOURCES "src/*.c")
add_library(diskformats ${SOURCES})