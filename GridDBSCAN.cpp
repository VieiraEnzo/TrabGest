#include <iostream>
#include <math.h>
#include "DBSCAN.h"
#include "read.h"
#include "write.h"
#include <chrono>
#include <map>
using namespace std;

int main(){
    
    cout << "read Points\n";
    vector<Point> pts = readPoints("data/Iris.csv");

    Output output("results/Dbscan.csv");

    //Prepare search
    vector<int> minPts;
    for(int i = 0; i <= 150; i++) minPts.push_back(i);

    vector<double> eps; 
    //Adds (0.00001 -> 100000) && (0.00002 -> 200000) -> ... -> (0.0009 -> 900000)
    for(int i = 1; i <= 9; i++){
        double a = (double)i / 1e5;
        for(; a <= (double)i*1e5; a *= 10) eps.push_back(a);
    }

    for(int mP : minPts){
        for(double ep : eps){

            //Process data
            auto inicio = chrono::steady_clock::now();
            DBSCAN dsbcan(ep, mP, pts, distEuclid);
            int r = dsbcan.run();
            auto fim = chrono::steady_clock::now();
            auto dur = fim - inicio;

            //Write Output
            output.add(mP);
            output.add(ep);
            output.add(dsbcan.label);
            output.add((chrono::duration_cast<chrono::duration<double>>(dur)).count()); //Segundos
            output.newRow();

        }
    }

    output.write();

}