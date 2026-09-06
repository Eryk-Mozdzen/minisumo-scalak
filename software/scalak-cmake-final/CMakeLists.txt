cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "avr-toolchain.cmake")

project(scalak-cmake-final)

set(TARGET_ELF ${CMAKE_PROJECT_NAME}.elf)
set(TARGET_HEX ${CMAKE_PROJECT_NAME}.hex)
set(TARGET_DUMP ${CMAKE_PROJECT_NAME}.dump)

add_executable(${TARGET_ELF}
	"src/main.c"
	"src/led.c"
	"src/periph.c"
	"src/motors.c"
	"src/robot.c"
	"src/scheduler.c"
	"src/state_machine.c"
)

target_include_directories(${TARGET_ELF} PRIVATE
	"inc"
)

target_compile_options(${TARGET_ELF} PRIVATE
	-Os
	-Wall
	-Wextra
	-Wpedantic
	-mmcu=atmega328p
)

target_compile_definitions(${TARGET_ELF} PRIVATE
	F_CPU=8000000UL
	TICK_MS=8
)

include(options.cmake)

target_link_options(${TARGET_ELF} PRIVATE
	-mmcu=atmega328p
)

add_custom_command(TARGET ${TARGET_ELF} POST_BUILD
	COMMAND ${CMAKE_OBJCOPY} -O ihex ${TARGET_ELF} ${TARGET_HEX}
	COMMAND ${CMAKE_OBJDUMP} -j .text -D ${TARGET_ELF} > ${TARGET_DUMP}
	COMMAND ${CMAKE_SIZE} ${TARGET_ELF}
)

add_custom_target(flash
	avrdude -p m328p -c usbasp -e -U flash:w:${TARGET_HEX}:i
	DEPENDS ${TARGET_ELF}
)
