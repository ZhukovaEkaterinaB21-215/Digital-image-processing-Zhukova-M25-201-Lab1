#include "image_analyzer.h"
#include "utils.h"
#include <fstream>
#include <cmath>
#include <algorithm>
#include <iostream>

ImageStatistics ImageAnalyzer::analyze(const cv::Mat& image) {
    ImageStatistics stats;
    stats.totalPixels = image.total();
    stats.histogram.assign(256, 0);

    if (stats.totalPixels == 0) {
        throw std::runtime_error("Empty image");
    }

    // Гистограмма
    for (int y = 0; y < image.rows; ++y) {
        const uchar* ptr = image.ptr<uchar>(y);
        for (int x = 0; x < image.cols; ++x) {
            stats.histogram[ptr[x]]++;
        }
    }

    // Среднее
    std::vector<double> probabilities(256);
    stats.mean = 0.0;

    for (int i = 0; i < 256; ++i) {
        probabilities[i] = static_cast<double>(stats.histogram[i]) / stats.totalPixels;
        stats.mean += i * probabilities[i];
    }

    //Дисперсия, Эксцесс, Ассиметрия
    double varianceSum = 0.0;
    double skewnessSum = 0.0;
    double kurtosisSum = 0.0;

    for (int i = 0; i < 256; ++i) {
        if (stats.histogram[i] == 0) continue;
        double diff = i - stats.mean;
        double p = probabilities[i];

        varianceSum += (diff * diff) * p;
        skewnessSum += (diff * diff * diff) * p;
        kurtosisSum += (diff * diff * diff * diff) * p;
    }

    stats.variance = varianceSum;
    stats.stdDev = std::sqrt(stats.variance);

    if (stats.stdDev > 1e-9) {
        stats.skewness = skewnessSum / std::pow(stats.stdDev, 3);
        stats.kurtosis = (kurtosisSum / std::pow(stats.stdDev, 4)) - 3.0;
    }
    else {
        stats.skewness = 0.0;
        stats.kurtosis = 0.0;
    }

    // Энергия, Энтропия
    stats.entropy = 0.0;
    stats.energy = 0.0;
    for (int i = 0; i < 256; ++i) {
        if (probabilities[i] > 0) {
            stats.entropy -= probabilities[i] * std::log2(probabilities[i]);
        }
        stats.energy += probabilities[i] * probabilities[i];
    }

    // Квартили
    calculateQuartiles(stats.histogram, stats.totalPixels, stats.q1, stats.q2, stats.q3);

    return stats;
}

void ImageAnalyzer::saveReport(const ImageStatistics& stats,
    const std::string& filename,
    const std::string& testName) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "  [ERROR] Cannot create report: " << filename << "\n";
        return;
    }

    if (!testName.empty()) {
        file << "Test: " << testName << "\n";
        file << std::string(50, '=') << "\n\n";
    }

    file << "Image Analysis Report\n";
    file << "=====================\n";
    file << "Total Pixels: " << stats.totalPixels << "\n\n";
    file << "--- Statistical Metrics ---\n";
    file << "Mean: " << stats.mean << "\n";
    file << "Variance: " << stats.variance << "\n";
    file << "Std Dev: " << stats.stdDev << "\n";
    file << "Quartiles (Q1, Q2, Q3): " << stats.q1 << ", " << stats.q2 << ", " << stats.q3 << "\n";
    file << "Entropy: " << stats.entropy << "\n";
    file << "Energy: " << stats.energy << "\n";
    file << "Skewness: " << stats.skewness << "\n";
    file << "Kurtosis (Excess): " << stats.kurtosis << "\n\n";

    file.close();
    std::cout << "  [SAVE] Report: " << filename << "\n";
}


void ImageAnalyzer::saveHistogramImage(const ImageStatistics& stats,
    const std::string& filename) {
    const int h = 400, w = 512;
    cv::Mat histImg(h, w, CV_8U, cv::Scalar(255)); 

    long long maxVal = *std::max_element(stats.histogram.begin(), stats.histogram.end());
    if (maxVal == 0) maxVal = 1;

    const double binWidth = static_cast<double>(w) / 256.0;

    for (int i = 0; i < 256; ++i) {
        int barHeight = static_cast<int>(static_cast<double>(stats.histogram[i]) / maxVal * (h - 20));
        int x1 = static_cast<int>(i * binWidth);
        int x2 = static_cast<int>((i + 1) * binWidth);
        int y1 = h - barHeight;
        int y2 = h;

        for (int y = std::max(0, y1); y < std::min(h, y2); ++y) {
            uchar* row = histImg.ptr<uchar>(y);
            for (int x = std::max(0, x1); x < std::min(w, x2); ++x) {
                row[x] = 0;
            }
        }
    }

    cv::imwrite(filename, histImg);
    std::cout << "  [SAVE] Histogram: " << filename << "\n";
}





void ImageAnalyzer::saveTestImage(const cv::Mat& image, const std::string& filename) {
    bool saved = cv::imwrite(filename, image);
    if (saved) {
        std::cout << "  [SAVE] Image: " << filename << "\n";
    }
    else {
        std::cerr << "  [ERROR] Failed to save image: " << filename << "\n";
    }
}

void ImageAnalyzer::calculateQuartiles(const std::vector<long long>& hist,
    long long total,
    double& q1, double& q2, double& q3) {
    long long cumSum = 0;
    q1 = q2 = q3 = 0;
    bool q1Found = false, q2Found = false, q3Found = false;

    for (int i = 0; i < 256; ++i) {
        cumSum += hist[i];
        double ratio = static_cast<double>(cumSum) / total;

        if (!q1Found && ratio >= 0.25) { q1 = static_cast<double>(i); q1Found = true; }
        if (!q2Found && ratio >= 0.50) { q2 = static_cast<double>(i); q2Found = true; }
        if (!q3Found && ratio >= 0.75) { q3 = static_cast<double>(i); q3Found = true; }

        if (q1Found && q2Found && q3Found) break;
    }
}