project(examples C)

add_definitions(-Os -Wall -Werror -Wextra --std=gnu99 -Wno-unused-parameter)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${library_SOURCE_DIR}
)

add_library(indy-ipj-util SHARED ipj_util.c)
target_link_libraries(indy-ipj-util indy-iri)
file(GLOB_RECURSE example_srcs "${CMAKE_CURRENT_SOURCE_DIR}/IRI_*.c")

foreach(src_file ${example_srcs})
	get_filename_component(bin_name ${src_file} NAME_WE)
	string(TOLOWER ${bin_name} bin_name)
	add_executable(${bin_name} ${src_file})
	target_link_libraries(${bin_name} indy-ipj-util indy-iri)
	list(APPEND executables ${bin_name})
endforeach()

install(TARGETS ${executables} RUNTIME DESTINATION bin)
install(TARGETS indy-ipj-util DESTINATION lib)
