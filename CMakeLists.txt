project(ogre-poc)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -ggdb)

# En caso de instalar Ogre en una ruta no estándar, se puede configurar acá de la siguiente forma:
set(OGRE_DIR $ENV{HOME}/opt/ogre/lib/OGRE/cmake)
find_package(OGRE)

# Estos includes suelen estar todos juntos en el sistema bajo la carpeta OGRE
#set(OGRE_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/ogre/OgreMain/include" "${OGRE_BASE_DIR}/include" "${OGRE_BASE_DIR}/Components/Bites/include" "${OGRE_BASE_DIR}/Components/RTShaderSystem/include" "${OGRE_BASE_DIR}/build/include")

message("Ogre include dirs: ${OGRE_INCLUDE_DIRS}")
message("Ogre base dirs: ${OGRE_BASE_DIRS}")
#include_directories(${OGRE_INCLUDE_DIRS})
#link_directories(${OGRE_BASE_DIR}/build/lib)
 
# Ogre necesita un archivo de recursos, podemos copiar el template del sistema si no tenemos uno
# file(COPY ${OGRE_CONFIG_DIR}/resources.cfg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
 
# add the source files as usual
add_executable(0_Bootstrap step-0/Bootstrap.cpp)
 
# this also sets the includes and pulls third party dependencies
target_link_libraries(0_Bootstrap zzip ${OGRE_LIBRARIES})
