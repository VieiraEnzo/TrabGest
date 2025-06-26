#include <iostream>
#include <functional>
#include "Point.h"
using namespace std;



struct Kohonen{

    int n, t, r, epochs ; //Numero de pontos, Tamanho da matriz (txt), Raio de vizinhança, número de épocas
    vector<Point> pts;

    Kohonen(int t, int r, int epochs,  vector<Point> pts) : pts(pts), n(pts.size()), t(t), r(r)
    {
  
    }


    /*
        Objetivo: 
        Complexidade:  
    */
    int run(){

    }

};