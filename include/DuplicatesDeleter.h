#include <vector>
#include <filesystem>
#include <opencv2/core.hpp>


namespace cpp_practicing {
    using string_vector = std::vector<std::string>;

    class DuplicatesDeleter {
    public:
        DuplicatesDeleter();

        ~DuplicatesDeleter() = default;

        void run(const std::string& files_path, const std::string& result_path);
    
    private:
        // bool areTheSame(const cv::Mat& first_image, const cv::Mat& second_image) const;
        bool areTheSame(const std::string& first_file, const std::string& second_file) const;

        void readFiles(const std::string& files_path, string_vector& image_files);

        void processFiles(const std::string& files_path, string_vector& image_files, string_vector& result_files);

        void saveResults(const std::string& files_path, const std::string& result_path, string_vector& result_files);
    };
} // namespace cpp_practicing