#pragma once
#ifndef IMAGE_STATISTICS_H
#define IMAGE_STATISTICS_H

#include <vector>

struct ImageStatistics {
    long long totalPixels;
    std::vector<long long> histogram;
    double mean;
    double variance;
    double stdDev;
    double q1, q2, q3;
    double entropy;
    double energy;
    double skewness;
    double kurtosis;

    ImageStatistics() : totalPixels(0), mean(0), variance(0), stdDev(0),
        q1(0), q2(0), q3(0), entropy(0), energy(0),
        skewness(0), kurtosis(0) {
        histogram.assign(256, 0);
    }

};

#endif