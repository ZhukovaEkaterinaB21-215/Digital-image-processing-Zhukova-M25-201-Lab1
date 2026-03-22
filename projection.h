#pragma once
#ifndef PROJECTION_H
#define PROJECTION_H

#include <vector>
#include <opencv2/opencv.hpp>
#include <string>

struct ProjectionData {
    std::vector<int> horizontal;
    std::vector<int> vertical; 
    int width;
    int height;

    ProjectionData() : width(0), height(0) {}
};

class ProjectionAnalyzer {
public:
    static ProjectionData computeProjections(const cv::Mat& image);

    static void saveProjections(const ProjectionData& proj, const std::string& filename);

    static void saveHorizontalProjection(const ProjectionData& proj,
        const std::string& filename,
        int plotHeight = 200);

    static void saveVerticalProjection(const ProjectionData& proj,
        const std::string& filename,
        int plotHeight = 200);
};

#endif // PROJECTION_H