//
// Created by Xiao Guo on 2019/1/16.
//

#ifndef FINALPROJECT_V2_DATABASE_H
#define FINALPROJECT_V2_DATABASE_H

#include <cstring>
#include "common.h"
#define NUM_CHARS 62

extern int TEST_TYPE;

struct Data{
    int id;
    char name[MAX_ATTR_LEN];
    int value[MAX_COL_SIZE-2];
};

struct Node{
    Data record;

    Node* pre;
    Node* succ;

    struct {
        Node* lchild;
        Node* rchild;
        int height;
    }AVL_Node;
};

struct Name_Node{
    char name[MAX_ATTR_LEN];
    Name_Node* next;
};

struct Tire_Node{
    Tire_Node *next[NUM_CHARS];
};

class _TIRE_TREE{
public:
    _TIRE_TREE(){
        root=new Tire_Node;
        for(int i=0;i<NUM_CHARS;i++)
            root->next[i]= nullptr;
    }
    ~_TIRE_TREE(){}
    bool Insert(char* str);
    bool Search(char* str);
private:
    Tire_Node *root;
};

class _AVL_TREE{
    friend class MyDatabase;
public:
    _AVL_TREE(){root= nullptr;}
    ~_AVL_TREE(){root= nullptr;}
    Node* Search(int id);
    bool Insert(Node *&parent,Node* node);
    Node* Remove(Node *&parent,int id);

private:
    Node* RotateL(Node* node);
    Node* RotateR(Node* node);
    Node* RotateLR(Node* node);
    Node* RotateRL(Node* node);


private:
    Node *root;
};

class _MinHeap{
    friend class MyDatabase;
public:
    _MinHeap(int size=100000,int col=0){
        current=0;
        max=size;
        this->col=col;
        heap=new Node*[max];
    }
    ~_MinHeap(){
        current=0;
        delete heap;
    }

    bool Insert(Node* d);
    bool Remove(Node* &d);
private:
    void siftDown(int start,int m);
    void siftUP(int start);

private:
    int max;
    int current;
    int col;
    Node** heap;
};

class _MaxHeap{
    friend class MyDatabase;
public:
    _MaxHeap(int size=1000000,int col=0){
        current=0;
        max=size;
        this->col=col;
        heap=new Node*[max];
    }
    ~_MaxHeap(){
        current=0;
        delete heap;
    }

    bool Insert(Node* d);
    bool Remove(Node* &d);
private:
    void siftDown(int start,int m);
    void siftUP(int start);

private:
    int max;
    int current;
    int col;
    Node** heap;

};

class MyDatabase {
    friend void Union(MyDatabase *dest,MyDatabase *src,Name_Node* &names);

    friend void Inter(MyDatabase *dest,MyDatabase *src,Name_Node* &names);
public:
    MyDatabase():avl_tree(){DataList_Head= nullptr;DataList_Tail= nullptr;names_head= nullptr;names_tail= nullptr;}
    ~MyDatabase();
    void Query_id(int id,Node* &result){
        Node* temp= avl_tree.Search(id);
        result=new Node;
        result->succ= nullptr;
        result->pre= nullptr;
        result->record.id=temp->record.id;
        strcpy(result->record.name,temp->record.name);
        for(int i=0;i<Record::getNumAttr()-2;i++)
            result->record.value[i]=temp->record.value[i];
    }

    void Query_name(const char* name,Node* &result);

    void Query_key_e(const char *key,int value,Node* &result);

    void Query_key_ne(const char *key,int value,Node* &result);

    void Query_key_g(const char *key,int value,Node* &result);

    void Query_key_ng(const char *key,int value,Node* &result);

    void Query_key_l(const char *key,int value,Node* &result);

    void Query_key_nl(const char *key,int value,Node* &result);

    void Query_key_e(int id1,int id2,const char *key,int value,Node* &result);

    void Query_key_ne(int id1,int id2,const char *key,int value,Node* &result);

    void Query_key_g(int id1,int id2,const char *key,int value,Node* &result);

    void Query_key_ng(int id1,int id2,const char *key,int value,Node* &result);

    void Query_key_l(int id1,int id2,const char *key,int value,Node* &result);

    void Query_key_nl(int id1,int id2,const char *key,int value,Node* &result);

    void Query_key_ASC(const char* key,int k,Node* &result);

    void Query_key_DESC(const char* key,int k,Node* &result);

    void Query_key_ASC_list(const char* key,int k,Node* &result);

    void Query_key_DESC_list(const char* key,int k,Node* &result);

    void Query_key_ASC(int id1,int id2,const char* key,int k,Node* &result);

    void Query_key_DESC(int id1,int id2,const char* key,int k,Node* &result);

    void Query_key_ASC_list(int id1,int id2,const char* key,int k,Node* &result);

    void Query_key_DESC_list(int id1,int id2,const char* key,int k,Node* &result);


    void Insert(Record* data);

    void Delete(int id);

    void Delete(int id1,int id2);

    void Set(int id, const char* key,int value);

    void Set(int id1,int id2, const char* key,int value);

    void Add(int id, const char* key,int value);

    void Add(int id1,int id2, const char* key,int value);

    int Sum(int id1,int id2, const char* key);




    void clear();

    void Remove(Node* data);

    void Insert(Node* data);

public:
    char Table_name[MAX_ATTR_LEN];
    Record Attribute;
private:
    Node* DataList_Head;
    Node* DataList_Tail;
    _AVL_TREE avl_tree;
    Name_Node *names_head;
    Name_Node *names_tail;


};

void Union(MyDatabase *dest,MyDatabase *src,Name_Node* &names);
void Inter(MyDatabase *dest,MyDatabase *src,Name_Node* &names);


#endif //FINALPROJECT_V2_DATABASE_H
