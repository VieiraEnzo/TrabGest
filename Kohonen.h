#include <iostream>
#include <functional>
#include "Point.h"
using namespace std;


typedef Point Neu;
struct Kohonen{

    int n, t, r, epochs ; //Numero de pontos, Tamanho da matriz (txt), Raio de vizinhança, número de épocas
    vector<vector<Neu>> mp;
    vector<Point> pts;
    double alfa;

    Kohonen(int t, int r, int epochs, double alfa, vector<Point> pts) : pts(pts), n(pts.size()), t(t), r(r), epochs(epochs), alfa(alfa), mp(t, vector<Neu>(t))
    {
        //Iniciar neuronios com tamanho randomico
        for(int i = 0; i < t; i++){
            for(int j = 0; j < t; j++){
                mp[i][j].x = pts[0].x;
            }
        }   
    }

    /*
        Find the best matching unit for a point
        Input: Indice de um ponto qualquer
        Output: i e j do Neuronio mais perto
    */
    pair<int,int> BMU(int pi){

        double minValue = 1e18;
        int ansI, ansJ;

        for(int i = 0; i < t; i++){
            for(int j = 0; j < t; j++){
                double cur = distEuclid(pts[pi], mp[i][j]);
                if(cur < minValue){
                    minValue = cur;
                    ansI = i;
                    ansJ = j;
                }
            }
        }

        return {ansI, ansJ};
    }

    //Calcula a distancia entre 2 neuronios, baseado na vizinhança
    double distMat(int i, int j, int bi, int bj){
        
    }

    void run(){

        for(int i = 0; i < n; i++){
            auto [mi, mj] = BMU(i); //Encontra o melhor

            //Atualiza todos os vizinhos baseado no melhor
            for(int a = 0; a < t; a++){
                for(int b = 0; b < t; b++){
                    double teta = distMat(a,b,mi,mj);
                    mp[a][b] = mp[a][b] + alfa * teta * (mp[a][b] - pts[i]); 
                }
            }
        }

    }

};