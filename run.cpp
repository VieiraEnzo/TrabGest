#include <iostream>
#include <math.h>
#include "DBSCAN.h"
#include "read.h"
#include <map>
using namespace std;

int main(){
    
    string filename = "data/Iris.csv";
    
    cout << "read Points\n";
    vector<Point> input = readPoints(filename);
    cout << input[0].x[0] << " " << input[0].x[1] << " " << input[0].x[2] << " " << input[0].x[3] << "\n";

    map<int,int> ans;
    for(int mP = 0; mP <= 151; mP++){
        for(double ep = 0.00005; ep < 10000; ep *= 10){
            DBSCAN dsbcan(ep, mP, input, distEuclid);
            int r = dsbcan.run();
            ans[r]++;
        }
    }
    
    for(auto a : ans){
        cout << a.first << ": " << a.second << "\n";
    }


    const double eps = 0.5;
    const int minPts = 130;
    DBSCAN dsbcan(eps, minPts, input, distEuclid);

    cout << dsbcan.run() << "\n";
    for(auto a : dsbcan.label) cout << a << " ";
    cout << "\n";


}