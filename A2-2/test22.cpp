// Testtreiber für Aufgabe 2.1: my::vector
// Autor: Hartmut Schirmacher

#include <iostream>
#include <cassert>
#include "my_vector.h"
#include "payload.h"

void test_22()
{
    using my::vector;
    using std::cout;
    using std::endl;

    {
        {
            cout << "construction/destruction, size() ... ";
            vector<Payload> v0;
            assert(v0.size() == 0);
            assert(Payload::count() == 0);

            v0.push_back(Payload(1, 2, 3));
            assert(v0.size() == 1);
            assert(Payload::count() == v0.size());
        }
        assert(Payload::count() == 0);
        cout << " done." << endl;
    }

    {
        {
            cout << "reserve ... ";
            vector<Payload> v0;
            v0.reserve(3);
            v0.push_back(Payload(1, 1, 1));
            v0.push_back(Payload(2, 2, 2));
            v0.push_back(Payload(3, 3, 3));
            assert(Payload::count() == v0.size());

            v0.push_back(Payload(4, 4, 4));
            assert(v0.size() == 4);
            assert(v0.capacity() == 7); // 7 = 3 * 2 + 1
        }
        {
            // shrink durch reserve
            vector<Payload> v1(3);
            v1.push_back(Payload(1, 1, 1));
            v1.push_back(Payload(2, 2, 2));
            v1.push_back(Payload(3, 3, 3));
            assert(v1.capacity() == 3);
            assert(v1.size() == 3);
            v1.reserve(1);
            assert(v1.size() == 1);
            assert(v1.capacity() == 1);
            assert(Payload::count() == 1);
        }
        cout << " done." << endl;
    }

    {
        cout << "pop_back ... ";
        vector<Payload> v0;
        v0.reserve(3);
        v0.push_back(Payload(1, 1, 1));
        v0.push_back(Payload(2, 2, 2));
        v0.push_back(Payload(3, 3, 3));
        Payload p = v0.pop_back();
        assert(p == Payload(3, 3, 3));
        v0.pop_back();
        assert(v0.size() + 1 == Payload::count()); // +1 => p
        cout << " done." << endl;
    }

    {
        cout << "expand ... ";
        vector<Payload> v0;
        v0.push_back(Payload(1, 1, 1));
        v0.reserve(5);
        assert(v0.capacity() == 5);
        assert(Payload::count() == v0.size());
        assert(v0[0] == Payload(1, 1, 1));
        assert(Payload::count() == 1);
        cout << " done." << endl;
    }

    {
        cout << "copy / move ... ";
        vector<Payload> v0;
        vector<Payload> v1;
        v1.push_back(Payload(1, 2, 3));
        v0 = v1; // copy
        assert(v0.size() == 1);
        assert(Payload::count() == 2);
        vector<Payload> v2(v1); // copy
        assert(Payload::count() == 3);
        vector<Payload> v3 = vector<Payload>(3); // move
        assert(Payload::count() == 3);
        assert(v3.capacity() == 3);
        cout << " done." << endl;
    }

    {
        cout << "shrink to fit ... ";
        vector<Payload> v0;
        v0.reserve(10);
        v0.push_back(Payload(1, 1, 1));
        v0.push_back(Payload(2, 2, 2));
        assert(v0.size() == 2);
        assert(v0.capacity() == 10);
        v0.shrink_to_fit();
        assert(v0.size() == 2);
        assert(v0.capacity() == 2);
        cout << " done." << endl;
    }

    {
        cout << "operator[] ... ";
        {
            vector<Payload> v;
            v.push_back(Payload(1, 2, 3));
            v.push_back(Payload(7, 8, 9));
            assert(v.size() == 2);

            // check reading
            assert(v[0] == Payload(1, 2, 3));
            assert(v[1] == Payload(7, 8, 9));

            // check writing
            v[1] = Payload(-1, -1, -1);
            assert(v[1] == Payload(-1, -1, -1));
        }
        cout << " done." << endl;
    }

    {
        cout << "push_back(), pop_back(), size(), empty() ... ";
        {
            vector<Payload> v;
            assert(v.size() == 0);
            assert(v.empty());

            v.push_back(Payload(0, 0, 0));
            v.push_back(Payload(1, 1, 1));
            v.push_back(Payload(2, 2, 2));
            assert(v.size() == 3);
            assert(!v.empty());
            assert(Payload::count() == 3);

            assert(v.pop_back() == Payload(2, 2, 2));
            assert(v.size() == 2);
            assert(Payload::count() == 2);

            assert(v.pop_back() == Payload(1, 1, 1));
            assert(v.size() == 1);
            assert(Payload::count() == 1);

            assert(v.pop_back() == Payload(0, 0, 0));
            assert(v.size() == 0);
            assert(v.empty());
            assert(Payload::count() == 0);
        }
        assert(Payload::count() == 0);
        cout << " done." << endl;
    }

    {
        cout << "at() ... ";
        {
            vector<Payload> v;
            v.push_back(Payload(1, 2, 3));
            v.push_back(Payload(7, 8, 9));
            assert(v.size() == 2);

            // check reading
            assert(v.at(0) == Payload(1, 2, 3));
            assert(v.at(1) == Payload(7, 8, 9));

            // check writing
            v.at(1) = Payload(-1, -1, -1);
            assert(v.at(1) == Payload(-1, -1, -1));

            // check exception
            try
            {
                v.at(2) = Payload(0, 0, 0);
                assert(!"should have thrown exception");
            }
            catch (std::out_of_range ex)
            {
                cout << "exception: " << ex.what();
            }
        }
        cout << " done." << endl;
    }

    {
        cout << "constness... ";
        {
            vector<Payload> v;
            v.push_back(Payload(1, 2, 3));
            v.push_back(Payload(7, 8, 9));

            // const ref (read-only ref)
            const vector<Payload> &vconst = v;

            // all this must compile without problems, else you are missing const :-)
            assert(!vconst.empty());
            assert(vconst.size() == 2);
            assert(vconst[0] == Payload(1, 2, 3));
            assert(vconst.at(0) == Payload(1, 2, 3));
        }
        cout << " done." << endl;
    }

    {
        cout << "mal ausprobieren - soll vorerst nicht (!) kompilieren ... ";
        // mal ausprobieren - soll vorerst nicht (!) kompilieren
        vector<Payload> v1;
        vector<Payload> v2 = v2;
        cout << " done." << endl;
    }
} // test22()
