#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <omp.h>

#include "Kohonen.h"
#include "read.h"
#include "normalize.h"

int main() {
    std::cout << "1. Loading and normalizing dataset..." << std::endl;
    auto pts = normalizePoints(readPoints("data/Iris.csv"));
    if (pts.empty()) {
        std::cerr << "Failed to load or normalize dataset. Exiting." << std::endl;
        return 1;
    }
    std::cout << "Dataset loaded successfully." << std::endl;

    system("mkdir -p results");

    std::vector<int> t_matriz_range = {5, 10, 15};
    std::vector<int> epochs_range;
    std::vector<double> lr_range;
    std::vector<double> sigma_range;

    for (int i = 5; i <= 15; i += 5) epochs_range.push_back(i);
    for (double i = 0.1; i <= 0.5; i += 0.2) lr_range.push_back(i);
    for (double i = 2.0; i <= 6.0; i += 2.0) sigma_range.push_back(i);

    for (int t : t_matriz_range) {
        std::string filename = "results/kohonen_umatrix_t" + std::to_string(t) + ".csv";
        std::ofstream csv_file(filename);

        if (!csv_file.is_open()) {
            std::cerr << "Error: Could not open file " << filename << std::endl;
            continue;
        }

        std::cout << "\n2. Starting U-Matrix grid search for T_matriz = " << t << "x" << t << std::endl;
        std::cout << "   Results will be saved to: " << filename << std::endl;

        csv_file << "epochs,lr,sigma";
        for (int i = 0; i < t; ++i) {
            for (int j = 0; j < t; ++j) {
                csv_file << ",umatrix_" << i << "_" << j;
            }
        }
        csv_file << '\n';

        #pragma omp parallel for collapse(3) shared(epochs_range, lr_range, sigma_range, t, pts, csv_file)
        for (int i = 0; i < epochs_range.size(); ++i) {
            for (int j = 0; j < lr_range.size(); ++j) {
                for (int k = 0; k < sigma_range.size(); ++k) {
                    
                    int current_epochs = epochs_range[i];
                    double current_lr = lr_range[j];
                    double current_sigma = sigma_range[k];

                    Kohonen model(t, current_epochs, current_sigma, current_lr, pts);
                    model.run();
                    
                    auto u_matrix = model.computeUMatrix(true);

                    #pragma omp critical
                    {
                        csv_file << current_epochs << ","
                                 << std::fixed << std::setprecision(4) << current_lr << ","
                                 << std::fixed << std::setprecision(4) << current_sigma;
                        
                        for (int row = 0; row < t; ++row) {
                            for (int col = 0; col < t; ++col) {
                                csv_file << "," << std::fixed << std::setprecision(8) << u_matrix[row][col];
                            }
                        }
                        csv_file << '\n';
                    }
                }
            }
        }
    }

    std::cout << "\n3. Grid search finished successfully." << std::endl;
    return 0;
}
