#pragma once
#include <array>
#include <iostream>
#include <vector>

namespace my
{
template <typename T, int N>
class Vec
{
  public:
    static constexpr int dimension = N;
    using value_type = T;

    Vec(std::array<T, N> const &v) : a_(v) {}

    //Vec() : a_(std::array<float, 3>{0.0, 0.0, 0.0}) {}
    Vec() : a_(std::array<T, N>()) {} // default constructor

    T &operator[](int i) // to write
    {
        return a_[i];
    }

    const T operator[](int i) const // to read
    {
        return a_[i];
    }

    bool operator==(const Vec<T, N> &rhs) const
    {
        for (int i = 0; i < N; i++)
        {
            if (a_[i] != rhs[i])
                return false;
        }
        return true;
    }

    bool operator!=(const Vec<T, N> &rhs) const
    {
        return !(*this == rhs);
    }

    Vec<T, N> &operator+=(const Vec<T, N> &rhs)
    {
        for (int i = 0; i < N; i++)
        {
            a_[i] += rhs[i];
        }
        return *this;
    }

    Vec<T, N> operator+(const Vec<T, N> &rhs) const
    {
        Vec<T, N> temp(*this);
        temp += rhs;
        return temp;
    }

    Vec<T, N> operator-() const
    {
        Vec<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = -a_[i];
        }
        return result;
    }

    T length()
    {
        T result = T();
        for (auto e : a_)
        {
            result += e * e;
        }
        return std::sqrt(result);
    }

    template <typename T, int N>
    friend std::ostream &operator<<(std::ostream &os, const Vec<T, N> &vec);

  private:
    std::array<T, N> a_;
};

template <typename T, int N>
std::ostream &operator<<(std::ostream &os, const Vec<T, N> &vec)
{
    for (int i = 0; i < N; i++)
    {
        os << vec.a_[i] << ", ";
    }
    return os;
}

template <typename T, int N>
T dot(const Vec<T, N> &lhs, const Vec<T, N> &rhs)
{
    T result = 0;
    for (int i = 0; i < N; i++)
    {
        result += lhs[i] * rhs[i];
    }
    return result;
}

template <typename T, int N>
void print_helper(const std::vector<Vec<T, N>> container)
{
    for (auto elem : container)
    {
        std::cout << elem << " " << elem.length() << std::endl;
    }
    std::cout << std::endl;
}
}