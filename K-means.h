#include <iostream>
#include <functional>
#include "Point.h"
using namespace std;


struct KMEANS{

    int n, k;
    vector<Point> pts;

    KMEANS(int k, vector<Point> pts) : pts(pts), n(pts.size()), k(k)
    {
  
    }


    /*
        Objetivo: 
        Complexidade:  
    */
    int run(){

    }

};