#include <iostream>
#include <cassert>
#include <typeinfo>
using namespace std;

#include "Vec3f.h"
using namespace my;

void test_Vec3f() {

#ifndef NDEBUG
    cout << "======================" << endl;
    cout << "     Testing Vec3f    " << endl;
    cout << "======================" << endl;

    {
        cout << "  value type and dimension: ";
        assert( typeid(Vec3f::value_type).name() == typeid(float).name() );
        assert( Vec3f::dimension == 3 );
        cout << "passed." << endl;
    }

    {
        // do not tolerate any memory overhead
        cout << "  sizeof(Vec3f) == 3 bytes: ";
        assert( sizeof(Vec3f) == 3*sizeof(float) );
        cout << "passed." << endl;
    }

    {
        cout << "  constructor & index operator: ";
        Vec3f a;
        assert( a[0] == 0 && a[1] == 0 && a[2] == 0 );
        Vec3f b(1, 2, 3);
        assert( b[0] == 1 && b[1] == 2 && b[2] == 3 );
        cout << "passed." << endl;
    }

    {
        cout << "  read-only access to const object: ";
        const Vec3f a(1,2,3);
        // the next line will throw a compiler error if there is no proper "operator[] const"
        assert( a[1] == 2 );
        cout << "passed." << endl;
    }

    {
        cout << "  write access to a non-const object: ";
        Vec3f a(1,2,3);
        a[1] = 4;
        assert( a[0] == 1 && a[1] == 4 && a[2] == 3 );
        cout << "passed." << endl;
    }

    {
        cout << "  comparison: ";        
        Vec3f a(1,2,3), b(1,2,3), c(1,2,9), d(4,5,6);
        assert( a == b );
        assert( a == a );
        assert( a != c );
        assert( b != d );
        assert( !(a != b) );
        cout << "passed." << endl;
    }

    {
        // should work out of the box when using std container (!)
        cout << "  assignment: "; 
        Vec3f a(1,2,3);
        cout << a[0] << " "; // to make sure these values are not optimized away! 
        a = Vec3f(4,5,6);
        assert( a[0] == 4 && a[1] == 5 && a[2] == 6 );
        cout << "passed." << endl;
    }
    
    {
        cout << "  addition: ";
        Vec3f a(1,2,3), b(4,5,6);
        a += b;
        assert( a == Vec3f(5,7,9) );
        auto c = a + Vec3f(1,1,1);
        assert( c == Vec3f(6,8,10) );
        Vec3f one(1,1,1), four(4,4,4);
        one += one+one+one;
        assert( one == four);
        Vec3f x(0,0,0), y(1,1,1), z(2,2,2);
        x += y += z;
        assert( y == Vec3f(3,3,3) );
        assert( x == y );
        cout << "passed." << endl;
    }

    {
        cout << "  unary minus: ";
        Vec3f a(1,2,3);
        assert( -a == Vec3f(-1,-2,-3) );
        cout << "passed." << endl;
    }

    {
        cout << "  dot product: ";
        Vec3f a(1,2,3);
        assert( dot(a,a) == 1*1+2*2+3*3 );
        cout << "passed." << endl;
    }

    {
        // these tests will not compile if you forgot to declare 
        // some methods const
        cout << "  constness: ";
        const Vec3f a(1,2,3);
        assert( a[1] == 2 );
        assert( a == a );
        assert( ! (a != a) );
        assert( a+a == Vec3f(2,4,6) );
        assert( -a == Vec3f(-1,-2,-3) );
        assert( dot(a,a) == 1*1+2*2+3*3 );
        cout << "passed." << endl;
    }

    cout << "all Vec3f tests passed." << endl << endl;


#endif // not NDEBUG

}
