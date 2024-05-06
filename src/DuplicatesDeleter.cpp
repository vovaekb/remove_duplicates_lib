#include <vector>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "DuplicatesDeleter.h"

// using fs = std::filesystem;
using namespace std::filesystem;
using namespace cv;

namespace cpp_practicing {
    using string_vector = std::vector<std::string>;

    void generate_images() {
        // image 1
        Mat img1 (300, 250, CV_8UC1, Scalar(0, 0, 0));
        imwrite("files/img1.jpg", img1);

        Mat img2 (300, 250, CV_8UC1, Scalar(0, 0, 0));
        Vec3b& pixel1_color = img2.at<Vec3b>(46, 100);
        pixel1_color[0] = 146;
        pixel1_color[1] = 78;
        img2.at<Vec3b>(Point(100, 46)) = pixel1_color;

        Vec3b& pixel2_color = img2.at<Vec3b>(85, 130);
        pixel1_color[0] = 210;
        pixel1_color[1] = 37;
        img2.at<Vec3b>(Point(130, 85)) = pixel2_color;

        imwrite("files/img2.jpg", img2);

        // image 3
        Mat img3 (300, 250, CV_8UC1, Scalar(0, 0, 0));
        imwrite("files/img3.jpg", img3);

        // image 4
        Mat img4 (300, 250, CV_8UC1, Scalar(0, 0, 0));
        imwrite("files/img4.jpg", img4);

        std::cout << "image was saved" << std::endl;
    }

    DuplicatesDeleter::DuplicatesDeleter() {}

    void DuplicatesDeleter::run(const std::string& files_path, const std::string& result_path) {
        std::cout << "run pipeline" << std::endl;
        string_vector image_files;
        string_vector result_files;

        generate_images();

        path dir_path = files_path;

        if (!exists(dir_path) || !is_directory(dir_path))
        {
            std::cerr << "folder not found" << std::endl;
        }

        getFiles(files_path, image_files);

        std::cout << "image files" << std::endl;
        for (const auto& file_name : image_files)
        {
            std::cout << file_name << std::endl;
        }

        processFiles(files_path, image_files, result_files);

        std::cout << "\nresult files after processFiles()" << std::endl;
        for (const auto& file_name : result_files)
        {
            std::cout << file_name << std::endl;
        }
        saveResults(files_path, result_path, result_files);
    }

    bool DuplicatesDeleter::areTheSame(const std::string& first_file, const std::string& second_file) const {
        std::cout << "areTheSame " << first_file << " -- " << second_file << std::endl;

        Mat first_image = imread(first_file, IMREAD_COLOR);
        Mat second_image = imread(second_file, IMREAD_COLOR);
        std::cout << "size: " << first_image.rows << ", " << first_image.cols << std::endl;

        cvtColor(first_image, first_image, COLOR_BGR2GRAY);
        cvtColor(second_image, second_image, COLOR_BGR2GRAY);

        Mat diff;
        compare(first_image, second_image, diff, CMP_EQ);
        auto total_pixels = first_image.rows * first_image.cols;
        auto different_pixels = total_pixels - countNonZero(diff);
        std::cout << "different_pixels: " << different_pixels << std::endl;
        if (different_pixels == 0) return true;

        return false;
    }

    void DuplicatesDeleter::getFiles(const std::string& files_path, string_vector& image_files) {
        std::cout << "get files" << std::endl;

        path dir_path = files_path;

        for (const auto& file : directory_iterator(dir_path))
        {
            auto file_path = file.path();
            auto file_name = file_path.filename();
            std::cout << "file " << file_path.filename() << ", " << file_path.extension() << std::endl;
            if (file_path.extension() == ".jpg" || file_path.extension() == ".png") {
                image_files.emplace_back(file_name);
            }
        }

        std::sort(image_files.begin(), image_files.end());

        std::cout << "image files" << std::endl;
        for (const auto& file_name : image_files)
        {
            std::cout << file_name << std::endl;
        }
    }

    void DuplicatesDeleter::processFiles(const std::string& files_path, string_vector& image_files, string_vector& result_files) {
        std::cout << "process files" << std::endl;

        result_files.emplace_back(image_files[0]);

        for (size_t i = 1; i < image_files.size(); ++i)
        {
            path first_image_file_path = files_path;
            first_image_file_path /= image_files[i - 1];
            auto second_image_file = image_files[i];
            path second_image_file_path = files_path;
            second_image_file_path /= second_image_file;

            auto are_the_same_images = areTheSame(first_image_file_path.c_str(), second_image_file_path.c_str());
            std::cout << "are_the_same_images: " << are_the_same_images << std::endl;
            if (!are_the_same_images) result_files.emplace_back(second_image_file);
        }

        std::cout << "\nresult files" << std::endl;
        for (const auto& file_name : result_files)
        {
            std::cout << file_name << std::endl;
        }
        
    }

    void DuplicatesDeleter::saveResults(const std::string& files_path, const std::string& result_path, string_vector& result_files) {
        std::cout << "save results" << std::endl;

        // path result_path = 
        if (!exists(result_path)) {
            create_directory(result_path);
            std::cout << "directory was created" << std::endl;
        }

        for (const auto& file_name : result_files)
        {
            path source_image_file_path = files_path;
            source_image_file_path /= file_name;

            path target_image_file_path = result_path;
            target_image_file_path /= file_name;
            std::cout << "copy files from " << source_image_file_path.c_str() << " to " << target_image_file_path.c_str() << std::endl;
            copy(source_image_file_path.c_str(), target_image_file_path.c_str());
        }
    }

}