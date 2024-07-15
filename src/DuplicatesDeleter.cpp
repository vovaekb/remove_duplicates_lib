#include <vector>
#include <iostream>
#include <algorithm>
#include <filesystem>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include "DuplicatesDeleter.h"

using namespace std::filesystem;
using namespace cv;

namespace cpp_practicing {
    using string_vector = std::vector<std::string>;

    void generate_images() {
        // image 1
        cv::Mat img1 (300, 250, CV_8UC3, Scalar(0, 0, 0));
        imwrite("files/img1.jpg", img1);

        cv::Mat img2 (300, 250, CV_8UC3, Scalar(0, 0, 0));
        cv::Vec3b& pixel1_color = img2.at<cv::Vec3b>(46, 100);
        pixel1_color[0] = 146;
        pixel1_color[1] = 78;
        img2.at<cv::Vec3b>(cv::Point(100, 46)) = pixel1_color;

        cv::Vec3b& pixel2_color = img2.at<cv::Vec3b>(85, 130);
        pixel1_color[0] = 210;
        pixel1_color[1] = 37;
        img2.at<cv::Vec3b>(cv::Point(130, 85)) = pixel2_color;

        imwrite("files/img2.jpg", img2);

        // image 3
        cv::Mat img3 (300, 250, CV_8UC3, Scalar(0, 0, 0));
        imwrite("files/img3.jpg", img3);

        // image 4
        cv::Mat img4 (300, 250, CV_8UC3, Scalar(0, 0, 0));
        imwrite("files/img4.jpg", img4);

    }

    DuplicatesDeleter::DuplicatesDeleter() {}

    void DuplicatesDeleter::run(const std::string& files_path, const std::string& result_path) {
        string_vector image_files;
        string_vector result_files;

        generate_images();

        path dir_path = files_path;

        if (!exists(dir_path) || !is_directory(dir_path))
        {
            std::cerr << "folder not found" << std::endl;
        }

        readFiles(files_path, image_files);

        processFiles(files_path, image_files, result_files);

        saveResults(files_path, result_path, result_files);
    }

    bool DuplicatesDeleter::areTheSame(const std::string& first_file, const std::string& second_file) const {

        cv::Mat first_image = imread(first_file, IMREAD_COLOR);
        cv::Mat second_image = imread(second_file, IMREAD_COLOR);

        cvtColor(first_image, first_image, COLOR_BGR2GRAY);
        cvtColor(second_image, second_image, COLOR_BGR2GRAY);

        cv::Mat diff;
        compare(first_image, second_image, diff, CMP_EQ);
        const auto total_pixels = first_image.rows * first_image.cols;
        auto different_pixels = total_pixels - countNonZero(diff);
        if (different_pixels == 0) return true;

        return false;
    }

    void DuplicatesDeleter::readFiles(const std::string& files_path, string_vector& image_files) {
        path dir_path = files_path;

        for (const auto& file : directory_iterator(dir_path))
        {
            auto file_path = file.path();
            auto file_name = file_path.filename();
            if (file_path.extension() == ".jpg" ||
                file_path.extension() == ".png") {
                image_files.emplace_back(file_path);
            }
        }

        std::sort(image_files.begin(), image_files.end());

    }

    void DuplicatesDeleter::processFiles(
        const std::string& files_path, 
        string_vector& image_files, 
        string_vector& result_files) {

        result_files.emplace_back(image_files[0]);

        for (size_t i = 1; i < image_files.size(); ++i)
        {
            path first_image_file_path = image_files[i - 1];
            path second_image_file_path = image_files[i];

            auto are_the_same_images = areTheSame(first_image_file_path.c_str(), second_image_file_path.c_str());
            if (!are_the_same_images) result_files.emplace_back(second_image_file_path);
        }
        
    }

    void DuplicatesDeleter::saveResults(const std::string& files_path, const std::string& result_path, string_vector& result_files) {

        if (!exists(result_path)) {
            create_directory(result_path);
        }

        for (const auto& file_path : result_files)
        {
            path source_image_file_path = file_path;
            std::string file_name = source_image_file_path.filename();

            path target_image_file_path = result_path;
            target_image_file_path /= file_name;
            copy(source_image_file_path.c_str(), target_image_file_path.c_str());
        }
    }

}