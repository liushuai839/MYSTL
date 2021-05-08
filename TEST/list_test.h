#ifndef MYSTL_LIST_TEST_H
#define MYSTL_LIST_TEST_H

#include "../MYSTL/list.h"
#include "test.h"
#include <list>



namespace mystl
{


string time_push_back(int times, mystl::list<int> q){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        int a = rand();
        q.push_back(a);
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
string time_push_back(int times, std::list<int> q){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        q.push_back(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}


void list_test(){
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[------------------ Run container test : list ------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 1,2,3,4,5 };
    mystl::list<int> l1;
    mystl::list<int> l2(5);
    mystl::list<int> l3(5, 1);
    mystl::list<int> l4(a, a + 5);
    mystl::list<int> l5(l2);
    mystl::list<int> l6(std::move(l2));
    mystl::list<int> l7{ 1,2,3,4,5,6,7,8,9 };
    mystl::list<int> l8;
    l8 = l3;
    mystl::list<int> l9;
    l9 = std::move(l3);
    mystl::list<int> l10;
    l10 = { 1, 2, 2, 3, 5, 6, 7, 8, 9 };

    FUN_AFTER(l1, l1.assign(8, 8));
    FUN_AFTER(l1, l1.assign(a, a + 5));
    FUN_AFTER(l1, l1.assign({ 1,2,3,4,5,6 }));
    FUN_AFTER(l1, l1.insert(l1.end(), 6));
    FUN_AFTER(l1, l1.insert(l1.end(), 2, 7));
    FUN_AFTER(l1, l1.insert(l1.begin(), a, a + 5));
    FUN_AFTER(l1, l1.push_back(2));
    FUN_AFTER(l1, l1.push_front(1));
    FUN_AFTER(l1, l1.emplace(l1.begin(),1));
    FUN_AFTER(l1, l1.emplace_front(0));
    FUN_AFTER(l1, l1.emplace_back(10));
    FUN_VALUE(l1.size());
    FUN_AFTER(l1, l1.pop_front());
    FUN_AFTER(l1, l1.pop_back());
    FUN_AFTER(l1, l1.erase(l1.begin()));
    FUN_AFTER(l1, l1.erase(l1.begin(), l1.end()));
    FUN_VALUE(l1.size());
    FUN_AFTER(l1, l1.resize(10));
    FUN_AFTER(l1, l1.resize(5, 1));
    FUN_AFTER(l1, l1.resize(8, 2));
    FUN_VALUE(l1.size());
    FUN_AFTER(l1, l1.splice(l1.end(), l4));
    FUN_AFTER(l1, l1.splice(l1.begin(), l5, l5.begin()));
    FUN_AFTER(l1, l1.splice(l1.end(), l6, l6.begin(), ++l6.begin()));
    FUN_VALUE(l1.size());
    // //   FUN_AFTER(l1, l1.remove(0));
    // //   FUN_AFTER(l1, l1.remove_if(is_odd));
    FUN_VALUE(l1.size());
    FUN_AFTER(l1, l1.assign({ 9,5,3,3,7,1,3,2,2,0,10 }));
    FUN_VALUE(l1.size());
    FUN_AFTER(l1, l1.sort());
    // //   FUN_AFTER(l1, l1.unique());
    // //   FUN_AFTER(l1, l1.unique([&](int a, int b) {return b == a + 1; }));
    FUN_AFTER(l1, l1.merge(l7));
    // //   FUN_AFTER(l1, l1.sort(mystl::greater<int>()));
    // //   FUN_AFTER(l1, l1.merge(l8, mystl::greater<int>()));
    FUN_AFTER(l1, l1.reverse());
    FUN_AFTER(l1, l1.clear());
    FUN_AFTER(l1, l1.swap(l9));
    FUN_VALUE(*l1.begin());
    FUN_VALUE(*l1.rbegin());
    FUN_VALUE(l1.front());
    FUN_VALUE(l1.back());
    std::cout << std::boolalpha;
    FUN_VALUE(l1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(l1.size());
    FUN_VALUE(l1.max_size());
    PASSED;


    mystl::list<int> my_list;
    std::list<int> std_list;
    
    int times1 = 100000;
    int times2 = 1000000;
    int times3 = 10000000;

    string mystl_times1 = time_push_back(times1, my_list);
    string mystl_times2 = time_push_back(times2, my_list);
    string mystl_times3 = time_push_back(times3, my_list);
    string stl_times1 = time_push_back(times1, std_list);
    string stl_times2 = time_push_back(times2, std_list);
    string stl_times3 = time_push_back(times3, std_list);
    std::cout << "[--------------------- Performance Testing ---------------------]\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    std::cout << "|      push_back      |    100000   |   1000000   |   10000000  |\n";
    std::cout << "|        mystl        |  "<<mystl_times1 + " | " << mystl_times2 + " |" + mystl_times3 + " |\n";
    std::cout << "|        std          |  "<<stl_times1 + " | " << stl_times2 + " |" + stl_times3 + " |\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";


}
}



#endif