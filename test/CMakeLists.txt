# Look for header files
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

# Set the source files
SET(TEST_SRC array_test.cpp
	     type_test.cpp
	     memory_test.cpp
	     hash_test.cpp
	     callback_test.cpp
	     pointer_test.cpp
	     limits_test.cpp
	     operators_test.cpp
	     binary_test.cpp
	     color_test.cpp
	     quaternion_test.cpp
	     iterator_test.cpp
	     thread_test.cpp
	     region_growing_test.cpp
	     machine_learning_test.cpp
	     wav_test.cpp
	     margin_array_test.cpp
	     median_test.cpp
	     hough_test.cpp
	     morphology_test.cpp
	     fft_test.cpp
	     singleton_test.cpp
	     integer_test.cpp
	     decimal_test.cpp
	     vector_test.cpp
	     set_test.cpp
	     crc_test.cpp
	     random_test.cpp
	     minimization_test.cpp
	     mixture_test.cpp
	     environment_test.cpp
	     matrix_test.cpp
	     matrix_util_test.cpp
	     matrix_util_test2.cpp
	     image_test.cpp
	     raw_test.cpp
	     volumerender_test.cpp
	)

SET_SOURCE_FILES_PROPERTIES(${TEST_SRC} PROPERTIES LANGUAGE CXX)

ADD_EXECUTABLE(array_test array_test.cpp)
TARGET_LINK_LIBRARIES (array_test ${LIBS})

ADD_EXECUTABLE(type_test type_test.cpp)
TARGET_LINK_LIBRARIES (type_test ${LIBS})

ADD_EXECUTABLE(memory_test memory_test.cpp)
TARGET_LINK_LIBRARIES (memory_test ${LIBS})

ADD_EXECUTABLE(hash_test hash_test.cpp)
TARGET_LINK_LIBRARIES (hash_test ${LIBS})

ADD_EXECUTABLE(callback_test callback_test.cpp)
TARGET_LINK_LIBRARIES (callback_test ${LIBS})

ADD_EXECUTABLE(pointer_test pointer_test.cpp)
TARGET_LINK_LIBRARIES (pointer_test ${LIBS})

ADD_EXECUTABLE(matrix_test matrix_test.cpp)
TARGET_LINK_LIBRARIES (matrix_test ${LIBS} ${LAPACKLIBS})

