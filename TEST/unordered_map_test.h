#ifndef MYSTL_UNORDERED_MAP_TEST_H_
#define MYSTL_UNORDERED_MAP_TEST_H_

#include <unordered_map>
#include "../MYSTL/unordered_map.h"
#include "map_test.h"
#include "test.h"

namespace mystl
{

string time_push_back(int times, mystl::unordered_map<int,int> mystl_unordered_map){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        mystl_unordered_map.emplace(rand(),rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
string time_push_back(int times, std::unordered_map<int,int> std_unordered_map){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        std_unordered_map.emplace(rand(),rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}

string time_push_back(int times, mystl::unordered_multimap<int,int> mystl_unordered_map){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        mystl_unordered_map.emplace(rand(),rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
string time_push_back(int times, std::unordered_multimap<int,int> std_unordered_map){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        std_unordered_map.emplace(rand(),rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}



void unordered_map_test()
{
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[-------------- Run container test : unordered_map -------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    mystl::vector<PAIR> v;
    for (int i = 0; i < 5; ++i)
        v.push_back(PAIR(5 - i, 5 - i));
    mystl::unordered_map<int, int> um1;
    mystl::unordered_map<int, int> um2(520);
    mystl::unordered_map<int, int> um3(520, mystl::hash<int>());
    mystl::unordered_map<int, int> um4(520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_map<int, int> um5(v.begin(), v.end());
    mystl::unordered_map<int, int> um6(v.begin(), v.end(), 100);
    mystl::unordered_map<int, int> um7(v.begin(), v.end(), 100, mystl::hash<int>());
    mystl::unordered_map<int, int> um8(v.begin(), v.end(), 100, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_map<int, int> um9(um5);
    // mystl::unordered_map<int, int> um10(std::move(um5));
    mystl::unordered_map<int, int> um11;
    um11 = um6;
    mystl::unordered_map<int, int> um12;
    um12 = std::move(um6);
    mystl::unordered_map<int, int> um13{ PAIR(1,1),PAIR(2,3),PAIR(3,3) };
    mystl::unordered_map<int, int> um14;
    um14 = { PAIR(1,1),PAIR(2,3),PAIR(3,3) };

    MAP_FUN_AFTER(um1, um1.emplace(1, 1));
    MAP_FUN_AFTER(um1, um1.emplace_hint(um1.begin(), 1, 2));
    MAP_FUN_AFTER(um1, um1.insert(PAIR(2, 2)));
    MAP_FUN_AFTER(um1, um1.insert(um1.end(), PAIR(3, 3)));
    MAP_FUN_AFTER(um1, um1.insert(v.begin(), v.end()));
    MAP_FUN_AFTER(um1, um1.erase(um1.begin()));
    MAP_FUN_AFTER(um1, um1.erase(um1.begin(), um1.find(3)));
    MAP_FUN_AFTER(um1, um1.erase(1));
    std::cout << std::boolalpha;
    FUN_VALUE(um1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(um1.size());
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.max_bucket_count());
    FUN_VALUE(um1.bucket(1));
    FUN_VALUE(um1.bucket_size(um1.bucket(5)));
    MAP_FUN_AFTER(um1, um1.clear());
    MAP_FUN_AFTER(um1, um1.swap(um7));
    MAP_VALUE(*um1.begin());
    FUN_VALUE(um1.at(1));
    FUN_VALUE(um1[1]);
    std::cout << std::boolalpha;
    FUN_VALUE(um1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(um1.size());
    FUN_VALUE(um1.max_size());
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.max_bucket_count());
    FUN_VALUE(um1.bucket(1));
    FUN_VALUE(um1.bucket_size(um1.bucket(1)));
    MAP_FUN_AFTER(um1, um1.reserve(1000));
    FUN_VALUE(um1.size());
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.bucket_size(1));
    FUN_VALUE(um1.bucket_size(2));
    FUN_VALUE(um1.bucket_size(3));
    MAP_FUN_AFTER(um1, um1.rehash(150));
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.count(1));
    MAP_VALUE(*um1.find(3));
    auto first = *um1.equal_range(3).first;
    auto second = *um1.equal_range(3).second;
    std::cout << " um1.equal_range(3) : from <" << first.first << ", " << first.second
        << "> to <" << second.first << ", " << second.second << ">" << std::endl;
    FUN_VALUE(um1.load_factor());
    FUN_VALUE(um1.max_load_factor());
    MAP_FUN_AFTER(um1, um1.max_load_factor(1.5f));
    FUN_VALUE(um1.max_load_factor());
    PASSED;

    mystl::unordered_map<int,int> mystl_map;
    std::unordered_map<int,int> std_map;
    int times1 = 100000;
    int times2 = 1000000;
    int times3 = 10000000;
    string mystl_times1 = time_push_back(times1, mystl_map);
    string mystl_times2 = time_push_back(times2, mystl_map);
    string mystl_times3 = time_push_back(times3, mystl_map);
    string stl_times1 = time_push_back(times1, std_map);
    string stl_times2 = time_push_back(times2, std_map);
    string stl_times3 = time_push_back(times3, std_map);
    std::cout << "[--------------------- Performance Testing ---------------------]\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    std::cout << "|      push_back      |    100000   |   1000000   |   10000000  |\n";
    std::cout << "|        mystl        | "<<mystl_times1 + " | " << mystl_times2 + "|" + mystl_times3 + " |\n";
    std::cout << "|        std          | "<<stl_times1 + " |" << stl_times2 + "|" + stl_times3 + " |\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    PASSED;


}

void unordered_multimap_test()
{
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[----------- Run container test : unordered_multimap -----------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    mystl::vector<PAIR> v;
    for (int i = 0; i < 5; ++i)
        v.push_back(PAIR(5 - i, 5 - i));
    mystl::unordered_multimap<int, int> um1;
    mystl::unordered_multimap<int, int> um2(520);
    mystl::unordered_multimap<int, int> um3(520, mystl::hash<int>());
    mystl::unordered_multimap<int, int> um4(520, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_multimap<int, int> um5(v.begin(), v.end());
    mystl::unordered_multimap<int, int> um6(v.begin(), v.end(), 100);
    mystl::unordered_multimap<int, int> um7(v.begin(), v.end(), 100, mystl::hash<int>());
    mystl::unordered_multimap<int, int> um8(v.begin(), v.end(), 100, mystl::hash<int>(), mystl::equal_to<int>());
    mystl::unordered_multimap<int, int> um9(um5);
    mystl::unordered_multimap<int, int> um10(std::move(um5));
    mystl::unordered_multimap<int, int> um11;
    um11 = um6;
    mystl::unordered_multimap<int, int> um12;
    um12 = std::move(um6);
    mystl::unordered_multimap<int, int> um13{ PAIR(1,1),PAIR(2,3),PAIR(3,3) };
    mystl::unordered_multimap<int, int> um14;
    um14 = { PAIR(1,1),PAIR(2,3),PAIR(3,3) };

    MAP_FUN_AFTER(um1, um1.emplace(1, 1));
    MAP_FUN_AFTER(um1, um1.emplace_hint(um1.begin(), 1, 2));
    MAP_FUN_AFTER(um1, um1.insert(PAIR(2, 2)));
    MAP_FUN_AFTER(um1, um1.insert(um1.end(), PAIR(3, 3)));
    MAP_FUN_AFTER(um1, um1.insert(v.begin(), v.end()));
    MAP_FUN_AFTER(um1, um1.erase(um1.begin()));
    MAP_FUN_AFTER(um1, um1.erase(um1.begin(), um1.find(3)));
    MAP_FUN_AFTER(um1, um1.erase(1));
    std::cout << std::boolalpha;
    FUN_VALUE(um1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(um1.size());
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.max_bucket_count());
    FUN_VALUE(um1.bucket(1));
    FUN_VALUE(um1.bucket_size(um1.bucket(5)));
    MAP_FUN_AFTER(um1, um1.clear());
    MAP_FUN_AFTER(um1, um1.swap(um7));
    MAP_VALUE(*um1.begin());
    std::cout << std::boolalpha;
    FUN_VALUE(um1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(um1.size());
    FUN_VALUE(um1.max_size());
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.max_bucket_count());
    FUN_VALUE(um1.bucket(1));
    FUN_VALUE(um1.bucket_size(um1.bucket(1)));
    MAP_FUN_AFTER(um1, um1.reserve(1000));
    FUN_VALUE(um1.size());
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.bucket_size(1));
    FUN_VALUE(um1.bucket_size(2));
    FUN_VALUE(um1.bucket_size(3));
    MAP_FUN_AFTER(um1, um1.rehash(150));
    FUN_VALUE(um1.bucket_count());
    FUN_VALUE(um1.count(1));
    MAP_VALUE(*um1.find(3));
    auto first = *um1.equal_range(3).first;
    auto second = *um1.equal_range(3).second;
    std::cout << " um1.equal_range(3) : from <" << first.first << ", " << first.second
        << "> to <" << second.first << ", " << second.second << ">" << std::endl;
    FUN_VALUE(um1.load_factor());
    FUN_VALUE(um1.max_load_factor());
    MAP_FUN_AFTER(um1, um1.max_load_factor(1.5f));
    FUN_VALUE(um1.max_load_factor());
    PASSED;

    mystl::unordered_multimap<int,int> mystl_map;
    std::unordered_multimap<int,int> std_map;
    int times1 = 100000;
    int times2 = 1000000;
    int times3 = 10000000;
    string mystl_times1 = time_push_back(times1, mystl_map);
    string mystl_times2 = time_push_back(times2, mystl_map);
    string mystl_times3 = time_push_back(times3, mystl_map);
    string stl_times1 = time_push_back(times1, std_map);
    string stl_times2 = time_push_back(times2, std_map);
    string stl_times3 = time_push_back(times3, std_map);
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