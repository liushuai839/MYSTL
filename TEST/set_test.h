#ifndef MYSTL_SET_H
#define MYSTL_SET_H
#include "../MYSTL/set.h"
#include <set>
#include "test.h"

namespace mystl
{

string time_push_back(int times, mystl::set<int> mystl_set){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        mystl_set.emplace(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
string time_push_back(int times, std::set<int> std_set){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        std_set.emplace(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}

string time_push_back(int times, mystl::multiset<int> mystl_set){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        mystl_set.emplace(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
string time_push_back(int times, std::multiset<int> std_set){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        std_set.emplace(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}



void set_test()
{
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[------------------ Run container test : set -------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 5,4,3,2,1 };
    mystl::set<int> s1;
    mystl::set<int, mystl::greater<int>> s2;
    mystl::set<int> s3(a, a + 5);
    mystl::set<int> s4(a, a + 5);
    mystl::set<int> s5(s3);
    mystl::set<int> s6(std::move(s3));
    mystl::set<int> s7;
    s7 = s4;
    mystl::set<int> s8;
    s8 = std::move(s4);
    mystl::set<int> s9{ 1,2,3,4,5 };
    mystl::set<int> s10;
    s10 = { 1,2,3,4,5 };

    for (int i = 5; i > 0; --i)
    {
        FUN_AFTER(s1, s1.emplace(i));
    }
    FUN_AFTER(s1, s1.emplace_hint(s1.begin(), 0));
    FUN_AFTER(s1, s1.erase(s1.begin()));
    FUN_AFTER(s1, s1.erase(0));
    FUN_AFTER(s1, s1.erase(1));
    FUN_AFTER(s1, s1.erase(s1.begin(), s1.end()));
    for (int i = 0; i < 5; ++i)
    {
        FUN_AFTER(s1, s1.insert(i));
    }
    FUN_AFTER(s1, s1.insert(a, a + 5));
    FUN_AFTER(s1, s1.insert(5));
    FUN_AFTER(s1, s1.insert(s1.end(), 5));
    FUN_VALUE(s1.count(5));
    FUN_VALUE(*s1.find(3));
    FUN_VALUE(*s1.lower_bound(3));
    FUN_VALUE(*s1.upper_bound(3));
    auto first = *s1.equal_range(3).first;
    auto second = *s1.equal_range(3).second;
    std::cout << " s1.equal_range(3) : from " << first << " to " << second << std::endl;
    FUN_AFTER(s1, s1.erase(s1.begin()));
    FUN_AFTER(s1, s1.erase(1));
    FUN_AFTER(s1, s1.erase(s1.begin(), s1.find(3)));
    FUN_AFTER(s1, s1.clear());
    FUN_AFTER(s1, s1.swap(s5));
    FUN_VALUE(*s1.begin());
    FUN_VALUE(*s1.rbegin());
    std::cout << std::boolalpha;
    FUN_VALUE(s1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(s1.size());
    FUN_VALUE(s1.max_size());
    PASSED;

    mystl::set<int> my_set;
    std::set<int> std_set;
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

void multiset_test()
{
  std::cout << "[===============================================================]" << std::endl;
  std::cout << "[---------------- Run container test : multiset ----------------]" << std::endl;
  std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
  int a[] = { 5,4,3,2,1 };
  mystl::multiset<int> s1;
  mystl::multiset<int, mystl::greater<int>> s2;
//   mystl::multiset<int> s3(a, a + 5);
//   mystl::multiset<int> s4(a, a + 5);
// //   mystl::multiset<int> s5(s3);
// //   mystl::multiset<int> s6(std::move(s3));
//   mystl::multiset<int> s7;
//   s7 = s4;
//   mystl::multiset<int> s8;
//   s8 = std::move(s4);
  mystl::multiset<int> s9{ 1,2,3,4,5 };
  mystl::multiset<int> s10;
  s10 = { 1,2,3,4,5 };

  for (int i = 5; i > 0; --i)
  {
    FUN_AFTER(s1, s1.emplace(i));
  }
    FUN_AFTER(s1, s1.emplace_hint(s1.begin(), 0));
    FUN_AFTER(s1, s1.erase(s1.begin()));
    FUN_AFTER(s1, s1.erase(0));
    FUN_AFTER(s1, s1.erase(1));
    FUN_AFTER(s1, s1.erase(s1.begin(), s1.end()));
    for (int i = 0; i < 5; ++i)
    {
        FUN_AFTER(s1, s1.insert(i));
    }
    FUN_AFTER(s1, s1.insert(a, a + 5));
    FUN_AFTER(s1, s1.insert(5));
    FUN_AFTER(s1, s1.insert(s1.end(), 5));
    FUN_VALUE(s1.count(5));
    FUN_VALUE(*s1.find(3));
    FUN_VALUE(*s1.lower_bound(3));
    FUN_VALUE(*s1.upper_bound(3));
    auto first = *s1.equal_range(3).first;
    auto second = *s1.equal_range(3).second;
    std::cout << " s1.equal_range(3) : from " << first << " to " << second << std::endl;
    FUN_AFTER(s1, s1.erase(s1.begin()));
    FUN_AFTER(s1, s1.erase(1));
    // FUN_AFTER(s1, s1.erase(s1.begin(), s1.find(3)));
    FUN_AFTER(s1, s1.clear());
    FUN_VALUE(*s1.begin());
    // FUN_VALUE(*s1.rbegin());
    std::cout << std::boolalpha;
    FUN_VALUE(s1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(s1.size());
    FUN_VALUE(s1.max_size());
    PASSED;

    mystl::multiset<int> my_set;
    std::multiset<int> std_set;
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