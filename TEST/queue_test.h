#ifndef MYSTL_TEST_H
#define MYSTL_TEST_H

#include "../MYSTL/queue.h"
#include "test.h"
#include <queue>


namespace mystl{


void queue_print(mystl::queue<int> q)
{
  while (!q.empty())
  {
    std::cout << " " << q.front();
    q.pop();
  }
  std::cout << std::endl;
}
//  queue 的遍历输出
#define QUEUE_COUT(q) do {                       \
    std::string q_name = #q;                     \
    std::cout << " " << q_name << " :";          \
    queue_print(q);                              \
} while(0)

#define QUEUE_FUN_AFTER(con, fun) do {           \
  std::string fun_name = #fun;                   \
  std::cout << " After " << fun_name << " :\n";  \
  fun;                                           \
  QUEUE_COUT(con);                               \
} while(0)

string time_push_back(int times, mystl::queue<int> q){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        int a = rand();
        q.push(a);
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
string time_push_back(int times, std::queue<int> q){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        q.push(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}



void queue_test()
{
    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[----------------- Run container test : queue ------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 1,2,3,4,5 };
    mystl::deque<int> d1(5);
    mystl::queue<int> q1;
    mystl::queue<int> q2(5);
    mystl::queue<int> q3(5, 1);
    mystl::queue<int> q4(a, a + 5);
    mystl::queue<int> q5(d1);
    mystl::queue<int> q6(std::move(d1));
    mystl::queue<int> q7(q2);
    mystl::queue<int> q8(std::move(q2));
    mystl::queue<int> q9;
    q9 = q3;
    mystl::queue<int> q10;
    q10 = std::move(q3);
    mystl::queue<int> q11{ 1,2,3,4,5 };
    mystl::queue<int> q12;
    q12 = { 1,2,3,4,5 };

    QUEUE_FUN_AFTER(q1, q1.push(1));
    QUEUE_FUN_AFTER(q1, q1.push(2));
    QUEUE_FUN_AFTER(q1, q1.push(3));
    QUEUE_FUN_AFTER(q1, q1.pop());
    QUEUE_FUN_AFTER(q1, q1.emplace(4));
    QUEUE_FUN_AFTER(q1, q1.emplace(5));
    std::cout << std::boolalpha;
    FUN_VALUE(q1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(q1.size());
    FUN_VALUE(q1.front());
    FUN_VALUE(q1.back());
    while (!q1.empty())
    {
        QUEUE_FUN_AFTER(q1, q1.pop());
    }
    QUEUE_FUN_AFTER(q1, q1.swap(q4));
    QUEUE_FUN_AFTER(q1, q1.clear());
    PASSED;

    mystl::queue<int> my_queue;
    std::queue<int> std_queue;
    
    int times1 = 100000;
    int times2 = 1000000;
    int times3 = 10000000;

    string mystl_times1 = time_push_back(times1, my_queue);
    string mystl_times2 = time_push_back(times2, my_queue);
    string mystl_times3 = time_push_back(times3, my_queue);
    string stl_times1 = time_push_back(times1, std_queue);
    string stl_times2 = time_push_back(times2, std_queue);
    string stl_times3 = time_push_back(times3, std_queue);
    std::cout << "[--------------------- Performance Testing ---------------------]\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    std::cout << "|      push_back      |    100000   |   1000000   |   10000000  |\n";
    std::cout << "|        mystl        |  "<<mystl_times1 + " | " << mystl_times2 + " |" + mystl_times3 + " |\n";
    std::cout << "|        std          |  "<<stl_times1 + " | " << stl_times2 + " |" + stl_times3 + " |\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";

}














}


#endif