CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

# Port to Win32 system in progress

PROJECT( IrradianceEnvMap )

#cmake .. -DCMAKE_BUILD_TYPE:STRING=Release
SET( CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG") #-msse (to enable SSE instruction)
SET( CMAKE_CXX_FLAGS_DEBUG   "-O0 -g -Wall")

FILE( GLOB_RECURSE SRC src/* )

INCLUDE_DIRECTORIES( src/ extern/ )

# Dynamic libraries from the system
SET( PLATFORM_LIBS GL glut m )

# Static libraries to link
LINK_LIBRARIES("-L ../extern/FreeImage/ -lfreeimage")

# Static libraries to build
FILE( GLOB GLEW extern/GL/*.c extern/GL/*.h )
ADD_LIBRARY( glew ${GLEW} )

FILE( GLOB GLSW extern/glsw/*.c extern/glsw/*.h )
ADD_LIBRARY( glsw ${GLSW} )


ADD_EXECUTABLE( ../iem ${SRC} )

TARGET_LINK_LIBRARIES( ../iem ${PLATFORM_LIBS} glew glsw )

