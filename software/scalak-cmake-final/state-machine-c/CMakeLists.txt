
add_library(fsm
	src/finite_state_machine.c
)

target_include_directories(fsm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
