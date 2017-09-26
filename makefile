all: main

SRC := main.cpp \
			vector_test.cpp \
			mat_operator.cpp \
			read_image.cpp \
			read_video.cpp \
			histogram.cpp \
			scalar_test.cpp 

main:
	g++ -std=c++11 $(SRC) `pkg-config --libs --cflags opencv` -o main

clean:
	rm -rf main
