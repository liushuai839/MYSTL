#ifndef MYTINYSTL_STACK_TEST_H_
#define MYTINYSTL_STACK_TEST_H_

#include <stack>
#include "../MYSTL/stack.h"

namespace mystl{

void stack_print(mystl::stack<int> s){
    while(!s.empty()){
        std::cout << " " << s.top();
        s.pop();
    }
    std::cout << std::endl;
}

// stack 的遍历输出
#define STACK_COUT(s) do {                       \
    std::string s_name = #s;                     \
    std::cout << " " << s_name << " :";          \
    stack_print(s);                              \
} while(0)

#define STACK_FUN_AFTER(con, fun) do {           \
  std::string fun_name = #fun;                   \
  std::cout << " After " << fun_name << " :\n";  \
  fun;                                           \
  STACK_COUT(con);                               \
} while(0)


string time_push_back(int times, mystl::stack<int> stk){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        stk.push(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}
string time_push_back(int times, std::stack<int> stk){
    const auto t1 = std::chrono::system_clock::now();
    for(int i=0; i<times; i++){
        stk.push(rand());
    }
    const auto t2 = std::chrono::system_clock::now();
    const auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() * 1e-3;
    string str1 = to_string(duration1) + "ms";
    return str1;
}


void stack_test(){

    std::cout << "[===============================================================]" << std::endl;
    std::cout << "[----------------- Run container test : stack ------------------]" << std::endl;
    std::cout << "[-------------------------- API test ---------------------------]" << std::endl;
    int a[] = { 1,2,3,4,5 };
    mystl::deque<int> d1(5);
    mystl::stack<int> s1;
    mystl::stack<int> s2(5);
    mystl::stack<int> s3(5, 1);
    mystl::stack<int> s4(a, a + 5);
    mystl::stack<int> s5(d1);
    mystl::stack<int> s6(std::move(d1));
    mystl::stack<int> s7(s2);
    mystl::stack<int> s8(std::move(s2));
    mystl::stack<int> s9;
    s9 = s3;
    mystl::stack<int> s10;
    s10 = std::move(s3);
    mystl::stack<int> s11{ 1,2,3,4,5 };
    mystl::stack<int> s12;
    s12 = { 1,2,3,4,5 };

    STACK_FUN_AFTER(s1, s1.push(1));
    STACK_FUN_AFTER(s1, s1.push(2));
    STACK_FUN_AFTER(s1, s1.push(3));
    STACK_FUN_AFTER(s1, s1.pop());
    STACK_FUN_AFTER(s1, s1.emplace(4));
    STACK_FUN_AFTER(s1, s1.emplace(5));
    std::cout << std::boolalpha;
    FUN_VALUE(s1.empty());
    std::cout << std::noboolalpha;
    FUN_VALUE(s1.size());
    FUN_VALUE(s1.top());
    while (!s1.empty())
    {
        STACK_FUN_AFTER(s1, s1.pop());
    }
    STACK_FUN_AFTER(s1, s1.swap(s4));
    STACK_FUN_AFTER(s1, s1.clear());
    PASSED;
    mystl::stack<int> my_stk;
    std::stack<int> std_stk;
    int times1 = 100000;
    int times2 = 1000000;
    int times3 = 10000000;
    string mystl_times1 = time_push_back(times1, my_stk);
    string mystl_times2 = time_push_back(times2, my_stk);
    string mystl_times3 = time_push_back(times3, my_stk);
    string stl_times1 = time_push_back(times1, std_stk);
    string stl_times2 = time_push_back(times2, std_stk);
    string stl_times3 = time_push_back(times3, std_stk);
    std::cout << "[--------------------- Performance Testing ---------------------]\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    std::cout << "|      push_back      |    100000   |   1000000   |   10000000  |\n";
    std::cout << "|        mystl        |"<<mystl_times1 + " | " << mystl_times2 + " |" + mystl_times3 + " |\n";
    std::cout << "|        std          |"<<stl_times1 + "| " << stl_times2 + " |" + stl_times3 + " |\n";
    std::cout << "|---------------------|-------------|-------------|-------------|\n";
    PASSED;









}




} //namespace mystl
#endif
