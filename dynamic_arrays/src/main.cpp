#include <iostream>
#include "darr.h"
#include <fstream>

template <class T>
std::ostream& operator<<(std::ostream& out, DArr<T>& d)
{
    for(size_t i=0; i<d.size();i++)
    {  
        out << d[i] << " ";
    }
    out << "\n";
    return out;
}

int main()
{
    std::cout << "Dynamic Array\n";
    DArr<int> d;
    d.push_back(1);
    d.push_back(2);
    d.push_back(3);
    std::cout << d[1] << "\n";
    d[1] = 5;
    std::cout << d[1] << "\n";

    std::cout << d;

    std::cout << "\n";


}