//
// Created by Xiao Guo on 2019/1/16.
//

#include "common.h"
#include "test.h"

void Timer::tic(char t[]){
    strcpy(test_name, t);
    cout << test_name << " test begin." << endl;
    start = clock();

}

void Timer::toc(){
    end = clock();
    used_time = double(end - start) / CLOCKS_PER_SEC;
    cout << test_name << " test done. Use time: " << used_time << endl;
}


Tester::Tester(const char *test_file, const char *result_file){
    fin.open(test_file);
    //fout.open(result_file);
    strcpy(this->result_file,result_file);


    //assert((fin && fout) || (INFO("open file failed!"), 0));
}

Tester::~Tester(){
    fin.close();
    fout.close();
}

void Tester::exec(){

    if(fp!= nullptr)
        fclose(fp);
    fp=fopen(result_file,"w");
    Command command;
    Timer timer;
    timer.tic(test_name);
    while (fin >> command){
        helper(command);
    }
    timer.toc();
}

SingleTester::SingleTester(const char *test_file, const char *result_file):Tester(test_file, result_file){
    strcpy(test_name, "Single");
}

SectionTester::SectionTester(const char *test_file, const char *result_file):Tester(test_file, result_file){
    strcpy(test_name, "Section");
}

SetTester::SetTester(const char *test_file, const char *result_file):Tester(test_file, result_file){
    strcpy(test_name, "Set");
}

