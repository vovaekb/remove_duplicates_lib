#include <string>
#include <iostream>

#include <gtest/gtest.h>
// #include <opencv2/core.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/highgui.hpp>

#include "DuplicatesDeleter.h"

using namespace std::filesystem;
using namespace cv;

namespace cpp_practicing {
    // void generate_images() {
    //     // image 1
    //     Mat img1 (300, 250, CV_8UC1, Scalar(0, 0, 0));
    //     //imwrite("img1.ppg", img1);
    //     std::cout << "image was saved" << std::endl;
    // }

    TEST(DuplicatesDeleterTest, Initial) {
        // generate_images();

        std::string files_path = "files";
        std::string result_path = "result_files";
        DuplicatesDeleter deleter;
        deleter.run(files_path, result_path);
        // ASSERT_TRUE(true);

        path dir_path = result_path;

        int files_number = 0;
        for (const auto& file : directory_iterator(dir_path))
        {
            ++files_number;
        }
        ASSERT_EQ(files_number, 3);
    }
}