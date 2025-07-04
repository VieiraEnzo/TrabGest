#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <random>

#include "Kohonen.h"
#include "read.h"
#include "normalize.h"

void saveMatrixToCSV(
    const std::vector<std::vector<double>>& matrix,
    const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Erro: não foi possível abrir " << filename << std::endl;
        return;
    }
    for (size_t i = 0; i < matrix.size(); ++i) {
        for (size_t j = 0; j < matrix[i].size(); ++j) {
            file << std::fixed << std::setprecision(8) << matrix[i][j]
                 << (j+1 < matrix[i].size() ? "," : "");
        }
        file << '\n';
    }
    std::cout << "U-Matrix salva em: " << filename << std::endl;
}

int main() {
    const int t_matriz       = 10;
    const int epochs         = 10;
    const double sigma_start = t_matriz / 2.0;
    const double lr_start    = 0.09;

    std::cout << "1. Carregando Iris..." << std::endl;
    auto pts = normalizePoints(readPoints("data/Iris.csv"));
    if (pts.empty()) return 1;

    std::cout << "2. Treinando SOM..." << std::endl;
    Kohonen model(t_matriz, epochs, sigma_start, lr_start, pts);
    model.run();
    std::cout << "Erro quantização final: " << model.quantizationError() << std::endl;

    std::cout << "3. Calculando U-Matrix..." << std::endl;
    auto um = model.computeUMatrix(true);

    system("mkdir -p results");
    saveMatrixToCSV(um, "results/kohonen_u_matrix.csv");
    std::cout << "Program finished successfully." << std::endl;
    return 0;
}