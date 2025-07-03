#include <iostream>
#include <functional>
#include <random>
#include "Point.h"
using namespace std;


typedef Point Neu;
struct Kohonen{

    int n, t, epochs ; // Numero de pontos, Tamanho da matriz (txt), número de épocas
    vector<vector<Neu>> mp; // Mapa de neurônios
    vector<Point> pts; // Entradas do dataset
    double lr_start; // Taxa de aprendizado inicial
    double sigma_start; // Raio inicial de vizinhança
    int total_iter; // Total de iterações

    Kohonen(int t, int epochs, int sigma_start, double lr_start, vector<Point> &pts) : t(t), n(pts.size()), epochs(epochs), sigma_start(sigma_start), lr_start(lr_start), pts(pts), mp(t, vector<Neu>(t))
    {   
        total_iter = epochs*n;

        // Inicia os neurônios em pontos randomicos do conjunto de dados
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(0, pts.size() - 1);

        for(int i = 0; i < t; i++){
            for(int j = 0; j < t; j++){
                mp[i][j] = pts[distr(gen)]; // Obtém os pesos de uma entrada aleatória
            }
        }   
    }

    // Encontra o Best Matching Unit (BMU) para um ponto
    // Input: Indice de um ponto qualquer
    // Output: i e j do Neuronio mais perto
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

   // Calcula a influência da vizinhança usando uma função Gaussiana.
   // A influência diminui à medida que a distância na grade a partir da BMU aumenta.
    double neighborhood_influence(int i, int j, int bi, int bj, double sigma){
        double dist_sq = (i - bi) * (i - bi) + (j - bj) * (j - bj);
        return exp(-dist_sq / (2 * sigma * sigma));
    }

    // Calcula a taxa de aprendizado não linear de acordo com a iteração atual
    double calculate_learning_rate(int k) { 
        return lr_start * exp(- k / (total_iter/2.0));
    }

    // Calcula o raio de vizinhança não linear de acordo com a iteração atual
    double calculate_sigma(int k) {
        return sigma_start * exp(- k / (total_iter/2.0));
    }

    void run(){
        
        for (int e = 0; e < epochs; e++) { // Itera sobre as epochs
            for (int p = 0; p < n; p++) { // Itera sobre os pontos
                auto [mi, mj] = BMU(p); // Encontra o melhor neurônio
                
                int k = e*n + p; // Iteração atual
                double lr = calculate_learning_rate(k);
                double sigma = calculate_sigma(k);

                // Atualiza todos os vizinhos baseado no melhor
                for(int a = 0; a < t; a++){
                    for(int b = 0; b < t; b++){
                        double theta = neighborhood_influence(a,b,mi,mj,sigma);
                        mp[a][b] = mp[a][b] + lr * theta * (pts[p] - mp[a][b]); 
                    }
                }
            }
        }
    }

};