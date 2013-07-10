PROJECT(MIST C CXX)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)

SET(CMAKE_COLOR_MAKEFILE ON)
SET(CMAKE_VERBOSE_MAKEFILE ON)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

FIND_PACKAGE(Threads REQUIRED)

# Find image libraries
FIND_PACKAGE(JPEG REQUIRED)
IF(NOT JPEG_FOUND)
        MESSAGE(SEND_ERROR 
                "Could not find JPEG library. Install it in Ubuntu using the following command
                sudo apt-get install libjpeg8 libjpeg8-dev")
ENDIF()

FIND_PACKAGE(PNG REQUIRED)
IF(NOT PNG_FOUND)
        MESSAGE(SEND_ERROR 
                "Could not find PNG library. Install it in Ubuntu using the following command
                sudo apt-get install libpng12-0 libpng12-dev")
ENDIF()

FIND_PACKAGE(TIFF REQUIRED)
IF(NOT TIFF_FOUND)
        MESSAGE(SEND_ERROR 
                "Could not find TIFF library. Install it in Ubuntu using the following command
                sudo apt-get install libtiff5 libtiff5-dev
                or sudo apt-get install libtiff4 libtiff4-dev")
ENDIF()

# Find Lapack libraries
SET(BLA_VENDOR "ATLAS")
FIND_PACKAGE(BLAS REQUIRED)
IF(NOT BLAS_FOUND)
ENDIF()
SET(BLAS_ATLAS_LIBRARIES ${BLAS_LIBRARIES})

SET(BLA_VENDOR "Generic")
FIND_PACKAGE(BLAS REQUIRED)
IF(NOT BLAS_FOUND)
ENDIF()
SET(BLAS_GENERIC_LIBRARIES ${BLAS_LIBRARIES})

FIND_PACKAGE(LAPACK REQUIRED)
IF(NOT LAPACK_FOUND)
ENDIF()

FIND_PACKAGE(F2C REQUIRED)
IF(NOT F2C_FOUND)
ENDIF()

SET(LIBS ${CMAKE_THREAD_LIBS_INIT} z m)
MESSAGE("LIBS: ${LIBS}")
SET(IMGLIBS ${JPEG_LIBRARY} ${PNG_LIBRARY} ${TIFF_LIBRARY})
MESSAGE("IMGLIBS: ${IMGLIBS}")
SET(LAPACKLIBS ${BLAS_GENERIC_LIBRARIES} ${BLAS_ATLAS_LIBRARIES} ${LAPACK_LIBRARIES} ${F2C_LIBRARIES})
MESSAGE("LAPACKLIBS: ${LAPACKLIBS}")

SET(CMAKE_CXX_FLAGS "-Wall -W -O2")

# Add directories where executables will be produced
ADD_SUBDIRECTORY(demo)
ADD_SUBDIRECTORY(test)
#ADD_SUBDIRECTORY(unittest)

