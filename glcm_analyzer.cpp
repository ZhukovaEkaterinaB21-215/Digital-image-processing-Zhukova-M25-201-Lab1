#include "glcm_analyzer.h"
#include "utils.h"
#include <fstream>
#include <cmath>
#include <algorithm>
#include <iostream>

GLCM GLCMAnalyzer::computeGLCM(const cv::Mat& image, int r, int c, int levels) {
    GLCM glcm(levels, std::vector<double>(levels, 0.0));

    if (image.empty() || image.rows <= abs(r) || image.cols <= abs(c)) {
        return glcm;
    }

    int rows = image.rows;
    int cols = image.cols;

    int yStart = (r >= 0) ? 0 : -r;
    int yEnd = (r >= 0) ? rows - r : rows;
    int xStart = (c >= 0) ? 0 : -c;
    int xEnd = (c >= 0) ? cols - c : cols;

    for (int y = yStart; y < yEnd; ++y) {
        const uchar* ptr1 = image.ptr<uchar>(y);
        const uchar* ptr2 = image.ptr<uchar>(y + r);

        for (int x = xStart; x < xEnd; ++x) {
            uchar p1 = ptr1[x];
            uchar p2 = ptr2[x + c];
            glcm[p1][p2] += 1.0;

        }
    }

    double sum = 0.0;
    for (int i = 0; i < levels; ++i) {
        for (int j = 0; j < levels; ++j) {
            sum += glcm[i][j];
        }
    }

    if (sum > 0) {
        for (int i = 0; i < levels; ++i) {
            for (int j = 0; j < levels; ++j) {
                glcm[i][j] /= sum;
            }
        }
    }

    return glcm;
}

double GLCMAnalyzer::computeEnergy(const GLCM& glcm) {
    double energy = 0.0;
    for (size_t i = 0; i < glcm.size(); ++i) {
        for (size_t j = 0; j < glcm[i].size(); ++j) {
            energy += glcm[i][j] * glcm[i][j];
        }
    }
    return energy;
}

GLCMFeatures GLCMAnalyzer::computeFeatures(const GLCM& glcm) {
    GLCMFeatures features;
    features.matrix = glcm;
    features.energy = computeEnergy(glcm);

    return features;
}

void GLCMAnalyzer::saveGLCM(const GLCM& glcm, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "  [ERROR] Cannot save GLCM: " << filename << "\n";
        return;
    }

    file << "GLCM Matrix\n";
    file << "===========\n\n";

    int nonZeroCount = 0;
    for (size_t i = 0; i < glcm.size(); ++i) {
        for (size_t j = 0; j < glcm[i].size(); ++j) {
            if (glcm[i][j] > 1e-10) {
                file << "(" << i << "," << j << "): " << glcm[i][j] << "\n";
                nonZeroCount++;
            }
        }
    }

    file << "\nTotal non-zero elements: " << nonZeroCount << "\n";
    file.close();
    std::cout << "  [SAVE] GLCM Matrix: " << filename << "\n";
}

void GLCMAnalyzer::saveReport(const GLCMFeatures& features, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "  [ERROR] Cannot save GLCM report: " << filename << "\n";
        return;
    }

    file << "GLCM Analysis Report\n";
    file << "=====================\n\n";
    file << "Energy: " << features.energy << "\n";

    file.close();
    std::cout << "  [SAVE] GLCM Report: " << filename << "\n";
}


void GLCMAnalyzer::saveGLCMVisualization(const GLCM& glcm,
    const std::string& filename,
    int imageSize) {
    try {
        if (glcm.empty() || glcm[0].empty()) {
            std::cerr << "  [WARNING] Empty GLCM, skipping visualization\n";
            return;
        }

        int levels = static_cast<int>(glcm.size());
        imageSize = std::max(256, std::min(1024, imageSize));

        cv::Mat result(imageSize, imageSize, CV_8UC1, cv::Scalar(255));

        double maxVal = 1e-10;
        for (const auto& row : glcm) {
            for (double val : row) {
                if (val > maxVal) maxVal = val;
            }
        }

        float cellSize = static_cast<float>(imageSize) / levels;

        for (int i = 0; i < levels; ++i) {
            for (int j = 0; j < levels; ++j) {
                if (glcm[i][j] < 1e-10) continue;

                int intensity = 255 - static_cast<int>(255.0 * glcm[i][j] / maxVal);

                int x1 = static_cast<int>(j * cellSize);
                int y1 = static_cast<int>(i * cellSize);
                int x2 = static_cast<int>((j + 1) * cellSize);
                int y2 = static_cast<int>((i + 1) * cellSize);

                if (x1 >= imageSize || y1 >= imageSize) continue;
                x2 = std::min(x2, imageSize);
                y2 = std::min(y2, imageSize);

                for (int y = y1; y < y2; ++y) {
                    uchar* row = result.ptr<uchar>(y);
                    for (int x = x1; x < x2; ++x) {
                        row[x] = static_cast<uchar>(intensity);
                    }
                }
            }
        }


        if (cv::imwrite(filename, result)) {
            std::cout << "  [SAVE] GLCM Visualization: " << filename << "\n";
        }
        else {
            std::cerr << "  [ERROR] Failed to save GLCM visualization\n";
        }

    }
    catch (const std::exception& e) {
        std::cerr << "  [ERROR] GLCM visualization failed: " << e.what() << "\n";
    }
}