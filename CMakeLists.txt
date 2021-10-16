cmake_minimum_required(VERSION 3.20)
project(dictzipWin64 C)

set(CMAKE_C_STANDARD 11)

add_executable(dictzip
        src/dictzip.c
        src/dictzip.h

        src/data.c
        src/data.h

        zlib/crc32.c
        zlib/zutil.c
        zlib/deflate.c
        zlib/inflate.c
        zlib/inftrees.c
        zlib/inffast.c
        zlib/adler32.c
        zlib/trees.c
        )

add_compile_definitions(
        DICT_VERSION="1.12.1-Win64-1.0"
)


target_include_directories(dictzip
        PRIVATE zlib
        )
