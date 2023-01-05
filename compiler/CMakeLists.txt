project(CrossCompilers)
cmake_minimum_required(VERSION 3.10)

function(set_option KEY_VAR KEY_NAME DEFAULT)
    if("${KEY_NAME}" STREQUAL "OFF")
        set(${KEY_VAR} "${DEFAULT}" PARENT_SCOPE)
    endif()
endfunction()

option(OUTPUT_DIR "Path of output directory for compiler" STRING)
option(TARGET "Target to build cross compiler for" STRING)

set(PATH "${PATH}:${OUTPUT_DIR}/bin")
set(CURDIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(BINUTILS_DIR "${CURDIR}/binutils/")
set(GCC_DIR "${CURDIR}/gcc/")
set(BUILD_BINUTILS_DIR "${CURDIR}/build/binutils")
set(BUILD_GCC_DIR "${CURDIR}/build/gcc")

set_option(OUTPUT_DIR ${OUTPUT_DIR} "${CURDIR}/output")
set_option(TARGET ${TARGET} "i686-elf")

enable_language(C)
enable_language(CXX)
enable_language(ASM_NASM)

add_custom_target(binutils
                    COMMAND mkdir -p ${BUILD_BINUTILS_DIR}
                    COMMAND cd ${BUILD_BINUTILS_DIR} && ../../binutils/configure --target=${TARGET} --prefix=${OUTPUT_DIR} --with-sysroot --disable-nls --disable-werror
                    COMMAND cd ${BUILD_BINUTILS_DIR} && make -j4
                    COMMAND cd ${BUILD_BINUTILS_DIR} && make -j4 install
                    COMMENT "Builds binutils for cross compiler"
)

add_custom_target(gcc
                    COMMAND mkdir -p ${BUILD_GCC_DIR}
                    COMMAND cd ${BUILD_GCC_DIR} && ../../gcc/configure --target=${TARGET} --prefix="${OUTPUT_DIR}" --disable-nls --enable-languages=c,c++ --without-headers
                    COMMAND cd ${BUILD_GCC_DIR} && make -j4 all-gcc
                    COMMAND cd ${BUILD_GCC_DIR} && make -j4 all-target-libgcc
                    COMMAND cd ${BUILD_GCC_DIR} && make -j4 install-gcc
                    COMMAND cd ${BUILD_GCC_DIR} && make -j4 install-target-libgcc
                    COMMENT "Build GCC for cross compiler"
)