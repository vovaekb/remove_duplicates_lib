#include <vector>
#include <filesystem>
#include <string_view>
#include <opencv2/core.hpp>


namespace cppPracticing {
    using string_vector = std::vector<std::string>;

    class DuplicatesDeleter {
    public:
        DuplicatesDeleter();

        ~DuplicatesDeleter() = default;

        void run(const std::string_view& filesPath, const std::string_view& resultPath);
    
    private:
        // bool areFilesTheSame(const cv::Mat& first_image, const cv::Mat& second_image) const;
        bool areFilesTheSame(const std::string_view& firstFile, const std::string_view& secondFile) const;

        void readFiles(const std::string_view& filesPath, string_vector& imageFiles);

        void processFiles(const std::string_view& filesPath, string_vector& imageFiles, string_vector& resultFiles);

        void saveResults(const std::string_view& filesPath, const std::string_view& resultPath, string_vector& resultFiles);
    };
} // namespace cppPracticing
