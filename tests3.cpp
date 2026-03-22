#include "tests3.h"
#include "projection.h"
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

void Tests3() {
    std::string outputDir = getExecutablePath() + "\\test_results3\\";
    createDirectory(outputDir);

    {
        cv::Mat img(100, 100, CV_8UC1, cv::Scalar(128));
        ProjectionData proj = ProjectionAnalyzer::computeProjections(img);
        ImageAnalyzer::saveTestImage(img, outputDir + "test3_1_homogeneous_image.png");
        ProjectionAnalyzer::saveProjections(proj, outputDir + "test3__projections.txt");
        ProjectionAnalyzer::saveHorizontalProjection(proj, outputDir + "test3_1_horizontal.png");
        ProjectionAnalyzer::saveVerticalProjection(proj, outputDir + "test3_1_vertical.png");

    }

    {
        cv::Mat img(100, 100, CV_8UC1, cv::Scalar(0));
        for (int x = 0; x < 100; x += 10) {
            img(cv::Rect(x, 0, 5, 100)).setTo(255);
        }

        ProjectionData proj = ProjectionAnalyzer::computeProjections(img);
        ImageAnalyzer::saveTestImage(img, outputDir + "test3_2_vertical_stripes.png");
        ProjectionAnalyzer::saveHorizontalProjection(proj, outputDir + "test3_2_horizontal.png");
        ProjectionAnalyzer::saveVerticalProjection(proj, outputDir + "test3_2_vertical.png");
        ProjectionAnalyzer::saveProjections(proj, outputDir + "test3_2_projections.txt");


    }

    {
        cv::Mat img(100, 100, CV_8UC1, cv::Scalar(0));
        for (int y = 0; y < 100; y += 10) {
            img(cv::Rect(0, y, 100, 5)).setTo(255);
        }

        ProjectionData proj = ProjectionAnalyzer::computeProjections(img);
        ImageAnalyzer::saveTestImage(img, outputDir + "test3_3_horizontal_stripes.png");
        ProjectionAnalyzer::saveHorizontalProjection(proj, outputDir + "test3_3_horizontal.png");
        ProjectionAnalyzer::saveVerticalProjection(proj, outputDir + "test3_3_vertical.png");
        ProjectionAnalyzer::saveProjections(proj, outputDir + "test3_3_projections.txt");
    }

}