#pragma once
#ifndef NOISE_PSNR_H
#define NOISE_PSNR_H

#include <opencv2/opencv.hpp>
#include <string>

struct NoiseParameters {
    double mean;
    double variance;
    double stdDev;

    NoiseParameters() : mean(0.0), variance(25.0), stdDev(5.0) {}
    NoiseParameters(double m, double v) : mean(m), variance(v), stdDev(std::sqrt(v)) {}
};

struct PSNRResult {
    double psnr;
    double mse;
    double rmse;
    int width;
    int height;

    PSNRResult() : psnr(0), mse(0), rmse(0), width(0), height(0) {}
};

class NoiseUtils {
public:
    static cv::Mat addGaussianNoise(const cv::Mat& image, const NoiseParameters& params);
    static PSNRResult computePSNR(const cv::Mat& original, const cv::Mat& distorted);
    static void saveNoisyImage(const cv::Mat& image, const std::string& filename);
    static void savePSNRReport(const PSNRResult& result, const std::string& filename, const NoiseParameters& noiseParams);
};

#endif // NOISE_PSNR_H