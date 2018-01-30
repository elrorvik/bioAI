#include<string>
#include<sstream>
#include<set>
#include<ios>
using namespace std;

struct city {
    int index;
    int x;
    int y;
    int duration;
    int demand;
}

struct depot{
    int max_duration_per_vehicle;
    int max_load_per_vehicle;
}

void foo(string filename, set cities, set depot, int &n_vehicles, int &n_customers, int &n_depot) {
    fstream  r_file;
    rFile.open(filename, ios::out | ios::in );

    string line;

    int i = 0;

    rFile >> line;
        
        istringstream iss(line)

        if (i == 0) {
            
            n_vehicles = 
        }

    while(!rFile.eof()) {
        rFile >> line;
        
        istringstream iss(line)

        if (i == 0) {

            n_vehicles = 
        }
    }
    rFile.close();
}