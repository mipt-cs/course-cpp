#ifndef __linked_list_hpp
#define __linked_list_hpp

#include <ostream>

#define nullptr     NULL


template<typename DataType>
struct tNode {
    DataType data;
    tNode* next;
};


template<typename DataType>
struct List {
    tNode<DataType> *begin;
    int size;
};

template<typename DataType>
void list_init(List<DataType> &lst)
{
    lst.begin = nullptr;
    lst.size = 0;
}

template<typename DataType>
tNode<DataType> * insert_node(tNode<DataType> *p_begin, DataType data)
{
    tNode<DataType> *p = new tNode<DataType>;
    p->data = data;
    p->next = p_begin;
    return p;
}

template<typename DataType>
void list_insert(List<DataType> &lst, const DataType &value)
{
    lst.size += 1;
    lst.begin = insert_node(lst.begin, value);
}

template<typename DataType>
void list_print(const List<DataType> &lst, std::ostream &out)
{
    tNode<DataType> *p = lst.begin;
    for(; p; p = p->next)
    {
        out << p->data << '\t';
    }
}

template<typename DataType>
void list_destroy(List<DataType> &lst)
{
    tNode<DataType> *p = lst.begin;
    tNode<DataType> *prev;
    while (p) {
        prev = p;
        p = p->next;
        delete prev;
    }
    lst.size = 0;
    lst.begin = nullptr;
}


#endif
