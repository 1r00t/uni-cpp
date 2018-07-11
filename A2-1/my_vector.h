#pragma once
#include <iostream>
#include <array>

namespace my
{

template <typename ValueT>
class vector
{

  ValueT *_mem = nullptr;
  size_t _dimension = 0;

  public:
    using value_type = ValueT;

    // verbiete kopieren und zuweisen
    vector(vector<ValueT> const &) = delete;
    vector<ValueT> &operator=(vector<ValueT> const &) = delete;

    // constructor 
    vector(size_t n = 0, ValueT data = ValueT())
    {
        _dimension = n;

        if (n == 0) return; // wenn n = 0 dann nichts weiter machen
        
        _mem = new ValueT[n];

        for (size_t i = 0; i < n; i++)
        {
            _mem[i] = data;
        }
    }

    ~vector()
    {
        clear();
    }

    size_t size() const { return _dimension; }

    bool empty() const { return size() == 0; }

    void clear() { if (!empty()) delete[] _mem;}

    void push_back(const ValueT &v)
    {
        // neuen speicher+1 reservieren
        ValueT *new_mem = new ValueT[size() + 1];

        // neuen speicher mit alten werten füllen
        for (size_t i = 0; i < size(); i++)
        {
            new_mem[i] = _mem[i];
        }
        // den neuen wert ans ende setzen
        new_mem[size()] = v;

        // den alten speicher freigeben wenn denn etwas reserviert war
        delete[] _mem;
        // zeiger auf neuen speicher zeigen lassen
        _mem = new_mem;
        _dimension ++;
    }

    ValueT pop_back()
    {
        if (size() <= 1)
        {
            _dimension --;
            ValueT result = *_mem;
            delete[] _mem;
            return result;
        }
        ValueT last = _mem[size() - 1];
        ValueT *new_mem = new ValueT[size() - 1];
        for (size_t i = 0; i < size() - 1; i++)
        {
            new_mem[i] = _mem[i];
        }
        if (!empty())
            delete[] _mem;
        _mem = new_mem;
        _dimension --;
        return last;
    }

    ValueT &operator[] (size_t i)
    {
        return _mem[i];
    }

    const ValueT operator[] (size_t i) const
    {
        return _mem[i];
    }

    ValueT &at(size_t i) const
    {
        if (i < 0 || i >= size())
        {
            throw std::out_of_range("Index out of range!");
        }
        return _mem[i];
    }

};

} // namespace my