#include "tests2.h"
#include "image_analyzer.h"
#include "glcm_analyzer.h"
#include "noise_psnr.h"
#include "utils.h"
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>


#ifdef _WIN32
#include <direct.h>
#include <windows.h>
#define PATH_SEPARATOR "\\"
#else
#include <sys/stat.h>
#include <unistd.h>
#define PATH_SEPARATOR "/"
#endif

void Tests2() {
    std::string exePath = getExecutablePath();
    std::string outputDir = exePath + PATH_SEPARATOR + "test_results2" + PATH_SEPARATOR;

    if (!createDirectory(outputDir)) {
        std::cerr << "ERROR: Cannot create output directory: " << outputDir << "\n";
        exit(1);
    }

    {
        cv::Mat img(50, 50, CV_8UC1, cv::Scalar(128));
        GLCM glcm = GLCMAnalyzer::computeGLCM(img, 1, 1);
        GLCMFeatures features = GLCMAnalyzer::computeFeatures(glcm);

        GLCMAnalyzer::saveGLCM(glcm, outputDir + "test2_1_glcm_matrix.txt");
        GLCMAnalyzer::saveReport(features, outputDir + "test2_1_glcm_report.txt");
        ImageAnalyzer::saveTestImage(img, outputDir + "test2_1_glcm_input.png");
        GLCMAnalyzer::saveGLCMVisualization(glcm, outputDir + "test2_1_glcm_gray.png");
    }

    {
        cv::Mat img(100, 100, CV_8UC1);
        for (int y = 0; y < 100; ++y) {
            for (int x = 0; x < 100; ++x) {
                img.at<uchar>(y, x) = static_cast<uchar>((x + y) / 2);
            }
        }

        GLCM glcm = GLCMAnalyzer::computeGLCM(img, 1, 1);
        GLCMFeatures features = GLCMAnalyzer::computeFeatures(glcm);

        GLCMAnalyzer::saveGLCM(glcm, outputDir + "test2_2_glcm_matrix.txt");
        GLCMAnalyzer::saveReport(features, outputDir + "test2_2_glcm_report.txt");
        ImageAnalyzer::saveTestImage(img, outputDir + "test2_2_glcm_input.png");
        GLCMAnalyzer::saveGLCMVisualization(glcm, outputDir + "test2_2_glcm_gray.png");
    }

    {
        cv::Mat img(100, 100, CV_8UC1);
        cv::randu(img, cv::Scalar(0), cv::Scalar(255));

        GLCM glcm1 = GLCMAnalyzer::computeGLCM(img, 1, 1);
        GLCM glcm2 = GLCMAnalyzer::computeGLCM(img, 2, 2);

        double energy1 = GLCMAnalyzer::computeEnergy(glcm1);
        double energy2 = GLCMAnalyzer::computeEnergy(glcm2);

        std::ofstream file(outputDir + "test2_3_glcm_offsets.txt");
        file << "GLCM Energy Comparison\n";
        file << "Offset (1,1): " << energy1 << "\n";
        file << "Offset (2,2): " << energy2 << "\n";
        file.close();
    }

    {
        cv::Mat original(200, 200, CV_8UC1, cv::Scalar(128));
        NoiseParameters params(0.0, 400.0);
        cv::Mat noisy = NoiseUtils::addGaussianNoise(original, params);

        NoiseUtils::saveNoisyImage(original, outputDir + "test2_4_original.png");
        NoiseUtils::saveNoisyImage(noisy, outputDir + "test2_4_noisy.png");

        PSNRResult psnr = NoiseUtils::computePSNR(original, noisy);
        NoiseUtils::savePSNRReport(psnr, outputDir + "test2_4_psnr_report.txt", params);
    }
    
    {

        cv::Mat original(100, 100, CV_8UC1, cv::Scalar(128));

        std::vector<double> variances = { 10.0, 25.0, 50.0, 100.0, 200.0, 400.0 };
        std::ofstream file(outputDir + "test2_5_psnr_comparison.txt");
        file << "PSNR vs Noise Variance\n";
        file << "======================\n\n";

        bool allOk = true;
        for (double var : variances) {
            NoiseParameters params(0.0, var);
            cv::Mat noisy = NoiseUtils::addGaussianNoise(original, params);
            PSNRResult psnr = NoiseUtils::computePSNR(original, noisy);

            file << "Variance: " << var << " -> PSNR: " << psnr.psnr << " dB, MSE: " << psnr.mse << "\n";

            NoiseUtils::saveNoisyImage(noisy, outputDir + "test2_5_noisy_var" + std::to_string(static_cast<int>(var)) + ".png");

            if (psnr.psnr < 10.0 || psnr.psnr > 100.0) {
                allOk = false;
            }
        }
        file.close();
    }
    
    {
        cv::Mat img(100, 100, CV_8UC1);
        cv::randn(img, cv::Scalar(128), cv::Scalar(40));
        cv::threshold(img, img, 0, 255, cv::THRESH_TOZERO);
        cv::threshold(img, img, 255, 255, cv::THRESH_TRUNC);

        GLCM glcm = GLCMAnalyzer::computeGLCM(img, 1, 1);
        GLCMFeatures features = GLCMAnalyzer::computeFeatures(glcm);


        GLCMAnalyzer::saveGLCM(glcm, outputDir + "test2_6_texture_matrix.txt");
        GLCMAnalyzer::saveReport(features, outputDir + "test2_6_texture_glcm_report.txt");
        ImageAnalyzer::saveTestImage(img, outputDir + "test2_6_texture_input.png");
        GLCMAnalyzer::saveGLCMVisualization(glcm, outputDir + "test2_6_texture_gray.png");
    }
}