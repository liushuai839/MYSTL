#ifndef MYSTL_ALGO_TEST_H
#define MYSTL_ALGO_TEST_H



#include <algorithm>
#include <functional>
#include <numeric>

#include "../MYSTL/algo.h"
#include "test.h"
#include "../MYSTL/vector.h"

namespace mystl
{

#define TEST(test_name)  \
        std::cout << "===============================================" << endl; \
        std::cout << "Run TestCase: " << #test_name << std::endl;

#define FINISHED(test_name)  \
        std::cout << "Finished: " << #test_name << std::endl;    \
        std::cout << "===============================================" << endl; 



#define CHECK(c1, c2) do {                                         \
  auto first1 = std::begin(c1), last1 = std::end(c1);               \
  auto first2 = std::begin(c2), last2 = std::end(c2);               \
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {  \
    if (*first1 != *first2)  break;                                 \
  }                                                                 \
  if (first1 == last1 && first2 == last2) {                         \
    std::cout << " CHECK succeeded!\n";                    \
  }                                                                 \
  else {                                                            \
    std::cout << " CHECK failed!\n";                         \
}} while(0)


// 一些可能会用到的辅助数据和函数
int  for_each_sum = 0;

int  gen() { return 5; }
int  r(int i) { return (i * 5 + 1) % 9; }
bool is_odd(int i) { return i & 1; }
bool is_even(int i) { return !(i & 1); }
void arr_sum(int i) { for_each_sum += i; }
bool cmp(const int& a, const int& b) { return b < a; }
int  unary_op(const int& x) { return x + 1; }
int  binary_op(const int& x, const int& y) { return x + y; }


void copy_test(){
  
  TEST(copy_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
  int exp[5], act[5];
  std::copy(arr1, arr1 + 5, exp);
  mystl::copy(arr1, arr1 + 5, act);
  CHECK(exp, act);
  std::copy(arr1 + 5, arr1 + 10, exp);
  mystl::copy(arr1 + 5, arr1 + 10, act);
  CHECK(exp, act);
  FINISHED(copy_test)
}

void copy_if_test(){
  
  TEST(copy_if_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
  int exp[5], act[5];
  std::copy_if(arr1, arr1 + 10, exp, is_odd);
  mystl::copy_if(arr1, arr1 + 10, act, is_odd);
  CHECK(exp, act);
  std::copy_if(arr1, arr1 + 10, exp, is_even);
  mystl::copy_if(arr1, arr1 + 10, act, is_even);
  CHECK(exp, act);
  FINISHED(copy_if_test)
}

void copy_n_test(){
  
  TEST(copy_n_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
  int exp[10], act[10];
  std::copy_n(arr1, 10, exp);
  mystl::copy_n(arr1, 10, act);
  CHECK(exp, act);
  std::copy_n(arr1 + 5, 5, exp);
  mystl::copy_n(arr1 + 5, 5, act);
  CHECK(exp, act);
  FINISHED(copy_n_test)
}

void move_test()
{ 
  TEST(move_test)
  int arr1[] = { 1,2,3,4,5 };
  int arr2[] = { 1,2,3,4,5 };
  int exp[5], act[5];
  std::move(arr1, arr1 + 5, exp);
  mystl::move(arr2, arr2 + 5, act);
  CHECK(exp, act);
  CHECK(arr1, arr2);
  FINISHED(move_test)
}

void move_backward_test()
{ 
  TEST(move_backward_test)
  int arr1[] = { 1,2,3,4,5 };
  int arr2[] = { 1,2,3,4,5 };
  int exp[5], act[5];
  std::move_backward(arr1, arr1 + 5, exp + 5);
  mystl::move_backward(arr2, arr2 + 5, act + 5);
  CHECK(exp, act);
  CHECK(arr1, arr2);
  FINISHED(move_backward_test)
}

void equal_test()
{ 
  TEST(equal_test)
  std::vector<int> v1{ 1,2,3,4,5 };
  std::vector<int> v2{ 1,2,3,4,5,6 };
  int arr1[] = { 1,2,3,4,5 };
  int arr2[] = { 1,2,3,4,6 };
  EXPECT_EQ(std::equal(v1.begin(), v1.end(), v2.begin()),
            mystl::equal(v1.begin(), v1.end(), v2.begin()));
  EXPECT_EQ(std::equal(arr1, arr1 + 5, arr2),
            mystl::equal(arr1, arr1 + 5, arr2));
  EXPECT_EQ(std::equal(v1.begin(), v1.end(), arr1),
            mystl::equal(v1.begin(), v1.end(), arr1));
  EXPECT_EQ(std::equal(v1.begin(), v1.end(), arr2, std::equal_to<int>()),
            mystl::equal(v1.begin(), v1.end(), arr2, std::equal_to<int>()));
  TEST(equal_test)
}

void fill_test()
{ 
  TEST(fill_test)
  int exp[10], act[10];
  mystl::vector<int> v1(10, 1);
  mystl::vector<int> v2(10, 2);
  std::fill(exp, exp + 10, 1);
  mystl::fill(act, act + 10, 1);
  CHECK(exp, act);
  std::fill(v1.begin(), v1.end(), 3);
  mystl::fill(v2.begin(), v2.end(), 3);
  CHECK(v1, v2);
  FINISHED(fill_test)
}

void fill_n_test()
{ 
  TEST(fill_n_test)
  int arr1[5];
  int arr2[5];
  std::fill_n(arr2, 5, 1);
  mystl::fill_n(arr1, 5, 1);
  CHECK(arr1, arr2);
  std::fill_n(arr1 + 2, 3, 2);
  mystl::fill_n(arr2 + 2, 3, 2);
  CHECK(arr1, arr2);
  FINISHED(fill_n_test)
}

void iter_swap_test()
{ 
  TEST(iter_swap_test)
  int a = 1;
  int b = 2;
  int *p1 = &a;
  int *p2 = &b;
  int *p3 = &a;
  int *p4 = &b;
  std::iter_swap(p1, p2);
  mystl::iter_swap(p3, p4);
  EXPECT_PTR_EQ(p1, p3);
  EXPECT_PTR_EQ(p2, p4);
  EXPECT_EQ(p1, p3);
  EXPECT_EQ(p2, p4);
  FINISHED(iter_swap_test)
}

void lexicographical_compare_test()
{ 
  TEST(lexicographical_compare_test)
  int arr1[] = { 1,2,3,4,5 };
  int arr2[] = { 0,2,4,6,8 };
  int arr3[] = { 1,2,3,4,5 };
  int arr4[] = { 1,2,3,4,5,6 };
  int arr5[] = { 2,3,4 };
  EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 5, arr2, arr2 + 5),
            mystl::lexicographical_compare(arr1, arr1 + 5, arr2, arr2 + 5));
  EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 5, arr3, arr3 + 5),
            mystl::lexicographical_compare(arr1, arr1 + 5, arr3, arr3 + 5));
  EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 5, arr4, arr4 + 6, std::less<int>()),
            mystl::lexicographical_compare(arr1, arr1 + 5, arr4, arr4 + 6, std::less<int>()));
  EXPECT_EQ(std::lexicographical_compare(arr1, arr1 + 5, arr5, arr5 + 3, std::less<int>()),
            mystl::lexicographical_compare(arr1, arr1 + 5, arr5, arr5 + 3, std::less<int>()));
  FINISHED(lexicographical_compare_test)
}

void max_test()
{ 
  TEST(max_test)
  int i1 = 1, i2 = 2;
  double d1 = 1.1, d2 = 2.2;
  char c1 = 'a', c2 = 'b';
  EXPECT_EQ(std::max(i1, i2), mystl::max(i1, i2));
  EXPECT_EQ(std::max(d1, d2), mystl::max(d1, d2));
  EXPECT_EQ(std::max(c1, c2), mystl::max(c1, c2));
  FINISHED(max_test)
}

void min_test()
{ 
  TEST(min_test)
  int i1 = 1, i2 = 2;
  double d1 = 1.1, d2 = 2.2;
  char c1 = 'a', c2 = 'b';
  EXPECT_EQ(std::min(i1, i2), mystl::min(i1, i2));
  EXPECT_EQ(std::min(d1, d2), mystl::min(d1, d2));
  EXPECT_EQ(std::min(c1, c2), mystl::min(c1, c2));
  FINISHED(min_test)
}

void mismatch_test()
{ 
  TEST(mismatch_test)
  int arr1[] = { 1,1,2,2,3,4,5 };
  int arr2[] = { 1,1,2,2,3,3,3 };
  int arr3[] = { 0,1,2,2,3,4,5 };
  int arr4[] = { 1,1,2,2,3,4,5 };
  auto p1 = std::mismatch(arr1, arr1 + 7, arr2);
  auto p2 = mystl::mismatch(arr1, arr1 + 7, arr2);
  auto p3 = std::mismatch(arr1, arr1 + 7, arr3);
  auto p4 = mystl::mismatch(arr1, arr1 + 7, arr3);
  auto p5 = std::mismatch(arr1, arr1 + 7, arr4, std::equal_to<int>());
  auto p6 = mystl::mismatch(arr1, arr1 + 7, arr4, std::equal_to<int>());
  EXPECT_EQ(p1.first, p2.first);
  EXPECT_EQ(p1.second, p2.second);
  EXPECT_EQ(p3.first, p4.first);
  EXPECT_EQ(p3.second, p4.second);
  EXPECT_EQ(p5.first, p6.first);
  EXPECT_EQ(p5.second, p6.second);
  FINISHED(mismatch_test)

}

// heap_algo test
void make_heap_test()
{ 
  TEST(make_heap_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9 };
  int arr2[] = { 1,2,3,4,5,6,7,8,9 };
  int arr3[] = { 2,1,6,5,4,9,8,7,6 };
  int arr4[] = { 2,1,6,5,4,9,8,7,6 };
  int arr5[] = { 1,1,2,2,3,3,4,4,5,5 };
  int arr6[] = { 1,1,2,2,3,3,4,4,5,5 };
  std::make_heap(arr1, arr1 + 9);
  mystl::make_heap(arr2, arr2 + 9);
  std::make_heap(arr3, arr3 + 9);
  mystl::make_heap(arr4, arr4 + 9);
  std::make_heap(arr5, arr5 + 10, std::greater<int>());
  mystl::make_heap(arr6, arr6 + 10, std::greater<int>());
  CHECK(arr1, arr2);
  CHECK(arr3, arr4);
  CHECK(arr5, arr6);
  FINISHED(make_heap_test)
}

void pop_heap_test()
{  
  TEST(pop_heap_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9 };
  int arr2[] = { 1,2,3,4,5,6,7,8,9 };
  std::make_heap(arr1, arr1 + 9);
  std::make_heap(arr2, arr2 + 9);
  for (int i = 9; i > 0; --i)
  {
    std::pop_heap(arr1, arr1 + i);
    mystl::pop_heap(arr2, arr2 + i);
    CHECK(arr1, arr2);
  }
  int arr3[] = { 1,2,3,4,5,6,7,8,9 };
  int arr4[] = { 1,2,3,4,5,6,7,8,9 };
  std::make_heap(arr3, arr3 + 9, std::greater<int>());
  std::make_heap(arr4, arr4 + 9, std::greater<int>());
  std::pop_heap(arr3, arr3 + 9, std::greater<int>());
  mystl::pop_heap(arr4, arr4 + 9, std::greater<int>());
  CHECK(arr3, arr4);
  FINISHED(pop_heap_test)
}

void push_heap_test()
{ 
  TEST(push_heap_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9 };
  int arr2[] = { 1,2,3,4,5,6,7,8,9 };
  std::make_heap(arr1, arr1 + 4);
  std::make_heap(arr2, arr2 + 4);
  for (int i = 4; i <= 9; ++i)
  {
    std::push_heap(arr1, arr1 + i);
    mystl::push_heap(arr2, arr2 + i);
    CHECK(arr1, arr2);
  }
  int arr3[] = { 1,2,3,4,5,6,7,8,9 };
  int arr4[] = { 1,2,3,4,5,6,7,8,9 };
  std::make_heap(arr3, arr3 + 9, std::greater<int>());
  std::make_heap(arr4, arr4 + 9, std::greater<int>());
  std::push_heap(arr3, arr3 + 9, std::greater<int>());
  mystl::push_heap(arr4, arr4 + 9, std::greater<int>());
  CHECK(arr3, arr4);
  FINISHED(push_heap_test)
}

void sort_heap_test()
{ 
  TEST(sort_heap_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9 };
  int arr2[] = { 1,2,3,4,5,6,7,8,9 };
  int arr3[] = { 1,2,3,4,5,6,7,8,9 };
  int arr4[] = { 1,2,3,4,5,6,7,8,9 };
  std::make_heap(arr1, arr1 + 9);
  std::make_heap(arr2, arr2 + 9);
  std::make_heap(arr3, arr3 + 9, std::greater<int>());
  std::make_heap(arr4, arr4 + 9, std::greater<int>());
  std::sort_heap(arr1, arr1 + 9);
  mystl::sort_heap(arr2, arr2 + 9);
  std::sort_heap(arr3, arr3 + 9, std::greater<int>());
  mystl::sort_heap(arr4, arr4 + 9, std::greater<int>());
  CHECK(arr1, arr2);
  CHECK(arr3, arr4);
  FINISHED(sort_heap_test)
}

// algo test
void adjacent_find_test()
{ 
  TEST(adjacent_find_test)
  int arr1[] = { 1,2,3,3,4 };
  int arr2[] = { 1,2,3,4,5 };
  EXPECT_EQ(std::adjacent_find(arr1, arr1 + 5),
            mystl::adjacent_find(arr1, arr1 + 5));
  EXPECT_EQ(std::adjacent_find(arr2, arr2 + 5),
            mystl::adjacent_find(arr2, arr2 + 5));
  EXPECT_EQ(std::adjacent_find(arr1, arr1 + 5, std::greater<int>()),
            mystl::adjacent_find(arr1, arr1 + 5, std::greater<int>()));
  FINISHED(adjacent_find_test)
}

void all_of_test()
{ 
  TEST(all_of_test)
  int arr1[] = { 1,3,5,7,9 };
  int arr2[] = { 1,3,5,7,8 };
  EXPECT_EQ(std::all_of(arr1, arr1 + 5, is_odd),
            mystl::all_of(arr1, arr1 + 5, is_odd));
  EXPECT_EQ(std::all_of(arr2, arr2 + 5, is_odd),
            mystl::all_of(arr2, arr2 + 5, is_odd));
  FINISHED(all_of_test)
}

void any_of_test()
{  
  TEST(any_of_test)
  int arr1[] = { 1,2,4,6,8 };
  int arr2[] = { 2,4,6,8,10 };
  EXPECT_EQ(std::any_of(arr1, arr1 + 5, is_odd),
            mystl::any_of(arr1, arr1 + 5, is_odd));
  EXPECT_EQ(std::any_of(arr2, arr2 + 5, is_odd),
            mystl::any_of(arr2, arr2 + 5, is_odd));
  FINISHED(any_of_test)
}

void binary_search_test()
{ 
  TEST(binary_search_test)
  int arr1[] = { 1,2,3,4,5 };
  EXPECT_EQ(std::binary_search(arr1, arr1 + 5, 1),
            mystl::binary_search(arr1, arr1 + 5, 1));
  EXPECT_EQ(std::binary_search(arr1, arr1 + 5, 6),
            mystl::binary_search(arr1, arr1 + 5, 6));
  FINISHED(binary_search_test)
}

void count_test()
{ 
  TEST(count_test)
  int arr1[] = { 1,2,2,3,3,3,4,5,8 };
  EXPECT_EQ(std::count(arr1, arr1 + 9, 2),
            mystl::count(arr1, arr1 + 9, 2));
  EXPECT_EQ(std::count(arr1, arr1 + 9, 3),
            mystl::count(arr1, arr1 + 9, 3));
  EXPECT_EQ(std::count(arr1, arr1 + 9, 6),
            mystl::count(arr1, arr1 + 9, 6));
  FINISHED(count_test)
}

void count_if_test()
{ 
  TEST(count_if_test)
  int arr1[] = { 1,2,2,3,3,3,4,5,8 };
  EXPECT_EQ(std::count_if(arr1, arr1 + 9, is_odd),
            mystl::count_if(arr1, arr1 + 9, is_odd));
  EXPECT_EQ(std::count_if(arr1, arr1 + 9, is_even),
            mystl::count_if(arr1, arr1 + 9, is_even));
  FINISHED(count_if_test)
}


void equal_range_test()
{ 
  TEST(equal_range_test)
  int arr1[] = { 1,2,3,3,3,4,5 };
  auto p1 = mystl::equal_range(arr1, arr1 + 7, 3);
  auto p2 = std::equal_range(arr1, arr1 + 7, 3);
  auto p3 = mystl::equal_range(arr1, arr1 + 7, 6, std::equal_to<int>());
  auto p4 = std::equal_range(arr1, arr1 + 7, 6, std::equal_to<int>());
  EXPECT_EQ(p2.first, p1.first);
  EXPECT_EQ(p2.second, p1.second);
  EXPECT_EQ(p4.first, p3.first);
  EXPECT_EQ(p4.second, p3.second);
  FINISHED(equal_range_test)
}

void find_test()
{ 
  TEST(find_test)
  int arr1[] = { 1,2,3,4,5 };
  EXPECT_EQ(std::find(arr1, arr1 + 5, 3), mystl::find(arr1, arr1 + 5, 3));
  EXPECT_EQ(std::find(arr1, arr1 + 5, 6), mystl::find(arr1, arr1 + 5, 6));
  FINISHED(find_test)
}

// void find_end_test()
// { 
//   TEST(find_end_test)
//   int arr1[] = { 1,2,3,2,2,3,4,5 };
//   int arr2[] = { 2,3 };
//   int arr3[] = { 4,5,6 };
//   EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr2, arr2 + 1),
//             mystl::find_end(arr1, arr1 + 8, arr2, arr2 + 1));
//   EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr3, arr3 + 2),
//             mystl::find_end(arr1, arr1 + 8, arr3, arr3 + 2));
//   EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr3, arr3 + 2),
//             mystl::find_end(arr1, arr1 + 8, arr3, arr3 + 2));
//   EXPECT_EQ(std::find_end(arr1, arr1 + 8, arr3, arr3, std::less<int>()),
//             mystl::find_end(arr1, arr1 + 8, arr3, arr3, std::less<int>()));
//   FINISHED(find_end_test)
// }

void find_first_of_test()
{ 
  TEST(find_first_of_test)
  int arr1[] = { 1,2,3,4,5 };
  int arr2[] = { 2,3,4 };
  int arr3[] = { 6,7,8 };
  EXPECT_EQ(std::find_first_of(arr1, arr1 + 5, arr2, arr2 + 3),
            mystl::find_first_of(arr1, arr1 + 5, arr2, arr2 + 3));
  EXPECT_EQ(std::find_first_of(arr1, arr1 + 5, arr3, arr3 + 3, std::equal_to<int>()),
            mystl::find_first_of(arr1, arr1 + 5, arr3, arr3 + 3, std::equal_to<int>()));
  FINISHED(find_first_of_test)
}

void find_if_test()
{ 
  TEST(find_if_test)
  int arr1[] = { 1,2,3,4,5 };
  EXPECT_EQ(std::find_if(arr1, arr1 + 5, is_odd),
            mystl::find_if(arr1, arr1 + 5, is_odd));
  EXPECT_EQ(std::find_if(arr1, arr1 + 5, is_even),
            mystl::find_if(arr1, arr1 + 5, is_even));
  FINISHED(find_if_test)
}

void find_if_not_test()
{ 
  TEST(find_if_not_test)
  int arr1[] = { 1,2,3,4,5 };
  EXPECT_EQ(std::find_if_not(arr1, arr1 + 5, is_odd),
            mystl::find_if_not(arr1, arr1 + 5, is_odd));
  EXPECT_EQ(std::find_if_not(arr1, arr1 + 5, is_even),
            mystl::find_if_not(arr1, arr1 + 5, is_even));
  FINISHED(find_if_not_test)
}

void for_each_test()
{ 
  TEST(for_each_test)
  std::vector<int> v1{ 1,2,3,4,5 };
  std::for_each(v1.begin(), v1.end(), arr_sum);
  EXPECT_EQ(15, for_each_sum);
  mystl::for_each(v1.begin(), v1.end(), arr_sum);
  EXPECT_EQ(30, for_each_sum);
  FINISHED(for_each_test)
}

// void generate_test()
// { 
//   TEST(generate_test)
//   int arr1[5];
//   int arr2[5];
//   std::generate(arr1, arr1 + 5, gen);
//   mystl::generate(arr2, arr2 + 5, gen);
//   CHECK(arr1, arr2);
//   FINISHED(generate_test)
// }

void generate_n_test()
{ 
  TEST(generate_n_test)
  int arr1[] = { 1,1,1,1,1,6,7,8,9,10 };
  int arr2[] = { 2,2,2,2,2,6,7,8,9,10 };
  std::generate_n(arr1, 5, gen);
  mystl::generate_n(arr2, 5, gen);
  CHECK(arr1, arr2);
  FINISHED(generate_n_test)
}

void includes_test()
{ 
  TEST(includes_test)
  std::vector<int> v1{ 1,2,3,4,5,6,7,8,9 };
  std::vector<int> v2{ 2,3,5,6,9 };
  std::vector<int> v3{ 0,1,2,3,4 };
  std::vector<int> v4{ 1,2,5,7,10 };
  std::vector<int> v5;
  EXPECT_EQ(std::includes(v1.begin(), v1.end(), v2.begin(), v2.end()),
            mystl::includes(v1.begin(), v1.end(), v2.begin(), v2.end()));
  EXPECT_EQ(std::includes(v1.begin(), v1.end(), v3.begin(), v3.end()),
            mystl::includes(v1.begin(), v1.end(), v3.begin(), v3.end()));
  EXPECT_EQ(std::includes(v1.begin(), v1.end(), v4.begin(), v4.end()),
            mystl::includes(v1.begin(), v1.end(), v4.begin(), v4.end()));
  EXPECT_EQ(std::includes(v1.begin(), v1.end(), v5.begin(), v5.end(), std::less<int>()),
            mystl::includes(v1.begin(), v1.end(), v5.begin(), v5.end(), std::less<int>()));
  FINISHED(includes_test)
}

// void inplace_merge_test()
// { 
//   TEST(inplace_merge_test)
//   int arr1[] = { 1,3,5,7,9,2,4,6,8,10 };
//   int arr2[] = { 1,3,5,7,9,2,4,6,8,10 };
//   int arr3[] = { 1,2,3,1,2,3,4,5 };
//   int arr4[] = { 1,2,3,1,2,3,4,5 };
//   std::inplace_merge(arr1, arr1 + 5, arr1 + 10);
//   mystl::inplace_merge(arr2, arr2 + 5, arr2 + 10);
//   std::inplace_merge(arr3, arr3 + 3, arr3 + 8, std::less<int>());
//   mystl::inplace_merge(arr4, arr4 + 3, arr4 + 8, std::less<int>());
//   CHECK(arr1, arr2);
//   CHECK(arr3, arr4);
//   FINISHED(inplace_merge_test)
// }

// void is_heap_test()
// { 
//   TEST(is_heap_test)
//   int arr1[] = { 1,2,3,4,5,6,7,8,9 };
//   int arr2[] = { 9,8,7,6,5,4,3,2,1 };
//   int arr3[] = { 1,3,5,7,9,2,4,6,8 };
//   int arr4[] = { 1,2,3,4,5,6,7,8,9 };
//   std::make_heap(arr4, arr4 + 10);
//   EXPECT_EQ(std::is_heap(arr1, arr1 + 10), mystl::is_heap(arr1, arr1 + 10));
//   EXPECT_EQ(std::is_heap(arr2, arr2 + 10, std::less<int>()),
//             mystl::is_heap(arr2, arr2 + 10, std::less<int>()));
//   EXPECT_EQ(std::is_heap(arr3, arr3 + 10), mystl::is_heap(arr3, arr3 + 10));
//   EXPECT_EQ(std::is_heap(arr4, arr4 + 10), mystl::is_heap(arr4, arr4 + 10));
//   TEST(is_heap_test)
// }

void is_sorted_test()
{ 
  TEST(is_sorted_test)
  int arr1[] = { 1,2,3,4,5 };
  int arr2[] = { 1,2,3,5,4 };
  int arr3[] = { 5,4,3,2,1 };
  int arr4[] = { 1,2,5,4,3 };
  EXPECT_EQ(std::is_sorted(arr1, arr1 + 5), mystl::is_sorted(arr1, arr1 + 5));
  EXPECT_EQ(std::is_sorted(arr2, arr2 + 5), mystl::is_sorted(arr2, arr2 + 5));
  EXPECT_EQ(std::is_sorted(arr3, arr3 + 5, std::less<int>()),
            mystl::is_sorted(arr3, arr3 + 5, std::less<int>()));
  EXPECT_EQ(std::is_sorted(arr4, arr4 + 5, std::less<int>()),
            mystl::is_sorted(arr4, arr4 + 5, std::less<int>()));
  FINISHED(is_sorted_test)
}

void lower_bound_test()
{ 
  TEST(lower_bound_test)
  int arr1[] = { 1,2,3,3,3,4,5 };
  EXPECT_EQ(std::lower_bound(arr1, arr1 + 7, 1),
            mystl::lower_bound(arr1, arr1 + 7, 1));
  EXPECT_EQ(std::lower_bound(arr1, arr1 + 7, 2),
            mystl::lower_bound(arr1, arr1 + 7, 2));
  EXPECT_EQ(std::lower_bound(arr1, arr1 + 7, 3),
            mystl::lower_bound(arr1, arr1 + 7, 3));
  EXPECT_EQ(std::lower_bound(arr1, arr1 + 7, 5, std::less<int>()),
            mystl::lower_bound(arr1, arr1 + 7, 5, std::less<int>()));
  FINISHED(lower_bound_test)
}

void max_elememt_test()
{ 
  TEST(max_elememt_test)
  int arr1[] = { 1,2,3,4,5,4,3,2,1 };
  double arr2[] = { 1.0,2.2,6.6,8.8,8.81,2.4 };
  EXPECT_PTR_EQ(std::max_element(arr1, arr1 + 9),
                mystl::max_element(arr1, arr1 + 9));
  EXPECT_PTR_EQ(std::max_element(arr2, arr2 + 6, std::less<double>()),
                mystl::max_element(arr2, arr2 + 6, std::less<double>()));
  FINISHED(max_elememt_test)
}

void median_test()
{ 
  TEST(median_test)
  int ia = 2, ib = 1, ic = 3;
  double da = 3.1, db = 3.0, dc = 3.2;
  EXPECT_EQ(2, mystl::median(ia, ib, ic));
  EXPECT_EQ(3.1, mystl::median(da, db, dc, std::greater<double>()));
  FINISHED(median_test)
}

// TEST(merge_test)
// { 
//   TEST(merge_test)
//   int arr1[] = { 1,2,5,9,10 };
//   int arr2[] = { 3,7,8,8,9 };
//   int arr3[] = { 1,2,5,9,10 };
//   int arr4[] = { 3,7,8,8,9 };
//   int exp[10], act[10];
//   std::merge(arr1, arr1 + 5, arr2, arr2 + 5, exp);
//   mystl::merge(arr3, arr3 + 5, arr4, arr4 + 5, act);
//   CHECK(exp, act);
//   std::merge(arr1, arr1 + 5, arr2, arr2 + 5, exp, std::less<int>());
//   mystl::merge(arr3, arr3 + 5, arr4, arr4 + 5, act, std::less<int>());
//   CHECK(exp, act);
//   FINISHED(merge_test)
// }

void min_elememt_test()
{ 
  TEST(min_elememt_test)
  int arr1[] = { 2,4,8,1,6,5,8,9,3 };
  double arr2[] = { 1.5,2.2,1.4,1.33,1.333,2.33 };
  EXPECT_PTR_EQ(std::max_element(arr1, arr1 + 9),
                mystl::max_element(arr1, arr1 + 9));
  EXPECT_PTR_EQ(std::max_element(arr2, arr2 + 6, std::less<double>()),
                mystl::max_element(arr2, arr2 + 6, std::less<double>()));
  FINISHED(min_elememt_test)
}

void is_permutation_test()
{ 
  TEST(is_permutation_test)
  int arr1[] = { 1,2,3,4,5 };
  int arr2[] = { 3,4,5,2,1 };
  int arr3[] = { 1,2,3,4,6 };
  // 因为提供的是 C++11 的支持，std::is_permutation 可能没有 C++14 的接口
  EXPECT_EQ(std::is_permutation(arr1, arr1 + 5, arr2),
            mystl::is_permutation(arr1, arr1 + 5, arr2, arr2 + 5));
  EXPECT_EQ(std::is_permutation(arr1, arr1 + 5, arr3),
            mystl::is_permutation(arr1, arr1 + 5, arr3, arr3 + 5));
  EXPECT_EQ(std::is_permutation(arr1, arr1 + 5, arr2, std::equal_to<int>()),
            mystl::is_permutation(arr1, arr1 + 5, arr2, arr2 + 5, std::equal_to<int>()));
  EXPECT_EQ(std::is_permutation(arr1, arr1 + 5, arr3, std::equal_to<int>()),
            mystl::is_permutation(arr1, arr1 + 5, arr3, arr3 + 5, std::equal_to<int>()));
  FINISHED(is_permutation_test)
}

void next_permutation_test()
{ 
  TEST(next_permutation_test)
  int arr1[] = { 1,2,3,3 };
  int arr2[] = { 1,2,3,3 };
  int n1 = 0, n2 = 0;
  while (std::next_permutation(arr1, arr1 + 4))    n1++;
  while (mystl::next_permutation(arr1, arr1 + 4))    n2++;
  EXPECT_EQ(n1, n2);
  for (; n1 > 0; n1--)
  {
    std::next_permutation(arr1, arr1 + 4);
    mystl::next_permutation(arr2, arr2 + 4);
    CHECK(arr1, arr2);
  }
  std::next_permutation(arr1, arr1 + 4, std::greater<int>());
  mystl::next_permutation(arr2, arr2 + 4, std::greater<int>());
  CHECK(arr1, arr2);
  FINISHED(next_permutation_test)
}

void none_of_test()
{ 
  TEST(none_of_test)
  int arr1[] = { 1,3,5,7,9 };
  int arr2[] = { 1,3,5,7,8 };
  int arr3[] = { 2,4,6,8,10 };
  EXPECT_EQ(std::none_of(arr1, arr1 + 5, is_even),
            mystl::none_of(arr1, arr1 + 5, is_even));
  EXPECT_EQ(std::none_of(arr2, arr2 + 5, is_even),
            mystl::none_of(arr2, arr2 + 5, is_even));
  EXPECT_EQ(std::none_of(arr3, arr3 + 5, is_even),
            mystl::none_of(arr3, arr3 + 5, is_even));
  FINISHED(none_of_test)
}

void nth_element_test()
{ 
  TEST(nth_element_test)
  int arr1[] = { 9,8,7,6,5,4,3,2,1 };
  int arr2[] = { 1,2,3,4,5,6,3,2,1 };
  int arr3[] = { 1,2,8,9,6,5,3,4,7 };
  int arr4[] = { 1,5,1,5,8,4,9,6,8,4,10,13,20,4,2,1 };
  mystl::nth_element(arr1, arr1 + 4, arr1 + 9);
  mystl::nth_element(arr2, arr2 + 2, arr2 + 9);
  mystl::nth_element(arr3, arr3 + 8, arr3 + 9, std::less<int>());
  mystl::nth_element(arr4, arr4 + 3, arr4 + 16, std::less<int>());
  bool arr1_left_less = true, arr1_right_greater = true;
  bool arr2_left_less = true, arr2_right_greater = true;
  bool arr3_left_less = true, arr3_right_greater = true;
  bool arr4_left_less = true, arr4_right_greater = true;
  for (int i = 0; i < 9; ++i)
  {
    if (i < 4 && arr1[i] > arr1[4])    arr1_left_less = false;
    else if (i > 4 && arr1[i] < arr1[4])    arr1_right_greater = false;
  }
  for (int i = 0; i < 9; ++i)
  {
    if (i < 2 && arr2[i] > arr2[2])    arr2_left_less = false;
    else if (i > 2 && arr2[i] < arr2[2])    arr2_right_greater = false;
  }
  for (int i = 0; i < 9; ++i)
  {
    if (i < 8 && arr3[i] > arr3[8])    arr3_left_less = false;
    else if (i > 8 && arr3[i] < arr3[8])    arr3_right_greater = false;
  }
  for (int i = 0; i < 16; ++i)
  {
    if (i < 3 && arr4[i] > arr4[3])    arr4_left_less = false;
    else if (i > 3 && arr4[i] < arr4[3])    arr4_right_greater = false;
  }
  EXPECT_TRUE(arr1_left_less);
  EXPECT_TRUE(arr1_right_greater);
  EXPECT_TRUE(arr2_left_less);
  EXPECT_TRUE(arr2_right_greater);
  EXPECT_TRUE(arr3_left_less);
  EXPECT_TRUE(arr3_right_greater);
  EXPECT_TRUE(arr4_left_less);
  EXPECT_TRUE(arr4_right_greater);
  FINISHED(nth_element_test)
}

void partial_sort_test()
{ 
  TEST(partial_sort_test)
  int arr1[] = { 3,2,1,9,8,7,6,5,4 };
  int arr2[] = { 3,2,1,9,8,7,6,5,4 };
  int arr3[] = { 5,1,5,8,6,4,8,4,1,3,5,8,4 };
  int arr4[] = { 5,1,5,8,6,4,8,4,1,3,5,8,4 };
  std::partial_sort(arr1, arr1 + 2, arr1 + 9);
  mystl::partial_sort(arr2, arr2 + 2, arr2 + 9);
  std::partial_sort(arr3, arr3 + 5, arr3 + 13, std::greater<int>());
  mystl::partial_sort(arr4, arr4 + 5, arr4 + 13, std::greater<int>());
  CHECK(arr1, arr2);
  CHECK(arr3, arr4);
  FINISHED(partial_sort_test)
}

void partial_sort_copy_test()
{ 
  TEST(partial_sort_copy_test)
  int arr1[] = { 3,2,1,9,8,7,6,5,4 };
  int arr2[] = { 1,2,3,4,5,6,7,8,9 };
  int arr3[] = { 1,6,8,4,2,1,6,8,4,7,6,2,1,3,6 };
  int exp[5], act[5];
  std::partial_sort_copy(arr1, arr1 + 9, exp, exp + 5);
  mystl::partial_sort_copy(arr1, arr1 + 9, act, act + 5);
  CHECK(exp, act);
  std::partial_sort_copy(arr2, arr2 + 9, exp, exp + 5);
  mystl::partial_sort_copy(arr2, arr2 + 9, act, act + 5);
  CHECK(exp, act);
  std::partial_sort_copy(arr3, arr3 + 15, exp, exp + 5, std::greater<int>());
  mystl::partial_sort_copy(arr3, arr3 + 15, act, act + 5, std::greater<int>());
  CHECK(exp, act);
  FINISHED(partial_sort_copy_test)
}

void partition_test()
{ 
  TEST(partition_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9 };
  int arr2[] = { 1,2,3,4,5,6,7,8,9 };
  std::partition(arr1, arr1 + 9, is_odd);
  mystl::partition(arr2, arr2 + 9, is_odd);
  CHECK(arr1, arr2);
  std::partition(arr1, arr1 + 9, is_even);
  mystl::partition(arr2, arr2 + 9, is_even);
  CHECK(arr1, arr2);
  FINISHED(partition_test)
}

void partition_copy_test()
{ 
  TEST(partition_copy_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
  int exp_true[5], exp_false[5];
  int act_true[5], act_false[5];
  std::partition_copy(arr1, arr1 + 10, exp_true, exp_false, is_odd);
  mystl::partition_copy(arr1, arr1 + 10, act_true, act_false, is_odd);
  CHECK(exp_true, act_true);
  CHECK(exp_false, act_false);
  FINISHED(partition_copy_test)
}

void prev_permutation_test()
{ 
  TEST(prev_permutation_test)
  int arr1[] = { 3,2,1,1 };
  int arr2[] = { 3,2,1,1 };
  int n1 = 0, n2 = 0;
  while (std::prev_permutation(arr1, arr1 + 4))    n1++;
  while (mystl::prev_permutation(arr1, arr1 + 4))    n2++;
  EXPECT_EQ(n1, n2);
  for (; n1 > 0; n1--)
  {
    std::prev_permutation(arr1, arr1 + 4);
    mystl::prev_permutation(arr2, arr2 + 4);
    CHECK(arr1, arr2);
  }
  std::prev_permutation(arr1, arr1 + 4, std::greater<int>());
  mystl::prev_permutation(arr2, arr2 + 4, std::greater<int>());
  CHECK(arr1, arr2);
  FINISHED(prev_permutation_test)
}

void random_shuffle_test()
{ 
  TEST(random_shuffle_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9 };
  int arr2[9];
  for (int i = 0; i < 9; ++i)
  {
    std::copy(arr1, arr1 + 9, arr2);
    mystl::random_shuffle(arr1, arr1 + 9);
    EXPECT_CON_NE(arr1, arr2);
  }
  std::copy(arr1, arr1 + 9, arr2);
  mystl::random_shuffle(arr1, arr1 + 9, r);
  EXPECT_CON_NE(arr1, arr2);
  FINISHED(random_shuffle_test)
}

void remove_test()
{ 
  TEST(remove_test)
  std::vector<int> v1{ 1,2,3,4,5,6,6,6 };
  std::vector<int> v2(v1);
  std::remove(v1.begin(), v1.end(), 3);
  mystl::remove(v2.begin(), v2.end(), 3);
  CHECK(v1, v2);
  std::remove(v1.begin(), v1.end(), 6);
  mystl::remove(v2.begin(), v2.end(), 6);
  CHECK(v1, v2);
  FINISHED(remove_test)
}

void remove_copy_test()
{ 
  TEST(remove_copy_test)
  int arr1[] = { 1,2,3,4,5,6,6,6 };
  int arr2[] = { 1,2,3,4,5,6,6,6 };
  int exp[5], act[5];
  std::remove_copy(arr1, arr1 + 8, exp, 6);
  mystl::remove_copy(arr2, arr2 + 8, act, 6);
  CHECK(exp, act);
  std::remove_copy(arr1, arr1 + 5, exp, 3);
  mystl::remove_copy(arr2, arr2 + 5, act, 3);
  CHECK(exp, act);
  FINISHED(remove_copy_test)
}

void remove_copy_if_test()
{ 
  TEST(remove_copy_if_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
  int exp[5], act[5];
  std::remove_copy_if(arr1, arr1 + 10, exp, is_odd);
  mystl::remove_copy_if(arr1, arr1 + 10, act, is_odd);
  CHECK(exp, act);
  std::remove_copy_if(arr1, arr1 + 10, exp, is_even);
  mystl::remove_copy_if(arr1, arr1 + 10, act, is_even);
  CHECK(exp, act);
  FINISHED(remove_copy_if_test)
}

void remove_if_test()
{ 
  TEST(remove_if_test)
  std::vector<int> v1{ 1,2,3,4,5,6,7,8,9,10 };
  std::vector<int> v2(v1);
  std::remove_if(v1.begin(), v1.end(), is_odd);
  mystl::remove_if(v2.begin(), v2.end(), is_odd);
  CHECK(v1, v2);
  FINISHED(remove_if_test)
}

void replace_test()
{ 
  TEST(replace_test)
  int arr1[] = { 1,1,1,2,2,2,3,3,3 };
  int arr2[] = { 1,1,1,2,2,2,3,3,3 };
  std::replace(arr1, arr1 + 9, 1, 4);
  mystl::replace(arr2, arr2 + 9, 1, 4);
  CHECK(arr1, arr2);
  std::replace(arr1, arr1 + 9, 2, 5);
  mystl::replace(arr2, arr2 + 9, 2, 5);
  CHECK(arr1, arr2);
  std::replace(arr1, arr1 + 9, 3, 6);
  mystl::replace(arr2, arr2 + 9, 3, 6);
  CHECK(arr1, arr2);
  FINISHED(replace_test)
}

void replace_copy_test()
{ 
  TEST(replace_copy_test)
  int arr1[] = { 1,1,1,2,2,2,3,3,3 };
  int exp[9], act[9];
  std::replace_copy(arr1, arr1 + 9, exp, 1, 4);
  mystl::replace_copy(arr1, arr1 + 9, act, 1, 4);
  CHECK(exp, act);
  std::replace_copy(arr1, arr1 + 9, exp, 2, 5);
  mystl::replace_copy(arr1, arr1 + 9, act, 2, 5);
  CHECK(exp, act);
  std::replace_copy(arr1, arr1 + 9, exp, 3, 6);
  mystl::replace_copy(arr1, arr1 + 9, act, 3, 6);
  CHECK(exp, act);
  FINISHED(replace_copy_test)
}

void replace_copy_if_test()
{ 
  TEST(replace_copy_if_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9,10 };
  int exp[10] = { 0 }, act[10] = { 0 };
  std::replace_copy_if(arr1, arr1 + 10, exp, is_odd, 1);
  mystl::replace_copy_if(arr1, arr1 + 10, act, is_odd, 1);
  CHECK(exp, act);
  std::replace_copy_if(arr1, arr1 + 10, exp, is_even, 2);
  mystl::replace_copy_if(arr1, arr1 + 10, act, is_even, 2);
  CHECK(exp, act);
  FINISHED(replace_copy_if_test)
}

void replace_if_test()
{ 
  TEST(replace_if_test)
  std::vector<int> v1{ 1,2,3,4,5,6,7,8,9,10 };
  std::vector<int> v2(v1);
  std::replace_if(v1.begin(), v1.end(), is_odd, 1);
  mystl::replace_if(v2.begin(), v2.end(), is_odd, 1);
  CHECK(v1, v2);
  std::replace_if(v1.begin(), v1.end(), is_even, 2);
  mystl::replace_if(v2.begin(), v2.end(), is_even, 2);
  CHECK(v1, v2);
  FINISHED(replace_if_test)
}

void reverse_test()
{ 
  TEST(reverse_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9 };
  int arr2[] = { 1,2,3,4,5,6,7,8,9 };
  std::reverse(arr1, arr1 + 9);
  mystl::reverse(arr2, arr2 + 9);
  CHECK(arr1, arr2);
  std::reverse(arr1, arr1 + 5);
  mystl::reverse(arr2, arr2 + 5);
  CHECK(arr1, arr2);
  FINISHED(reverse_test)
}

void reverse_copy_test()
{ 
  TEST(reverse_copy_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9 };
  int exp[5], act[5];
  std::reverse_copy(arr1, arr1 + 5, exp);
  mystl::reverse_copy(arr1, arr1 + 5, act);
  CHECK(exp, act);
  std::reverse_copy(arr1 + 4, arr1 + 9, exp);
  mystl::reverse_copy(arr1 + 4, arr1 + 9, act);
  CHECK(exp, act);
  FINISHED(reverse_copy_test)
}

// void rotate_test()
// { 
//   TEST(rotate_test)
//   int arr1[] = { 1,2,3,4,5,6,7,8,9 };
//   int arr2[] = { 1,2,3,4,5,6,7,8,9 };
//   std::rotate(arr1, arr1 + 3, arr1 + 9);
//   mystl::rotate(arr2, arr2 + 3, arr2 + 9);
//   CHECK(arr1, arr2);
//   std::rotate(arr1 + 3, arr1 + 5, arr1 + 9);
//   mystl::rotate(arr2 + 3, arr2 + 5, arr2 + 9);
//   CHECK(arr1, arr2);
//   std::rotate(arr1, arr1 + 9, arr1 + 9);
//   mystl::rotate(arr2, arr2 + 9, arr2 + 9);
//   CHECK(arr1, arr2);
//   FINISHED(rotate_test)
// }

// TEST(rotate_copy_test)
// {
//   int arr1[] = { 1,2,3,4,5,6,7,8,9 };
//   int exp[9], act[9];
//   std::rotate_copy(arr1, arr1 + 5, arr1 + 9, exp);
//   mystl::rotate_copy(arr1, arr1 + 5, arr1 + 9, act);
//   CHECK(exp, act);
//   std::rotate_copy(arr1, arr1, arr1 + 9, exp);
//   mystl::rotate_copy(arr1, arr1, arr1 + 9, act);
//   CHECK(exp, act);
//   std::rotate_copy(arr1, arr1 + 9, arr1 + 9, exp);
//   mystl::rotate_copy(arr1, arr1 + 9, arr1 + 9, act);
//   CHECK(exp, act);
// }

void search_test()
{ 
  TEST(search_test)
  int arr1[] = { 1,2,3,3,3,4,5,6,6, };
  int arr2[] = { 1 };
  int arr3[] = { 3,3 };
  int arr4[] = { 5,6,6,6 };
  EXPECT_EQ(std::search(arr1, arr1 + 9, arr2, arr2 + 1),
            mystl::search(arr1, arr1 + 9, arr2, arr2 + 1));
  EXPECT_EQ(std::search(arr1, arr1 + 9, arr3, arr3 + 2),
            mystl::search(arr1, arr1 + 9, arr3, arr3 + 2));
  EXPECT_EQ(std::search(arr1, arr1 + 9, arr4, arr4 + 3, std::less<int>()),
            mystl::search(arr1, arr1 + 9, arr4, arr4 + 3, std::less<int>()));
  EXPECT_EQ(std::search(arr1, arr1 + 9, arr4, arr4 + 4, std::less<int>()),
            mystl::search(arr1, arr1 + 9, arr4, arr4 + 4, std::less<int>()));
  FINISHED(search_test)
}

void search_n_test()
{ 
  TEST(search_n_test)
  int arr1[] = { 1,2,2,3,3,3,6,6,9 };
  EXPECT_EQ(std::search_n(arr1, arr1 + 9, 1, 0),
            mystl::search_n(arr1, arr1 + 9, 1, 0));
  EXPECT_EQ(std::search_n(arr1, arr1 + 9, 2, 2),
            mystl::search_n(arr1, arr1 + 9, 2, 2));
  EXPECT_EQ(std::search_n(arr1, arr1 + 9, 1, 3),
            mystl::search_n(arr1, arr1 + 9, 1, 3));
  EXPECT_EQ(std::search_n(arr1, arr1 + 9, 3, 6, std::less<int>()),
            mystl::search_n(arr1, arr1 + 9, 3, 6, std::less<int>()));
  EXPECT_EQ(std::search_n(arr1, arr1 + 9, 2, 10, std::less<int>()),
            mystl::search_n(arr1, arr1 + 9, 2, 10, std::less<int>()));
  FINISHED(search_n_test)
}

void sort_test()
{ 
  TEST(sort_test)
  int arr1[] = { 6,1,2,5,4,8,3,2,4,6,10,2,1,9 };
  int arr2[] = { 6,1,2,5,4,8,3,2,4,6,10,2,1,9 };
  int arr3[] = { 80,30,51,65,12,10,24,87,62,51,32,45,1,33,66,20,35,84,62,14 };
  int arr4[] = { 80,30,51,65,12,10,24,87,62,51,32,45,1,33,66,20,35,84,62,14 };
  int arr5[] = { 9,9,9,8,8,8,7,7,7 };
  int arr6[] = { 9,9,9,8,8,8,7,7,7 };
  std::sort(arr1, arr1 + 14);
  mystl::sort(arr2, arr2 + 14);
  std::sort(arr3, arr3 + 20);
  mystl::sort(arr4, arr4 + 20);
  std::sort(arr5, arr5 + 9, std::greater<int>());
  mystl::sort(arr6, arr6 + 9, std::greater<int>());
  CHECK(arr1, arr2);
  CHECK(arr3, arr4);
  CHECK(arr5, arr6);
  FINISHED(sort_test)
}

void swap_ranges_test()
{ 
  TEST(swap_ranges_test)
  int arr1[] = { 4,5,6,1,2,3 };
  int arr2[] = { 4,5,6,1,2,3 };
  int arr3[] = { 1,2,3,4,5,6 };
  int arr4[] = { 1,2,3,4,5,6 };
  int arr5[] = { 1,1,1 };
  int arr6[] = { 1,1,1 };
  std::swap_ranges(arr1, arr1 + 6, arr3);
  mystl::swap_ranges(arr2, arr2 + 6, arr4);
  std::swap_ranges(arr1, arr1 + 3, arr5);
  mystl::swap_ranges(arr2, arr2 + 3, arr6);
  CHECK(arr1, arr2);
  CHECK(arr1, arr2);
  FINISHED(swap_ranges_test)
}

void transform_test()
{ 
  TEST(transform_test)
  int arr1[] = { 1,2,3,4,5,6,7,8,9 };
  int arr2[] = { 9,8,7,6,5,4,3,2,1 };
  int exp[9], act[9];
  std::transform(arr1, arr1 + 9, exp, unary_op);
  mystl::transform(arr1, arr1 + 9, act, unary_op);
  CHECK(exp, act);
  std::transform(arr1, arr1 + 9, arr2, exp, binary_op);
  mystl::transform(arr1, arr1 + 9, arr2, act, binary_op);
  CHECK(exp, act);
  FINISHED(transform_test)
}

void unique_test()
{ 
  TEST(unique_test)
  int arr1[] = { 1,1,1,2,2,3,4,4,5,6 };
  int arr2[] = { 1,1,1,2,2,3,4,4,5,6 };
  int arr3[] = { 1,2,3,6,6,6,8,8,9 };
  int arr4[] = { 1,2,3,6,6,6,8,8,9 };
  std::unique(arr1, arr1 + 10);
  mystl::unique(arr2, arr2 + 10);
  std::unique(arr3, arr3 + 9, std::equal_to<int>());
  std::unique(arr4, arr4 + 9, std::equal_to<int>());
  CHECK(arr1, arr2);
  CHECK(arr3, arr4);
  FINISHED(unique_test)
}

void unique_copy_test()
{ 
  TEST(unique_copy_test)
  int arr1[] = { 1,1,1,2,2,3,4,4,5,6 };
  int arr2[] = { 1,2,3,6,6,6,8,8,9 };
  int exp[6], act[6];
  std::unique_copy(arr1, arr1 + 10, exp);
  mystl::unique_copy(arr1, arr1 + 10, act);
  CHECK(exp, act);
  std::unique_copy(arr2, arr2 + 9, exp, std::equal_to<int>());
  mystl::unique_copy(arr2, arr2 + 9, act, std::equal_to<int>());
  CHECK(exp, act);
  FINISHED(unique_copy_test)
}

void upper_bound_test()
{ 
  TEST(upper_bound_test)
  int arr1[] = { 1,2,3,3,3,4,5,6,6 };
  EXPECT_EQ(std::upper_bound(arr1, arr1 + 9, 0),
            mystl::upper_bound(arr1, arr1 + 9, 0));
  EXPECT_EQ(std::upper_bound(arr1, arr1 + 9, 1),
            mystl::upper_bound(arr1, arr1 + 9, 1));
  EXPECT_EQ(std::upper_bound(arr1, arr1 + 9, 3),
            mystl::upper_bound(arr1, arr1 + 9, 3));
  EXPECT_EQ(std::upper_bound(arr1, arr1 + 9, 6, std::less<int>()),
            mystl::upper_bound(arr1, arr1 + 9, 6, std::less<int>()));
  EXPECT_EQ(std::upper_bound(arr1, arr1 + 9, 7, std::less<int>()),
            mystl::upper_bound(arr1, arr1 + 9, 7, std::less<int>()));
  FINISHED(upper_bound_test)
}



void algorithm_test(){
    // algo_base
    copy_test();
    copy_if_test();
    copy_n_test();
    move_test();
    move_backward_test();
    equal_test();
    fill_test();
    fill_n_test();
    iter_swap_test();
    lexicographical_compare_test();
    max_test();
    min_test();
    mismatch_test();
    //heap algorithm
    make_heap_test();
    pop_heap_test();
    push_heap_test();
    sort_heap_test();
    // algorithm
    adjacent_find_test();
    all_of_test();
    any_of_test();
    binary_search_test();
    count_test();
    count_if_test();
    equal_range_test();
    find_test();
    find_first_of_test();
    find_if_test();
    find_if_not_test();
    for_each_test();
    generate_n_test();
    includes_test();
    //is_heap_test();
    is_sorted_test();
    lower_bound_test();
    max_elememt_test();
    median_test();
    min_elememt_test();
    is_permutation_test();
    // next_permutation_test();
    none_of_test();
    nth_element_test();
    partial_sort_test();
    partial_sort_copy_test();
    partition_test();
    partition_copy_test();
    // prev_permutation_test();
    random_shuffle_test();
    remove_test();
    remove_copy_test();
    remove_copy_if_test();
    remove_if_test();
    replace_test();
    replace_copy_test();
    replace_copy_if_test();
    replace_if_test();
    reverse_test();
    reverse_copy_test();
    search_test();
    search_n_test();
    sort_test();
    swap_ranges_test();
    transform_test();
    unique_test();
    unique_copy_test();
    upper_bound_test();
}




}




// } // namespace test

// } // namsepace mystl


#endif






