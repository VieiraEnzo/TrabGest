#include <iostream>
#include <math.h>
#include "K-means.h"
#include "read.h"
#include "write.h"
#include <chrono>
#include <map>
using namespace std;

int main(){
    
    cout << "read Points\n";
    vector<Point> pts = readPoints("data/Iris.csv");

    Output output("results/Kmeans.csv");

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

    }

    output.write();

}