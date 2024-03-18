#include <iostream>
#include <random>
#include <vector>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <new>
#include <ctime>
#include "Allocator.hpp"
#include "MemoryPool.h"

template <class T>
using Allocator = MyAllocator<T>;
using Point2D = std::pair<int, int>;

int TestSize = 10000;
int PickSize = 1000;
int main()
{
  std::cin >> TestSize;
  std::cin >> PickSize;
  clock_t start = clock();
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(1, TestSize);

  // vector creation
  using IntVec = std::vector<int, Allocator<int>>;
  std::vector<IntVec, Allocator<IntVec>> vecints(TestSize);
  for (int i = 0; i < TestSize; i++)
    vecints[i].resize(dis(gen));

  using PointVec = std::vector<Point2D, Allocator<Point2D>>;
  std::vector<PointVec, Allocator<PointVec>> vecpts(TestSize);
  for (int i = 0; i < TestSize; i++)
    vecpts[i].resize(dis(gen));

  // vector resize
  for (int i = 0; i < PickSize; i++)
  {
    int idx = dis(gen) - 1;
    int size = dis(gen);
    vecints[idx].resize(size);
    vecpts[idx].resize(size);
  }

  // vector element assignment
  {
    int val = 10;
    int idx1 = dis(gen) - 1;
    int idx2 = vecints[idx1].size() / 2;
    vecints[idx1][idx2] = val;
    if (vecints[idx1][idx2] == val)
      std::cout << "correct assignment in vecints: " << idx1 << std::endl;
    else
      std::cout << "incorrect assignment in vecints: " << idx1 << std::endl;
  }
  {
    Point2D val(11, 15);
    int idx1 = dis(gen) - 1;
    int idx2 = vecpts[idx1].size() / 2;
    vecpts[idx1][idx2] = val;
    if (vecpts[idx1][idx2] == val)
      std::cout << "correct assignment in vecpts: " << idx1 << std::endl;
    else
      std::cout << "incorrect assignment in vecpts: " << idx1 << std::endl;
  }
  std::cout << "The total time using my memory pool allocator: "
            << (clock() - start) * 1.0 / CLOCKS_PER_SEC << "s."
            << std::endl
            << std::endl;

  return 0;
}