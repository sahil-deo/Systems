#include <iostream>
#include "darr.h"

int main()
{
    DArr<int> d(7);
    d.push_back(1);
    d.push_back(999);
    d.push_back(333);
    for(int i=0;i<d.size();i++)
    {
        std::cout << d.get(i) << "\n";
    }
}