# set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
# set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)

# Set the compiler flags for EFI
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -m64 -nostdlib -ffreestanding")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")

add_library(gpt gpt.c)