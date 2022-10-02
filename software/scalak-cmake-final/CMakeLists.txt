cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "avr-toolchain.cmake")

project(scalak-cmake-final)

set(TARGET ${CMAKE_PROJECT_NAME}.elf)

add_subdirectory(scheduler-c)
add_subdirectory(state-machine-c)

add_executable(${TARGET}
	"app/src/main.c"
	"app/src/ws2812b.c"
	"app/src/rc5.c"
	"app/src/uart.c"
	"app/src/periph.c"
	"app/src/motors.c"
	"app/src/robot.c"
)

target_include_directories(${TARGET} PRIVATE
	"app/inc"
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

target_link_options(${TARGET} PRIVATE
	-mmcu=atmega328p
	-Wl,-u,vfprintf
	-lprintf_flt
)

target_link_libraries(${TARGET} PRIVATE
	scheduler
	fsm
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
