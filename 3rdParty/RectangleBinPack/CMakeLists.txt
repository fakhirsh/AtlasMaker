cmake_minimum_required(VERSION 2.8)


set ( HDR

	)
	
set ( SRC
		GuillotineBinPack.cpp
		MaxRectsBinPack.cpp
		Rect.cpp
		ShelfBinPack.cpp
		SkylineBinPack.cpp
	)

ADD_LIBRARY(RectangleBinPack STATIC
		${HDR}
		${SRC}
	)