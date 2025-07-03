#include <string>
#include <vector>
#include <iostream>
#include "Point.h"

vector<Point> normalizePoints(const vector<Point> &points, const vector<double> &minValues, const vector<double> &maxValues) {
    vector<Point> normPoints = points;

    for(int p = 0; p < points.size(); p++) {  // Itera sobre os pontos
        for(int a = 0; a < points[p].x.size(); a++) {  // Itera sobre os atributos

            double range = maxValues[a] - minValues[a];
            if (range == 0.0) {  // Evita erro de divisão por 0
                cerr << "Error: Same maximum and minimum values for attribute at index" << a << "\n";
                return {};
            }
            normPoints[p].x[a] = (points[p].x[a] - minValues[a]) / range;
        }
    }
    return normPoints;
}