#include <iostream>
#include <functional>
#include <random>
#include "Point.h"
using namespace std;


typedef Point Neu;
struct Kohonen{

    int n, t, epochs ;
    vector<vector<Neu>> mp;
    vector<Point> pts;
    double lr_start;
    double sigma_start;

    Kohonen(int t, int epochs, double sigma_start, double lr_start, const vector<Point> &pts) : t(t), n(pts.size()), epochs(epochs), sigma_start(sigma_start), lr_start(lr_start), pts(pts), mp(t, vector<Neu>(t, Point(vector<double>{})))
    {   
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distr(0, pts.size() - 1);

        for(int i = 0; i < t; i++){
            for(int j = 0; j < t; j++){
                mp[i][j] = pts[distr(gen)];
            }
        }   
    }

    pair<int,int> BMU(int pi) const {

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

    double neighborhood_influence(int i, int j, int bi, int bj, double sigma){
        double dist_sq = (i - bi) * (i - bi) + (j - bj) * (j - bj);
        return exp(-dist_sq / (2 * sigma * sigma));
    }

    void run(){
        
        double tau = epochs*n / 2.0;
        double k = 0;
        for (int e = 0; e < epochs; e++) {
            for (int p = 0; p < n; p++) {
                auto [mi, mj] = BMU(p);
                
                double lr = lr_start * exp(- k / tau);
                double sigma = sigma_start * exp(- k / tau);

                for(int a = 0; a < t; a++){
                    for(int b = 0; b < t; b++){
                        double theta = neighborhood_influence(a,b,mi,mj,sigma);
                        mp[a][b] = mp[a][b] + lr * theta * (pts[p] - mp[a][b]); 
                    }
                }
                k++;
            }
        }
    }

    double quantizationError() const {
        double sum = 0.0;
        for (int p = 0; p < n; ++p) {
            auto [bi, bj] = BMU(p);
            double d = distEuclid(pts[p], mp[bi][bj]);
            sum += d;
        }
        return sum / static_cast<double>(n);
    }

    pair<pair<int, int>, pair<int, int>> BMU_2(int pi) const {
        double min_dist1 = 1e18;
        double min_dist2 = 1e18;
        pair<int, int> bmu1 = {-1, -1};
        pair<int, int> bmu2 = {-1, -1};

        for (int i = 0; i < t; ++i) {
            for (int j = 0; j < t; ++j) {
                double d = distEuclid(pts[pi], mp[i][j]);
                if (d < min_dist1) {
                    min_dist2 = min_dist1;
                    bmu2 = bmu1;
                    min_dist1 = d;
                    bmu1 = {i, j};
                } else if (d < min_dist2) {
                    min_dist2 = d;
                    bmu2 = {i, j};
                }
            }
        }
        return {bmu1, bmu2};
    }

    double topographicError() const {
        double error_count = 0.0;
        for (int p = 0; p < n; ++p) {
            auto [bmu1, bmu2] = BMU_2(p);

            if (bmu1.first == -1 || bmu2.first == -1) continue;

            int dist_on_grid = abs(bmu1.first - bmu2.first) + abs(bmu1.second - bmu2.second);

            if (dist_on_grid > 1) {
                error_count++;
            }
        }
        return error_count / static_cast<double>(n);
    }

    vector<vector<double>> computeUMatrix(bool eightNeighborhood = false) const {
        vector<vector<double>> U(t, vector<double>(t, 0));
        const int d4[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
        const int d8[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
        for(int i = 0; i < t; ++i) {
            for(int j = 0; j < t; ++j) {
                double total = 0;
                int count = 0;
                for(auto &d : d4) {
                    int ni = i + d[0], nj = j + d[1];
                    if(ni>=0 && ni<t && nj>=0 && nj<t) {
                        total += distEuclid(mp[i][j], mp[ni][nj]);
                        ++count;
                    }
                }
                if(eightNeighborhood) {
                    for(auto &d : d8) {
                        int ni = i + d[0], nj = j + d[1];
                        if(ni>=0 && ni<t && nj>=0 && nj<t) {
                            total += distEuclid(mp[i][j], mp[ni][nj]);
                            ++count;
                        }
                    }
                }
                U[i][j] = (count > 0) ? (total / count) : 0;
            }
        }
        return U;
    }

    vector<vector<double>> getBMUClusters(int attr_idx1, int attr_idx2) const {
        vector<vector<double>> projection;
        projection.reserve(n);
        for (int p = 0; p < n; ++p) {
            pair<int, int> bmu = BMU(p);
            double bmu_flat_index = static_cast<double>(bmu.first * t + bmu.second);
            vector<double> row = {pts[p].x[attr_idx1], pts[p].x[attr_idx2], bmu_flat_index};
            projection.push_back(row);
        }
        return projection;
    }
};