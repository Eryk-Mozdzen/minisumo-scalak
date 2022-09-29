
add_library(scheduler
	src/scheduler.c
)

target_include_directories(scheduler PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
