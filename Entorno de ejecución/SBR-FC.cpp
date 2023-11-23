#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <queue>
#include <math.h>

using namespace std;

#define MAX_PRE 8
#define MAX_LENGHT_STRING 128
#define MAX_LENGHT_NAME 16
#define MAX_LENGHT_FACT 16
#define CHAR_COLON ":"
#define CHAR_COMMA ","
#define CHAR_EQUAL "="
#define CHAR_HYPHEN "-"
#define THEN "Entonces"
#define AND "y"
#define OR "o"
#define OBJECTIVE "Objetivo"
#define TXT_EXTENSION_LENGHT 4

struct ruleRep {
    char* name;
    int nPre;
    char* pre[MAX_PRE];
    char op;
    char* post;
    double fc;
};
typedef struct ruleRep* rule;
int nRules;
rule* BC;

struct factRep {
    char* name;
    double fc;
};
typedef struct factRep* fact;
int nFacts;
fact* BH;
char objective[MAX_LENGHT_STRING];

void readBC(char* nameBC) {
    ifstream fichBC(nameBC);
    char linea[MAX_LENGHT_STRING];
    fichBC >> linea;
    nRules = atoi(linea);
    BC = (rule*) malloc(nRules*sizeof(rule));
    for(int i=0; i<nRules; i=i+1) {
        BC[i] = (rule) malloc(sizeof(struct ruleRep));
        fichBC >> linea;
        char* tok = strtok(linea, CHAR_COLON);
        BC[i]->name = (char*) malloc(sizeof(char)*MAX_LENGHT_NAME);
        strcpy(BC[i]->name, tok);
        fichBC >> linea; // Leemos "Si" para descartarlo.
        fichBC >> linea; // Leemos el primer antecedente.
        int countPre = 0;
        while(strcmp(linea, THEN)) {
            if(!strcmp(linea, AND)) {
                BC[i]->op = 'y';
            } else if(!strcmp(linea, OR)) {
                BC[i]->op = 'o';
            } else {
                BC[i]->pre[countPre] = (char*) malloc(sizeof(char)*MAX_LENGHT_FACT);
                strcpy(BC[i]->pre[countPre], linea);
                countPre = countPre + 1;
            }
            fichBC >> linea;
        }
        BC[i]->nPre = countPre;
        fichBC >> linea;
        BC[i]->post = (char*) malloc(sizeof(char)*MAX_LENGHT_FACT);
        tok = strtok(linea, CHAR_COMMA);
        strcpy(BC[i]->post, tok);
        fichBC >> linea;
        tok = strtok(linea, CHAR_EQUAL);
        tok = strtok(NULL, CHAR_EQUAL);
        BC[i]->fc = atof(tok);
    }
    fichBC.close();

    /*
    for(int i=0; i<nRules; i=i+1) {
        cout << BC[i]->name << ": Si ";
        for(int j=0; j<((BC[i]->nPre)-1); j++) {
            cout << BC[i]->pre[j] << " " << BC[j]->op << " ";
        }
        cout << BC[i]->pre[(BC[i]->nPre)-1] << " Entonces " << BC[i]->post << ", FC=" << BC[i]->fc << endl;
    }
    */
}

void readBH(char* nameBH) {
    ifstream fileBH(nameBH);
    char linea[MAX_LENGHT_STRING];
    fileBH >> linea;
    nFacts = atoi(linea);
    BH = (fact*) malloc((nRules+nFacts)*sizeof(fact));
    for(int i=0; i<nFacts; i=i+1) {
        BH[i] = (fact) malloc(sizeof(struct factRep));
        fileBH >> linea;
        char* tok = strtok(linea, CHAR_COMMA);
        BH[i]->name = (char*) malloc(MAX_LENGHT_FACT*sizeof(char));
        strcpy(BH[i]->name, tok);
        fileBH >> linea;
        tok = strtok(linea, CHAR_EQUAL);
        tok = strtok(NULL, CHAR_EQUAL);
        BH[i]->fc = atof(tok);
    }
    fileBH >> linea; // Leemos "Objetivo" para descartarlo.
    fileBH >> linea; // Leemos el objetivo.
    strcpy(objective, linea);
    fileBH.close();

    /*for(int i=0; i<nFacts; i=i+1) {
        cout << BH[i]->name << ", FC=" << BH[i]->fc << endl;
    }
    cout << "Objetivo" << endl << objective << endl;*/
}

void freeBC() {
    for(int i=0; i<nRules; i=i+1) {
        free(BC[i]->name);
        for(int j=0; j<BC[i]->nPre; j=j+1) {
            free(BC[i]->pre[j]);
        }
        free(BC[i]->post);
        free(BC[i]);
    }
    free(BC);
}

void freeBH() {
    for(int i=0; i<nFacts; i=i+1) {
        free(BH[i]->name);
        free(BH[i]);
    }
    free(BH);
}

fact contained(char* goal) {
    int i = 0;
    while((i<nFacts)&&(strcmp(goal, BH[i]->name))) {
        i = i + 1;
    }
    if(i<nFacts) {
        return BH[i];
    } else {
        return NULL;
    }
}

queue<rule> equate(char* goal) {
    queue<rule> conflictSet;
    for(int i=0; i<nRules; i=i+1) {
        if(!strcmp(goal, BC[i]->post)) {
            conflictSet.push(BC[i]);
        }
    }
    return conflictSet;
}

rule solve(queue<rule> conflictSet) {
    return conflictSet.front();
}

queue<rule> removeRule(queue<rule> conflictSet) {
    conflictSet.pop();
    return conflictSet;
}

queue<char*> extractPre(rule r) {
    queue<char*> pres;
    for(int i=0; i<r->nPre; i=i+1) {
        pres.push(r->pre[i]);
    }
    return pres;
}

char* selectGoal(queue<char*> goals) {
    return goals.front();
}

queue<char*> removeGoal(queue<char*> goals) {
    goals.pop();
    return goals;
}

double firstCase(rule r, double fc1, double fc2) {
    if(r->op=='o') {
        return max(fc1, fc2);
    } else {
        return min(fc1, fc2);
    }
}

double secondCase(queue<double> fcs, char* goal, FILE* outputFile) {
    double fc1, fc2;
    fc1 = fcs.front();
    fcs.pop();
    while(!fcs.empty()) {
        fc2 = fcs.front();
        fcs.pop();
        if((fc1>=0) && (fc2>=0)) {
            fc1 = fc1+fc2*(1-fc1);
        } else if((fc1<=0) && (fc2<=0)) {
            fc1 = fc1+fc2*(1+fc1);
        } else {
            fc1 = (fc1+fc2)/(1-min(fabs(fc1), fabs(fc2)));
        }
        fprintf(outputFile, "CASO 2: combinamos dos reglas para llegar al hecho %s. FC=%f.\n", goal, fc1);
    }
    return fc1;
}

double thirdCase(rule r, double fc) {
    return r->fc * max(0.0, fc);
}

void add(char* goal, double fc) {
    BH[nFacts] = (fact) malloc(sizeof(struct factRep));
    BH[nFacts]->name = (char*) malloc(MAX_LENGHT_FACT*sizeof(char));
    strcpy(BH[nFacts]->name, goal);
    BH[nFacts]->fc = fc;
    nFacts = nFacts + 1;
}

double verify(char* goal, FILE* outputFile) {
    fact goalStruct = contained(goal);
    if(goalStruct != NULL) {
        fprintf(outputFile, "%s está en la base de hechos, FC=%f.\n", goalStruct->name, goalStruct->fc);
        return goalStruct->fc;
    } else {
        fprintf(outputFile, "%s no está en la base de hechos.\n", goal);
    }
    queue<rule> conflictSet = equate(goal);
    queue<char*> newGoals;
    char* newGoal;
    double fc, fcAux;
    queue<double> setAux;
    rule r;
    while(!conflictSet.empty()) {
        r = solve(conflictSet);
        fprintf(outputFile, "Regla elegida: %s. ", r->name);
        if(r->nPre>1) {
            fprintf(outputFile, "Sus antecedentes son:");
            for(int i=0; i<r->nPre; i=i+1) {
                fprintf(outputFile, " %s", r->pre[i]);
            }
            fprintf(outputFile, ". El operador que se va a utilizar es: %c.\n", r->op);
        } else {
            fprintf(outputFile, "Su antecedente es: %s.\n", r->pre[0]);
        }

        conflictSet = removeRule(conflictSet);
        newGoals = extractPre(r);

        newGoal = selectGoal(newGoals);
        newGoals = removeGoal(newGoals);
        fc = verify(newGoal, outputFile);
        while(!newGoals.empty()) {
            newGoal = selectGoal(newGoals);
            newGoals = removeGoal(newGoals);
            fcAux = verify(newGoal, outputFile);

            fc = firstCase(r, fc, fcAux);
            fprintf(outputFile, "CASO 1: se combinan los antecedentes de la regla %s. FC=%f.\n", r->name, fc);
        }

        fc = thirdCase(r, fc);
        fprintf(outputFile, "CASO 3: se combina la evidencia con la regla %s. FC=%f.\n", r->name, fc);
        setAux.push(fc);
    }

    if(setAux.size()>1) {
        fc = secondCase(setAux, goal, outputFile);
    }
    add(goal, fc);
    return fc;
}

double backguardRouting(FILE* outputFile) {
    char* goal = (char*) malloc(strlen(objective)*sizeof(char));
    strcpy(goal, objective);
    double fc = verify(goal, outputFile);
    free(goal);
    return fc;
}

void removeTXTextension(char* fileName) {
    int lenght = strlen(fileName);
    if(lenght >= TXT_EXTENSION_LENGHT) {
        fileName[lenght-TXT_EXTENSION_LENGHT] = '\0';
    }
}

char* outputNameGenerator(char* BC, char* BH) {
    int outputFileNameLenght = strlen(BC) + strlen(BC) + TXT_EXTENSION_LENGHT + 2*strlen("-");
    char* outputName = (char*) malloc(outputFileNameLenght*sizeof(char));
    strcpy(outputName, BC);
    strcat(outputName, CHAR_HYPHEN);
    strcat(outputName, BH);
    strcat(outputName, ".txt");
    return outputName;
}

int main(int argc, char **argv) {
    char* nameBC = argv[1];
    char* nameBH = argv[2];
    readBC(nameBC);
    readBH(nameBH);

    char notExtensionNameBC[strlen(nameBC)-4];
    strcpy(notExtensionNameBC, nameBC);
    removeTXTextension(notExtensionNameBC);
    char notExtensionNameBH[strlen(nameBH)-4];
    strcpy(notExtensionNameBH, nameBH);
    removeTXTextension(notExtensionNameBH);

    char* outputFileName = outputNameGenerator(notExtensionNameBC, notExtensionNameBH);
    FILE* outputFile = fopen(outputFileName, "w");
    fprintf(outputFile, "Base de Conocimiento: %s\nBase de Hechos: %s\nObjetivo: %s\n", nameBC, nameBH, objective);

    double fc = backguardRouting(outputFile);
    fprintf(outputFile, "Objetivo %s encontrado, FC=%f\n", objective, fc);

    freeBC();
    freeBH();
}
