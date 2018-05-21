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
    ValueT *_mem = nullptr;
    size_t _dimension = 0;


  public:
    using value_type = ValueT;

    vector(size_t n = 0, ValueT data = ValueT())
    {
        _dimension = n;
        _mem = new ValueT[n];

        for (size_t i = 0; i < n; i++)
        {
            *(_mem + i) = data;
        }
    }

    ~vector()
    {
        if (!empty()) delete[] _mem;
    }

    size_t size() const { return _dimension; }

    bool empty() const { return size() == 0; }

    void push_back(const ValueT &v)
    {
        ValueT *new_mem = new ValueT[size() + 1];
        for (size_t i = 0; i < size(); i++)
        {
            *(new_mem + i) = _mem[i];
        }
        *(new_mem + size()) = v;
        if (!empty()) delete[] _mem;
        _mem = new_mem;
        _dimension += 1;
    }

    ValueT pop_back()
    {
        if (size() <= 1)
        {
            _dimension -= 1;
            ValueT result = *_mem;
            delete[] _mem;
            return result;
        }
        ValueT last = _mem[size() - 1];
        ValueT *new_mem = new ValueT[size() - 1];
        for (size_t i = 0; i < size() - 1; i++)
        {
            *(new_mem + i) = _mem[i];
        }
        if (!empty())
            delete[] _mem;
        _mem = new_mem;
        _dimension -= 1;
        return last;
    }

    ValueT &operator[] (size_t i)
    {
        return *(_mem + i);
    }

    const ValueT operator[] (size_t i) const
    {
        return *(_mem + i);
    }

    ValueT &at(size_t i) const
    {
        if (i < 0 || i >= size())
        {
            throw std::out_of_range("index nicht gefunden!");
        }
        return *(_mem + i);
    }
  
};

} // namespace my