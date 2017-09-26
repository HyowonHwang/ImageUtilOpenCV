#include <iostream>
#include "vector_test.h"
#include "scalar_test.h"
#include "mat_operator.h"
#include "read_image.h"
#include "read_video.h"

int main(int argc, char** argv) {
		std::cout << "----------vector----------" << std::endl;
		test_vector();
		std::cout << "----------scalar----------" << std::endl;
		test_scalar();
		std::cout << "----------mat_operator----------" << std::endl;
		test_mat_operator();

		if(argv[1] != nullptr) {
				std::string option(argv[1]);
				std::cout << "option " << option << std::endl;

				if(option == "image") {
						std::cout << "----------image_read----------" << std::endl;
						test_read_image("a.jpg");
				} else if ( option == "video") {
						std::cout << "----------video_read----------" << std::endl;
						test_read_video("blackpink_888.mp4");
				}
		}
		return 0;
}
