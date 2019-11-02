cmake_minimum_required(VERSION 2.8)

set ( HDR
		zlib.h
		zconf.h
	)
	
set ( SRC
		adler32.c
		compress.c
		crc32.c
		crc32.h
		deflate.c
		deflate.h
		gzclose.c
		gzguts.h
		gzlib.c
		gzread.c
		gzwrite.c
		infback.c
		inffast.c
		inffast.h
		inffixed.h
		inflate.c
		inflate.h
		inftrees.c
		inftrees.h
		trees.c
		trees.h
		uncompr.c
		zutil.c
		zutil.h
	)

ADD_LIBRARY(z STATIC
		${HDR}
		${SRC}
	)