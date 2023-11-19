#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

#define MAX_ANT 32

int nRules;

struct ruleRep {
    char* name;
    char* premises[MAX_ANT];
    char* objetive;
    float fc;
};

void readBC(char* nameBC) {
    ifstream file(nameBC);
    string line;
    getline(file, line);
    nRules = atoi(line.c_str());
    cout << nRules;
}

void readBH(char* nameBH) {
    ifstream file(nameBH);
}

int main(int argc, char **argv) {
    char* nameBC = argv[1];
    char* nameBH = argv[2];
    readBC(nameBC);
    readBH(nameBH);

}
