#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <opencv2/opencv.hpp>

#include "image_statistics.h"
#include "utils.h"
#include "image_analyzer.h"
#include "tests1.h"
#include"glcm_analyzer.h"
#include "tests2.h"
#include "noise_psnr.h"
#include "projection.h"
#include "tests3.h"

#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define PATH_SEPARATOR "\\"
#else
#include <sys/stat.h>
#include <unistd.h>
#define PATH_SEPARATOR "/"
#endif

int main(int argc, char** argv)
{
    //Tests1();
    //Tests2();
    Tests3();
    /*
    std::string inputPath = argv[1];
    cv::Mat image = cv::imread(inputPath, cv::IMREAD_GRAYSCALE);

    if (image.empty()) {
        std::cerr << "Error: Could not load image or image is not grayscale 8bpp.\n";
        return 1;
    }

    try {
        std::cout << "Analyzing image: " << inputPath << " (" << image.cols << "x" << image.rows << ")\n";

        ImageStatistics stats = ImageAnalyzer::analyze(image);

        std::cout << "\n--- Statistics ---\n";
        std::cout << "Mean: " << stats.mean << "\n";
        std::cout << "Variance: " << stats.variance << "\n";
        std::cout << "Std Dev: " << stats.stdDev << "\n";
        std::cout << "Entropy: " << stats.entropy << "\n";
        std::cout << "Energy: " << stats.energy << "\n";
        std::cout << "Skewness: " << stats.skewness << "\n";
        std::cout << "Kurtosis (Excess): " << stats.kurtosis << "\n";
        std::cout << "Quartiles: Q1=" << stats.q1 << ", Q2=" << stats.q2 << ", Q3=" << stats.q3 << "\n";

        ImageAnalyzer::saveReport(stats, "stats_report_kirpich.txt");
        ImageAnalyzer::saveHistogramImage(stats, "result_histogram_kirpich.png");

        std::cout << "\n[SAVE] Report: stats_report_kirpich.txt\n";
        std::cout << "[SAVE] Histogram: result_histogram_kirpich.png\n";

        cv::imshow("Original Image", image);

        cv::Mat histImg = cv::imread("result_histogram_kirpich.png");
        if (!histImg.empty()) {
            cv::imshow("Histogram Result", histImg);
        }

        cv::waitKey(0);
        cv::destroyAllWindows();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    */

    
    

    /*
    std::string inputPath = argv[1];
    cv::Mat image = cv::imread(inputPath, cv::IMREAD_GRAYSCALE);

    if (image.empty()) {
        std::cerr << "Error: Could not load image or image is not grayscale 8bpp.\n";
        return 1;
    }

    try {
        std::cout << "Analyzing image: " << inputPath << " (" << image.cols << "x" << image.rows << ")\n";

        int r = -10;
        int c = -10;

        std::cout << "Computing GLCM (offset r=" << r << ", c=" << c << ") for: " << inputPath << "\n";

        GLCM glcm = GLCMAnalyzer::computeGLCM(image, r, c);
        GLCMFeatures features = GLCMAnalyzer::computeFeatures(glcm);

        GLCMAnalyzer::saveGLCM(glcm, "glcm_matrix_kirpich_negative.txt");
        GLCMAnalyzer::saveReport(features, "glcm_report_kirpich_negative.txt");
        GLCMAnalyzer::saveGLCMVisualization(glcm, "user_glcm_gray_kirpich_negative.png");
        cv::waitKey(0);
        cv::destroyAllWindows();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    */
    
    /*
    std::string inputPath = argv[1];
    cv::Mat image = cv::imread(inputPath, cv::IMREAD_GRAYSCALE);

    
    if (image.empty()) {
        std::cerr << "Error: Could not load image or image is not grayscale 8bpp.\n";
        return 1;
    }

    try {
        std::cout << "\nAdding noise and computing PSNR\n";
        NoiseParameters params(0.0, 400.0);
        cv::Mat noisy = NoiseUtils::addGaussianNoise(image, params);
        PSNRResult psnr = NoiseUtils::computePSNR(image, noisy);

        std::cout << "PSNR: " << psnr.psnr << " dB\n";
        std::cout << "MSE: " << psnr.mse << "\n";

        NoiseUtils::saveNoisyImage(noisy, "user_noisy_enot.png");
        NoiseUtils::savePSNRReport(psnr, "user_psnr_enot.txt", params);

        cv::waitKey(0);
        cv::destroyAllWindows();

    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    */
/*
    std::cout << "\nComputing projections\n";

    ProjectionData proj = ProjectionAnalyzer::computeProjections(image);

    ProjectionAnalyzer::saveProjections(proj, "user_projections.txt");
    ProjectionAnalyzer::saveHorizontalProjection(proj, "user_horizontal_projection.png");
    ProjectionAnalyzer::saveVerticalProjection(proj, "user_vertical_projection.png");
    */
    
    return 0;
}

