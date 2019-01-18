//
// Created by Xiao Guo on 2019/1/16.
//

#include "database.h"

#define HEIGHT(node) ((node==nullptr)?-1:node->AVL_Node.height)
#define MAX(a,b) a>b?a:b

bool _TIRE_TREE::Insert(char *str) {
    int len=strlen(str);
    char *current=str;
    for(Tire_Node *p=root;*current!='\0';current++){
        int col;
        char c=*current;
        if(c<='9')
            col=c-'0';
        else if(c>='a')
            col=c-'a'+36;
        else col=c-'A'+10;
        if(p->next[col]== nullptr){
            p->next[col]=new Tire_Node;
            p=p->next[col];
            for(int i=0;i<NUM_CHARS;i++)
                p->next[i]= nullptr;
        }
        else{
            len--;
            p=p->next[col];
        }
    }
    if(len>0)
        return true;
    else
        return false;
}

bool _TIRE_TREE::Search(char *str) {
    char *current=str;
    for(Tire_Node *p=root;*current!='\0';current++){
        int col;
        char c=*current;
        if(c<='9')
            col=c-'0';
        else if(c>='a')
            col=c-'a'+36;
        else col=c-'A'+10;
        if(p->next[col]== nullptr)
            return false;
        else
            p=p->next[col];
    }
    return true;
}

Node* _AVL_TREE::Search(int id) {
    Node* ret=root;
    while(ret!= nullptr){
        if(ret->record.id > id)
            ret=ret->AVL_Node.lchild;
        else if(ret->record.id == id)
            return ret;
        else
            ret=ret->AVL_Node.rchild;
    }
    return ret;
}

bool _AVL_TREE::Insert(Node *&parent,Node *node) {
    bool ret;
    if (parent == nullptr){
        parent=node;
        return true;
    }
    else if (node->record.id < parent->record.id)
    {
        ret=Insert(parent->AVL_Node.lchild, node);

        // 插入节点后，若AVL树失去平衡，则进行相应的调节。
        if (HEIGHT(parent->AVL_Node.lchild) - HEIGHT(parent->AVL_Node.rchild) == 2)
            {
                if (node->record.id < parent->AVL_Node.lchild->record.id)
                    parent = RotateR(parent);
                else
                    parent = RotateLR(parent);
            }
    }
    else if (node->record.id > parent->record.id) {
        ret = Insert(parent->AVL_Node.rchild, node);
        // 插入节点后，若AVL树失去平衡，则进行相应的调节。
        if (HEIGHT(parent->AVL_Node.rchild) - HEIGHT(parent->AVL_Node.lchild) == 2)
        {
            if (node->record.id > parent->AVL_Node.rchild->record.id)
                parent = RotateL(parent);
            else
                parent = RotateRL(parent);
        }
    }
    else
        return false;

    parent->AVL_Node.height = MAX( HEIGHT(parent->AVL_Node.lchild), HEIGHT(parent->AVL_Node.rchild)) + 1;

    return ret;
}

Node* _AVL_TREE::Remove(Node *&parent,int id) {
    // 根为空 或者 没有要删除的节点，直接返回NULL
    // 。
    Node *ret;
    if (parent== nullptr)
        ret= nullptr;

    else if(id<parent->record.id)         // 待删除的节点在"tree的左子树"中
    {
        ret = Remove(parent->AVL_Node.lchild, id);
        // 删除节点后，若AVL树失去平衡，则进行相应的调节。

        if (HEIGHT(parent->AVL_Node.rchild) - HEIGHT(parent->AVL_Node.lchild) == 2)
        {
            Node *r =  parent->AVL_Node.rchild;
            if (HEIGHT(r->AVL_Node.lchild) > HEIGHT(r->AVL_Node.rchild))
                parent = RotateRL(parent);
            else
                parent = RotateL(parent);
        }
    }
    else if (id>parent->record.id)// 待删除的节点在"tree的右子树"中
    {
        ret = Remove(parent->AVL_Node.rchild, id);
        // 删除节点后，若AVL树失去平衡，则进行相应的调节。
        if (HEIGHT(parent->AVL_Node.lchild) - HEIGHT(parent->AVL_Node.rchild) == 2)
        {
            Node *l =  parent->AVL_Node.lchild;
            if (HEIGHT(l->AVL_Node.rchild) > HEIGHT(l->AVL_Node.lchild))
                parent = RotateLR(parent);
            else
                parent = RotateR(parent);
        }
    }
    else    // tree是对应要删除的节点。
    {
        // tree的左右孩子都非空
        if ((parent->AVL_Node.lchild!= nullptr) && (parent->AVL_Node.rchild!= nullptr))
        {
            if (HEIGHT(parent->AVL_Node.lchild) > HEIGHT(parent->AVL_Node.rchild))
            {
                Node *replace = parent->AVL_Node.lchild;
                while(replace->AVL_Node.rchild != nullptr)
                    replace = replace->AVL_Node.rchild;

                parent->record.id=replace->record.id;
                strcpy(parent->record.name,replace->record.name);
                for(int i=0;i<Record::getNumAttr()-2;i++)
                    parent->record.value[i]=replace->record.value[i];


                ret = Remove(parent->AVL_Node.lchild,replace->record.id);
                assert(replace==ret);
            }
            else
            {
                Node *replace = parent->AVL_Node.rchild;
                while(replace->AVL_Node.lchild != nullptr)
                    replace = replace->AVL_Node.lchild;

                parent->record.id=replace->record.id;
                strcpy(parent->record.name,replace->record.name);
                for(int i=0;i<Record::getNumAttr()-2;i++)
                    parent->record.value[i]=replace->record.value[i];

                ret = Remove(parent->AVL_Node.rchild,replace->record.id);
                assert(replace==ret);
            }
        }
        else{
            Node* temp=parent;
            parent = (parent->AVL_Node.lchild!= nullptr) ? parent->AVL_Node.lchild : parent->AVL_Node.rchild;
            return temp;
        }
    }
    if(parent!= nullptr)
        parent->AVL_Node.height = MAX( HEIGHT(parent->AVL_Node.lchild), HEIGHT(parent->AVL_Node.rchild)) + 1;

    return ret;
}

Node* _AVL_TREE::RotateL(Node* node){
    Node* ptr=node->AVL_Node.rchild;
    node->AVL_Node.rchild = ptr->AVL_Node.lchild;
    ptr->AVL_Node.lchild = node;

    node->AVL_Node.height = MAX( HEIGHT(node->AVL_Node.lchild), HEIGHT(node->AVL_Node.rchild)) + 1;
    ptr->AVL_Node.height = MAX( HEIGHT(ptr->AVL_Node.rchild), node->AVL_Node.height) + 1;

    return ptr;
}

Node* _AVL_TREE::RotateR(Node* node){
    Node* ptr=node->AVL_Node.lchild;
    node->AVL_Node.lchild = ptr->AVL_Node.rchild;
    ptr->AVL_Node.rchild = node;

    node->AVL_Node.height = MAX( HEIGHT(node->AVL_Node.lchild), HEIGHT(node->AVL_Node.rchild)) + 1;
    ptr->AVL_Node.height = MAX( HEIGHT(ptr->AVL_Node.lchild), node->AVL_Node.height) + 1;

    return ptr;

}

Node* _AVL_TREE::RotateLR(Node* node){
    node->AVL_Node.lchild = RotateL(node->AVL_Node.rchild);

    return RotateR(node);

}

Node* _AVL_TREE::RotateRL(Node* node){
    node->AVL_Node.rchild = RotateR(node->AVL_Node.lchild);

    return RotateL(node);
}

bool _MinHeap::Insert(Node *d) {
    heap[current]=d;
    siftUP(current);
    current++;
    return true;
}

bool _MinHeap::Remove(Node *&d) {
    d=heap[0];
    heap[0]=heap[current-1];
    current--;
    siftDown(0,current-1);
    return true;
}

void _MinHeap::siftDown(int start, int m) {
    int i=start,j=2*i+1;
    Node* temp=heap[i];
    while(j<=m){
        if(j<m){
            if(heap[j]->record.value[col]>heap[j+1]->record.value[col])
                j++;
            else if(heap[j]->record.value[col]==heap[j+1]->record.value[col]&&heap[j]->record.id>heap[j+1]->record.id)
                j++;
            else;
        }
        if(temp->record.value[col]<heap[j]->record.value[col])
            break;
        else if(temp->record.value[col]==heap[j]->record.value[col]&&temp->record.id<heap[j]->record.id)
            break;
        else{
            heap[i]=heap[j];
            i=j;
            j=2*j+1;
        }
    }
    heap[i]=temp;
}

void _MinHeap::siftUP(int start) {
    int j=start,i=(j-1)/2;
    Node* temp=heap[j];
    while(j>0){
        if(heap[i]->record.value[col]<temp->record.value[col])
            break;
        else if(heap[i]->record.value[col]==temp->record.value[col]){
            if(heap[i]->record.id>temp->record.value[col])
                break;
            else{
                heap[j]=heap[i];
                j=i;
                i=(i-1)/2;
            }
        }
        else{
            heap[j]=heap[i];
            j=i;
            i=(i-1)/2;
        }
    }
    heap[j]=temp;
}

bool _MaxHeap::Insert(Node *d) {
    heap[current]=d;
    siftUP(current);
    current++;
    return true;
}

bool _MaxHeap::Remove(Node *&d) {
    d=heap[0];
    heap[0]=heap[current-1];
    current--;
    siftDown(0,current-1);
    return true;
}

void _MaxHeap::siftDown(int start, int m) {
    int i=start,j=2*i+1;
    Node* temp=heap[i];
    while(j<=m){
        if(j<m){
            if(heap[j]->record.value[col]<heap[j+1]->record.value[col])
                j++;
            else if(heap[j]->record.value[col]==heap[j+1]->record.value[col]&&heap[j]->record.id>heap[j+1]->record.id)
                j++;
            else;
        }
        if(temp->record.value[col]>heap[j]->record.value[col])
            break;
        else if(temp->record.value[col]==heap[j]->record.value[col]&&temp->record.id<heap[j]->record.id)
            break;
        else{
            heap[i]=heap[j];
            i=j;
            j=2*j+1;
        }
    }
    heap[i]=temp;
}

void _MaxHeap::siftUP(int start) {
    int j=start,i=(j-1)/2;
    Node* temp=heap[j];
    while(j>0){
        if(heap[i]->record.value[col]>temp->record.value[col])
            break;
        else if(heap[i]->record.value[col]==temp->record.value[col]){
            if(heap[i]->record.id>temp->record.value[col])
                break;
            else{
                heap[j]=heap[i];
                j=i;
                i=(i-1)/2;
            }
        }
        else{
            heap[j]=heap[i];
            j=i;
            i=(i-1)/2;
        }
    }
    heap[j]=temp;
}

MyDatabase::~MyDatabase() {
    Node* p=DataList_Head;
    while(p!= nullptr){
        Node* tmp=p;
        p=p->succ;
        delete tmp;
    }
    DataList_Head= nullptr;
    DataList_Tail= nullptr;

}

void MyDatabase::Query_name(const char* name,Node* &result){
    for(Node *p=DataList_Tail;p!= nullptr;p=p->pre)
        if(strcmp(name,p->record.name)==0){
            Node* temp=new Node;
            temp->pre= nullptr;
            temp->record.id=p->record.id;
            strcpy(temp->record.name,p->record.name);
            for(int i=0;i<Record::getNumAttr()-2;i++)
                temp->record.value[i]=p->record.value[i];
            temp->succ=result;
            result=temp;
        }
};

void MyDatabase::Query_key_e(const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    for(Node* p=DataList_Tail;p!= nullptr;p=p->pre)
        if(p->record.value[col]==value){
            Node* temp=new Node;
            temp->pre= nullptr;
            temp->record.id=p->record.id;
            strcpy(temp->record.name,p->record.name);
            for(int m=0;m<Record::getNumAttr()-2;m++)
                temp->record.value[m]=p->record.value[m];
            temp->succ=result;
            result=temp;
        }
}

void MyDatabase::Query_key_ne(const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    for(Node* p=DataList_Tail;p!= nullptr;p=p->pre)
        if(p->record.value[col]!=value){
            Node* temp=new Node;
            temp->pre= nullptr;
            temp->record.id=p->record.id;
            strcpy(temp->record.name,p->record.name);
            for(int m=0;m<Record::getNumAttr()-2;m++)
                temp->record.value[m]=p->record.value[m];
            temp->succ=result;
            result=temp;
        }
}

void MyDatabase::Query_key_g(const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    for(Node* p=DataList_Tail;p!= nullptr;p=p->pre)
        if(p->record.value[col]>value){
            Node* temp=new Node;
            temp->pre= nullptr;
            temp->record.id=p->record.id;
            strcpy(temp->record.name,p->record.name);
            for(int m=0;m<Record::getNumAttr()-2;m++)
                temp->record.value[m]=p->record.value[m];
            temp->succ=result;
            result=temp;
        }
}

void MyDatabase::Query_key_ng(const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    for(Node* p=DataList_Tail;p!= nullptr;p=p->pre)
        if(p->record.value[col]<=value){
            Node* temp=new Node;
            temp->pre= nullptr;
            temp->record.id=p->record.id;
            strcpy(temp->record.name,p->record.name);
            for(int m=0;m<Record::getNumAttr()-2;m++)
                temp->record.value[m]=p->record.value[m];
            temp->succ=result;
            result=temp;
        }
};

void MyDatabase::Query_key_l(const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    for(Node* p=DataList_Tail;p!= nullptr;p=p->pre)
        if(p->record.value[col]<value){
            Node* temp=new Node;
            temp->pre= nullptr;
            temp->record.id=p->record.id;
            strcpy(temp->record.name,p->record.name);
            for(int m=0;m<Record::getNumAttr()-2;m++)
                temp->record.value[m]=p->record.value[m];
            temp->succ=result;
            result=temp;
        }
};

void MyDatabase::Query_key_nl(const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    for(Node* p=DataList_Tail;p!= nullptr;p=p->pre)
        if(p->record.value[col]>=value){
            Node* temp=new Node;
            temp->pre= nullptr;
            temp->record.id=p->record.id;
            strcpy(temp->record.name,p->record.name);
            for(int m=0;m<Record::getNumAttr()-2;m++)
                temp->record.value[m]=p->record.value[m];
            temp->succ=result;
            result=temp;
        }
}

void MyDatabase::Query_key_e(int id1,int id2,const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    Node* tail;
    int real_id2=id2;
    do{
        tail=avl_tree.Search(real_id2);
        real_id2--;
    }while(id1<=real_id2&&tail== nullptr);

    if(tail!= nullptr){
        for(Node* p=tail;p!= nullptr&&p->record.id>=id1;p=p->pre){
            if(p->record.value[col]==value){
                Node* temp=new Node;
                temp->pre= nullptr;
                temp->record.id=p->record.id;
                strcpy(temp->record.name,p->record.name);
                for(int m=0;m<Record::getNumAttr()-2;m++)
                    temp->record.value[m]=p->record.value[m];
                temp->succ=result;
                result=temp;
            }
        }

    }
}

void MyDatabase::Query_key_ne(int id1,int id2,const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    Node* tail;
    int real_id2=id2;
    do{
        tail=avl_tree.Search(real_id2);
        real_id2--;
    }while(id1<=real_id2&&tail== nullptr);

    if(tail!= nullptr){
        for(Node* p=tail;p!= nullptr&&p->record.id>=id1;p=p->pre){
            if(p->record.value[col]!=value){
                Node* temp=new Node;
                temp->pre= nullptr;
                temp->record.id=p->record.id;
                strcpy(temp->record.name,p->record.name);
                for(int m=0;m<Record::getNumAttr()-2;m++)
                    temp->record.value[m]=p->record.value[m];
                temp->succ=result;
                result=temp;
            }
        }

    }
};

void MyDatabase::Query_key_g(int id1,int id2,const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    Node* tail;
    int real_id2=id2;
    do{
        tail=avl_tree.Search(real_id2);
        real_id2--;
    }while(id1<=real_id2&&tail== nullptr);

    if(tail!= nullptr){
        for(Node* p=tail;p!= nullptr&&p->record.id>=id1;p=p->pre){
            if(p->record.value[col]>value){
                Node* temp=new Node;
                temp->pre= nullptr;
                temp->record.id=p->record.id;
                strcpy(temp->record.name,p->record.name);
                for(int m=0;m<Record::getNumAttr()-2;m++)
                    temp->record.value[m]=p->record.value[m];
                temp->succ=result;
                result=temp;
            }
        }

    }
};

void MyDatabase::Query_key_ng(int id1,int id2,const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    Node* tail;
    int real_id2=id2;
    do{
        tail=avl_tree.Search(real_id2);
        real_id2--;
    }while(id1<=real_id2&&tail== nullptr);

    if(tail!= nullptr){
        for(Node* p=tail;p!= nullptr&&p->record.id>=id1;p=p->pre){
            if(p->record.value[col]<=value){
                Node* temp=new Node;
                temp->pre= nullptr;
                temp->record.id=p->record.id;
                strcpy(temp->record.name,p->record.name);
                for(int m=0;m<Record::getNumAttr()-2;m++)
                    temp->record.value[m]=p->record.value[m];
                temp->succ=result;
                result=temp;
            }
        }

    }
};

void MyDatabase::Query_key_l(int id1,int id2,const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    Node* tail;
    int real_id2=id2;
    do{
        tail=avl_tree.Search(real_id2);
        real_id2--;
    }while(id1<=real_id2&&tail== nullptr);

    if(tail!= nullptr){
        for(Node* p=tail;p!= nullptr&&p->record.id>=id1;p=p->pre){
            if(p->record.value[col]<value){
                Node* temp=new Node;
                temp->pre= nullptr;
                temp->record.id=p->record.id;
                strcpy(temp->record.name,p->record.name);
                for(int m=0;m<Record::getNumAttr()-2;m++)
                    temp->record.value[m]=p->record.value[m];
                temp->succ=result;
                result=temp;
            }
        }

    }
};

void MyDatabase::Query_key_nl(int id1,int id2,const char *key,int value,Node* &result){
    int col=Record::locateAttr(key)-2;
    Node* tail;
    int real_id2=id2;
    do{
        tail=avl_tree.Search(real_id2);
        real_id2--;
    }while(id1<=real_id2&&tail== nullptr);

    if(tail!= nullptr){
        for(Node* p=tail;p!= nullptr&&p->record.id>=id1;p=p->pre){
            if(p->record.value[col]>=value){
                Node* temp=new Node;
                temp->pre= nullptr;
                temp->record.id=p->record.id;
                strcpy(temp->record.name,p->record.name);
                for(int m=0;m<Record::getNumAttr()-2;m++)
                    temp->record.value[m]=p->record.value[m];
                temp->succ=result;
                result=temp;
            }
        }

    }
};

void MyDatabase::Query_key_ASC(const char* key,int k,Node* &result){
    _MaxHeap hp(1000000,Record::locateAttr(key)-2);
    Node* start= DataList_Head;
    while (start!= nullptr){
        hp.Insert(start);
        start=start->succ;
    }
    Node* temp;
    for(int i=1;i<k;i++)
        hp.Remove(temp);
    hp.Remove(temp);
    result=new Node;
    result->succ= nullptr;
    result->pre= nullptr;
    result->record.id=temp->record.id;
    strcpy(result->record.name,temp->record.name);
    for(int i=0;i<Record::getNumAttr()-2;i++)
        result->record.value[i]=temp->record.value[i];
};

void MyDatabase::Query_key_DESC(const char* key,int k,Node* &result){
    _MinHeap hp(1000000,Record::locateAttr(key)-2);
    Node* start= DataList_Head;
    while (start!= nullptr){
        hp.Insert(start);
        start=start->succ;
    }
    Node* temp;
    for(int i=1;i<k;i++)
        hp.Remove(temp);
    hp.Remove(temp);
    result=new Node;
    result->succ= nullptr;
    result->pre= nullptr;
    result->record.id=temp->record.id;
    strcpy(result->record.name,temp->record.name);
    for(int i=0;i<Record::getNumAttr()-2;i++)
        result->record.value[i]=temp->record.value[i];
}

void MyDatabase::Query_key_ASC_list(const char* key,int k,Node* &result){
    Node* ret_tail= nullptr;
    _MaxHeap hp(1000000,Record::locateAttr(key)-2);
    Node* start= DataList_Head;
    while (start!= nullptr){
        hp.Insert(start);
        start=start->succ;
    }
    for(int i=0;i<k;i++){
        Node* p;
        hp.Remove(p);
        Node* temp=new Node;
        temp->succ= nullptr;
        temp->record.id=p->record.id;
        strcpy(temp->record.name,p->record.name);
        for(int m=0;m<Record::getNumAttr()-2;m++)
            temp->record.value[m]=p->record.value[m];
        if(result== nullptr){
            result=temp;
            ret_tail=temp;
        }
        else{
            ret_tail->succ=temp;
            ret_tail=temp;
        }
    }
};

void MyDatabase::Query_key_DESC_list(const char* key,int k,Node* &result){
    Node* ret_tail= nullptr;
    _MinHeap hp(1000000,Record::locateAttr(key)-2);
    Node* start= DataList_Head;
    while (start!= nullptr){
        hp.Insert(start);
        start=start->succ;
    }
    for(int i=0;i<k;i++){
        Node* p;
        hp.Remove(p);
        Node* temp=new Node;
        temp->succ= nullptr;
        temp->record.id=p->record.id;
        strcpy(temp->record.name,p->record.name);
        for(int m=0;m<Record::getNumAttr()-2;m++)
            temp->record.value[m]=p->record.value[m];
        if(result== nullptr){
            result=temp;
            ret_tail=temp;
        }
        else{
            ret_tail->succ=temp;
            ret_tail=temp;
        }
    }
}

void MyDatabase::Query_key_ASC(int id1,int id2,const char* key,int k,Node* &result){
    _MaxHeap hp(id2-id1+1,Record::locateAttr(key)-2);
    Node* start= nullptr;
    for(int read_id1=id1;start== nullptr;read_id1++)
        start=avl_tree.Search(read_id1);
    while (start!= nullptr&&start->record.id<=id2){
        hp.Insert(start);
        start=start->succ;
    }
    Node* temp;
    for(int i=1;i<k;i++)
        hp.Remove(temp);
    hp.Remove(temp);
    result=new Node;
    result->succ= nullptr;
    result->pre= nullptr;
    result->record.id=temp->record.id;
    strcpy(result->record.name,temp->record.name);
    for(int i=0;i<Record::getNumAttr()-2;i++)
        result->record.value[i]=temp->record.value[i];

}

void MyDatabase::Query_key_DESC(int id1,int id2,const char* key,int k,Node* &result){
    _MinHeap hp(id2-id1+1,Record::locateAttr(key)-2);
    Node* start= nullptr;
    for(int read_id1=id1;start== nullptr;read_id1++)
        start=avl_tree.Search(read_id1);
    while (start!= nullptr&&start->record.id<=id2){
        hp.Insert(start);
        start=start->succ;
    }
    Node* temp;
    for(int i=1;i<k;i++)
        hp.Remove(temp);
    hp.Remove(temp);
    result=new Node;
    result->succ= nullptr;
    result->pre= nullptr;
    result->record.id=temp->record.id;
    strcpy(result->record.name,temp->record.name);
    for(int i=0;i<Record::getNumAttr()-2;i++)
        result->record.value[i]=temp->record.value[i];
}

void MyDatabase::Query_key_ASC_list(int id1,int id2,const char* key,int k,Node* &result){
    Node* ret_tail= nullptr;
    _MaxHeap hp(id2-id1+1,Record::locateAttr(key)-2);
    Node* start= nullptr;
    for(int real_id1=id1;start== nullptr&&real_id1<=id2;real_id1++)
        start=avl_tree.Search(real_id1);
    //建立堆
    while (start!= nullptr&&start->record.id<=id2){
        hp.Insert(start);
        start=start->succ;
    }
    //获得前k大
    for(int i=0;i<k;i++){
        Node* p;
        hp.Remove(p);
        Node* temp=new Node;
        temp->succ= nullptr;
        temp->record.id=p->record.id;
        strcpy(temp->record.name,p->record.name);
        for(int m=0;m<Record::getNumAttr()-2;m++)
            temp->record.value[m]=p->record.value[m];
        if(result== nullptr){
            result=temp;
            ret_tail=temp;
        }
        else{
            ret_tail->succ=temp;
            ret_tail=temp;
        }
    }
}

void MyDatabase::Query_key_DESC_list(int id1,int id2,const char* key,int k,Node* &result){
    Node* ret_tail= nullptr;
    _MinHeap hp(id2-id1+1,Record::locateAttr(key)-2);
    Node* start= nullptr;
    for(int real_id1=id1;start== nullptr&&real_id1<=id2;real_id1++)
        start=avl_tree.Search(real_id1);
    while (start!= nullptr&&start->record.id<=id2){
        hp.Insert(start);
        start=start->succ;
    }
    for(int i=0;i<k;i++){
        Node* p;
        hp.Remove(p);
        Node* temp=new Node;
        temp->succ= nullptr;
        temp->record.id=p->record.id;
        strcpy(temp->record.name,p->record.name);
        for(int m=0;m<Record::getNumAttr()-2;m++)
            temp->record.value[m]=p->record.value[m];
        if(result== nullptr){
            result=temp;
            ret_tail=temp;
        }
        else{
            ret_tail->succ=temp;
            ret_tail=temp;
        }
    }
}

void MyDatabase::clear() {
    if(DataList_Head!= nullptr){
        do{
            Node* tmp=DataList_Head;
            DataList_Head=DataList_Head->succ;
            delete tmp;
        }while (DataList_Head!= nullptr);
        DataList_Tail= nullptr;

        avl_tree.~_AVL_TREE();
    }
}

void MyDatabase::Insert(Record* data){
    if(TEST_TYPE<3){
        Node* p=new Node;
        p->record.id=atoi(data->attrv[0]);
        strcpy(p->record.name,data->attrv[1]);
        for(int i=2;i<Record::getNumAttr();i++)
            p->record.value[i-2]=atoi(data->attrv[i]);
        p->AVL_Node.rchild= nullptr;
        p->AVL_Node.lchild= nullptr;
        p->AVL_Node.height=0;

        if(avl_tree.Insert(avl_tree.root,p))
            Insert(p);//插入到有序链表中
    }
    else{
        Name_Node *p=new Name_Node;
        p->next= nullptr;
        strcpy(p->name,data->attrv[1]);
        if(names_head== nullptr){
            names_head=p;
            names_tail=p;
        }
        else{
            names_tail->next=p;
            names_tail=p;
        }
    }
}

void MyDatabase::Delete(int id){
    Node *p=avl_tree.Remove(avl_tree.root,id);
    if(p!= nullptr){
        Remove(p);
    }

}

void MyDatabase::Delete(int id1,int id2){
    for(int i=id1;i<=id2;i++)
        Delete(i);
    //assert(0);

}

void MyDatabase::Set(int id, const char* key,int value){
    int col=Record::locateAttr(key)-2;
    Node *p=avl_tree.Search(id);
    if(p!= nullptr)
        p->record.value[col]=value;
}

void MyDatabase::Set(int id1,int id2, const char* key,int value){
    for(int i=id1;i<=id2;i++)
        Set(i,key,value);
}

void MyDatabase::Add(int id, const char* key,int value){
    int col=Record::locateAttr(key)-2;
    Node *p=avl_tree.Search(id);
    if(p!= nullptr)
        p->record.value[col]+=value;
}

void MyDatabase::Add(int id1,int id2, const char* key,int value){
    for(int i=id1;i<=id2;i++)
        Add(i,key,value);
}

int MyDatabase::Sum(int id1,int id2, const char* key){
    int i=Record::locateAttr(key)-2;
    int sum=0;
    Node* start= nullptr;
    for(int real_id1=id1;start== nullptr&&real_id1<=id2;real_id1++)
        start=avl_tree.Search(real_id1);
    while (start!= nullptr&&start->record.id<=id2){
        sum+=start->record.value[i];
        start=start->succ;
    }
    return sum;
}

void MyDatabase::Remove(Node* data){
    if(data->pre== nullptr){
        DataList_Head=data->succ;
        if(DataList_Head== nullptr)
            DataList_Tail= nullptr;
        else
            DataList_Head->pre= nullptr;
        delete data;
    }
    else{
        data->pre->succ=data->succ;
        if(data->succ!= nullptr)
            data->succ->pre=data->pre;
        else
            DataList_Tail=data->pre;
        delete data;
    }
}

void MyDatabase::Insert(Node* data){
    data->succ= nullptr;
    if(DataList_Head== nullptr){
        DataList_Head=data;
        DataList_Tail=data;
        data->pre=nullptr;
    }
    else{
        if(data->record.id>DataList_Tail->record.id){
            data->pre=DataList_Tail;
            DataList_Tail->succ=data;
            DataList_Tail=data;
        }
        else{
            Node *p =DataList_Tail->pre;
            while(p!=nullptr&&p->record.id>data->record.id)
                p=p->pre;
            if(p== nullptr){
                data->pre= nullptr;
                data->succ=DataList_Head;
                DataList_Head->pre=data;
                DataList_Head=data;
            }
            else{
                data->pre=p;
                data->succ=p->succ;
                p->succ->pre=data;
                p->succ=data;
            }
        }
    }
}

void Union(MyDatabase *dest,MyDatabase *src,Name_Node* &names){
    _TIRE_TREE tire_tree;
    Name_Node *ret_tail= nullptr;
    for(Name_Node *p=dest->names_head;p!= nullptr;p=p->next){
        if(tire_tree.Insert(p->name)){
            Name_Node *temp=new Name_Node;
            strcpy(temp->name,p->name);
            temp->next= nullptr;
            if(names==nullptr){
                names=temp;
                ret_tail=temp;
            }
            else{
                ret_tail->next=temp;
                ret_tail=temp;
            }
        }
    }

    for(Name_Node *p=src->names_head;p!= nullptr;p=p->next){
        if(tire_tree.Insert(p->name)){
            Name_Node *temp=new Name_Node;
            strcpy(temp->name,p->name);
            temp->next= nullptr;
            if(names==nullptr){
                names=temp;
                ret_tail=temp;
            }
            else{
                ret_tail->next=temp;
                ret_tail=temp;
            }
        }
    }
}

void Inter(MyDatabase *dest,MyDatabase *src,Name_Node* &names){
    _TIRE_TREE tire_tree;
    for(Name_Node *p=src->names_head;p!= nullptr;p=p->next)
        tire_tree.Insert(p->name);

    Name_Node *ret_tail= nullptr;
    for(Name_Node *p=dest->names_head;p!= nullptr;p=p->next){
        if(tire_tree.Search(p->name)){
            Name_Node *temp=new Name_Node;
            strcpy(temp->name,p->name);
            temp->next= nullptr;
            if(names==nullptr){
                names=temp;
                ret_tail=temp;
            }
            else{
                ret_tail->next=temp;
                ret_tail=temp;
            }
        }
    }
}

