project(IbisOS)
cmake_minimum_required(VERSION 3.10)

function(set_option KEY_VAR KEY_NAME DEFAULT)
    if("${KEY_NAME}" STREQUAL "OFF")
        set(${KEY_VAR} "${DEFAULT}" PARENT_SCOPE)
    endif()
endfunction()

option(ARCH "Processor architecture to build for" STRING)
option(TARGET "Target for GCC" STRING)
option(OUTPUT_DIR "Path of output directory" STRING)
option(OBJECT_FORMAT "Compiled object file format" STRING)
option(ACOMPILER "Assembly compiler" STRING)
option(CCOMPILER "C compiler" STRING)
option(CXXCOMPILER "C++ compiler" STRING)
option(AFLAGS "ASM flags" STRING)
option(CFLAGS "Extra C compiler flags" STRING)
option(CXXFLAGS "Extra C++ compiler flags" STRING)

set_option(ARCH "${ARCH}" "i686")
set_option(TARGET "${TARGET}" "i686-pc-none-elf")
set_option(OUTPUT_DIR "${OUTPUT_DIR}" "output/${TARGET}/isodir/boot")
set_option(OBJECT_FORMAT "${OBJECT_FORMAT}" "elf32")
set_option(ACOMPILER "${ACOMPILER}" "nasm")
set_option(CCOMPILER "${CCOMPILER}" "clang")
set_option(CXXCOMPILER "${CXXCOMPILER}" "clang++")
set_option(AFLAGS "${AFLAGS}" "-felf32")
set_option(CFLAGS "${CFLAGS}" "")
set_option(CXXFLAGS "${CXXFLAGS}" "")

enable_language(C)
enable_language(CXX)
enable_language(ASM_NASM)

file(GLOB LINKER_SCRIPT "build/${ARCH}/linker.ld")
file(GLOB_RECURSE C_SOURCE "c/*.c")
file(GLOB_RECURSE CXX_SOURCE "*.cpp")
file(GLOB_RECURSE ASM_NASM_SOURCE "asm/${ARCH}/**.asm")

set(ASM_NASM_COMPILER ${ACOMPILER})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER ${CCOMPILER})
set(CMAKE_C_FLAGS "-ffreestanding -O2 -Wall -march=${ARCH} -nostdlib ${CFLAGS}")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER ${CXXCOMPILER})
set(CMAKE_CXX_FLAGS "-ffreestanding -O2 -Wall -fno-exceptions -fno-rtti -nostdlib -nostdinc -march=${ARCH} ${CXXFLAGS}")
if(NOT "${TARGET}" STREQUAL "")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --target=${TARGET}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --target=${TARGET}")
endif()
set(CMAKE_ASM_NASM_OBJECT_FORMAT "${OBJECT_FORMAT}")
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> -o <OBJECT> <SOURCE> ${AFLAGS}")

add_executable(${PROJECT_NAME}.bin ${CXX_SOURCE} ${C_SOURCE} ${ASM_SOURCE} ${ASM_NASM_SOURCE})

set_target_properties(${PROJECT_NAME}.bin PROPERTIES LINK_FLAGS "-T ${LINKER_SCRIPT}")

target_include_directories(${PROJECT_NAME}.bin PRIVATE "include")

configure_file("build/${ARCH}/grub.cfg" "${OUTPUT_DIR}/grub/grub.cfg" COPYONLY)

add_custom_target(iso
                COMMAND grub-mkrescue -o "output/${TARGET}-IbisOS.iso"
                "${OUTPUT_DIR}/../"
                COMMENT "Building IbisOS image file"
                )
add_custom_target(qemu
                COMMAND cmake src
                COMMAND make
                COMMAND make iso
                COMMAND qemu-system-i386 -cdrom "output/${TARGET}-IbisOS.iso"
                COMMENT "Launching compiled source code"
)
message("Architecture: " "${ARCH}")
message("Output directory: " "${OUTPUT_DIR}")
message("Object format: " "${OBJECT_FORMAT}")
message("ASM files: " "${ASM_NASM_SOURCE}")
message("C files: " "${C_SOURCE}")
message("CXX files: " "${CXX_SOURCE}")
message("ASM compiler: " "${ACOMPILER}")
message("C compiler: " "${CCOMPILER}")
message("C++ compiler: " "${CXXCOMPILER}")
message("C flags: " "${CMAKE_C_FLAGS}")
message("C++ flags: " "${CMAKE_CXX_FLAGS}")
message("C++ command: ${CXXCOMPILER} ${CXX_SOURCE} ${CMAKE_CXX_FLAGS}")