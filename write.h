#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


struct Output{

    ofstream file;
    vector<vector<string>> data;

    Output(string filename) : file(filename), data(1) {}

    void add(string s){
        data.back().push_back(s);
    }

    void add(vector<string> s){
        for(auto a : s) data.back().push_back(a);
    }

    void add(vector<double> s){
        for(auto a : s) data.back().push_back(to_string(a));
    }

    void add(vector<int> s){
        for(auto a : s) data.back().push_back(to_string(a));
    }

    void add(double a){
        data.back().push_back(to_string(a));
    }

    void add(int a){
        data.back().push_back(to_string(a));
    }

    void newRow(){
        data.push_back({});
    }

    void write(){
        for(int i = 0; i < data.size(); i++){
            for(auto s : data[i]){
                file << s << ", ";
            }file << "\n";
        }
    }

};