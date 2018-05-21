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
    ValueT *mem = nullptr;
    size_t _dimension = 0;


  public:
    using value_type = ValueT;

    vector(size_t n = 0, ValueT data = ValueT())
    {
        _dimension = n;

        mem = new ValueT[n];

        for (size_t i = 0; i < n; i++)
        {
            *(mem + i) = data;
        }
        

    }

    ~vector()
    {
        if (!empty()) delete[] mem;
    }

    size_t size() const { return _dimension; }

    bool empty() const { return size() == 0; }

    void push_back(const ValueT &v)
    {
        ValueT *new_mem = new ValueT[size() + 1];
        for (size_t i = 0; i < size(); i++)
        {
            *(new_mem + i) = mem[i];
        }
        *(new_mem + size()) = v;
        if (!empty()) delete[] mem;
        mem = new_mem;
        _dimension += 1;
    }

    ValueT pop_back()
    {
        if (size() <= 1)
        {
            _dimension -= 1;
            ValueT result = *mem;
            delete[] mem;
            return result;
        }

        ValueT last = mem[size() - 1];

        ValueT *new_mem = new ValueT[size() - 1];
        for (size_t i = 0; i < size() - 1; i++)
        {
            *(new_mem + i) = mem[i];
        }
        if (!empty())
            delete[] mem;
        mem = new_mem;
        _dimension -= 1;
        return last;
    }
  
};

} // namespace my