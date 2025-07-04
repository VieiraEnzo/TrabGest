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

        std::string filename = "results/kohonen_clusters.csv";
    std::ofstream csv_file(filename);

    if (!csv_file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return 1;
    }

    // Write header
    csv_file << "epochs,lr,sigma,Tmatriz,attr1,attr2,bmu_index\n";

    std::vector<int> t_matriz_range = {2, 3, 4};
    std::vector<int> epochs_range;
    std::vector<double> lr_range;
    std::vector<double> sigma_range;

    for (int i = 5; i <= 15; i += 5) epochs_range.push_back(i);
    for (double i = 0.1; i <= 0.5; i += 0.2) lr_range.push_back(i);
    for (double i = 2.0; i <= 6.0; i += 2.0) sigma_range.push_back(i);

    int attr_idx1 = 2;
    int attr_idx2 = 3;

    std::cout << "\n2. Starting BMU Projection grid search..." << std::endl;
    std::cout << "   Results will be saved to: " << filename << std::endl;

    #pragma omp parallel for collapse(4) shared(t_matriz_range, epochs_range, lr_range, sigma_range, pts, csv_file)
    for (int t_idx = 0; t_idx < t_matriz_range.size(); ++t_idx) {
        for (int e_idx = 0; e_idx < epochs_range.size(); ++e_idx) {
            for (int l_idx = 0; l_idx < lr_range.size(); ++l_idx) {
                for (int s_idx = 0; s_idx < sigma_range.size(); ++s_idx) {
                    
                    int t = t_matriz_range[t_idx];
                    int current_epochs = epochs_range[e_idx];
                    double current_lr = lr_range[l_idx];
                    double current_sigma = sigma_range[s_idx];

                    Kohonen model(t, current_epochs, current_sigma, current_lr, pts);
                    model.run();
                    
                    auto projection = model.getBMUClusters(attr_idx1, attr_idx2);

                    #pragma omp critical
                    {
                        for(const auto& row : projection) {
                            csv_file << current_epochs << ","
                                     << std::fixed << std::setprecision(4) << current_lr << ","
                                     << std::fixed << std::setprecision(4) << current_sigma << ","
                                     << t << ","
                                     << std::fixed << std::setprecision(8) << row[0] << ","
                                     << std::fixed << std::setprecision(8) << row[1] << ","
                                     << static_cast<int>(row[2]) << '\n';
                        }
                    }
                }
            }
        }
    }

    std::cout << "\n3. Grid search finished successfully." << std::endl;
    csv_file.close();
    return 0;
}