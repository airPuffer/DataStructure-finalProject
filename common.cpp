//
// Created by Xiao Guo on 2019/1/16.
//

#include "common.h"
#include <cstring>
#include <assert.h>

void INFO(const char* msg){
    cout << msg << endl;
}


int Record::attrc = 0;
char Record::attr_name[MAX_COL_SIZE][MAX_ATTR_LEN] = {'\0'};

Record::Record(){}

Record::~Record(){
    for (int i = 0; i < attrc; i++){
        delete[] attrv[i];
    }
}

int Record::getAttr(char attrs[][MAX_ATTR_LEN]){
    for(int i=0;i<attrc;i++)
        strcpy(attrs[i],attr_name[i]);
    return attrc;
}

bool Record::setAttr(int num_attr, const char attrs[][MAX_ATTR_LEN]){
    attrc=num_attr;
    for(int i=0;i<num_attr;i++)
        strcpy(attr_name[i],attrs[i]);
    return true;
}


ifstream & operator >> (ifstream &input, Record &record){
    assert (record.attrc > 0 || (INFO("attributes should be set first."), 0));

    char tmp[MAX_ATTR_LEN * MAX_COL_SIZE], *p;
    const char * dlim = " ,:\r";
    int i = 0;
    input.getline(tmp, MAX_ATTR_LEN*MAX_COL_SIZE);
    p = strtok(tmp, dlim);
    while (p){
        record.attrv[i] = new char[MAX_ATTR_LEN];
        strcpy(record.attrv[i++], p);
        p = strtok(NULL, dlim);
    }

    //assert (i == record.attrc || (INFO("attributes per record should be equal."), 0));
    return input;
}


Command::Command(){argc = 0;}

Command::~Command(){
    for (int i = 0; i < argc; i++){
        delete[] argv[i];
    }
}

ifstream & operator >> (ifstream &input, Command &command){
    if (command.argc > 0){
        for (int i = 0; i < command.argc; i++){
            delete[] command.argv[i];
        }
        command.argc = 0;
    }

    assert(command.argc == 0);

    char tmp[MAX_ATTR_LEN * MAX_COL_SIZE], *p;
    const char * dlim = "() ,:\t\r";
    input.getline(tmp, MAX_COMMAND_SEG * MAX_COMMAND_SEG_LEN);
    p = strtok(tmp, dlim);
    int i = 0;
    while (p){
        command.argv[i] = new char[MAX_COMMAND_SEG_LEN];
        strcpy(command.argv[i++], p);
        p = strtok(NULL, dlim);
    }
    command.argc = i;
    return input;
}