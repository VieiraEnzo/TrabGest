#include <iostream>
#include <functional>
#include "Point.h"
#include <random>
using namespace std;


struct KMEANS{

    int n, k;
    vector<Point> pts;
    vector<Point> centroides;
    vector<int> label; //determinar grupo de cada ponto 
    double tolerancia;

    KMEANS(int k, vector<Point> pts) : pts(pts), n(pts.size()), k(k), label(n)
    {
        random_device rd;
        mt19937 gerador(rd()); 
        uniform_int_distribution<int> distribuicao(0, n - 1);
        tolerancia = 1e-4;

        vector<int> escolhidos;

        while (centroides.size() < k){
            int indice_escolhido = distribuicao(gerador);
            bool ja_escolhido = false;

            for (int i = 0; i < escolhidos.size(); i++){
                if (escolhidos[i] == indice_escolhido){
                    ja_escolhido = true;
                    break;
                }
            }
            if (ja_escolhido == false){
                escolhidos.push_back(indice_escolhido);
                centroides.push_back(pts[indice_escolhido]);
            }
        }
    }

    void selecionar_grupos(){
        for (int i = 0; i < n; i++){
            double melhor_dist = 100000;
            int melhor_centroide = -1;
            for (int j = 0; j < k; j++){
                double distancia_atual = distEuclid(pts[i], centroides[j]);
                if (distancia_atual < melhor_dist) {
                    melhor_dist = distancia_atual;
                    melhor_centroide = j;
                }
            }
            label[i] = melhor_centroide;
        }
    }

    void recalcular_centroide(){
        int dimensao = 0;
        dimensao = pts[0].x.size();
        vector<Point> somas(k, Point(vector<double>(dimensao, 0.0)));
        vector<int> contagem(k, 0);

        for (int i = 0; i < n; i++){
            int centroide_atual = label[i];
            somas[centroide_atual] = somas[centroide_atual] + pts[i];
            contagem[centroide_atual]++;

        }
        for (int i = 0; i < k; i++){
            if (contagem[i] > 0) {
                centroides[i] = somas[i] / contagem[i];
            }
        }

    }

    /*
        Objetivo: 
        Complexidade:  
    */
    void run(){
        int max = 1000;
        for (int i = 0; i < max; i++){
            vector<Point> centroides_antigos = centroides;
            selecionar_grupos();
            recalcular_centroide();
            double max_movimento = 0.0;
            for (int j = 0; j < k; j++) {
                
                double movimento_atual = distEuclid(centroides[j], centroides_antigos[j]);
                
                
                if (movimento_atual > max_movimento) {
                    max_movimento = movimento_atual;
                }
            }
            if (max_movimento < tolerancia) {
                cout << "Convergencia atingida na iteracao " << i + 1 << " com movimento maximo de " << max_movimento << endl;
                break; 
            }
        }
    }

};