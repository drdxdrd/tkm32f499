cmake_minimum_required(VERSION 3.5)

#
set(CMAKE_ASM_COMPILER_WORKS TRUE)
set(CMAKE_C_COMPILER_WORKS TRUE)
set(CMAKE_CXX_COMPILER_WORKS TRUE)

project(tkm32f499 LANGUAGES  CXX C ASM)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_SYSTEM_NAME Generic)
enable_language(ASM)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(RELATIVE_PRJ_DIR ".")
get_filename_component(PRJ_DIR ${RELATIVE_PRJ_DIR}  ABSOLUTE)

set(SRC_DIR "${PRJ_DIR}/src")

include_directories(${PRJ_DIR}/src)
include_directories(${PRJ_DIR}/src/hal)

file (GLOB_RECURSE H_FILES ${SRC_DIR}/*.h)
file (GLOB_RECURSE CPP_FILES ${SRC_DIR}/*.cc)
file (GLOB_RECURSE C_FILES ${SRC_DIR}/*.c)


# Create bin directory
set(BIN_DIR "${PRJ_DIR}/bin")
if(NOT EXISTS ${BIN_DIR})
    file(MAKE_DIRECTORY ${BIN_DIR})
endif()


# check toolcain path
if ("${TOOLCHAIN_PATH}" STREQUAL "")
  message(WARNING "variable TOOLCHAIN_PATH not set, use system toolchain")
else()
  message("TOOLCHAIN_PATH: ${TOOLCHAIN_PATH}")
  set(TOOLCHAIN_PATH "${TOOLCHAIN_PATH}/")
endif()


SET(CMAKE_ASM_COMPILER "${TOOLCHAIN_PATH}arm-none-eabi-gcc" )
SET(CMAKE_C_COMPILER "${TOOLCHAIN_PATH}arm-none-eabi-gcc" )
SET(CMAKE_CXX_COMPILER "${TOOLCHAIN_PATH}arm-none-eabi-c++" )
SET(CMAKE_C_AR "${TOOLCHAIN_PATH}arm-none-eabi-ar" )
SET(CMAKE_C_LINKER "${TOOLCHAIN_PATH}arm-none-eabi-ld" )
SET(CMAKE_OBJCOPY "${TOOLCHAIN_PATH}arm-none-eabi-objcopy" )
SET(CMAKE_OBJDUMP "${TOOLCHAIN_PATH}arm-none-eabi-objdump" )
SET(CMAKE_SIZE "${TOOLCHAIN_PATH}arm-none-eabi-size" )


set ( CMAKE_C_FLAGS "-g3 -O0 -Wall -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fdata-sections -ffunction-sections")
set ( CMAKE_CXX_FLAGS "-g3 -O0 -Wall -mthumb -mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fdata-sections -ffunction-sections")

#LDFLAGS
set(LD_FILE ${SRC_DIR}/system/tkm32f499_flash.ld)
set ( CMAKE_EXE_LINKER_FLAGS "-specs=nano.specs -specs=nosys.specs -Wl,--gc-sections" )
set ( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${LD_FILE}" )

#ASMFLAGS
set ( CMAKE_ASM_FLAGS "-x assembler-with-cpp ${CMAKE_C_FLAGS}")

# startup file
set(STARTUP_FILE ${SRC_DIR}/system/startup_tkm32f499.s)

add_executable ( ${PROJECT_NAME}.elf ${H_FILES} ${C_FILES} ${CPP_FILES} ${STARTUP_FILE})

# Create project.bin and copy it to bin directory
add_custom_command(TARGET ${CMAKE_PROJECT_NAME}.elf
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Obinary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMAND ${CMAKE_SIZE} ${PROJECT_NAME}.elf
    COMMENT "Copy to ${BIN_DIR}:------------"
    COMMAND cp ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin ${BIN_DIR}
    DEPENDS ${CMAKE_PROJECT_NAME}
    )


set (CLEAN_FILES
    ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
    ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.bin
    ${BIN_DIR}/${CMAKE_PROJECT_NAME}.bin
)

set_property(DIRECTORY PROPERTY
    ADDITIONAL_MAKE_CLEAN_FILES ${CLEAN_FILES}
)


