#include <iostream>
#include "x.h"
#include "y.h"

int main() {
    X x;
    Y y;
    std::cout << x.info() << std::endl;
    std::cout << y.info() << std::endl;
    return 0;
}