#include "tests1.h"
#include "image_analyzer.h"
#include "noise_psnr.h"
#include "utils.h"
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


#include <cstdlib>
#include <ctime>
#include <cmath>

static double generateGaussian(double mean, double stdDev) {
    static bool hasSpare = false;
    static double spare;

    if (hasSpare) {
        hasSpare = false;
        return mean + stdDev * spare;
    }

    double u, v, s;
    do {
        u = (std::rand() / static_cast<double>(RAND_MAX)) * 2.0 - 1.0;
        v = (std::rand() / static_cast<double>(RAND_MAX)) * 2.0 - 1.0;
        s = u * u + v * v;
    } while (s >= 1.0 || s == 0.0);

    s = std::sqrt(-2.0 * std::log(s) / s);
    spare = v * s;
    hasSpare = true;

    return mean + stdDev * u * s;
}

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

        static bool initialized = false;
        if (!initialized) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            initialized = true;
        }

        for (int y = 0; y < img.rows; ++y) {
            uchar* row = img.ptr<uchar>(y);
            for (int x = 0; x < img.cols; ++x) {
                double noise = generateGaussian(128.0, 40.0);

                if (noise < 0.0) noise = 0.0;
                if (noise > 255.0) noise = 255.0;

                row[x] = static_cast<uchar>(noise + 0.5);
            }
        }


        auto stats = ImageAnalyzer::analyze(img);

        ImageAnalyzer::saveTestImage(img, outputDir + "test4_gaussian_input.png");
        ImageAnalyzer::saveReport(stats, outputDir + "test4_gaussian_report.txt", "Gaussian-like Distribution");
        ImageAnalyzer::saveHistogramImage(stats, outputDir + "test4_gaussian_histogram.png");
    }

    {
        cv::Mat img(100, 100, CV_8UC1);
        static bool initialized = false;
        if (!initialized) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            initialized = true;
        }

        for (int y = 0; y < img.rows; ++y) {
            uchar* row = img.ptr<uchar>(y);
            for (int x = 0; x < img.cols; ++x) {
                double t = static_cast<double>(std::rand()) / RAND_MAX;  // [0, 1]
                double val = 50*t;


                if (val < 0.0) val = 0.0;
                if (val > 255.0) val = 255.0;

                row[x] = static_cast<uchar>(val + 0.5);
            }
        }

        auto stats = ImageAnalyzer::analyze(img);

        ImageAnalyzer::saveTestImage(img, outputDir + "test5_dark_input.png");
        ImageAnalyzer::saveReport(stats, outputDir + "test5_dark_report.txt", "Dark Image (0-50)");
        ImageAnalyzer::saveHistogramImage(stats, outputDir + "test5_dark_histogram.png");
    }

    {
        cv::Mat img(100, 100, CV_8UC1);
        static bool initialized = false;
        if (!initialized) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
            initialized = true;
        }

        for (int y = 0; y < img.rows; ++y) {
            uchar* row = img.ptr<uchar>(y);
            for (int x = 0; x < img.cols; ++x) {
                double t = static_cast<double>(std::rand()) / RAND_MAX;  // [0, 1]
                double val = 200 + t * 55;


                if (val < 0.0) val = 0.0;
                if (val > 255.0) val = 255.0;

                row[x] = static_cast<uchar>(val + 0.5);
            }
        }

        auto stats = ImageAnalyzer::analyze(img);

        ImageAnalyzer::saveTestImage(img, outputDir + "test6_bright_input.png");
        ImageAnalyzer::saveReport(stats, outputDir + "test6_bright_report.txt", "Bright Image (200-255)");
        ImageAnalyzer::saveHistogramImage(stats, outputDir + "test6_bright_histogram.png");
    }

}