//
// Created by Xiao Guo on 2019/1/16.
//

#ifndef FINALPROJECT_V2_TEST_H
#define FINALPROJECT_V2_TEST_H


#include "common.h"

extern FILE* fp;

class Timer{
public:
    char test_name[20];
    double used_time;
    clock_t start, end;
    void tic(char t[]);
    void toc();
};

class Tester{
public:
    char test_name[20];
    char result_file[100];
    ifstream fin;
    ofstream fout;
    Tester(const char *test_file, const char *result_file);
    ~Tester();
    void exec();
};


class SingleTester:public Tester{
public:
    SingleTester(const char *test_file, const char *result_file);
};
class SectionTester:public Tester{
public:
    SectionTester(const char *test_file, const char *result_file);
};
class SetTester:public Tester{
public:
    SetTester(const char *test_file, const char *result_file);
};



#endif //FINALPROJECT_V2_TEST_H
