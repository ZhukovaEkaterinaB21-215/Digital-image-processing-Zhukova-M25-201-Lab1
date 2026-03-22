#pragma once
#ifndef IMAGE_ANALYZER_H
#define IMAGE_ANALYZER_H

#include "image_statistics.h"
#include <opencv2/opencv.hpp>
#include <string>

class ImageAnalyzer {
public:
    static ImageStatistics analyze(const cv::Mat& image);

    static void saveReport(const ImageStatistics& stats,
        const std::string& filename,
        const std::string& testName = "");

    static void saveHistogramImage(const ImageStatistics& stats,
        const std::string& filename);

    static void saveTestImage(const cv::Mat& image, const std::string& filename);

private:

    static void calculateQuartiles(const std::vector<long long>& hist,
        long long total,
        double& q1, double& q2, double& q3);
};

#endif