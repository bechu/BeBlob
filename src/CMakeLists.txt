CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(beblob CXX)

SET(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

FIND_PACKAGE(SFML 2 REQUIRED system window graphics network audio)
FIND_PACKAGE(Zip REQUIRED)

ENABLE_LANGUAGE(CXX)
SET(CMAKE_CXX_FLAGS "-std=c++11 -Wall -Werror ${CMAKE_CXX_FLAGS}")

#SET(CMAKE_CXX_FLAGS " -g -O0 -fprofile-arcs -ftest-coverage ${CMAKE_CXX_FLAGS}")

#SET(CMAKE_CXX_FLAGS="-std=c++11 -Wall -Werror -g -O0 -Wall -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings -fprofile-arcs -ftest-coverage")

#SET(CMAKE_C_FLAGS="-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
#SET(CMAKE_EXE_LINKER_FLAGS="-fprofile-arcs -ftest-coverage")

INCLUDE_DIRECTORIES(
  ${SFML_INCLUDE_DIR}
  ${ZIP_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${SFML_INCLUDE_DIR}
  )

FOREACH(MODULE
    common
    systems
    components
    sf/
    math/
    sf/particles/
    ai/
    ai/jobs/
    ai/policy/
    ai/rl/
    graphic/
    graphic/ui/
    physic/
    ecs/
    )

  FILE(GLOB MODULES_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/${MODULE}/*.h)
  SET(SOURCES ${MODULES_SOURCES} ${SOURCES})
ENDFOREACH(MODULE)

ADD_EXECUTABLE(${PROJECT_NAME} blob.cpp ${SOURCES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${ZIP_LIBRARY} ${SFML_LIBRARIES})
