#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void copyTodataStruct(FSMD* nd,dataStruct ds);

FSMD* copyToNode(dataStruct ds);

int writeOneToFile(FILE* fp,FSMD* nd);

FSMD* readOneFromFile(FILE* fp);

int writeToFile(char* filepath,FSMD* root);

FSMD* readFromFile(char* filepath);