project (SDL2_SetAlphaColor)

ADD_DEFINITIONS(-DUNICODE -D_UNICODE)

#include header_files
#SET(SDL_TTF_INCLUDE ${PROJECT_SOURCE_DIR}/../lib/SDL2_ttf-devel-2.0.15-VC/SDL2_ttf-2.0.15/include)

SET(SDL_INCLUDE ${PROJECT_SOURCE_DIR}/../lib/SDL2-2.0.9/)

SET(EXTERN_INCLUDE ${PROJECT_SOURCE_DIR}/../include/)

INCLUDE_DIRECTORIES (${SDL_INCLUDE} ${EXTERN_INCLUDE} ${SDL_TTF_INCLUDE})

#lib 
IF(WIN32)
	SET(SDL_LIB 
	${PROJECT_SOURCE_DIR}/../lib/SDL2-2.0.9/lib/x86 
	${PROJECT_SOURCE_DIR}/../lib/SDL2_ttf-devel-2.0.15-VC/SDL2_ttf-2.0.15/lib/x86
	)
ELSE()
	SET(SDL_LIB 
	${PROJECT_SOURCE_DIR}/../lib/SDL2-2.0.9/lib/x64  
	${PROJECT_SOURCE_DIR}/../lib/SDL2_ttf-devel-2.0.15-VC/SDL2_ttf-2.0.15/lib/x64
	)
ENDIF(WIN32)
	
LINK_DIRECTORIES(${SDL_LIB})

#SDL2 lib库链接
LINK_LIBRARIES(SDL2 SDL2main SDL2test SDL2_image SDL2_ttf)

#extension lib SDL_IMAGE
#LINK_LIBRARIES(SDL2_image SDL2_ttf)

FILE (GLOB header_files *.h)
FILE (GLOB source_files *.c *.cpp)

ADD_EXECUTABLE (SDL2_SetAlphaColor ${header_files} ${source_files})

TARGET_LINK_LIBRARIES (SDL2_SetAlphaColor)