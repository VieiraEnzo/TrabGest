#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;


vector<Point> readPoints(string filename){
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << "\n";
        return {};
    }

    string line;
    if (getline(file, line)) {
        cout << "Header: " << line << "\n";
    }

    vector<Point> csv_data;

    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string field;

        while (getline(ss, field, ',')) {
            row.push_back(field);
        }
        csv_data.emplace_back(vector<double> {stod(row[1]), stod(row[2]), stod(row[3]), stod(row[4])});
    }

    file.close();

    return csv_data;
}