#include "projection.h"
#include "utils.h"
#include <fstream>
#include <algorithm>
#include <iostream>

ProjectionData ProjectionAnalyzer::computeProjections(const cv::Mat& image) {
    ProjectionData proj;

    if (image.empty() || image.channels() != 1) {
        throw std::runtime_error("Image must be grayscale for projection analysis");
    }

    proj.width = image.cols;
    proj.height = image.rows;
    proj.horizontal.assign(proj.height, 0);
    proj.vertical.assign(proj.width, 0);

    for (int y = 0; y < proj.height; ++y) {
        const uchar* ptr = image.ptr<uchar>(y);
        for (int x = 0; x < proj.width; ++x) {
            uchar val = ptr[x];
            proj.horizontal[y] += val;
            proj.vertical[x] += val;
        }
    }

    return proj;
}

void ProjectionAnalyzer::saveProjections(const ProjectionData& proj, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "  [ERROR] Cannot save projections: " << filename << "\n";
        return;
    }

    file << "Image Projections Report\n";
    file << "========================\n\n";

    file << "Image Size: " << proj.width << " x " << proj.height << "\n\n";

    file << "--- Horizontal Projection (row sums) ---\n";
    for (int i = 0; i < proj.height; ++i) {
        file << i << ": " << proj.horizontal[i] << "\n";
    }

    file << "\n--- Vertical Projection (column sums) ---\n";
    for (int i = 0; i < proj.width; ++i) {
        file << i << ": " << proj.vertical[i] << "\n";
    }

    auto maxH = std::max_element(proj.horizontal.begin(), proj.horizontal.end());
    auto minH = std::min_element(proj.horizontal.begin(), proj.horizontal.end());
    auto maxV = std::max_element(proj.vertical.begin(), proj.vertical.end());
    auto minV = std::min_element(proj.vertical.begin(), proj.vertical.end());

    file << "\n--- Statistics ---\n";
    file << "Horizontal: min=" << *minH << ", max=" << *maxH << "\n";
    file << "Vertical:   min=" << *minV << ", max=" << *maxV << "\n";

    file.close();
    std::cout << "  [SAVE] Projections: " << filename << "\n";
}



void ProjectionAnalyzer::saveHorizontalProjection(const ProjectionData& proj,
    const std::string& filename,
    int plotHeight) {
    if (proj.horizontal.empty()) return;

    const int margin = 40;
    const int plotWidth = 400;
    const int w = plotWidth + 2 * margin;
    const int h = plotHeight + 2 * margin;

    cv::Mat img(h, w, CV_8UC1, cv::Scalar(255));

    int maxVal = *std::max_element(proj.horizontal.begin(), proj.horizontal.end());
    if (maxVal == 0) maxVal = 1;

    float yScale = static_cast<float>(plotHeight) / proj.height;

    for (int i = 0; i < proj.height; ++i) {
        int barWidth = static_cast<int>(static_cast<float>(proj.horizontal[i]) / maxVal * plotWidth);
        barWidth = std::max(1, barWidth);

        int x1 = margin;
        int y1 = margin + static_cast<int>(i * yScale);
        int x2 = x1 + barWidth;
        int y2 = y1 + static_cast<int>(yScale);


        y1 = std::max(margin, std::min(y1, h - margin));
        y2 = std::max(y1 + 1, std::min(y2, h - margin));


        for (int y = y1; y < y2; ++y) {
            uchar* row = img.ptr<uchar>(y);
            for (int x = x1; x < x2; ++x) {
                row[x] = 0; 
            }
        }
    }

    cv::imwrite(filename, img);
    std::cout << "  [SAVE] Horizontal Projection: " << filename << "\n";
}


void ProjectionAnalyzer::saveVerticalProjection(const ProjectionData& proj,
    const std::string& filename,
    int plotHeight) {
    if (proj.vertical.empty()) return;

    const int margin = 40;
    const int plotWidth = 400;
    const int w = plotWidth + 2 * margin;
    const int h = plotHeight + 2 * margin;

    cv::Mat img(h, w, CV_8UC1, cv::Scalar(255));

    int maxVal = *std::max_element(proj.vertical.begin(), proj.vertical.end());
    if (maxVal == 0) maxVal = 1;

    float xScale = static_cast<float>(plotWidth) / proj.width;

    for (int i = 0; i < proj.width; ++i) {
        int barHeight = static_cast<int>(static_cast<float>(proj.vertical[i]) / maxVal * plotHeight);
        barHeight = std::max(1, barHeight);

        int x1 = margin + static_cast<int>(i * xScale);
        int y1 = margin + plotHeight - barHeight;
        int x2 = x1 + static_cast<int>(xScale);
        int y2 = margin + plotHeight;

        x1 = std::max(margin, std::min(x1, w - margin));
        x2 = std::max(x1 + 1, std::min(x2, w - margin));

        for (int y = y1; y < y2; ++y) {
            uchar* row = img.ptr<uchar>(y);
            for (int x = x1; x < x2; ++x) {
                row[x] = 0;
            }
        }
    }

    cv::imwrite(filename, img);
    std::cout << "  [SAVE] Vertical Projection: " << filename << "\n";
}
