#include <iostream>
#include <functional>
#include "Point.h"
#include <queue>
using namespace std;


struct DBSCAN{

    double eps; 
    int n, minPts;
    function<double(Point,Point)> dist;

    const int undefined = -1, noise = 0;

    vector<Point> pts;
    vector<int> label; 
    vector<vector<int>> grafo;

    DBSCAN(double eps, int minPts, vector<Point> pts, function<double(Point,Point)> dist) : eps(eps), minPts(minPts), pts(pts), dist(dist), 
                                                                                                n(pts.size()), grafo(n), label(n, undefined)
    {
        /*
        Objetivo: Pre-processa vizinhança
        Complexidade:  O(n*n)
        */
        for(int i = 0; i < n; i++){
            for(int j = i+1; j < n; j++){
                if(dist(pts[i] ,pts[j]) < eps){
                    grafo[i].push_back(j);
                    grafo[j].push_back(i);
                }
            }
        }        
    }


    /*
        Objetivo: Acha os clusters
        Complexidade:  O(n*n)
    */
    int run(){
        int c = 0;
        for(int i = 0; i < n; i++){
            if(label[i] != undefined) continue;
            if(grafo[i].size()+1 < minPts){
                label[i] = noise;
                continue;
            }
            c++;
            label[i] = c;

            queue<int> q;
            for(int nei : grafo[i]) q.push(nei);

            //Da pra otimizar colocando fila de visitados (+-)
            while(!q.empty()){
                int j = q.front();
                q.pop();

                if(label[j] == noise) label[j] = c;
                if(label[j] != undefined) continue;
                label[j] = c;
                if(grafo[j].size()+1 >= minPts){
                    for(auto novo : grafo[j]){
                        if(label[novo] == noise) label[novo] = c;
                        if(label[novo] != undefined) continue;
                        q.push(novo);
                    }
                }
            }
        }
        return c;
    }

};