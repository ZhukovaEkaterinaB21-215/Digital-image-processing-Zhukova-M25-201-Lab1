#include "tests1.h"
#include "image_analyzer.h"
#include "utils.h"

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define PATH_SEPARATOR "\\"
#else
#include <sys/stat.h>
#include <unistd.h>
#define PATH_SEPARATOR "/"
#endif

void Tests1() {

    std::string exePath = getExecutablePath();
    std::string outputDir = exePath + PATH_SEPARATOR + "test_results" + PATH_SEPARATOR;


    if (!createDirectory(outputDir)) {
        std::cerr << "ERROR: Cannot create output directory: " << outputDir << "\n";
        exit(1);
    }

    {
        cv::Mat img(100, 100, CV_8UC1, cv::Scalar(128));
        auto stats = ImageAnalyzer::analyze(img);

        ImageAnalyzer::saveTestImage(img, outputDir + "test1_uniform_input.png");
        ImageAnalyzer::saveReport(stats, outputDir + "test1_uniform_report.txt", "Uniform Image (128)");
        ImageAnalyzer::saveHistogramImage(stats, outputDir + "test1_uniform_histogram.png");
    }

    {
        cv::Mat img(100, 100, CV_8UC1);
        img.setTo(0);
        img(cv::Rect(0, 0, 50, 100)).setTo(255);

        auto stats = ImageAnalyzer::analyze(img);

        ImageAnalyzer::saveTestImage(img, outputDir + "test2_binary_input.png");
        ImageAnalyzer::saveReport(stats, outputDir + "test2_binary_report.txt", "Binary Image (0/255)");
        ImageAnalyzer::saveHistogramImage(stats, outputDir + "test2_binary_histogram.png");
    }

    {
        cv::Mat img(256, 256, CV_8UC1);
        for (int i = 0; i < 256; ++i){
            for (int j = 0; j < 256; ++j) {
                img.at<uchar>(i, j) = static_cast<uchar>(i);
            }
        }

        auto stats = ImageAnalyzer::analyze(img);

        ImageAnalyzer::saveTestImage(img, outputDir + "test3_gradient_input.png");
        ImageAnalyzer::saveReport(stats, outputDir + "test3_gradient_report.txt", "Gradient Image (0-255)");
        ImageAnalyzer::saveHistogramImage(stats, outputDir + "test3_gradient_histogram.png");
    }

    {
        cv::Mat img(200, 200, CV_8UC1);
        cv::randn(img, cv::Scalar(128), cv::Scalar(40));
        cv::threshold(img, img, 0, 255, cv::THRESH_TOZERO);
        cv::threshold(img, img, 255, 255, cv::THRESH_TRUNC);

        auto stats = ImageAnalyzer::analyze(img);

        ImageAnalyzer::saveTestImage(img, outputDir + "test4_gaussian_input.png");
        ImageAnalyzer::saveReport(stats, outputDir + "test4_gaussian_report.txt", "Gaussian-like Distribution");
        ImageAnalyzer::saveHistogramImage(stats, outputDir + "test4_gaussian_histogram.png");
    }

    {
        cv::Mat img(100, 100, CV_8UC1);
        cv::randu(img, cv::Scalar(0), cv::Scalar(50));

        auto stats = ImageAnalyzer::analyze(img);

        ImageAnalyzer::saveTestImage(img, outputDir + "test5_dark_input.png");
        ImageAnalyzer::saveReport(stats, outputDir + "test5_dark_report.txt", "Dark Image (0-50)");
        ImageAnalyzer::saveHistogramImage(stats, outputDir + "test5_dark_histogram.png");
    }

    {
        cv::Mat img(100, 100, CV_8UC1);
        cv::randu(img, cv::Scalar(200), cv::Scalar(255));

        auto stats = ImageAnalyzer::analyze(img);

        ImageAnalyzer::saveTestImage(img, outputDir + "test6_bright_input.png");
        ImageAnalyzer::saveReport(stats, outputDir + "test6_bright_report.txt", "Bright Image (200-255)");
        ImageAnalyzer::saveHistogramImage(stats, outputDir + "test6_bright_histogram.png");
    }

}