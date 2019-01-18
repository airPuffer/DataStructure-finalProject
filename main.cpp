#include <iostream>

#include "common.h"
#include "test.h"
#include "database.h"

MyDatabase Table1;
MyDatabase Table2;

FILE *fp;

void helper(Command &command){
    //TODO
    switch (command.argv[0][0]){
        case 'A': {
            if(command.argc==4)
                Table1.Add(atoi(command.argv[1]),command.argv[2],atoi(command.argv[3]));
            else
                Table1.Add(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[4]));
            break;
        }
        case 'D': {
            if (command.argc == 2)
                Table1.Delete(atoi(command.argv[1]));
            else
                Table1.Delete(atoi(command.argv[1]),atoi(command.argv[2]));
            break;
        }

        case 'I':{
            if(command.argv[0][2]=='S'){
                Record data;
                for(int i=0;i<Record::getNumAttr();i++){
                    data.attrv[i]=new char[MAX_ATTR_LEN];
                    strcpy(data.attrv[i],command.argv[i+1]);
                }

                Table1.Insert(&data);
            }
            else{
                Name_Node *result= nullptr;
                if(strcmp(Table1.Table_name,command.argv[1])==0)
                    Inter(&Table1,&Table2,result);
                else
                    Inter(&Table2,&Table1,result);

                /*for(Name_Node *p = result;p!= nullptr;p=p->next)
                    cout<<p->name<<endl;*/

                for(Name_Node *p = result;p!= nullptr;p=p->next)
                    fprintf(fp,"%s\n",p->name);

                /*for(Name_Node *p = result;p!= nullptr;){
                    Name_Node *temp=p;
                    p=p->next;
                    delete temp;
                }*/
            }
            break;
        }
        case 'Q':{
            Node *result= nullptr;
            if(command.argc==2)
                Table1.Query_id(atoi(command.argv[1]),result);
            else if(command.argc==4){
                if(strcmp(command.argv[1],"name")==0)
                    Table1.Query_name(command.argv[3],result);
                else if(command.argv[3][0]!='A'&&command.argv[3][0]!='D'){
                    switch (command.argv[2][0]){
                        case '>':{
                            if(command.argv[2][1]=='='){
                                Table1.Query_key_nl(command.argv[1],atoi(command.argv[3]),result);
                            }
                            else{
                                Table1.Query_key_g(command.argv[1],atoi(command.argv[3]),result);
                            }
                            break;
                        }
                        case '<':{
                            if(command.argv[2][1]=='='){
                                Table1.Query_key_ng(command.argv[1],atoi(command.argv[3]),result);
                            }
                            else{
                                Table1.Query_key_l(command.argv[1],atoi(command.argv[3]),result);
                            }
                            break;
                        }
                        case '=':{
                            Table1.Query_key_e(command.argv[1],atoi(command.argv[3]),result);
                            break;
                        }
                        case '!':{
                            Table1.Query_key_ne(command.argv[1],atoi(command.argv[3]),result);
                            break;
                        }
                        default:break;
                    }
                }
                else{
                    if(command.argv[3][0]=='A')
                        Table1.Query_key_ASC(command.argv[1],atoi(command.argv[2]),result);

                    else
                        Table1.Query_key_DESC(command.argv[1],atoi(command.argv[2]),result);
                }
            }
            else if(command.argc==5){
                if(command.argv[4][0]=='A')
                    Table1.Query_key_ASC_list(command.argv[1],atoi(command.argv[2]),result);
                else
                    Table1.Query_key_DESC_list(command.argv[1],atoi(command.argv[2]),result);

            }
            else if(command.argc==6){
                if(command.argv[5][0]=='A')
                    Table1.Query_key_ASC(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[4]),result);
                else if(command.argv[5][0]=='D')
                    Table1.Query_key_DESC(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[4]),result);
                else{
                    switch (command.argv[4][0]){
                        case '>':{
                            if(command.argv[4][1]=='=')
                                Table1.Query_key_nl(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[5]),result);
                            else
                                Table1.Query_key_g(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[5]),result);
                            break;
                        }
                        case '<':
                            if(command.argv[4][1]=='=')
                                Table1.Query_key_ng(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[5]),result);
                            else
                                Table1.Query_key_l(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[5]),result);
                            break;
                        case '=':
                            Table1.Query_key_e(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[5]),result);
                            break;
                        case '!':
                            Table1.Query_key_ne(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[5]),result);
                            break;
                        default:break;
                    }
                }
            }
            else if(command.argc==7){
                if(command.argv[6][0]=='A')
                    Table1.Query_key_ASC_list(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[4]),result);
                else
                    Table1.Query_key_DESC_list(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[4]),result);
            }
            else;

            /*for(Node* p=result;p!= nullptr;p=p->succ){
                cout<<p->record.id <<" "<<p->record.name<<" ";
                for(int i=0;i<Record::getNumAttr()-2;i++)
                    cout<<" "<<p->record.value[i];
                cout<<endl;

            }*/

            for(Node* p=result;p!= nullptr;p=p->succ){
                fprintf(fp,"%d %s ",p->record.id,p->record.name);
                for(int i=0;i<Record::getNumAttr()-2;i++)
                    fprintf(fp,"%d ",p->record.value[i]);
                fprintf(fp,"\n");
            }

            /*for(Node* p=result;p!= nullptr;){
                Node* tmp=p;
                p=p->succ;
                delete tmp;
            }*/

            break;
        }
        case 'S':{
            if(command.argv[0][1]=='E'){
                if(command.argc==4)
                    Table1.Set(atoi(command.argv[1]),command.argv[2],atoi(command.argv[3]));
                else
                    Table1.Set(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3],atoi(command.argv[4]));
            }
            else{
                int sum=Table1.Sum(atoi(command.argv[1]),atoi(command.argv[2]),command.argv[3]);

                //cout<<sum<<endl;
                fprintf(fp,"%d\n",sum);
            }
            break;
        }
        case 'U':{
            Name_Node *result= nullptr;
            if(strcmp(Table1.Table_name,command.argv[1])==0)
                Union(&Table1,&Table2,result);
            else
                Union(&Table2,&Table1,result);

            /*for(Name_Node *p = result;p!= nullptr;p=p->next)
                cout<<p->name<<endl;*/

            for(Name_Node *p = result;p!= nullptr;p=p->next)
                fprintf(fp,"%s\n",p->name);

            /*for(Name_Node *p = result;p!= nullptr;){
                Name_Node *temp=p;
                p=p->next;
                delete temp;
            }*/

        }
        default:break;
    }
}

void loadData(const char *file,MyDatabase* Table){
    //TODO
    //load data file
    Table->clear();
    ifstream fin;
    fin.open(file);

    char table_name[MAX_ATTR_LEN];
    fin.getline(table_name,MAX_ATTR_LEN);
    strcpy(Table->Table_name,strtok(table_name,"\r"));

    char tmp[MAX_ATTR_LEN * MAX_COL_SIZE], attrv[MAX_COL_SIZE][MAX_ATTR_LEN],*p;
    const char * dlim = " ,:\r";
    int i = 0;
    fin.getline(tmp, MAX_ATTR_LEN*MAX_COL_SIZE);
    p = strtok(tmp, dlim);
    while (p){
        strcpy(attrv[i++], p);
        p = strtok(nullptr, dlim);
    }
    Record::setAttr(i,attrv);

    Record data;
    while(fin>>data){
        Table->Insert(&data);
    }
    fin.close();
}

int TEST_TYPE = 0;


int main(int argc, char const *argv[]) {
    /* code */
    Timer timer;
    timer.tic("All");

    fp= nullptr;

    SingleTester single_tester("single.txt", "result_single.txt");
    SectionTester section_tester1("section1.txt", "result_section1.txt");

    SectionTester section_tester2("section2.txt", "result_section2.txt");

    SetTester set_tester("set.txt", "result_set.txt");

    //单点测试
    TEST_TYPE = 0;
    loadData("data_single.txt",&Table1);
    INFO("=====================================");
    single_tester.exec();
    INFO("=====================================\n");

    //区间测试1
    TEST_TYPE = 1;
    loadData("data_section1.txt",&Table1);
    INFO("=====================================");
    section_tester1.exec();
    INFO("=====================================\n");

    //区间测试2
    TEST_TYPE = 2;
    loadData("data_section2.txt",&Table1);
    INFO("=====================================");
    section_tester2.exec();
    INFO("=====================================\n");

    //集合测试
    TEST_TYPE = 3;
    loadData("data_set1.txt",&Table1);
    loadData("data_set2.txt",&Table2);
    INFO("=====================================");
    set_tester.exec();
    INFO("=====================================\n");

    fclose(fp);

    timer.toc();
    INFO("You are a genius!");

    return 0;
}



