#include "myinodelist.hpp"
//simple list functions, nothing of note
MyINodeList::Node::Node(): content(NULL), next(NULL) {}

MyINodeList::Node::Node(MyINode* mind): content(mind), next(NULL)
{}

MyINodeList::Node::~Node()
{
    if(content != NULL)
        delete content;
}

MyINodeList::MyINodeList():head(NULL), tail(NULL), items(0) {}

MyINodeList::~MyINodeList()
{
    Node* temp;
    while(head != NULL)
    {
        temp = head;
        head = head->next;
        delete temp;
    }
}

bool MyINodeList::isEmpty()
{
    if(head == NULL)
        return true;
    else
        return false;
}

void MyINodeList::insertEntry(MyINode* mind)
{
    Node* n = new Node(mind);
    if(isEmpty())
    {
        head = n;
        tail = n;
    }
    else
    {
        tail->next = n;
        tail = n;
    }
    items++;
}

void MyINodeList::print()
{
    Node* current = head;
    while(current != NULL)
    {
        current->content->print();
        current=current->next;
    }
}

MyINode* MyINodeList::popTop()
{
    MyINode* temp = head->content;
    Node* toBeDeleted = head;
    head = head->next;
    items--;
    toBeDeleted->content = NULL;
    delete toBeDeleted;
    return temp;
}

MyINode* MyINodeList::getTop()
{
    return head->content;
}

void MyINodeList::removeTopEntry()
{
    Node* temp = head;
    head = head->next;
    items--;
    delete temp;
}

int MyINodeList::length()
{
    return items;
}

bool MyINodeList::exists(std::string name)
{
    Node* n = head;
    while(n != NULL)
    {
        if(name == n->content->getName())
            return true;
        n = n->next;
    }
    return false;
}

int MyINodeList::getPosition(std::string name)
{
    Node* n = head;
    int i = 0;
    while(n != NULL)
    {
        if(name == n->content->getName())
            return i;
        n = n->next;
        i++;
    }
    return -1;
}

MyINode* MyINodeList::itemAt(int i)
{
    if(i <= items)
    {
        Node* n = head;
        for(int j = 0; j < i; j++)
            n = n->next;
        return n->content;
    }
    else
    {
        return NULL;
    }
}
