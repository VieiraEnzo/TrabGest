#include <iostream>
#include <math.h>
#include "Kohonen.h"
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

    vector<int> Tmatriz, epocas;
    vector<double> alfa, raio;

    for(int i = 1; i <= 10; i++) Tmatriz.push_back(i);
    for(int i = 1; i <= 10; i++) epocas.push_back(i);
    for(double i = 0.01; i <= 1; i+= 0.01) alfa.push_back(i);
    for(double i = 0.5; i <= 20; i+= 0.5) raio.push_back(i);


    for(auto t : Tmatriz)
        for(auto ep : epocas)
            for(auto a : alfa)
                for(auto r : raio){

                    //Process data
                    auto inicio = chrono::steady_clock::now();
                    Kohonen kohonen(t,r,ep,a, pts);
                    kohonen.run();
                    auto fim = chrono::steady_clock::now();
                    auto dur = fim - inicio;

                    //Write Output
                    output.add(t);
                    output.add(ep);
                    output.add(a);
                    output.add(r);
                    // output.add(kohonen.label);
                    output.add((chrono::duration_cast<chrono::duration<double>>(dur)).count()); //Segundos
                    output.newRow();

                }

    output.write();

}