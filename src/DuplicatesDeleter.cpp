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

namespace cppPracticing {
    using string_vector = std::vector<std::string>;

    void generateImages() {
        // image 1
        cv::Mat image1 (300, 250, CV_8UC3, Scalar(0, 0, 0));
        imwrite("files/image1.jpg", image1);

        cv::Mat image2 (300, 250, CV_8UC3, Scalar(0, 0, 0));
        cv::Vec3b& pixel1Color = image2.at<cv::Vec3b>(46, 100);
        pixel1Color[0] = 146;
        pixel1Color[1] = 78;
        image2.at<cv::Vec3b>(cv::Point(100, 46)) = pixel1Color;

        cv::Vec3b& pixel2Color = image2.at<cv::Vec3b>(85, 130);
        pixel2Color[0] = 210;
        pixel2Color[1] = 37;
        image2.at<cv::Vec3b>(cv::Point(130, 85)) = pixel2Color;

        imwrite("files/image2.jpg", image2);

        // image 3
        cv::Mat img3 (300, 250, CV_8UC3, Scalar(0, 0, 0));
        imwrite("files/img3.jpg", img3);

        // image 4
        cv::Mat img4 (300, 250, CV_8UC3, Scalar(0, 0, 0));
        imwrite("files/img4.jpg", img4);

    }

    DuplicatesDeleter::DuplicatesDeleter() {}

    void DuplicatesDeleter::run(const std::string_view& filesPath, const std::string_view& resultPath) {
        string_vector imageFiles;
        string_vector resultFiles;

        generateImages();

        path dirPath = filesPath;

        if (!exists(dirPath) || !is_directory(dirPath))
        {
            std::cerr << "folder not found" << std::endl;
        }

        readFiles(filesPath, imageFiles);

        processFiles(filesPath, imageFiles, resultFiles);

        saveResults(filesPath, resultPath, resultFiles);
    }

    bool DuplicatesDeleter::areTheSame(const std::string_view& firstFile, const std::string_view& secondFile) const {
        cv::Mat firstImage = imread(static_cast<std::string>(firstFile), IMREAD_COLOR);
        cv::Mat secondImage = imread(static_cast<std::string>(secondFile), IMREAD_COLOR);

        cvtColor(firstImage, firstImage, COLOR_BGR2GRAY);
        cvtColor(secondImage, secondImage, COLOR_BGR2GRAY);

        cv::Mat diff;
        compare(firstImage, secondImage, diff, CMP_EQ);
        const auto totalPixels = firstImage.rows * firstImage.cols;
        auto different_pixels = totalPixels - countNonZero(diff);
        if (different_pixels == 0) return true;

        return false;
    }

    void DuplicatesDeleter::readFiles(const std::string_view& filesPath, string_vector& imageFiles) {
        path dirPath = filesPath;

        for (const auto& file : directory_iterator(dirPath))
        {
            auto filePath = file.path();
            auto fileName = filePath.filename();
            if (filePath.extension() == ".jpg" ||
                filePath.extension() == ".png") {
                imageFiles.emplace_back(filePath);
            }
        }

        std::sort(imageFiles.begin(), imageFiles.end());

    }

    void DuplicatesDeleter::processFiles(
        const std::string_view& filesPath, 
        string_vector& imageFiles, 
        string_vector& resultFiles) {

        resultFiles.emplace_back(imageFiles[0]);

        for (size_t i = 1; i < imageFiles.size(); ++i)
        {
            path firstImageFilePath = imageFiles[i - 1];
            path secondImageFilePath = imageFiles[i];

            auto areTheSameImages = areTheSame(firstImageFilePath.c_str(), secondImageFilePath.c_str());
            if (!areTheSameImages) resultFiles.emplace_back(secondImageFilePath);
        }
        
    }

    void DuplicatesDeleter::saveResults(const std::string_view& filesPath, const std::string_view& resultPath, string_vector& resultFiles) {

        if (!exists(resultPath)) {
            create_directory(resultPath);
        }

        for (const auto& filePath : resultFiles)
        {
            path sourceImageFilePath = filePath;
            std::string fileName = sourceImageFilePath.filename();

            path targetImageFilePath = resultPath;
            targetImageFilePath /= fileName;
            copy(sourceImageFilePath.c_str(), targetImageFilePath.c_str());
        }
    }

}
