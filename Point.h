#include <iostream>
#include <math.h>
using namespace std;


struct Point{
    
    vector<double> x;

    Point(vector<double> k) : x(k) {}

};

//Dá pra otimizar removendo a sqrt
function<double(Point, Point)> distEuclid = [](Point a, Point b){
    double ans = 0;
    for(int i = 0; i < a.x.size(); i++) ans += (a.x[i] - b.x[i]) * (a.x[i] - b.x[i]); 
    return sqrt(ans);
};