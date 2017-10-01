CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (image_util)

# set dir to store library
SET (BINARY_OUTPUT_PATH ../Build)

find_package(OpenCV REQUIRED)

# set dir for header
INCLUDE_DIRECTORIES (.)
SET(CMAKE_CXX_FLAGS 
    "-ggdb --std=c++11")

INCLUDE_DIRECTORIES(${OPENCV_INCLUDE_DIR})

# sources
SET (BINSRCS 
    main.cpp 
    vector_test.cpp
    mat_operator.cpp
    read_image.cpp
    read_video.cpp
    histogram.cpp
    image_merge.cpp
    image_filter.cpp
    deblurring.cpp
    scalar_test.cpp
)

ADD_EXECUTABLE (image_util ${BINSRCS})
TARGET_LINK_LIBRARIES(image_util ${OpenCV_LIBS})
