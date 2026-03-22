#pragma once
#ifndef GLCM_ANALYZER_H
#define GLCM_ANALYZER_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <string>

using GLCM = std::vector<std::vector<double>>;

struct GLCMFeatures {
    double energy;
    GLCM matrix;

    GLCMFeatures() : energy(0) {}
};

class GLCMAnalyzer {
public:

    static GLCM computeGLCM(const cv::Mat& image, int r = 1, int c = 1, int levels = 256);

    static double computeEnergy(const GLCM& glcm);

    static GLCMFeatures computeFeatures(const GLCM& glcm);

    static void saveGLCM(const GLCM& glcm, const std::string& filename);

    static void saveReport(const GLCMFeatures& features, const std::string& filename);

    static void saveGLCMVisualization(const GLCM& glcm, const std::string& filename, int imageSize = 512);
};

#endif // GLCM_ANALYZER_H