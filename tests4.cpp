#include "tests4.h"
#include "glcm_analyzer.h"
#include "noise_psnr.h"
#include "utils.h"
#include "image_analyzer.h"
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



void Tests4() {
    std::string outputDir = getExecutablePath() + "\\test_results4\\";
    createDirectory(outputDir);

    {
        cv::Mat gradient(100, 100, CV_8UC1);
        for (int y = 0; y < 100; ++y) {
            uchar* row = gradient.ptr<uchar>(y);
            for (int x = 0; x < 100; ++x) {
                row[x] = static_cast<uchar>(2.55 * x);  // 0..255
            }
        }
        GLCM glcm_right = GLCMAnalyzer::computeGLCM(gradient, 0, 1);
        GLCMFeatures feat_right = GLCMAnalyzer::computeFeatures(glcm_right);

        GLCM glcm_left = GLCMAnalyzer::computeGLCM(gradient, 0, -1);
        GLCMFeatures feat_left = GLCMAnalyzer::computeFeatures(glcm_left);


        GLCM glcm_down = GLCMAnalyzer::computeGLCM(gradient, 1, 0);
        GLCMFeatures feat_down = GLCMAnalyzer::computeFeatures(glcm_down);

        GLCM glcm_up = GLCMAnalyzer::computeGLCM(gradient, -1, 0);
        GLCMFeatures feat_up = GLCMAnalyzer::computeFeatures(glcm_up);


        GLCMAnalyzer::saveReport(feat_right, outputDir + "test_glcm_right.txt");
        GLCMAnalyzer::saveReport(feat_left, outputDir + "test_glcm_left.txt");
        GLCMAnalyzer::saveReport(feat_down, outputDir + "test_glcm_down.txt");
        GLCMAnalyzer::saveReport(feat_up, outputDir + "test_glcm_up.txt");

        ImageAnalyzer::saveTestImage(gradient, outputDir + "test_glcm_input.png");


        GLCMAnalyzer::saveGLCMVisualization(glcm_right, outputDir + "test_glcm_right_gray.png");
        GLCMAnalyzer::saveGLCMVisualization(glcm_left, outputDir + "test_glcm_left_gray.png");
        GLCMAnalyzer::saveGLCMVisualization(glcm_down, outputDir + "test_glcm_down_gray.png");
        GLCMAnalyzer::saveGLCMVisualization(glcm_up, outputDir + "test_glcm_up_gray.png");
    }


    {
        cv::Mat img1(100, 100, CV_8UC1, cv::Scalar(128));
        cv::Mat img2(100, 100, CV_8UC1, cv::Scalar(128));

        PSNRResult result = NoiseUtils::computePSNR(img1, img2);

        std::cout << "MSE: " << result.mse << "\n";
        std::cout << "PSNR: " << result.psnr << " dB\n";

 
        std::ofstream file(outputDir + "test_psnr_identical.txt");
        file << "PSNR for Identical Images\n";
        file << "=========================\n";
        file << "MSE: " << result.mse << "\n";
        file << "PSNR: " << result.psnr << " dB\n";
        file.close();
    }


    {
        cv::Mat img1(1000, 1000, CV_8UC1, cv::Scalar(128));
        cv::Mat img2(1000, 1000, CV_8UC1, cv::Scalar(128));
        img2.at<uchar>(50, 50) = 129;

        PSNRResult result = NoiseUtils::computePSNR(img1, img2);

        std::cout << "MSE: " << result.mse << "\n";
        std::cout << "PSNR: " << result.psnr << " dB\n";


        std::ofstream file(outputDir + "test_psnr_one_pixel.txt");
        file << "PSNR for One Pixel Difference\n";
        file << "==============================\n";
        file << "MSE: " << result.mse << "\n";
        file << "PSNR: " << result.psnr << " dB\n";

        file.close();
    }

}
