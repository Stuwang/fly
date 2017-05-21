INCLUDE_DIRECTORIES(
	${CMAKE_CURRENT_SOURCE_DIR}/../fly
	)

add_executable(main main.cpp)
target_link_libraries(main fly pthread)
# add_dependencies(main install)
set_property(TARGET main PROPERTY OUTPUT_NAME "test_net")