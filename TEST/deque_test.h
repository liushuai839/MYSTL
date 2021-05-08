#ifndef MYTINYSTL_DEQUE_TEST_H_
#define MYTINYSTL_DEQUE_TEST_H_

#include "test.h"
#include "../MYSTL/deque.h"
// #include "head_file.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <deque>

string time_push_back(int times, mystl::deque<int> dq){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        dq.push_back(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
string time_push_back(int times, std::deque<int> dq){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        dq.push_back(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}


namespace mystl{

void deque_test(){

    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[----------------- Run container test : deque ------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 1,2,3,4,5 };
    mystl::deque<int> d1(5,1);
    mystl::deque<int> d2(5);
    mystl::deque<int> d3(5, 1);
    mystl::deque<int> d4(a, a + 5);
    mystl::deque<int> d5(d2);
    mystl::deque<int> d6(std::move(d2));
    mystl::deque<int> d7;
    d7 = d3;
    mystl::deque<int> d8;
    d8 = std::move(d3);
    mystl::deque<int> d9{ 1,2,3,4,5,6,7,8,9 };
    mystl::deque<int> d10;
    d10 = { 1,2,3,4,5,6,7,8,9 };
    PASSED; 
    //! assign 函数里面的 ‘+’ 操作符重载可能有些问题
    // FUN_AFTER(d1, d1.assign(5, 1));
    // FUN_AFTER(d1, d1.assign(8, 8));
    // FUN_AFTER(d1, d1.assign(a, a + 5));
    //   FUN_AFTER(d1, d1.assign({ 1,2,3,4,5 }));
    FUN_AFTER(d1, d1.insert(d1.end(), 6));
    //   FUN_AFTER(d1, d1.insert(d1.end() - 1, 2, 7));
    FUN_AFTER(d1, d1.insert(d1.begin(), a, a + 5));
    //   FUN_AFTER(d1, d1.erase(d1.begin()));
    //   FUN_AFTER(d1, d1.erase(d1.begin(), d1.begin() + 4));
    FUN_AFTER(d1, d1.emplace_back(8));
    FUN_AFTER(d1, d1.emplace_front(8));
    //   FUN_AFTER(d1, d1.emplace(d1.begin() + 1, 9));
    FUN_AFTER(d1, d1.push_front(1));
    FUN_AFTER(d1, d1.push_back(2));
    FUN_AFTER(d1, d1.pop_back());
    FUN_AFTER(d1, d1.pop_front());
    FUN_AFTER(d1, d1.shrink_to_fit());
    //   FUN_AFTER(d1, d1.resize(5));
    //   FUN_AFTER(d1, d1.resize(8, 8));
    FUN_AFTER(d1, d1.clear());
    FUN_AFTER(d1, d1.shrink_to_fit());
    FUN_AFTER(d1, d1.swap(d4));
    FUN_VALUE(*(d1.begin()));
    FUN_VALUE(*(d1.end() - 1));
    FUN_VALUE(*(d1.rbegin()));
    FUN_VALUE(*(d1.rend() - 1));
    FUN_VALUE(d1.front());
    FUN_VALUE(d1.back());
    FUN_VALUE(d1.at(1));
    FUN_VALUE(d1[2]);
    std::cout << std::boolalpha;
    FUN_VALUE(d1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(d1.size());
    FUN_VALUE(d1.max_size());
    PASSED;



    mystl::deque<int> my_dq;
    std::deque<int> std_dq;
    
    int times1 = 100000;
    int times2 = 1000000;
    int times3 = 10000000;
    string mystl_times1 = time_push_back(times1, my_dq);
    string mystl_times2 = time_push_back(times2, my_dq);
    string mystl_times3 = time_push_back(times3, my_dq);
    string stl_times1 = time_push_back(times1, std_dq);
    string stl_times2 = time_push_back(times2, std_dq);
    string stl_times3 = time_push_back(times3, std_dq);
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