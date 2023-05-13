cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "avr-toolchain.cmake")

project(scalak-cmake-final)

set(TARGET ${CMAKE_PROJECT_NAME}.elf)

add_executable(${TARGET}
	"src/main.c"
	"src/led.c"
	"src/periph.c"
	"src/motors.c"
	"src/robot.c"
	"src/scheduler.c"
	"src/state_machine.c"
)

target_include_directories(${TARGET} PRIVATE
	"inc"
)

target_compile_options(${TARGET} PRIVATE
	-Os
	-Wall
	-Wextra
	-Wpedantic
	-mmcu=atmega328p
)

target_compile_definitions(${TARGET} PRIVATE
	F_CPU=8000000UL
	TICK_MS=8
)

set(PRINT OFF)
set(PRINT ${PRINT} CACHE BOOL "")
if(NOT DEFINED PRINT)
    set(PRINT ${PRINT})
endif()

set(MODULE OFF)
set(MODULE ${MODULE} CACHE BOOL "")
if(NOT DEFINED MODULE)
    set(MODULE ${MODULE})
endif()

message(STATUS "printf over UART: ${PRINT}")
message(STATUS "external module:  ${MODULE}")

if(PRINT)
	target_compile_definitions(${TARGET} PRIVATE PRINT)
	target_link_options(${TARGET} PRIVATE -Wl,-u,vfprintf -lprintf_flt)
	target_sources(${TARGET} PRIVATE "src/uart.c")
endif()

if(MODULE)
	target_compile_definitions(${TARGET} PRIVATE EXTERNAL_MODULE)
else()
	target_sources(${TARGET} PRIVATE "src/rc5.c")
endif()

target_link_options(${TARGET} PRIVATE
	-mmcu=atmega328p
)

add_custom_command(
	TARGET ${TARGET} POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -O ihex ${TARGET} ${CMAKE_PROJECT_NAME}.hex
	COMMAND ${CMAKE_OBJDUMP} -j .text -D ${TARGET} > ${CMAKE_PROJECT_NAME}.dump
	COMMAND ${CMAKE_SIZE} ${TARGET}
)

add_custom_target(flash
	avrdude -p m328p -c usbasp -e -U flash:w:${CMAKE_PROJECT_NAME}.hex:i
	DEPENDS ${TARGET}
)
