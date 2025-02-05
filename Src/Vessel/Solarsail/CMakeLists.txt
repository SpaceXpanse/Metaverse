# Copyright (c) Martin Schweiger
# Licensed under the MIT License

set(MESH_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Meshes)
#set(TEXTURE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Textures)
set(CONFIG_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Config)

add_library(Solarsail SHARED
	Solarsail.cpp
	SailLua.cpp
)

set_target_properties(Solarsail
	PROPERTIES
	FOLDER Vessels/Solarsail
)

target_include_directories(Solarsail
	PUBLIC ${SPACEXPANSE_SOURCE_SDK_INCLUDE_DIR}
	PUBLIC ${CMAKE_CURRENT_BINARY_DIR}                 # for the dynamically generated header files
)

target_link_libraries(Solarsail
	${SPACEXPANSE_LIB}
	${SPACEXPANSE_SDK_LIB}
	${LUA_LIB}
)

add_custom_command(
	TARGET Solarsail PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CONFIG_SOURCE_DIR}/ ${CMAKE_BINARY_DIR}/Config/Vessels
)

add_custom_command(
	TARGET Solarsail PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${MESH_SOURCE_DIR}/ ${CMAKE_BINARY_DIR}/Meshes
)

#add_custom_command(
#	TARGET Solarsail PRE_BUILD
#	COMMAND ${CMAKE_COMMAND} -E copy_directory ${TEXTURE_SOURCE_DIR}/ ${SPACEXPANSE_BINARY_TEXTURES_DIR}   # this should really go to its own subdirectory
#)

# scan mesh files for labels and output to header file
add_custom_command(OUTPUT meshres.h
	COMMAND ${SPACEXPANSE_TOOL_MESHC} /I ${MESH_SOURCE_DIR}/SolarSail.msh /P - /O meshres.h
	DEPENDS ${MESH_SOURCE_DIR}/SolarSail.msh
)
add_custom_target(Solarsail_Prerequisites
	DEPENDS meshres.h
)
add_dependencies(Solarsail_Prerequisites
	meshc
)

add_dependencies(Solarsail
	${SpaceXpanseTgt}
	SpaceXpansesdk
	Solarsail_Prerequisites
)

set_target_properties(Solarsail_Prerequisites
	PROPERTIES
	FOLDER Vessels/Solarsail
)


# Installation
install(TARGETS Solarsail
	RUNTIME
	DESTINATION ${SPACEXPANSE_INSTALL_MODULE_DIR}
)
install(DIRECTORY ${CONFIG_SOURCE_DIR}/
	DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}/Config/Vessels
)
install(DIRECTORY ${MESH_SOURCE_DIR}/
	DESTINATION ${SPACEXPANSE_INSTALL_ROOT_DIR}/Meshes
)
#install(DIRECTORY ${TEXTURE_SOURCE_DIR}/
#	DESTINATION ${SPACEXPANSE_INSTALL_TEXTURES_DIR}
#)
