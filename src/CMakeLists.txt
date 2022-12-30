project(IbisOS)
cmake_minimum_required(VERSION 3.10)

option(ARCH "Processor architecture to build for" STRING)
if("${ARCH}" STREQUAL "OFF")
    set(ARCH "i686")
endif()

option(BUILD "PC to build for" STRING)
if("${BUILD}" STREQUAL "OFF")
    set(BUILD "i686-pc-none-elf")
endif()

option(OUTPUT_DIR "Path of output directory" STRING)
if("${OUTPUT_DIR}" STREQUAL "OFF")
    set(OUTPUT_DIR "output/${BUILD}/isodir/boot/")
endif()

option(OBJECT_FORMAT "Compiled object file format" STRING)
if("${OBJECT_FORMAT}" STREQUAL "OFF")
    set(OBJECT_FORMAT "elf32")
endif()

enable_language(C)
enable_language(CXX)
enable_language(ASM_NASM)

file(GLOB LINKER_SCRIPT "build/${ARCH}/linker.ld")
file(GLOB_RECURSE C_SOURCE "c/*.c")
file(GLOB_RECURSE CXX_SOURCE "*.cpp")
file(GLOB_RECURSE ASM_NASM_SOURCE "asm/${ARCH}/*.asm")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_C_FLAGS "-ffreestanding -O2 -Wall --target=${BUILD} -march=${ARCH} -nostdlib")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "clang++")
set(ASM_NASM_COMPILER "nasm")
set(CMAKE_CXX_FLAGS "-ffreestanding -O2 -Wall -fno-exceptions -fno-rtti -nostdlib -nostdinc --target=${BUILD} -march=${ARCH}")
set(CMAKE_ASM_NASM_OBJECT_FORMAT "${OBJECT_FORMAT}")
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -f ${CMAKE_ASM_NASM_OBJECT_FORMAT} -o <OBJECT> <SOURCE>")

add_executable(${PROJECT_NAME}.bin ${CXX_SOURCE} ${C_SOURCE} ${ASM_SOURCE} ${ASM_NASM_SOURCE})

set_target_properties(${PROJECT_NAME}.bin PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT}")

target_include_directories(${PROJECT_NAME}.bin PRIVATE "include")

configure_file("build/${ARCH}/grub.cfg" "${OUTPUT_DIR}/grub/grub.cfg" COPYONLY)

add_custom_target(iso
                COMMAND grub-mkrescue -o "output/${BUILD}-IbisOS.iso"
                "${OUTPUT_DIR}../"
                COMMENT "Building IbisOS image file"
                )
add_custom_target(qemu
                COMMAND cmake src
                COMMAND make
                COMMAND make iso
                COMMAND qemu-system-i386 -cdrom "output/${BUILD}-IbisOS.iso"
                COMMENT "Launching compiled source code"
)
message("Architecture: " "${ARCH}")
message("Build: " "${BUILD}")
message("Object format: " "${OBJECT_FORMAT}")
message("Output directory: " "${OUTPUT_DIR}")