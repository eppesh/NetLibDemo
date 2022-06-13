# src目录只是网络库（不是可执行文件），因此把src/*.cpp文件编译链接成多个目标文件后，再链接到一个共享库中
file(GLOB_RECURSE pine_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(pine_shared SHARED ${pine_sources})							# 利用/src/下所有.cpp文件,生成名为 libpine_shared.so 的动态库