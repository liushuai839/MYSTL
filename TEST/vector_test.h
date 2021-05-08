#ifndef MYTINYSTL_VECTOR_TEST_H_
#define MYTINYSTL_VECTOR_TEST_H_

// vector test : 测试 vector 的接口与 push_back 的性能

#include <vector>
#include <iostream>
#include <chrono>
#include <iomanip>

#include "../MYSTL/vector.h"
#include "time.h"
#include "test.h"
using namespace std;

string time_push_back(int times, mystl::vector<int> vec){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        vec.push_back(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
string time_push_back(int times, std::vector<int> vec){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        vec.push_back(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
namespace mystl{

void vector_test(){
    std::cout << "[===============================================================]\n";
    std::cout << "[----------------- Run container test : vector -----------------]\n";
    std::cout << "[-------------------------- API test ---------------------------]\n";
    int a[] = { 1,2,3,4,5 };
    mystl::vector<int> v1;
    mystl::vector<int> v2(10);
    mystl::vector<int> v3(10, 1);
    mystl::vector<int> v4(a, a + 5);
    mystl::vector<int> v5(v2);
    mystl::vector<int> v6(std::move(v2));
    mystl::vector<int> v7{ 1,2,3,4,5,6,7,8,9 };
    mystl::vector<int> v8, v9, v10;
    v9 = std::move(v3);
    v10 = { 1,2,3,4,5,6,7,8,9 };

    FUN_AFTER(v1, v1.assign(8, 8));
    FUN_AFTER(v1, v1.emplace(v1.begin(), 0));
    FUN_AFTER(v1, v1.emplace_back(6));
    FUN_AFTER(v1, v1.push_back(6));
    FUN_AFTER(v1, v1.insert(v1.end(), 7));
    FUN_AFTER(v1, v1.insert(v1.begin(), a, a + 5));
    FUN_AFTER(v1, v1.pop_back());
    FUN_AFTER(v1, v1.erase(v1.begin()));
    FUN_AFTER(v1, v1.erase(v1.begin(), v1.begin() + 2));
    FUN_AFTER(v1, v1.swap(v4));
    FUN_VALUE(*v1.begin());
    FUN_VALUE(*(v1.end() - 1));
    FUN_VALUE(v1.front());
    FUN_VALUE(v1.back());
    FUN_VALUE(v1[0]);
    FUN_VALUE(v1.at(1));
    int* p = v1.data();
    *p = 10;
    *++p = 20;
    p[1] = 30;
    std::cout << " After change v1.data() :" << "\n";
    COUT(v1);
    std::cout << std::boolalpha;
    FUN_VALUE(v1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.max_size());
    FUN_VALUE(v1.capacity());
    FUN_AFTER(v1, v1.resize(10));
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_AFTER(v1, v1.resize(6, 6));
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_AFTER(v1, v1.clear());
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    FUN_VALUE(v1.size());
    FUN_VALUE(v1.capacity());
    PASSED;
    std::vector<int> v11;
    int times1 = 100000;
    int times2 = 1000000;
    int times3 = 10000000;
    string mystl_times1 = time_push_back(times1, v8);
    string mystl_times2 = time_push_back(times2, v8);
    string mystl_times3 = time_push_back(times3, v8);
    string stl_times1 = time_push_back(times1, v11);
    string stl_times2 = time_push_back(times2, v11);
    string stl_times3 = time_push_back(times3, v11);
    std::cout << "[--------------------- Performance Testing ---------------------]\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    std::cout << "|      push_back      |    100000   |   1000000   |   10000000  |\n";
    std::cout << "|        mystl        |  "<<mystl_times1 + " | " << mystl_times2 + " |" + mystl_times3 + " |\n";
    std::cout << "|        std          |  "<<stl_times1 + " | " << stl_times2 + " |" + stl_times3 + " |\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    PASSED;




}







}



#endif
