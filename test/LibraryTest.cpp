#include <string>
#include <iostream>

#include <gtest/gtest.h>

#include "DuplicatesDeleter.h"

using namespace std::filesystem;
using namespace cv;

namespace cpp_practicing {

    TEST(DuplicatesDeleterTest, Initial) {

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