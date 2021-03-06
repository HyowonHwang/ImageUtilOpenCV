CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (image_util)

find_package(OpenCV REQUIRED)

# set dir for header
INCLUDE_DIRECTORIES (.)
SET(CMAKE_CXX_FLAGS 
    "-ggdb --std=c++11")

SET(TESSERACT_INCLUDE_DIR "/usr/local/include/tesseract")
SET(Tesseract_LIBS "-L/usr/local/lib -ltesseract -llept")

INCLUDE_DIRECTORIES(${OPENCV_INCLUDE_DIR})
include_directories(${TESSERACT_INCLUDE_DIR})

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
    edge_detection.cpp
    deblurring.cpp
    similarity.cpp
    face_detection.cpp
    scalar_test.cpp
    ocr_operation.cpp
)

ADD_EXECUTABLE (image_util ${BINSRCS})
TARGET_LINK_LIBRARIES(image_util ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(image_util ${Tesseract_LIBS})
