project(library C)

add_definitions(-Os -Wall -Werror -Wextra --std=gnu99 -Wno-unused-parameter)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Nanopb
	${CMAKE_CURRENT_SOURCE_DIR}/PbMessages
)

if(WIN32)
	list(APPEND platform_src platform_win32.c)
	list(APPEND platform_libs winmm)
else(WIN32)
	if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		list(APPEND platform_src platform_osx.c)
	else()
		list(APPEND platform_src platform_linux.c)
	endif()
endif(WIN32)

file(GLOB_RECURSE nanopb_srcs "${CMAKE_CURRENT_SOURCE_DIR}/Nanopb/*.c")
file(GLOB_RECURSE nanopb_msg_srcs "${CMAKE_CURRENT_SOURCE_DIR}/PbMessages/*.c")
add_library(indy-iri SHARED iri.c ${nanopb_srcs} ${nanopb_msg_srcs} ${platform_src})
target_link_libraries(indy-iri ${platform_libs})
# No missing field initializers is necessary to prevent GCC from freaking out at the protobufs
add_definitions(-DPB_FIELD_16BIT -Wno-missing-field-initializers)

install(FILES iri.h DESTINATION include/iri)
install(FILES platform.h DESTINATION include/iri)
install(TARGETS indy-iri DESTINATION lib)
