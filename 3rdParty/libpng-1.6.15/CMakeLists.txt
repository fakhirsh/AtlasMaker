cmake_minimum_required(VERSION 2.8)

# Set include directories
include_directories(../zlib-1.2.8)

# Add library subdirectories
add_subdirectory(../zlib-1.2.8 ${CMAKE_BINARY_DIR}/libz)

ADD_LIBRARY(png STATIC
		png.c
		pngerror.c
		pngget.c
		pngmem.c
		pngpread.c
		pngread.c
		pngrio.c
		pngrtran.c
		pngrutil.c
		pngset.c
		pngtrans.c
		pngwio.c
		pngwrite.c
		pngwtran.c
		pngwutil.c
	)

target_link_libraries( png 
			z
			  )