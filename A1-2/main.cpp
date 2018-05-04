#include <iostream>
#include <string>
#include "test12.h"
#include "vec.h"
#include <vector>
#include <random>

using namespace my;

int main()
{

    std::array<float, 3> test = {0.1f, 0.5f, 0.2f};
    test_Vec<float, 3>(test);

    std::cout << std::endl;

    // 1.
    std::cout << "1. length():" << std::endl;
    Vec<float, 3> teste_das({0.1f, 0.5f, 0.2f});
    std::cout << teste_das.length() << std::endl;
    std::cout << std::endl;

    // 2.
    std::cout << "2. template print_helper function:" << std::endl;
    std::vector<Vec<float, 3>> a;
    a.push_back(Vec<float, 3>({0.1f, 0.2f, 0.3f}));
    a.push_back(Vec<float, 3>({0.4f, 0.5f, 0.6f}));
    a.push_back(Vec<float, 3>({0.7f, 0.8f, 0.9f}));
    print_helper(a);

    // 3.
    std::cout << "3. random:" << std::endl;
    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<float> dist(0, 100);
    std::vector<Vec<float, 3>> v;
    for (int i = 0; i < 10; i++)
    {
        v.push_back(Vec<float, 3>({dist(e2), dist(e2), dist(e2)}));
    }
    print_helper(v);

    // 4.
    std::cout << "4. range based for:" << std::endl;
    for (auto &ve : v)
    {
        for (int i = 0; i < 3; i++)
            ve[i] += 1.0f;
    }
    print_helper(v);

    // 5.
    // warum funktioniert das ? ich übergebe der lambda funk einen const
    std::cout << "5. transform:" << std::endl;
    std::transform(v.begin(), v.end(), v.begin(), [](const Vec<float, 3> x) {
        return x + Vec<float, 3>({1.f, 1.f, 1.f});
    });
    print_helper(v);

    // 6.
    std::cout << "6. stable_partition:" << std::endl;
    std::stable_partition(v.begin(), v.end(), [](Vec<float, 3> x) {
        return x.length() < 90;
    });
    print_helper(v);

    // 7.
    std::cout << "7. sort:" << std::endl;
    std::sort(v.begin(), v.end(), [](Vec<float, 3> x1, Vec<float, 3> x2) {
        return x1.length() > x2.length();
    });
    print_helper(v);

    // 8.
    std::cout << "8. Median:" << std::endl;
    std::cout << v[v.size() / 2] << v[v.size() / 2].length() << std::endl;
    std::cout << std::endl;

    // 9.
    std::cout << "9. copy_if < 80:" << std::endl;
    std::vector<Vec<float, 3>> v2;
    std::copy_if(v.begin(), v.end(), std::back_inserter(v2), [](Vec<float, 3> x) {
        return x.length() < 80;
    });
    print_helper(v2);

    // 10.
    std::cout << "10. remove:" << std::endl;
    v.erase(std::remove_if(v.begin(), v.end(), [](Vec<float, 3> x) { return x.length() > 90 && x.length() < 120; }),
            v.end());
    print_helper(v);

    system("pause");
    return 0;
}
