#include <iostream>
#include <math.h>
using namespace std;


struct Point {
    
    vector<double> x;

    Point(vector<double> k) : x(k) {}

    Point operator+(Point other){
        Point ans = x; 
        for(int i = 0; i < x.size(); i++) ans.x[i] += other.x[i];
        return ans;
    }

    Point operator-(Point other){
        Point ans = *this; 
        for(int i = 0; i < x.size(); i++) ans.x[i] -= other.x[i];
        return ans;
    }

    Point operator*(double k) const{
        Point ans = *this; 
        for(int i = 0; i < x.size(); i++) ans.x[i] *= k;
        return ans;
    }

};

Point operator*(double k, const Point p) {
    return p * k;
}

//Dá pra otimizar removendo a sqrt
function<double(Point, Point)> distEuclid = [](Point a, Point b){
    double ans = 0;
    for(int i = 0; i < a.x.size(); i++) ans += (a.x[i] - b.x[i]) * (a.x[i] - b.x[i]); 
    return sqrt(ans);
};