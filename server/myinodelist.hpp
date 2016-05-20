#ifndef __MYINODELIST__HPP
#define __MYINODELIST__HPP
#include <iostream>
#include <string>	
#include "myinode.hpp"

class MyINodeList{
    struct Node{
        MyINode* content;
        Node* next;

        Node(MyINode* mind);
        Node();
	~Node();
    };
    Node* head;
    Node* tail;
    int items;

    public:
        MyINodeList();
        ~MyINodeList();
        bool isEmpty();
        int length();

        MyINode* operator[](unsigned int i);

        void insertEntry(MyINode* mind);
        MyINode* getTop();
	MyINode* popTop();
        void removeTopEntry();
	bool exists(std::string name);
	int getPosition(std::string name);
        void print();
	MyINode* itemAt(int i);
};

#endif
