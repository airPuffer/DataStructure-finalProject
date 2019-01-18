//
// Created by Xiao Guo on 2019/1/16.
//

#ifndef FINALPROJECT_V2_COMMON_H
#define FINALPROJECT_V2_COMMON_H


#include <iostream>
#include <fstream>
#include <assert.h>
#include <cstring>
#include <time.h>

using namespace std;

#define MAX_COL_SIZE 10
#define MAX_ATTR_LEN 30
#define MAX_COMMAND_SEG 20
#define MAX_COMMAND_SEG_LEN 20

class Record{
    static int attrc;
    static char attr_name[MAX_COL_SIZE][MAX_ATTR_LEN];

public:
    char *attrv[MAX_COL_SIZE];
    //TODO
    //realize get and set attr;
    static int getAttr(char attrs[][MAX_ATTR_LEN]); //get attribute per record
    static bool setAttr(int num_attr, const char attrs[][MAX_ATTR_LEN]); //set attribute for record before reading records
    static int getNumAttr(){ return attrc;}
    static int locateAttr(const char* str){
        for(int i=0;i<MAX_COL_SIZE;i++){
            if(strcmp(str,attr_name[i])==0)
                return i;
        }
        cout<<"Failed:"<<str<<endl;
        for(int i=0;i<attrc;i++)
            cout<<attr_name[i]<<" ";
        assert(0);
        return -1;
    }
    Record();
    ~Record();
    friend ifstream & operator >> (ifstream &input, Record &record);
};

class Command{
public:
    int argc;
    char *argv[MAX_COMMAND_SEG];
    Command();
    ~Command();
    friend ifstream & operator >> (ifstream &input, Command &command);
};

ifstream & operator >> (ifstream &input, Record &record);
ifstream & operator >> (ifstream &input, Command &command);

void INFO(const char* msg);
void helper(Command &command);
void loadData(char *datafile);



#endif //FINALPROJECT_V2_COMMON_H
