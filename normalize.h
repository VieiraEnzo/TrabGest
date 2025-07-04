#ifndef NORMALIZE_H
#define NORMALIZE_H

#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
#include "Point.h"

/**
 * 1. Computes per‐attribute min/max over all points.
 * 2. Scales each coordinate into [0,1].
 */
static inline std::vector<Point>
normalizePoints(const std::vector<Point>& points)
{
    if (points.empty()) return {};

    size_t dim = points[0].x.size();
    // Initialize min/max
    std::vector<double> minV(dim, std::numeric_limits<double>::max()),
                        maxV(dim, std::numeric_limits<double>::lowest());

    // Compute global min/max
    for (const auto& p : points) {
        for (size_t i = 0; i < dim; ++i) {
            minV[i] = std::min(minV[i], p.x[i]);
            maxV[i] = std::max(maxV[i], p.x[i]);
        }
    }

    // Now normalize in place
    std::vector<Point> out = points;
    for (auto& p : out) {
        for (size_t i = 0; i < dim; ++i) {
            double range = maxV[i] - minV[i];
            if (range <= 0) {
                std::cerr << "Error: constant attribute at index " << i << "\n";
                return {};
            }
            p.x[i] = (p.x[i] - minV[i]) / range;
        }
    }
    return out;
}

#endif // NORMALIZE_H
