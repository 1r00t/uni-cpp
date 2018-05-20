#pragma once
#include <iostream>
#include <array>

namespace my
{

template <typename ValueT>
class vector
{
// Alle Elemente mit ValueT initialisieren, wenn kein Payload mitgegeben dann mit ValueT() initialisieren.
  private:
    ValueT *_data = nullptr;
    size_t _dimension = 0;


  public:
    using value_type = ValueT;

    vector(size_t n = 0, ValueT data = ValueT())
    {
        _dimension = n;

        _data = new ValueT[n];

        for (size_t i = 0; i < n; i++)
        {
            *(_data + i) = data;
        }
        

    }

    ~vector()
    {
        delete[] _data;
    }

    size_t size() { return _dimension; }

    bool empty() { return size() == 0; }

/*     void push_back(const ValueT& v)
    {

    }

    ValueT pop_back()
    {

    } */
  
};

} // namespace my