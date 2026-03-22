#include "noise_psnr.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <cmath>

cv::Mat NoiseUtils::addGaussianNoise(const cv::Mat& image, const NoiseParameters& params) {
    if (image.empty()) {
        throw std::runtime_error("Empty image for noise addition");
    }

    cv::Mat noisyImage;
    image.convertTo(noisyImage, CV_32F);

    cv::Mat noise(image.size(), CV_32F);
    cv::randn(noise, cv::Scalar(params.mean), cv::Scalar(params.stdDev));

    noisyImage += noise;

    cv::Mat result;
    noisyImage.convertTo(result, CV_8U);

    for (int y = 0; y < result.rows; ++y) {
        uchar* ptr = result.ptr<uchar>(y);
        for (int x = 0; x < result.cols; ++x) {
            ptr[x] = std::max(0, std::min(255, static_cast<int>(ptr[x])));
        }
    }

    return result;
}

PSNRResult NoiseUtils::computePSNR(const cv::Mat& original, const cv::Mat& distorted) {
    PSNRResult result;
    result.width = original.cols;
    result.height = original.rows;

    if (original.empty() || distorted.empty()) {
        throw std::runtime_error("Empty images for PSNR computation");
    }

    if (original.size() != distorted.size()) {
        throw std::runtime_error("Image sizes must match for PSNR computation");
    }

    cv::Mat s1;
    cv::absdiff(original, distorted, s1);
    s1.convertTo(s1, CV_32F);

    cv::Mat s2;
    cv::multiply(s1, s1, s2);

    cv::Scalar s = cv::sum(s2);

    double mse = s.val[0] / (original.total() * original.channels());
    result.mse = mse;
    result.rmse = std::sqrt(mse);

    if (mse < 1e-10) {
        result.psnr = 100.0;
    }
    else {
        result.psnr = 10.0 * std::log10((255.0 * 255.0) / mse);
    }

    return result;
}

void NoiseUtils::saveNoisyImage(const cv::Mat& image, const std::string& filename) {
    try {
        if (!image.empty()) {
            cv::imwrite(filename, image);
            std::cout << "  [SAVE] Noisy Image: " << filename << "\n";
        }
    }
    catch (const cv::Exception& e) {
        std::cerr << "  [ERROR] Failed to save noisy image: " << e.what() << "\n";
    }
}

void NoiseUtils::savePSNRReport(const PSNRResult& result, const std::string& filename, const NoiseParameters& noiseParams) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "  [ERROR] Cannot save PSNR report: " << filename << "\n";
        return;
    }

    file << "PSNR Analysis Report\n";
    file << "=====================\n\n";

    file << "--- Noise Parameters ---\n";
    file << "Mean: " << noiseParams.mean << "\n";
    file << "Variance: " << noiseParams.variance << "\n";

    file << "--- PSNR Metrics ---\n";
    file << "PSNR: " << result.psnr << " dB\n";
    file << "MSE: " << result.mse << "\n";

    file << "--- Image Properties ---\n";
    file << "Width: " << result.width << "\n";
    file << "Height: " << result.height << "\n";

    file.close();
    std::cout << "  [SAVE] PSNR Report: " << filename << "\n";
}