#ifndef MYSTL_UNORDERED_SET_TEST_H_
#define MYSTL_UNORDERED_SET_TEST_H_

#include "../MYSTL/unordered_set.h"
#include "test.h"
#include <unordered_set>

namespace mystl
{


string time_push_back(int times, mystl::unordered_set<int> mystl_set){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        mystl_set.emplace(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
string time_push_back(int times, std::unordered_set<int> std_set){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        std_set.emplace(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}

string time_push_back(int times, mystl::unordered_multiset<int> mystl_set){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        mystl_set.emplace(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
string time_push_back(int times, std::unordered_multiset<int> std_set){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        std_set.emplace(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}


void unordered_set_test()
{
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[-------------- Run container test : unordered_set -------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 5,4,3,2,1 };
    mystl::unordered_set<int> us1;
    mystl::unordered_set<int> us2(520);
    mystl::unordered_set<int> us3(520, mystl::hash<int>());
    mystl::unordered_set<int> us4(520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_set<int> us5(a, a + 5);
    mystl::unordered_set<int> us6(a, a + 5, 100);
    mystl::unordered_set<int> us7(a, a + 5, 100, mystl::hash<int>());
    mystl::unordered_set<int> us8(a, a + 5, 100, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_set<int> us9(us5);
    mystl::unordered_set<int> us10(std::move(us5));
    mystl::unordered_set<int> us11;
    us11 = us6;
    mystl::unordered_set<int> us12;
    us12 = std::move(us6);
    mystl::unordered_set<int> us13{ 1,2,3,4,5 };
    mystl::unordered_set<int> us14;
    us13 = { 1,2,3,4,5 };

    FUN_AFTER(us1, us1.emplace(1));
    FUN_AFTER(us1, us1.emplace_hint(us1.end(), 2));
    FUN_AFTER(us1, us1.insert(5));
    FUN_AFTER(us1, us1.insert(us1.begin(), 5));
    FUN_AFTER(us1, us1.insert(a, a + 5));
    FUN_AFTER(us1, us1.erase(us1.begin()));
    FUN_AFTER(us1, us1.erase(us1.begin(), us1.find(3)));
    FUN_AFTER(us1, us1.erase(1));
    std::cout << std::boolalpha;
    FUN_VALUE(us1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(us1.size());
    FUN_VALUE(us1.bucket_count());
    FUN_VALUE(us1.max_bucket_count());
    FUN_VALUE(us1.bucket(1));
    FUN_VALUE(us1.bucket_size(us1.bucket(5)));
    FUN_AFTER(us1, us1.clear());
    FUN_AFTER(us1, us1.swap(us7));
    FUN_VALUE(*us1.begin());
    std::cout << std::boolalpha;
    FUN_VALUE(us1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(us1.size());
    FUN_VALUE(us1.max_size());
    FUN_VALUE(us1.bucket_count());
    FUN_AFTER(us1, us1.reserve(1000));
    FUN_VALUE(*us1.begin(us1.bucket(1)));
    FUN_VALUE(us1.size());
    FUN_VALUE(us1.bucket_count());
    FUN_VALUE(us1.bucket_size(1));
    FUN_VALUE(us1.bucket_size(2));
    FUN_VALUE(us1.bucket_size(3));
    FUN_AFTER(us1, us1.rehash(150));
    FUN_VALUE(us1.bucket_count());
    FUN_VALUE(us1.count(1));
    FUN_VALUE(*us1.find(3));
    auto first = *us1.equal_range(3).first;
    auto second = *us1.equal_range(3).second;
    std::cout << " us1.equal_range(3) : from " << first << " to " << second << std::endl;
    FUN_VALUE(us1.load_factor());
    FUN_VALUE(us1.max_load_factor());
    FUN_AFTER(us1, us1.max_load_factor(1.5f));
    FUN_VALUE(us1.max_load_factor());
    PASSED;
    
    mystl::unordered_set<int> my_set;
    std::unordered_set<int> std_set;
    int times1 = 100000;
    int times2 = 1000000;
    int times3 = 10000000;
    string mystl_times1 = time_push_back(times1, my_set);
    string mystl_times2 = time_push_back(times2, my_set);
    string mystl_times3 = time_push_back(times3, my_set);
    string stl_times1 = time_push_back(times1, std_set);
    string stl_times2 = time_push_back(times2, std_set);
    string stl_times3 = time_push_back(times3, std_set);
    std::cout << "[--------------------- Performance Testing ---------------------]\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    std::cout << "|      push_back      |    100000   |   1000000   |   10000000  |\n";
    std::cout << "|        mystl        | "<<mystl_times1 + " | " << mystl_times2 + "|" + mystl_times3 + " |\n";
    std::cout << "|        std          | "<<stl_times1 + " |" << stl_times2 + "|" + stl_times3 + " |\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    PASSED;
}

void unordered_multiset_test()
{
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[------------ Run container test : unordered_multiset ----------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 5,4,3,2,1 };
    mystl::unordered_multiset<int> us1;
    mystl::unordered_multiset<int> us2(520);
    mystl::unordered_multiset<int> us3(520, mystl::hash<int>());
    mystl::unordered_multiset<int> us4(520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_multiset<int> us5(a, a + 5);
    mystl::unordered_multiset<int> us6(a, a + 5, 100);
    mystl::unordered_multiset<int> us7(a, a + 5, 100, mystl::hash<int>());
    mystl::unordered_multiset<int> us8(a, a + 5, 100, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_multiset<int> us9(us5);
    mystl::unordered_multiset<int> us10(std::move(us5));
    mystl::unordered_multiset<int> us11;
    us11 = us6;
    mystl::unordered_multiset<int> us12;
    us12 = std::move(us6);
    mystl::unordered_multiset<int> us13{ 1,2,3,4,5 };
    mystl::unordered_multiset<int> us14;
    us14 = { 1,2,3,4,5 };

    FUN_AFTER(us1, us1.emplace(1));
    FUN_AFTER(us1, us1.emplace_hint(us1.end(), 2));
    FUN_AFTER(us1, us1.insert(5));
    FUN_AFTER(us1, us1.insert(us1.begin(), 5));
    FUN_AFTER(us1, us1.insert(a, a + 5));
    FUN_AFTER(us1, us1.erase(us1.begin()));
    FUN_AFTER(us1, us1.erase(us1.begin(), us1.find(3)));
    FUN_AFTER(us1, us1.erase(1));
    std::cout << std::boolalpha;
    FUN_VALUE(us1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(us1.size());
    FUN_VALUE(us1.bucket_count());
    FUN_VALUE(us1.max_bucket_count());
    FUN_VALUE(us1.bucket(1));
    FUN_VALUE(us1.bucket_size(us1.bucket(5)));
    FUN_AFTER(us1, us1.clear());
    FUN_AFTER(us1, us1.swap(us7));
    FUN_VALUE(*us1.begin());
    std::cout << std::boolalpha;
    FUN_VALUE(us1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(us1.size());
    FUN_VALUE(us1.max_size());
    FUN_VALUE(us1.bucket_count());
    FUN_AFTER(us1, us1.reserve(1000));
    FUN_VALUE(*us1.begin(us1.bucket(1)));
    FUN_VALUE(us1.size());
    FUN_VALUE(us1.bucket_count());
    FUN_VALUE(us1.bucket_size(1));
    FUN_VALUE(us1.bucket_size(2));
    FUN_VALUE(us1.bucket_size(3));
    FUN_AFTER(us1, us1.rehash(150));
    FUN_VALUE(us1.bucket_count());
    FUN_VALUE(us1.count(1));
    FUN_VALUE(*us1.find(3));
    auto first = *us1.equal_range(3).first;
    auto second = *us1.equal_range(3).second;
    std::cout << " us1.equal_range(3) : from " << first << " to " << second << std::endl;
    FUN_VALUE(us1.load_factor());
    FUN_VALUE(us1.max_load_factor());
    FUN_AFTER(us1, us1.max_load_factor(1.5f));
    FUN_VALUE(us1.max_load_factor());
    PASSED;

    mystl::unordered_multiset<int> my_set;
    std::unordered_multiset<int> std_set;
    int times1 = 100000;
    int times2 = 1000000;
    int times3 = 10000000;
    string mystl_times1 = time_push_back(times1, my_set);
    string mystl_times2 = time_push_back(times2, my_set);
    string mystl_times3 = time_push_back(times3, my_set);
    string stl_times1 = time_push_back(times1, std_set);
    string stl_times2 = time_push_back(times2, std_set);
    string stl_times3 = time_push_back(times3, std_set);
    std::cout << "[--------------------- Performance Testing ---------------------]\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    std::cout << "|      push_back      |    100000   |   1000000   |   10000000  |\n";
    std::cout << "|        mystl        | "<<mystl_times1 + " | " << mystl_times2 + "|" + mystl_times3 + " |\n";
    std::cout << "|        std          | "<<stl_times1 + " |" << stl_times2 + "|" + stl_times3 + " |\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    PASSED;
}


}
#endif