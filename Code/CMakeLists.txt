cmake_minimum_required(VERSION 3.10)

PROJECT (ICP3038-img-class VERSION 0.2)


# Use C++ 11
set(CMAKE_CXX_STANDARD 11) # C++11
set(CMAKE_CXX_STANDARD_REQUIRED ON) # C++11 is required (i.e. not optional)
set(CMAKE_CXX_EXTENSIONS OFF) # without compiler extensions like gnu++11

# Find OpenCV
LIST(APPEND CMAKE_PREFIX_PATH  "C:/opencv/build")
FIND_PACKAGE(OpenCV REQUIRED)
IF(OpenCV_FOUND)
    add_definitions(-DHAS_OPENCV)

    # If windows is used, copy the dlls into the project directory
    SET (CV_VERSION_STRING ${OpenCV_VERSION_MAJOR}${OpenCV_VERSION_MINOR}${OpenCV_VERSION_PATCH})
    IF (WIN32)

        IF ( ${OpenCV_VERSION_MAJOR} EQUAL 4)
            IF (EXISTS "${OpenCV_DIR}/x64/vc15/bin/opencv_videoio_ffmpeg${CV_VERSION_STRING}_64.dll")
                FILE (COPY        "${OpenCV_DIR}/x64/vc15/bin/opencv_videoio_ffmpeg${CV_VERSION_STRING}_64.dll"
                      DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
            ELSE ()
                MESSAGE (WARNING "opencv_videoio_ffmpeg${CV_VERSION_STRING}_64.dll is not in ${OpenCV_DIR}/x64/vc15/bin/, you have to make sure is it in the PATH or to copy it manually in your project binary directory")
            ENDIF ()
        ELSE ()
            IF (EXISTS "${OpenCV_DIR}/x64/vc15/bin/opencv_ffmpeg${CV_VERSION_STRING}_64.dll")
                FILE (COPY        "${OpenCV_DIR}/x64/vc15/bin/opencv_ffmpeg${CV_VERSION_STRING}_64.dll"
                      DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
            ELSE ()
                MESSAGE (WARNING "opencv_ffmpeg${CV_VERSION_STRING}_64.dll is not in ${OpenCV_DIR}/x64/vc15/bin/, you have to make sure is it in the PATH or to copy it manually in your project binary directory")
            ENDIF ()
        ENDIF ()

        IF (EXISTS "${OpenCV_DIR}/x64/vc15/bin/opencv_world${CV_VERSION_STRING}.dll")
            FILE (COPY        "${OpenCV_DIR}/x64/vc15/bin/opencv_world${CV_VERSION_STRING}.dll"
                  DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
        ELSE ()
            MESSAGE (WARNING "opencv_world${CV_VERSION_STRING}.dll is not in ${OpenCV_DIR}/x64/vc15/bin/, you have to make sure is it in the PATH or to copy it manually in your project binary directory")
        ENDIF ()

        IF (EXISTS "${OpenCV_DIR}/x64/vc15/bin/opencv_world${CV_VERSION_STRING}d.dll")
            FILE (COPY        "${OpenCV_DIR}/x64/vc15/bin/opencv_world${CV_VERSION_STRING}d.dll"
                  DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")
        ELSE ()
            MESSAGE (WARNING "opencv_world${CV_VERSION_STRING}d.dll is not in ${OpenCV_DIR}/x64/vc15/bin/, you have to make sure is it in the PATH or to copy it manually in your project binary directory")
        ENDIF ()

    ENDIF (WIN32)

ELSE(OpenCV_FOUND)
    MESSAGE(WARNING "OpenCV not found.")
ENDIF(OpenCV_FOUND)


# Build GoogleTest
INCLUDE(cmake/External_GTest.cmake)

# Enable unit testing
enable_testing()

#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(test-constructors
    include/Image.h
    src/Image.cxx
    src/test-constructors.cxx)

# Add dependency
ADD_DEPENDENCIES(test-constructors googletest)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(test-constructors PUBLIC include)
target_include_directories(test-constructors PUBLIC ${GTEST_INCLUDE_DIRS})

IF(OpenCV_FOUND)
    target_include_directories(test-constructors PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_directories(test-constructors PUBLIC ${GTEST_LIBS_DIR})
target_link_libraries(test-constructors ${GTEST_LIBRARIES} ${OpenCV_LIBS})

# Add the unit test
add_test (Constructors test-constructors)

#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(test-operators
    include/Image.h
    src/Image.cxx
    src/test-operators.cxx)

# Add dependency
ADD_DEPENDENCIES(test-operators googletest)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(test-operators PUBLIC include)
target_include_directories(test-operators PUBLIC ${GTEST_INCLUDE_DIRS})

IF(OpenCV_FOUND)
    target_include_directories(test-operators PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_directories(test-operators PUBLIC ${GTEST_LIBS_DIR})
target_link_libraries(test-operators ${GTEST_LIBRARIES} ${OpenCV_LIBS})

# Add the unit test
add_test (Operators test-operators)

#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(test-load-save
    include/Image.h
    src/Image.cxx
    src/test-load-save.cxx)

# Add dependency
ADD_DEPENDENCIES(test-load-save googletest)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(test-load-save PUBLIC include)
target_include_directories(test-load-save PUBLIC ${GTEST_INCLUDE_DIRS})

IF(OpenCV_FOUND)
    target_include_directories(test-load-save PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_directories(test-load-save PUBLIC ${GTEST_LIBS_DIR})
target_link_libraries(test-load-save ${GTEST_LIBRARIES} ${OpenCV_LIBS})

# Add the unit test
add_test (LoadSave test-load-save)

#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(display
    include/Image.h
    src/Image.cxx
    src/display.cxx)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(display PUBLIC include)

IF(OpenCV_FOUND)
    target_include_directories(display PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_libraries(display ${OpenCV_LIBS})

#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(contrastEnhancement
    include/Image.h
    src/Image.cxx
    src/contrastEnhancement.cxx)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(contrastEnhancement PUBLIC include)

IF(OpenCV_FOUND)
    target_include_directories(contrastEnhancement PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_libraries(contrastEnhancement ${OpenCV_LIBS})

#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(negative
    include/Image.h
    src/Image.cxx
    src/negative.cxx)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(negative PUBLIC include)

IF(OpenCV_FOUND)
    target_include_directories(negative PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_libraries(negative ${OpenCV_LIBS})

#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(blending
    include/Image.h
    src/Image.cxx
    src/blending.cxx)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(blending PUBLIC include)

IF(OpenCV_FOUND)
    target_include_directories(blending PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_libraries(blending ${OpenCV_LIBS})

#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(rmse-test
    include/Image.h
    src/Image.cxx
    src/rmse-test.cxx)

# Add dependency
ADD_DEPENDENCIES(rmse-test googletest)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(rmse-test PUBLIC include)
target_include_directories(rmse-test PUBLIC ${GTEST_INCLUDE_DIRS})

IF(OpenCV_FOUND)
    target_include_directories(rmse-test PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_directories(rmse-test PUBLIC ${GTEST_LIBS_DIR})
target_link_libraries(rmse-test ${GTEST_LIBRARIES} ${OpenCV_LIBS})

# Add the unit test
add_test (testrmse rmse-test)

#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(zncc-test
    include/Image.h
    src/Image.cxx
    src/zncc-test.cxx)

# Add dependency
ADD_DEPENDENCIES(zncc-test googletest)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(zncc-test PUBLIC include)
target_include_directories(zncc-test PUBLIC ${GTEST_INCLUDE_DIRS})

IF(OpenCV_FOUND)
    target_include_directories(zncc-test PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_directories(zncc-test PUBLIC ${GTEST_LIBS_DIR})
target_link_libraries(zncc-test ${GTEST_LIBRARIES} ${OpenCV_LIBS})

# Add the unit test
add_test (testzncc zncc-test)
#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(gaussianFilter
    include/Image.h
    src/Image.cxx
    src/gaussianFilter.cxx)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(gaussianFilter PUBLIC include)

IF(OpenCV_FOUND)
    target_include_directories(gaussianFilter PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_libraries(gaussianFilter ${OpenCV_LIBS})
#---------------------------------------------------------------------------

# Compilation
ADD_EXECUTABLE(boxFilter
    include/Image.h
    src/Image.cxx
    src/boxFilter.cxx)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(boxFilter PUBLIC include)

IF(OpenCV_FOUND)
    target_include_directories(boxFilter PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_libraries(boxFilter ${OpenCV_LIBS})
#---------------------------------------------------------------------------

# Compilation
ADD_EXECUTABLE(laplacianFilter
    include/Image.h
    src/Image.cxx
    src/laplacianFilter.cxx)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(laplacianFilter PUBLIC include)

IF(OpenCV_FOUND)
    target_include_directories(laplacianFilter PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_libraries(laplacianFilter ${OpenCV_LIBS})

#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(gradientMagnitude
    include/Image.h
    src/Image.cxx
    src/gradientMagnitude.cxx)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(gradientMagnitude PUBLIC include)

IF(OpenCV_FOUND)
    target_include_directories(gradientMagnitude PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_libraries(gradientMagnitude ${OpenCV_LIBS})
#---------------------------------------------------------------------------

# Compilation
ADD_EXECUTABLE(sharpen
    include/Image.h
    src/Image.cxx
    src/sharpen.cxx)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(sharpen PUBLIC include)

IF(OpenCV_FOUND)
    target_include_directories(sharpen PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_libraries(sharpen ${OpenCV_LIBS})
#---------------------------------------------------------------------------
# Compilation
ADD_EXECUTABLE(test_assignment
    include/Image.h
    src/Image.cxx
    src/test_assignment.cxx)

# Add dependency
ADD_DEPENDENCIES(test_assignment googletest)

# Add include directories
TARGET_INCLUDE_DIRECTORIES(test_assignment PUBLIC include)
target_include_directories(test_assignment PUBLIC ${GTEST_INCLUDE_DIRS})

IF(OpenCV_FOUND)
    target_include_directories(test_assignment PUBLIC ${OpenCV_INCLUDE_DIRS})
ENDIF(OpenCV_FOUND)

# Add linkage
target_link_directories(test_assignment PUBLIC ${GTEST_LIBS_DIR})
target_link_libraries(test_assignment ${GTEST_LIBRARIES} ${OpenCV_LIBS})

# Add the unit test
add_test (testzncc_ta test_assignment)
add_test (testrmse_ta test_assignment)
add_test (conv2d test_assignment)
add_test (test_constructor test_assignment)
add_test (load_save test_assignment)
add_test (operators_ta test_assignment)
add_test (test_assignment_outputs test_assignment)





#---------------------------------------------------------------------------

# The documentation build is an option. Set it to ON by default
option(BUILD_DOC "Build documentation" ON)

# Check if Doxygen is installed
find_package(Doxygen)
if (DOXYGEN_FOUND)

    SET (PROJECT_NAME ICP3038-img-class)
    SET (PROJECT_DESCRIPTION "An Image class used in ICP3038 at Bangor University")
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    # Add a custom target
    add_custom_target( doc_doxygen ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM )
else (DOXYGEN_FOUND)
    message(WARNING "Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/docs/tulips.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
