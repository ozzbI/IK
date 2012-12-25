#ifndef LINK_VEC_H
#define LINK_VEC_H
#include <kchain_link.h>

class link_vec
{
public:
    Kchain_link** data;
    int size;

    link_vec()
    {
        size=0;
    }

    void push_back(Kchain_link* new_link)
    {
        size++;
        Kchain_link** new_data;
        new_data=new Kchain_link*[size];
        for(int i=0;i<(size-1);i++)
        {
            new_data[i]=data[i];
        }

        new_data[size-1]=new_link;
        delete[]data;
        data=new_data;

    }

    Kchain_link* operator[](int i){return data[i];}

};

#endif // LINK_VEC_H
