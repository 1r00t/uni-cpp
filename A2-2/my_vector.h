#pragma once
#include <iostream>
#include <array>

namespace my
{

template <typename ValueT>
class vector
{

    ValueT *_mem = nullptr;
    size_t _capacity = 0;
    size_t _size = 0;

  public:
    using value_type = ValueT;

    // normal constructor
    vector(size_t n = 0)
    {
        _mem = (ValueT *)malloc(sizeof(ValueT) * n);
        _capacity = n;
    }

    // copy constructor
    vector(const vector &rhs)
    {
        _size = rhs._size;
        _capacity = rhs._capacity;
        _mem = (ValueT *)malloc(sizeof(ValueT) * rhs._capacity);
        for (size_t i = 0; i < _size; i++)
        {
            new (_mem + i) ValueT(rhs._mem[i]);
        }
    }

    // copy asso
    vector &operator=(vector rhs)
    {
        swap(*this, rhs);
        return *this;
    }

    // destructor
    ~vector()
    {
        for (size_t i = 0; i < _size; i++)
        {
            _mem[i].~ValueT();
        }
        free(_mem);
    }

    size_t size() const { return _size; }

    size_t capacity() const { return _capacity; }

    void reserve(size_t n)
    {
        // reserviert n byte speicher
        // gibt nichts zurück, sondern verändert _mem in place falls es muss
        ValueT *new_mem = (ValueT *)malloc(n * sizeof(ValueT));

        // n < _size dann shrink und den rest wegschneiden
        size_t new_size = std::min(n, _size);

        // jedes element einzeln konstruieren ohne neue allokation
        for (size_t i = 0; i < new_size; i++)
            new (new_mem + i) ValueT(_mem[i]);

        // alte objekte einzeln zerstören
        for (size_t i = 0; i < _size; i++)
            _mem[i].~ValueT();

        // neuen speicher freigeben und _mem zeiger updaten
        free(_mem);
        _mem = new_mem;
        _capacity = n;
        _size = new_size;
    }

    bool empty() const { return size() == 0; }

    void clear()
    {
        for (size_t i = 0; i < _size; i++)
        {
            _mem[i].~ValueT();
        }
        _size = 0;
    }

    void push_back(const ValueT &v)
    {
        // wenn kein platz mehr ist, neu reservieren
        if (_size >= _capacity)
        {
            reserve(_capacity * 2 + 1);
        }

        new (_mem + _size) ValueT(v);
        _size++;
    }

    ValueT pop_back()
    {
        if (_size <= 0)
            throw new std::out_of_range("Index out of range!");
        ValueT result = _mem[_size - 1];
        _mem[_size - 1].~ValueT();
        _size--;
        return result;
    }

    void shrink_to_fit()
    {
        reserve(_size);
    }

    ValueT &operator[](size_t i)
    {
        return _mem[i];
    }

    const ValueT operator[](size_t i) const
    {
        return _mem[i];
    }

    ValueT& at(size_t i) const
    {
        if (i < 0 || i >= size())
        {
            throw std::out_of_range("Index out of range!");
        }
        return _mem[i];
    }

    friend void swap(vector &first, vector &second)
    {
        using std::swap;
        swap(first._size, second._size);
        swap(first._capacity, second._capacity);
        swap(first._mem, second._mem);
    }
};

} // namespace my