# Look for header files
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})


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

ADD_EXECUTABLE(matrix_util_test matrix_util_test.cpp)
TARGET_LINK_LIBRARIES (matrix_util_test ${LIBS} ${LAPACKLIBS})

ADD_EXECUTABLE(matrix_util_test2 matrix_util_test2.cpp)
TARGET_LINK_LIBRARIES (matrix_util_test2 ${LIBS} ${LAPACKLIBS})

ADD_EXECUTABLE(vector_test vector_test.cpp)
TARGET_LINK_LIBRARIES (vector_test ${LIBS} ${LAPACKLIBS})

ADD_EXECUTABLE(region_growing_test region_growing_test.cpp)
TARGET_LINK_LIBRARIES (region_growing_test ${LIBS} ${LAPACKLIBS})

ADD_EXECUTABLE(limits_test limits_test.cpp)
TARGET_LINK_LIBRARIES (limits_test ${LIBS})

ADD_EXECUTABLE(operators_test operators_test.cpp)
TARGET_LINK_LIBRARIES (operators_test ${LIBS})

ADD_EXECUTABLE(binary_test binary_test.cpp)
TARGET_LINK_LIBRARIES (binary_test ${LIBS})

ADD_EXECUTABLE(color_test color_test.cpp)
TARGET_LINK_LIBRARIES (color_test ${LIBS})

ADD_EXECUTABLE(quaternion_test quaternion_test.cpp)
TARGET_LINK_LIBRARIES (quaternion_test ${LIBS})

ADD_EXECUTABLE(iterator_test iterator_test.cpp)
TARGET_LINK_LIBRARIES (iterator_test ${LIBS})

ADD_EXECUTABLE(thread_test thread_test.cpp)
TARGET_LINK_LIBRARIES (thread_test ${LIBS})

ADD_EXECUTABLE(image_test image_test.cpp image_test_window.cpp)
TARGET_LINK_LIBRARIES (image_test ${LIBS} ${FLTKLIBS} ${IMGLIBS} ${OPENGLLIBS} ${CMAKE_DL_LIBS})

ADD_EXECUTABLE(raw_test raw_test.cpp ct_image_window.cpp)
TARGET_LINK_LIBRARIES (raw_test ${LIBS} ${FLTKLIBS} ${IMGLIBS} ${OPENGLLIBS} ${CMAKE_DL_LIBS})

ADD_EXECUTABLE(volumerender_test volumerender_test.cpp volr_image_window.cpp)
TARGET_LINK_LIBRARIES (volumerender_test ${LIBS} ${FLTKLIBS} ${IMGLIBS} ${OPENGLLIBS} ${CMAKE_DL_LIBS})

ADD_EXECUTABLE(wav_test wav_test.cpp)
TARGET_LINK_LIBRARIES (wav_test ${LIBS})

ADD_EXECUTABLE(margin_array_test margin_array_test.cpp)
TARGET_LINK_LIBRARIES (margin_array_test ${LIBS})

ADD_EXECUTABLE(median_test median_test.cpp)
TARGET_LINK_LIBRARIES (median_test ${LIBS} ${IMGLIBS})

ADD_EXECUTABLE(hough_test hough_test.cpp)
TARGET_LINK_LIBRARIES (hough_test ${LIBS} ${IMGLIBS})

ADD_EXECUTABLE(morphology_test morphology_test.cpp)
TARGET_LINK_LIBRARIES (morphology_test ${LIBS} ${IMGLIBS})

ADD_EXECUTABLE(fft_test fft_test.cpp)
TARGET_LINK_LIBRARIES (fft_test ${LIBS})

ADD_EXECUTABLE(singleton_test singleton_test.cpp)
TARGET_LINK_LIBRARIES (singleton_test ${LIBS})

ADD_EXECUTABLE(integer_test integer_test.cpp)
TARGET_LINK_LIBRARIES (integer_test ${LIBS})

ADD_EXECUTABLE(decimal_test decimal_test.cpp)
TARGET_LINK_LIBRARIES (decimal_test ${LIBS})

ADD_EXECUTABLE(set_test set_test.cpp)
TARGET_LINK_LIBRARIES (set_test ${LIBS})

ADD_EXECUTABLE(crc_test crc_test.cpp)
TARGET_LINK_LIBRARIES (crc_test ${LIBS})

ADD_EXECUTABLE(random_test random_test.cpp)
TARGET_LINK_LIBRARIES (random_test ${LIBS})

ADD_EXECUTABLE(minimization_test minimization_test.cpp)
TARGET_LINK_LIBRARIES (minimization_test ${LIBS})

ADD_EXECUTABLE(mixture_test mixture_test.cpp)
TARGET_LINK_LIBRARIES (mixture_test ${LIBS})

ADD_EXECUTABLE(environment_test environment_test.cpp)
TARGET_LINK_LIBRARIES (environment_test ${LIBS})

ADD_EXECUTABLE(machine_learning_test machine_learning_test.cpp)
TARGET_LINK_LIBRARIES (machine_learning_test ${LIBS})

