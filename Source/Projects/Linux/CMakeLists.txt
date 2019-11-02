cmake_minimum_required(VERSION 2.8)

set (PROJECT_NAME SpriteSheetGen)

project (${PROJECT_NAME})

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../../../Bin/Linux)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -lstdc++fs" )

CONFIGURE_FILE(../../Common/listdir.py ../../Bin/Linux/listdir.py COPYONLY)

include_directories(../../../3rdParty/tinyxml2)
include_directories(../../../3rdParty/glm-0.9.8.1)
include_directories(../../../3rdParty/RectangleBinPack)
include_directories(../../../3rdParty/libpng-1.6.15)

add_subdirectory(../../../3rdParty/tinyxml2 ${CMAKE_BINARY_DIR}/tinyxml2)
add_subdirectory(../../../3rdParty/libpng-1.6.15 ${CMAKE_BINARY_DIR}/libpng)
add_subdirectory(../../../3rdParty/RectangleBinPack ${CMAKE_BINARY_DIR}/RectangleBinPack)

set ( HDR
    ../../Common/Texture.hpp
	)

set ( SRC
    ../../Common/Texture.cpp
    main.cpp
	)


# Executable
add_executable(${PROJECT_NAME}
                ${HDR} 
                ${SRC} 
            )

	
TARGET_LINK_LIBRARIES( ${PROJECT_NAME}
                       tinyxml2 
                       png
                       RectangleBinPack
                       )
                       


