set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR avr)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_AR                        avr-ar)
set(CMAKE_ASM_COMPILER              avr-gcc)
set(CMAKE_C_COMPILER                avr-gcc)
set(CMAKE_CXX_COMPILER              avr-g++)
set(CMAKE_LINKER                    avr-ld)
set(CMAKE_OBJCOPY                   avr-objcopy CACHE INTERNAL "")
set(CMAKE_RANLIB                    avr-ranlib CACHE INTERNAL "")
set(CMAKE_SIZE                      avr-size CACHE INTERNAL "")
set(CMAKE_STRIP                     avr-strip CACHE INTERNAL "")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
