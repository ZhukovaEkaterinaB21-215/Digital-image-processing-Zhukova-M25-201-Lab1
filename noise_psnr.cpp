#include "noise_psnr.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <cmath>

cv::Mat NoiseUtils::addGaussianNoise(const cv::Mat& image, const NoiseParameters& params) {
    if (image.empty()) {
        throw std::runtime_error("Empty image for noise addition");
    }

    cv::Mat result(image.rows, image.cols, CV_8U);

    for (int y = 0; y < result.rows; ++y) {
        const uchar* srcPtr = image.ptr<uchar>(y);
        uchar* dstPtr = result.ptr<uchar>(y);
        for (int x = 0; x < result.cols; ++x) {

            double u, v, s;
            s = 0;
            while (s >= 1.0 || s == 0.0) {
                u = (std::rand() / static_cast<double>(RAND_MAX)) * 2.0 - 1.0;
                v = (std::rand() / static_cast<double>(RAND_MAX)) * 2.0 - 1.0;
                s = u * u + v * v;
            }
            s = std::sqrt(-2.0 * std::log(s) / s);
            double spare = v * s;
            double noise = params.mean + params.stdDev * u * s;
            double val = static_cast<double>(srcPtr[x]) + noise;

            if (val < 0.0) val = 0.0;
            if (val > 255.0) val = 255.0;

            dstPtr[x] = static_cast<uchar>(val);
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

    long long sumSquaredDiff = 0;
    size_t totalPixels = 0;

    for (int y = 0; y < original.rows; ++y) {
        const uchar* ptr1 = original.ptr<uchar>(y);
        const uchar* ptr2 = distorted.ptr<uchar>(y);

        for (int x = 0; x < original.cols; ++x) {
            int diff = static_cast<int>(ptr1[x]) - static_cast<int>(ptr2[x]);
            sumSquaredDiff += diff * diff;
            totalPixels++;
        }
    }

    double mse = static_cast<double>(sumSquaredDiff) / totalPixels;
    result.mse = mse;
    result.rmse = std::sqrt(mse);

    if (mse < 1e-10) {
        result.psnr = DBL_MAX;
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