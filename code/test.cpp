#include <iostream>
#include <random>
#include <vector>
#include <string>
#include "Allocator.hpp"
using namespace std;

template<class T>
using myAllocator = MyAllocator<T>;
using Point2D = std::pair<int, int>;

const int TestSize = 10000;
const int PickSize = 1000;

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, TestSize);
    clock_t start, end;

    // use the default allocator
    start = clock();
    // vector creation
    // create vector of int
    using IntVec = std::vector<int, std::allocator<int>>;
    std::vector<IntVec, std::allocator<IntVec>> vecints1(TestSize);
    for (int i = 0; i < TestSize; i++)
       vecints1[i].resize(dis(gen));
    // create vector of Point2D
    using PointVec = std::vector<Point2D, std::allocator<Point2D>>;
    std::vector<PointVec, std::allocator<PointVec>> vecpts1(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecpts1[i].resize(dis(gen));
    // vector resize
    for (int i = 0; i < PickSize; i++)
    {
        int idx = dis(gen) - 1;
        int size = dis(gen);
        vecints1[idx].resize(size);
        vecpts1[idx].resize(size);
    }
    // vector element assignment
    {
       int val = 10;
       int idx1 = dis(gen) - 1;
       int idx2 = vecints1[idx1].size() / 2;
       vecints1[idx1][idx2] = val;
       if (vecints1[idx1][idx2] == val)
           std::cout << "correct assignment in vecints: " << idx1 << std::endl;
       else
           std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
    }
    {
        Point2D val(11, 15);
        int idx1 = dis(gen) - 1;
        int idx2 = vecpts1[idx1].size() / 2;
        vecpts1[idx1][idx2] = val;
        if (vecpts1[idx1][idx2] == val)
            std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
        else
            std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
    }

    // print the total time using default allocator
    end = clock();
    cout << "The total time using the default allocator: " << ((double)end - start) / CLOCKS_PER_SEC << "s.\n\n";


    // use my memory pool allocator
    start = clock();
    // vector creation
    // create vector of int
    std::vector<IntVec, myAllocator<IntVec>> vecints2(TestSize);
    for (int i = 0; i < TestSize; i++)
       vecints2[i].resize(dis(gen));
    // create vector of Point2D
    std::vector<PointVec, myAllocator<PointVec>> vecpts2(TestSize);
    for (int i = 0; i < TestSize; i++)
        vecpts2[i].resize(dis(gen));

    // vector resize
    for (int i = 0; i < PickSize; i++)
    {
        int idx = dis(gen) - 1;
        int size = dis(gen);
        vecints2[idx].resize(size);
        vecpts2[idx].resize(size);
    }

    // vector element assignment
    {
       int val = 10;
       int idx1 = dis(gen) - 1;
       int idx2 = vecints2[idx1].size() / 2;
       vecints2[idx1][idx2] = val;
       if (vecints2[idx1][idx2] == val)
           std::cout << "correct assignment in vecints: " << idx1 << std::endl;
       else
           std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
    }
    {
        Point2D val(11, 15);
        int idx1 = dis(gen) - 1;
        int idx2 = vecpts2[idx1].size() / 2;
        vecpts2[idx1][idx2] = val;
        if (vecpts2[idx1][idx2] == val)
            std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
        else
            std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
    }

    // print the total time using my memory pool allocator
    end = clock();
    cout << "The total time using my memory pool allocator: " << ((double)end - start) / CLOCKS_PER_SEC << "s.\n\n";

    return 0;
}
