#!/bin/bash


g++ -std=c++11 MemoryPool.cpp Test_MyAllocator.cpp   -o test1 
g++ -std=c++11 MemoryPool.cpp Test_StdAllocator.cpp   -o test2


for testsize in 10000 12000 15000 18000 20000 25000
do
echo "---------------------------------------"
echo "TestSize is $testsize, PickSize is 1000"
echo "$testsize 1500" | ./test1
echo "$testsize 1500" | ./test2
done

for picksize in 1000 1200 1500 1800 2000 2500 5000 10000
do
echo "----------------------------------------"
echo "TestSize is 15000, PickSize is $picksize"
echo "15000 $picksize" | ./test1
echo "15000 $picksize" | ./test2
done

rm ./test1
rm ./test2




