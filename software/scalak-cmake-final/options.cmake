
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
	target_compile_definitions(${TARGET_ELF} PRIVATE PRINT)
	target_link_options(${TARGET_ELF} PRIVATE -Wl,-u,vfprintf -lprintf_flt)
	target_sources(${TARGET_ELF} PRIVATE "src/uart.c")
endif()

if(MODULE)
	target_compile_definitions(${TARGET_ELF} PRIVATE EXTERNAL_MODULE)
else()
	target_sources(${TARGET_ELF} PRIVATE "src/rc5.c")
endif()
