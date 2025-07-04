#include <iostream>
#include <math.h>
#include "Kohonen.h"
#include "read.h"
#include "write.h"
#include "normalize.h"
#include <chrono>
#include <map>
#include <omp.h>

using namespace std;

int main(){
    cout << "read Points\n";    
    vector<Point> pts = normalizePoints(readPoints("data/Iris.csv"));    
    Output output("results/Kohonen.csv"); 

    //Add header    
    output.add("Tmatriz");
    output.add("epocas");
    output.add("lr");
    output.add("sigma");
    output.add("quantizationError");
    output.add("time");
    output.newRow();
    
    //Prepare search    
    vector<int> Tmatriz, epocas;    
    vector<double> lr, sigma;

    for(int i = 1; i <= 10; i++) Tmatriz.push_back(i);
    for(int i = 1; i <= 10; i++) epocas.push_back(i);
    for(double i = 0.01; i <= 0.3; i+= 0.01) lr.push_back(i);
    for(double i = 0.1; i <= 5; i+= 0.1) sigma.push_back(i);
    
    // This pragma directive tells OpenMP to parallelize the following four loops.
    #pragma omp parallel for collapse(4) default(none) shared(Tmatriz, epocas, lr, sigma, pts, output)
    for(int i = 0; i < Tmatriz.size(); ++i)
        for(int j = 0; j < epocas.size(); ++j)
            for(int k = 0; k < lr.size(); ++k)
                for(int l = 0; l < sigma.size(); ++l){
                    
                    // Get the parameters for this specific iteration
                    int t = Tmatriz[i];
                    int ep = epocas[j];
                    double a = lr[k];
                    double r = sigma[l];

                    //Process data
                    auto inicio = chrono::steady_clock::now();
                    Kohonen kohonen(t,ep,r,a,pts);
                    kohonen.run();
                    auto fim = chrono::steady_clock::now();
                    auto dur = fim - inicio;
                    
                    double qError = kohonen.quantizationError();
                    double time_taken = (chrono::duration_cast<chrono::duration<double>>(dur)).count();

                    // Only one thread can execute this block at a time, preventing race conditions.
                    #pragma omp critical
                    {
                        //Write Output
                        output.add(t);
                        output.add(ep);
                        output.add(a);
                        output.add(r);
                        output.add(qError);
                        output.add(time_taken);
                        output.newRow();
                    }
                }

    output.write();

}