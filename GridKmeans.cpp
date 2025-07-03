#include <iostream>
#include <math.h>
#include "K-means.h"
#include "read.h"
#include "write.h"
#include <chrono>
#include <map>
#include <sstream>
using namespace std;

int main(){
    
    cout << "read Points\n";
    vector<Point> pts = readPoints("data/Iris.csv");

    Output output("results/Kmeans.csv");
    Output output_centroids("results/Kmeans_centroids.csv");

    output.add("k"); 

    for (int i = 0; i < pts.size(); ++i) {
        output.add("label_ponto_" + to_string(i + 1));
    }

    const int dimensoes = pts.empty() ? 0 : pts[0].x.size();

    output.add("tempo_execucao_segundos");

    output.newRow();

    output_centroids.add("k");
    output_centroids.add("id_centroide");
    for (int d = 0; d < dimensoes; ++d) {
        output_centroids.add("dim" + std::to_string(d + 1));
    }
    output_centroids.newRow();

    //Prepare search
    vector<int> K;
    for(int i = 1; i <= 150; i++) K.push_back(i);

    for(int k : K){

        //Process data
        auto inicio = chrono::steady_clock::now();
        KMEANS kmeans(k, pts);
        kmeans.run();
        auto fim = chrono::steady_clock::now();
        auto dur = fim - inicio;

        //Write Output
        output.add(k);
        output.add(kmeans.label);
        
        output.add((chrono::duration_cast<chrono::duration<double>>(dur)).count()); //Segundos
        output.newRow();

        for (int j = 0; j < kmeans.centroides.size(); ++j) {
            output_centroids.add(k);
            output_centroids.add(j); 
            for (double coord : kmeans.centroides[j].x) {
                output_centroids.add(coord);
            }
            output_centroids.newRow();
        }

    }

    output.write();
    output_centroids.write();

}