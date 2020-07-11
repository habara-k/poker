project(${CMAKE_PROJECT_NAME})

####################################################################################
# For poker library
####################################################################################

file(GLOB SOURCE_FILES *.cpp)
file(GLOB HEADER_FILES *.h)
add_library(${CMAKE_PROJECT_NAME} STATIC
            ${SOURCE_FILES} ${HEADER_FILES})
