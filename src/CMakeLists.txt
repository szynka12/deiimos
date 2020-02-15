cmake_minimum_required(VERSION 3.1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


FIND_PACKAGE(deal.II 9.1.0 QUIET
  HINTS
    ${deal.II_DIR} ${DEAL_II_DIR} ../ ../../  $ENV{DEAL_II_DIR}
  )
IF(NOT ${deal.II_FOUND})
  MESSAGE(FATAL_ERROR "\n"
    "*** Could not locate a (sufficiently recent) version of deal.II. ***\n\n"
    "You may want to either pass a flag -DDEAL_II_DIR=/path/to/deal.II to cmake\n"
    "or set an environment variable \"DEAL_II_DIR\" that contains this path."
    )
ENDIF ()


set(TARGET "main")


DEAL_II_INITIALIZE_CACHED_VARIABLES()
PROJECT(${TARGET})


# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

set(TARGET_SRC  "main.cpp" 
                "${PROJECT_SOURCE_DIR}/io/xml/XmlNode.cpp"
                "${PROJECT_SOURCE_DIR}/io/console/msgstream.cpp"
                "${PROJECT_SOURCE_DIR}/io/xml/xmlexceptions.cpp")

# The following folder will be included
include_directories("${PROJECT_SOURCE_DIR}",  
                    "${PROJECT_SOURCE_DIR}/../external/src/tinyxml2"
                    "${PROJECT_SOURCE_DIR}/io/xml"
                    "${PROJECT_SOURCE_DIR}/io/console")

# add_executable(main ${PROJECT_SOURCE_DIR}/main.cpp ${SOURCES})


# find_library(TINYXML2 names tinyxml2
#              #paths "${PROJECT_SOURCE_DIR}/../external/lib/tinyxml2")
#              hints ../ ../external/lib/tinyxml2 )



DEAL_II_INVOKE_AUTOPILOT()

target_include_directories(${TARGET} PRIVATE "${PROJECT_SOURCE_DIR}" )
target_link_libraries(${TARGET} "${PROJECT_SOURCE_DIR}/../external/lib/tinyxml2/libtinyxml2.so")