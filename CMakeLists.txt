cmake_minimum_required(VERSION 3.0)
add_subdirectory(ITK_C)

SET(REMOTE_ADDR 192.168.1.20)
ADD_CUSTOM_TARGET(upload
	COMMAND scp ${CMAKE_BINARY_DIR}/ITK_C/Examples/iri_* root@${REMOTE_ADDR}:/usr/bin
	COMMAND scp
		${CMAKE_BINARY_DIR}/ITK_C/Library/libindy-iri.*
		${CMAKE_BINARY_DIR}/ITK_C/Examples/libindy-ipj-util.*
	root@${REMOTE_ADDR}:/usr/lib
)
