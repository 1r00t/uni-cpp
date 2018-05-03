#include <iostream>
#include <cassert>
#include <typeinfo>
using namespace std;

#include "vec.h"
using namespace my;

template <typename T, int N>
void test_Vec(std::array<T, N> teste_das)
{

    //using Vec = Vec<T, N>;

#ifndef NDEBUG
    cout << "======================" << endl;
    cout << "     Testing Vec    " << endl;
    cout << "======================" << endl;

    {
        cout << "  value type and dimension: ";
        assert(typeid(Vec<T, N>::value_type).name() == typeid(T).name());
        assert((Vec<T, N>::dimension == N));
        cout << "passed." << endl;
    }

    {
        // do not tolerate any memory overhead
        cout << "  sizeof(Vec) == 3 bytes: ";
        assert(sizeof(Vec<T, N>) == N * sizeof(T));
        cout << "passed." << endl;
    }

    {
        cout << "  constructor & index operator: ";
        auto a = Vec<T, N>();
        for (int i = 0; i < a.dimension; i++) assert(a[i] == T());

        Vec<T, N> b({1, 2, 3});
        assert(b[0] == 1 && b[1] == 2 && b[2] == 3);

        cout << "passed." << endl;
    }

    {
        cout << "  read-only access to const object: ";
        const Vec<T, N> a(teste_das);
        // the next line will throw a compiler error if there is no proper "operator[] const"
        assert(a[N - 1] == teste_das[N - 1]);
        cout << "passed." << endl;
    }

    {
        cout << "  write access to a non-const object: ";
        Vec<T, N> a(teste_das);
        a[N - 1] = T();
        for(int i = 0; i < N - 1; i++) assert(a[i] == teste_das[i]);
        assert(a[N - 1] == T());
        cout << "passed." << endl;
    }

    {
        cout << "  comparison: ";

        auto cc = teste_das;
        cc[0] = T();
        auto dd = teste_das;
        dd[N - 1] = T();

        Vec<T, N> a(teste_das), b(teste_das), c(cc), d(dd);
        assert(a == b);
        assert(a == a);
        assert(a != c);
        assert(b != d);
        assert(!(a != b));
        cout << "passed." << endl;
    }

    {
        // should work out of the box when using std container (!)
        cout << "  assignment: ";
        Vec<T, N> a;
        for (int i = 0; i < N; i++) a[i] = teste_das[i] * 2;
        cout << a[0] << " "; // to make sure these values are not optimized away!
        a = Vec<T, N>(teste_das);
        for (int i = 0; i < N; i++) assert(a[i] == teste_das[i]);
        cout << "passed." << endl;
    }

#if 0
    {
        cout << "  addition: ";
        Vec a({1, 2, 3}), b({4, 5, 6});
        a += b;
        assert(a == Vec({5, 7, 9}));
        auto c = a + Vec({1, 1, 1});
        assert(c == Vec({6, 8, 10}));
        Vec one({1, 1, 1}), four({4, 4, 4});
        one += one + one + one;
        assert(one == four);
        Vec x({0, 0, 0}), y({1, 1, 1}), z({2, 2, 2});
        x += y += z;
        assert(y == Vec({3, 3, 3}));
        assert(x == y);
        cout << "passed." << endl;
    }

    {
        cout << "  unary minus: ";
        Vec a({1, 2, 3});
        assert(-a == Vec({-1, -2, -3}));
        cout << "passed." << endl;
    }

    {
        cout << "  dot product: ";
        Vec a({1, 2, 3});
        assert(dot(a, a) == 1 * 1 + 2 * 2 + 3 * 3);
        cout << "passed." << endl;
    }

    {
        // these tests will not compile if you forgot to declare
        // some methods const
        cout << "  constness: ";
        const Vec a({1, 2, 3});
        assert(a[1] == 2);
        assert(a == a);
        assert(!(a != a));
        assert(a + a == Vec({2, 4, 6}));
        assert(-a == Vec({-1, -2, -3}));
        assert(dot(a, a) == 1 * 1 + 2 * 2 + 3 * 3);
        cout << "passed." << endl;
    }

    cout << "all Vec tests passed." << endl
         << endl;
#endif
#endif // not NDEBUG
}
