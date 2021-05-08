#ifndef MYSTL_ALGO_H
#define MYSTL_ALGO_H


#include <cstddef>
#include <ctime>

#include "algobase.h"
#include "memory.h"
#include "heap_algo.h"
#include "functional.h"

namespace mystl
{

/*****************************************************************************************/
// all_of
// 检查[first, last)内是否全部元素都满足一元操作 unary_pred 为 true 的情况，满足则返回 true
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
bool all_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  for(; first != last; ++first)
  {
    if(!unary_pred(*first))
      return false;
  }
  return true;
}

/*****************************************************************************************/
// any_of
// 检查[first, last)内是否存在某个元素满足一元操作 unary_pred 为 true 的情况，满足则返回 true
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
bool any_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  for(; first != last; ++first)
  {
    if(unary_pred(*first))
      return true;
  }
  return false;
}

/*****************************************************************************************/
// none_of
// 检查[first, last)内是否全部元素都不满足一元操作 unary_pred 为 true 的情况，满足则返回 true
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
bool none_of(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  for(; first != last; ++first)
  {
    if(unary_pred(*first))
      return false;
  }
  return true;
}

/*****************************************************************************************/
// count
// 对[first, last)区间内的元素与给定值进行比较，缺省使用 operator==，返回元素相等的个数
/*****************************************************************************************/
template <class InputIter, class T>
size_t count(InputIter first, InputIter last, const T& value)
{
  size_t n = 0;
  for(; first != last; first++)
  {
    if(*first == value)
      ++n;
  }
  return n;
}

/*****************************************************************************************/
// count_if
// 对[first, last)区间内的每个元素都进行一元 unary_pred 操作，返回结果为 true 的个数
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
size_t count_if(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  size_t n = 0;
  for(; first != last; first++)
  {
    if(unary_pred(*first))
      ++n;
  }
  return n;
}

/*****************************************************************************************/
// find
// 在[first, last)区间内找到等于 value 的元素，返回指向该元素的迭代器
/*****************************************************************************************/
template <class InputIter, class T>
InputIter 
find(InputIter first, InputIter last, const T& value)
{
  while (first != last && *first != value)
    ++first;
  return first;
}

/*****************************************************************************************/
// find_if
// 在[first, last)区间内找到第一个令一元操作 unary_pred 为 true 的元素并返回指向该元素的迭代器
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
InputIter 
find_if(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  while (first != last && !unary_pred(*first))
    ++first;
  return first;
}

/*****************************************************************************************/
// find_if_not
// 在[first, last)区间内找到第一个令一元操作 unary_pred 为 false 的元素并返回指向该元素的迭代器
/*****************************************************************************************/
template <class InputIter, class UnaryPredicate>
InputIter 
find_if_not(InputIter first, InputIter last, UnaryPredicate unary_pred)
{
  while (first != last && unary_pred(*first))
    ++first;
  return first;
}

/*****************************************************************************************/
// search: 查找子序列
// 在[first1, last1)中查找[first2, last2)的首次出现点
/*****************************************************************************************/
template <class ForwardIter1, class ForwardIter2>
ForwardIter1
search(ForwardIter1 first1, ForwardIter1 last1,
       ForwardIter2 first2, ForwardIter2 last2)
{
  auto d1 = mystl::distance(first1, last1);
  auto d2 = mystl::distance(first2, last2);
  if(d1 < d2)
    return last1;
  auto current1 = first1;
  auto current2 = first2;
  while(current2 != last2)
  { // 匹配到相同的字符
    if(*current1 == *current2)
    {
      ++current1;
      ++current2;
    }
    // 没有匹配到相同的字符
    else
    { // 当S1的剩余字符程度 等于 S2 的字符长度， 而且还不相等的时候， 直接返回， 不再遍历
      if(d1 == d2)
      {
        return last1;
      }
      else{
        current1 = ++first1;
        current2 = first2;
        --d1;
      }
    }
  }
  return first1;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter1, class ForwardIter2, class Compared>
ForwardIter1
search(ForwardIter1 first1, ForwardIter1 last1,
       ForwardIter2 first2, ForwardIter2 last2, Compared comp)
{
  auto d1 = mystl::distance(first1, last1);
  auto d2 = mystl::distance(first2, last2);
  if (d1 < d2)
    return last1;
  auto current1 = first1;
  auto current2 = first2;
  while (current2 != last2)
  {
    if (comp(*current1, *current2))
    {
      ++current1;
      ++current2;
    }
    else
    {
      if (d1 == d2)
      {
        return last1;
      }
      else
      {
        current1 = ++first1;
        current2 = first2;
        --d1;
      }
    }
  }
  return first1;
}

/*****************************************************************************************/
// search_n
// 在[first, last)中查找连续 n 个 value 所形成的子序列，返回一个迭代器指向该子序列的起始处
/*****************************************************************************************/
template <class ForwardIter, class Size, class T>
ForwardIter
search_n(ForwardIter first, ForwardIter last, Size n, const T& value)
{
  if(n <= 0)
    return first;
  else{
    // 找到第一个值的位置
    first = mystl::find(first, last, value);
    while(first != last)
    {
      auto m = n - 1;
      auto i = first;
      ++i;
      // 值相同的话， 指针向前移动， 数量同时递减
      while(i != last && m != 0 && *i == value)
      {
        ++i;
        --m;
      }
      // 如果数量正好相等
      if(m == 0)
      {
        return first;
      }
      // 如果数量不相等， 则需要重新寻找first
      else{
        first = mystl::find(i, last, value);
      }
    }
    return last;
  }
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Size, class T, class Compared>
ForwardIter
search_n(ForwardIter first, ForwardIter last,
         Size n, const T& value, Compared comp)
{
  if (n <= 0)
  {
    return first;
  }
  else
  {
    while (first != last)
    {
      if (comp(*first, value))
        break;
      ++first;
    }
    while (first != last)
    {
      auto m = n - 1;
      auto i = first;
      ++i;
      while (i != last && m != 0 && comp(*i, value))
      {
        ++i;
        --m;
      }
      if (m == 0)
      {
        return first;
      }
      else
      {
        while (i != last)
        {
          if (comp(*i, value))
            break;
          ++i;
        }
        first = i;
      }
    }
    return last;
  }
}

/*****************************************************************************************/
// find_first_of
// 在[first1, last1)中查找[first2, last2)中的某些元素，返回指向第一次出现的元素的迭代器
/*****************************************************************************************/
template <class InputIter, class ForwardIter>
InputIter
find_first_of(InputIter first1, InputIter last1,
              ForwardIter first2, ForwardIter last2)
{
  for(; first1 != last1; ++first1)
  {
    for(auto iter = first2; iter != last2; ++iter)
    {
      if(*first1 == *iter)
        return first1;
    }
  }
  return last1;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter, class ForwardIter, class Compared>
InputIter
find_first_of(InputIter first1, InputIter last1,
              ForwardIter first2, ForwardIter last2, Compared comp)
{
  for (; first1 != last1; ++first1)
  {
    for (auto iter = first2; iter != last2; ++iter)
    {
      if (comp(*first1, *iter))
        return first1;
    }
  }
  return last1;
}

/*****************************************************************************************/
// for_each
// 使用一个函数对象 f 对[first, last)区间内的每个元素执行一个 operator() 操作，但不能改变元素内容
// f() 可返回一个值，但该值会被忽略
/*****************************************************************************************/
template <class InputIter, class Function>
Function for_each(InputIter first, InputIter last, Function f)
{
  for(; first != last; ++first)
  {
    f(*first);
  }
  return f;
}

/*****************************************************************************************/
// adjacent_find
// 找出第一对匹配的相邻元素，缺省使用 operator== 比较，如果找到返回一个迭代器，指向这对元素的第一个元素
/*****************************************************************************************/
template <class ForwardIter>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last)
{
  if(first == last)
    return last;
  auto next = first;
  while(++next != last)
  {
    if(*first == *next)
      return first;
    first = next;
  }
  return last;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
ForwardIter adjacent_find(ForwardIter first, ForwardIter last, Compared comp)
{
  if (first == last)  return last;
  auto next = first;
  while (++next != last)
  {
    if (comp(*first, *next))  return first;
    first = next;
  }
  return last;
}

/*****************************************************************************************/
// lower_bound
// 在[first, last)中查找第一个不小于 value 的元素，并返回指向它的迭代器，若没有则返回 last
/*****************************************************************************************/
// lbound_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T>
ForwardIter
lbound_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle;
  // 二分查找
  while(len > 0)
  {
    half = len >> 1;
    middle = first;
    // advance: 让迭代器前进 n 个距离
    mystl::advance(middle, half);
    // 由于是找第一个大于等于的值的位置
    // 所以 *middle 严格小于 value 的时候, 要继续向右边寻找
    if(*middle < value)
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    // 不然就向左边寻找
    else{
      len = half;  // 这一句就相当于右边界向左收缩了
    }
  }
  return first;
}

// lbound_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter, class T>
RandomIter
lbound_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (*middle < value)
    {
      first = middle + 1;
      len = len - half - 1;
    }
    else
    {
      len = half;
    }
  }
  return first;
}

template <class ForwardIter, class T>
ForwardIter
lower_bound(ForwardIter first, ForwardIter last, const T& value)
{
  return mystl::lbound_dispatch(first, last, value, iterator_category(first));
}

// 重载版本使用函数对象 comp 代替比较操作
// lbound_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T, class Compared>
ForwardIter
lbound_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag, Compared comp)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first;
    mystl::advance(middle, half);
    if (comp(*middle, value))
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else
    {
      len = half;
    }
  }
  return first;
}

// lbound_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter, class T, class Compared>
RandomIter
lbound_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag, Compared comp)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (comp(*middle, value))
    {
      first = middle + 1;
      len = len - half - 1;
    }
    else
    {
      len = half;
    }
  }
  return first;
}

template <class ForwardIter, class T, class Compared>
ForwardIter
lower_bound(ForwardIter first, ForwardIter last, const T& value, Compared comp)
{
  return mystl::lbound_dispatch(first, last, value, iterator_category(first), comp);
}

/*****************************************************************************************/
// upper_bound
// 在[first, last)中查找第一个大于value 的元素，并返回指向它的迭代器，若没有则返回 last
/*****************************************************************************************/
// ubound_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T>
ForwardIter
ubound_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first;
    mystl::advance(middle, half);
    if (value < *middle)
    {
      len = half;
    }
    // 由于是要找第一个大于value的元素的位置， 取其补集，当 *middle 严格小于等于value的时候， 向右边继续寻找
    else
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}

// ubound_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter, class T>
RandomIter
ubound_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (value < *middle)
    {
      len = half;
    }
    else
    {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}

template <class ForwardIter, class T>
ForwardIter
upper_bound(ForwardIter first, ForwardIter last, const T& value)
{
  return mystl::ubound_dispatch(first, last, value, iterator_category(first));
}

// 重载版本使用函数对象 comp 代替比较操作
// ubound_dispatch 的 forward_iterator_tag 版本
template <class ForwardIter, class T, class Compared>
ForwardIter
ubound_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag, Compared comp)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first;
    mystl::advance(middle, half);
    if (comp(value, *middle))
    {
      len = half;
    }
    else
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}

// ubound_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter, class T, class Compared>
RandomIter
ubound_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag, Compared comp)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (comp(value, *middle))
    {
      len = half;
    }
    else
    {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}

template <class ForwardIter, class T, class Compared>
ForwardIter
upper_bound(ForwardIter first, ForwardIter last, const T& value, Compared comp)
{
  return mystl::ubound_dispatch(first, last, value, iterator_category(first), comp);
}

/*****************************************************************************************/
// binary_search
// 二分查找，如果在[first, last)内有等同于 value 的元素，返回 true，否则返回 false
/*****************************************************************************************/
template <class ForwardIter, class T>
bool binary_search(ForwardIter first, ForwardIter last, const T& value)
{ // i 是保证第一个大于等于value的元素，第二行 保证 *i 小于等于value, 所以可以保证 i 等于 value 
  auto i = mystl::lower_bound(first, last, value);
  return i != last && !(value < *i);
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class T, class Compared>
bool binary_search(ForwardIter first, ForwardIter last, const T& value, Compared comp)
{
  auto i = mystl::lower_bound(first, last, value);
  return i != last && !comp(value, *i);
}

/*****************************************************************************************/
// equal_range
// 查找[first,last)区间中与 value 相等的元素所形成的区间，返回一对迭代器指向区间首尾
// 第一个迭代器指向第一个不小于 value 的元素，第二个迭代器指向第一个大于 value 的元素
/*****************************************************************************************/
template <class ForwardIter, class T>
mystl::pair<ForwardIter, ForwardIter>
erange_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle, left, right;
  while(len > 0)
  {
    half = len >> 1;
    middle = first;
    mystl::advance(middle, half);
    if(*middle < value)
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else if(value < *middle)
    {
      len = half;
    }
    // 相比lower_bound 而言， 多了下面一个else
    // value == *middle
    else
    { // 如果遇到相等的值， 则继续寻找其左右边界
      left = mystl::lower_bound(first, last, value);
      mystl::advance(first, len);
      right = mystl::upper_bound(++middle, first, value);
      return mystl::pair<ForwardIter, ForwardIter>(left, right);
    }
  }
  // 如果没有找到相等的元素， 则返回两个last
  return mystl::pair<ForwardIter, ForwardIter>(last, last);
}

// erange_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter, class T>
mystl::pair<RandomIter, RandomIter>
erange_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle, left, right;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (*middle < value)
    {
      first = middle + 1;
      len = len - half - 1;
    }
    else if (value < *middle)
    {
      len = half;
    }
    else
    {
      left = mystl::lower_bound(first, middle, value);
      right = mystl::upper_bound(++middle, first + len, value);
      return mystl::pair<RandomIter, RandomIter>(left, right);
    }
  }
  return mystl::pair<RandomIter, RandomIter>(last, last);
}

template <class ForwardIter, class T>
mystl::pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const T& value)
{
  return erange_dispatch(first, last, value, iterator_category(first));
}

// 重载版本使用函数对象 comp 代替比较操作
// erange_dispatch 的 forward iterator 版本
template <class ForwardIter, class T, class Compared>
mystl::pair<ForwardIter, ForwardIter>
erange_dispatch(ForwardIter first, ForwardIter last,
                const T& value, forward_iterator_tag, Compared comp)
{
  auto len = mystl::distance(first, last);
  auto half = len;
  ForwardIter middle, left, right;
  while (len > 0)
  {
    half = len >> 1;
    middle = first;
    mystl::advance(middle, half);
    if (comp(*middle, value))
    {
      first = middle;
      ++first;
      len = len - half - 1;
    }
    else if (comp(value, *middle))
    {
      len = half;
    }
    else
    {
      left = mystl::lower_bound(first, last, value, comp);
      mystl::advance(first, len);
      right = mystl::upper_bound(++middle, first, value, comp);
      return mystl::pair<ForwardIter, ForwardIter>(left, right);
    }
  }
  return mystl::pair<ForwardIter, ForwardIter>(last, last);
}

// erange_dispatch 的 random access iterator 版本
template <class RandomIter, class T, class Compared>
mystl::pair<RandomIter, RandomIter>
erange_dispatch(RandomIter first, RandomIter last,
                const T& value, random_access_iterator_tag, Compared comp)
{
  auto len = last - first;
  auto half = len;
  RandomIter middle, left, right;
  while (len > 0)
  {
    half = len >> 1;
    middle = first + half;
    if (comp(*middle, value))
    {
      first = middle + 1;
      len = len - half - 1;
    }
    else if (comp(value, *middle))
    {
      len = half;
    }
    else
    {
      left = mystl::lower_bound(first, middle, value, comp);
      right = mystl::upper_bound(++middle, first + len, value, comp);
      return mystl::pair<RandomIter, RandomIter>(left, right);
    }
  }
  return mystl::pair<RandomIter, RandomIter>(last, last);
}

template <class ForwardIter, class T, class Compared>
mystl::pair<ForwardIter, ForwardIter>
equal_range(ForwardIter first, ForwardIter last, const T& value, Compared comp)
{
  return mystl::erange_dispatch(first, last, value, iterator_category(first), comp);
}

/*****************************************************************************************/
// generate
// 将函数对象 gen 的运算结果对[first, last)内的每个元素赋值
/*****************************************************************************************/
template <class ForwardIter, class Generator>
void generator(ForwardIter first, ForwardIter last, Generator gen)
{
  for(; first != last; ++first)
  {
    *first = gen();
  }
}

/*****************************************************************************************/
// generate_n
// 用函数对象 gen 连续对 n 个元素赋值
/*****************************************************************************************/
template <class ForwardIter, class Size, class Generator>
void generate_n(ForwardIter first, Size n, Generator gen)
{
  for (; n > 0; --n, ++first)
  {
    *first = gen();
  }
}

/*****************************************************************************************/
// includes
// 判断序列一 S1 传入的元素是否小于序列二 S2 传入的元素
// 判断 S2 中的每一个元素是否都来自 S1 中
/*****************************************************************************************/
template <class InputIter1, class InputIter2>
bool includes(InputIter1 first1, InputIter1 last1,
              InputIter2 first2, InputIter2 last2)
{
  while (first1 != last1 && first2 != last2)
  {
    if(*first2 < *first1)
    {
      return false;
    }
    else if (*first1 < *first2)
    {
      ++first1;
    }
    else
    {
      ++first1;
      ++first2;
    }
  }
  return first2 == last2;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter1, class InputIter2, class Compared>
bool includes(InputIter1 first1, InputIter1 last1,
              InputIter2 first2, InputIter2 last2, Compared comp)
{
  while (first1 != last1 && first2 != last2)
  {
    if (comp(*first2, *first1))
    {
      return false;
    }
    else if (comp(*first1, *first2))
    {
      ++first1;
    }
    else
    {
      ++first1, ++first2;
    }
  }
  return first2 == last2;
}

/*****************************************************************************************/
// is_heap
// 检查[first, last)内的元素是否为一个堆，如果是，则返回 true
// 保证是个大顶堆， 即父节点的值都大于等于子节点的值
/*****************************************************************************************/
// template <class RandomIter>
// bool is_heap(RandomIter first, RandomIter last)
// {
//   auto n = mystl::distance(first, last);
//   auto parent = 0;
//   for(auto child = 1; child < n; ++child)
//   {
//     if(first[parent] < first[last])
//     {
//       return false;
//     }
//     // 说明当前节点为右子节点， 遍历完一个右子节点后， 需要寻找下一个父节点
//     if((child) & 1 == 0)
//       ++parent;
//   }
//   return true;
// }

// // 重载版本使用函数对象 comp 代替比较操作
// template <class RandomIter, class Compared>
// bool is_heap(RandomIter first, RandomIter last, Compared comp)
// {
//   auto n = mystl::distance(first, last);
//   auto parent = 0;
//   for (auto child = 1; child < n; ++child)
//   {
//     if (comp(first[parent], first[child]))
//       return false;
//     if ((child & 1) == 0)
//       ++parent;
//   }
//   return true;
// }

/*****************************************************************************************/
// is_sorted
// 检查[first, last)内的元素是否升序，如果是升序，则返回 true
/*****************************************************************************************/
template <class ForwardIter>
bool is_sorted(ForwardIter first, ForwardIter last)
{
  if(first == last)
    return true;
  auto next = first;
  ++next;
  for(; next != last; first = next, ++next)
  { // 只要有一个不是升序， 则返回false
    if(*next < *first)
      return false;
  }
  return true;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
bool is_sorted(ForwardIter first, ForwardIter last, Compared comp)
{
  if (first == last)
    return true;
  auto next = first;
  ++next;
  for (; next != last; first = next, ++next)
  {
    if (comp(*next, *first))
      return false;
  }
  return true;
}

/*****************************************************************************************/
// median
// 找出三个值的中间值
/*****************************************************************************************/
template <class T>
const T& median(const T& left, const T& mid, const T& right)
{
  if(left < mid)
    if(mid < right)      // left < mid < right
      return mid;
    else if(left < right)  // left < right <= mid
      return right;
    else                  // right <= left < mid
      return left;
  // left >= mid
  else if(left < right)     // mid <= left < right
    return left;
  // left >= mid && left >= right
  else if(mid < right)       // mid < right < left
    return right;
  else  
    return mid;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class T, class Compared>
const T& median(const T& left, const T& mid, const T& right, Compared comp)
{
  if (comp(left, mid))
    if (comp(mid, right))
      return mid;
    else if (comp(left, right))
      return right;
    else
      return left;
  else if (comp(left, right))
    return left;
  else if (comp(mid, right))
    return right;
  else
    return mid;
}

/*****************************************************************************************/
// max_element
// 返回一个迭代器，指向序列中最大的元素
/*****************************************************************************************/
template <class ForwardIter>
ForwardIter max_element(ForwardIter first, ForwardIter last)
{
  if(first == last)
    return first;
  auto result = first;
  while(++first != last)
  {
    if(*first > *result)
      result = first;
  }
  return result;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
ForwardIter max_element(ForwardIter first, ForwardIter last, Compared comp)
{
  if (first == last)
    return first;
  auto result = first;
  while (++first != last)
  {
    if (comp(*result, *first))
      result = first;
  }
  return result;
}

/*****************************************************************************************/
// min_element
// 返回一个迭代器，指向序列中最小的元素
/*****************************************************************************************/
template <class ForwardIter>
ForwardIter min_elememt(ForwardIter first, ForwardIter last)
{
  if (first == last)
    return first;
  auto result = first;
  while (++first != last)
  {
    if (*first < *result)
      result = first;
  }
  return result;
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
ForwardIter min_elememt(ForwardIter first, ForwardIter last, Compared comp)
{
  if (first == last)
    return first;
  auto result = first;
  while (++first != last)
  {
    if (comp(*first, *result))
      result = first;
  }
  return result;
}

/*****************************************************************************************/
// swap_ranges
// 将[first1, last1)从 first2 开始，交换相同个数元素
// 交换的区间长度必须相同，两个序列不能互相重叠，返回一个迭代器指向序列二最后一个被交换元素的下一位置
/*****************************************************************************************/
template <class ForwardIter1, class ForwardIter2>
ForwardIter2
swap_ranges(ForwardIter1 first1, ForwardIter1 last1,
            ForwardIter2 first2)
{
  for (; first1 != last1; ++first1, ++first2)
  {
    mystl::iter_swap(first1, first2);
  }
  return first2;
}

/*****************************************************************************************/
// transform
// 第一个版本以函数对象 unary_op 作用于[first, last)中的每个元素并将结果保存至 result 中
// 第二个版本以函数对象 binary_op 作用于两个序列[first1, last1)、[first2, last2)的相同位置
/*****************************************************************************************/
template <class InputIter, class OutputIter, class UnaryOperation>
OutputIter
transform(InputIter first, InputIter last,
          OutputIter result, UnaryOperation unary_op)
{
  for(; first != last; ++first, ++result)
  {
    *result = unary_op(*first);
  }
  return result;
}

template <class InputIter1, class InputIter2, class OutputIter, class BinaryOperation>
OutputIter
transform(InputIter1 first1, InputIter1 last1,
          InputIter2 first2, OutputIter result, BinaryOperation binary_op)
{
  for(; first1 != last1; ++first1, ++first2, ++result)
  {
    *result = binary_op(*first1, *first2);
  }
  return result;
}

/*****************************************************************************************/
// remove_copy
// 移除区间内与指定 value 相等的元素，并将结果复制到以 result 标示起始位置的容器上
/*****************************************************************************************/
template <class InputIter, class OutputIter, class T>
OutputIter 
remove_copy(InputIter first, InputIter last, OutputIter result, const T& value)
{
  for(; first != last; ++first)
  { // 如果遇到相等的值， 则直接跳过
    if(*first != value)
      *result++ = *first;
  }
  return result;
}

/*****************************************************************************************/
// remove
// 移除所有与指定 value 相等的元素
// 并不从容器中删除这些元素，所以 remove 和 remove_if 不适用于 array
/*****************************************************************************************/
template <class ForwardIter, class T>
ForwardIter remove(ForwardIter first, ForwardIter last, const T& value)
{
  first = mystl::find(first, last, value);  // 利用 find 找出第一个匹配的地方
  auto next = first;
  return first == last ? first : mystl::remove_copy(++next, last, first, value);
}

/*****************************************************************************************/
// remove_copy_if
// 移除区间内所有令一元操作 unary_pred 为 true 的元素，并将结果复制到以 result 为起始位置的容器上
/*****************************************************************************************/
template <class InputIter, class OutputIter, class UnaryPredicate>
OutputIter
remove_copy_if(InputIter first, InputIter last,
               OutputIter result, UnaryPredicate unary_pred)
{
  for (; first != last; ++first)
  {
    if (!unary_pred(*first))
    {
      *result = *first;
      ++result;
    }
  }
  return result;
}

/*****************************************************************************************/
// remove_if
// 移除区间内所有令一元操作 unary_pred 为 true 的元素
/*****************************************************************************************/
template <class ForwardIter, class UnaryPredicate>
ForwardIter
remove_if(ForwardIter first, ForwardIter last, UnaryPredicate unary_pred)
{
  first = mystl::find_if(first, last, unary_pred);  // 利用 find_if 找出第一个匹配的地方
  auto next = first;
  return first == last ? first : mystl::remove_copy_if(++next, last, first, unary_pred);
}

/*****************************************************************************************/
// replace
// 将区间内所有的 old_value 都以 new_value 替代
/*****************************************************************************************/
template <class ForwardIter, class T>
void replace(ForwardIter first, ForwardIter last,
             const T& old_value, const T& new_value)
{
  for (; first != last; ++first)
  {
    if (*first == old_value)
      *first = new_value;
  }
}

/*****************************************************************************************/
// replace_copy
// 行为与 replace 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
/*****************************************************************************************/
template <class InputIter, class OutputIter, class T>
OutputIter
replace_copy(InputIter first, InputIter last,
            OutputIter result, const T& old_value, const T& new_value)
{
  for(; first != last; ++first, ++result)
    *result = *first == old_value ? new_value : *first;
  return result;
}

/*****************************************************************************************/
// replace_if
// 将区间内所有令一元操作 unary_pred 为 true 的元素都用 new_value 替代
/*****************************************************************************************/
template <class ForwardIter, class UnaryPredicate, class T>
void replace_if(ForwardIter first, ForwardIter last,
                UnaryPredicate unary_pred, const T& new_value)
{
  for(; first != last; ++first)
  {
    if(unary_pred(*first))
      *first = new_value;
  }
}

/*****************************************************************************************/
// replace_copy_if
// 行为与 replace_if 类似，不同的是将结果复制到 result 所指的容器中，原序列没有改变
/*****************************************************************************************/
template <class InputIter, class OutputIter, class UnaryPredicate, class T>
OutputIter
replace_copy_if(InputIter first, InputIter last, 
                OutputIter result, UnaryPredicate unary_pred, const T& new_value)
{
  for(; first != last; ++first, ++result)
  {
    *result = unary_pred(*first) ?  new_value : *first;
  }
}

/*****************************************************************************************/
// reverse
// 将[first, last)区间内的元素反转
/*****************************************************************************************/
// reverse_dispatch 的 bidirectional_iterator_tag 版本
template <class BidirectionalIter>
void reverse_dispatch(BidirectionalIter first, BidirectionalIter last, bidirectional_iterator_tag)
{
  while(true)
  {
    if(first == last || first == --last)
      return;
    mystl::iter_swap(first++, last); 
  }
}

// reverse_dispatch 的 random_access_iterator_tag 版本
template <class RandomIter>
void reverse_dispatch(RandomIter first, RandomIter last,
                      random_access_iterator_tag)
{
  while (first < last)
    mystl::iter_swap(first++, --last);
}

template <class BidirectionalIter>
void reverse(BidirectionalIter first, BidirectionalIter last)
{
  mystl::reverse_dispatch(first, last, iterator_category(first));
}

/*****************************************************************************************/
// reverse_copy
// 行为与 reverse 类似，不同的是将结果复制到 result 所指容器中
/*****************************************************************************************/
template <class BidirectionalIter, class OutputIter>
OutputIter
reverse_copy(BidirectionalIter first, BidirectionalIter last,
             OutputIter result)
{
  while (first != last)
  {
    --last;
    *result = *last;
    ++result;
  }
  return result;
}

/*****************************************************************************************/
// random_shuffle
// 将[first, last)内的元素次序随机重排
// 重载版本使用一个产生随机数的函数对象 rand
/*****************************************************************************************/
template <class RandomIter>
void random_shuffle(RandomIter first, RandomIter last)
{
  if(first == last)
    return;
  srand((unsigned)time(0));
  for(auto i = first + 1; i != last; i++)
  {                     // 在[i, last) 中随机挑选一个位置进行交换，i 之前已经确定的元素不再改变
    mystl::iter_swap(i, first + (rand() % (i - first + 1)));
  }
}

// 重载版本使用一个产生随机数的函数对象 rand
template <class RandomIter, class RandomNumberGenerator>
void random_shuffle(RandomIter first, RandomIter last,
                    RandomNumberGenerator& rand)
{
  if (first == last)
    return;
  auto len = mystl::distance(first, last);
  for (auto i = first + 1; i != last; ++i)
  {
    mystl::iter_swap(i, first + (rand(i - first + 1) % len));
  }
}

//! 中间rotate一系列函数没写




















/*****************************************************************************************/
//! 先没看
// is_permutation
// 判断[first1,last1)是否为[first2, last2)的排列组合
/*****************************************************************************************/
template <class ForwardIter1, class ForwardIter2, class BinaryPred>
bool is_permutation_aux(ForwardIter1 first1, ForwardIter1 last1,
                        ForwardIter2 first2, ForwardIter2 last2,
                        BinaryPred pred)
{
  constexpr bool is_ra_it = mystl::is_random_access_iterator<ForwardIter1>::value
    && mystl::is_random_access_iterator<ForwardIter2>::value;
  if (is_ra_it)
  {
    auto len1 = last1 - first1;
    auto len2 = last2 - first2;
    if (len1 != len2)
      return false;
  }

  // 先找出相同的前缀段
  for (; first1 != last1 && first2 != last2; ++first1, (void) ++first2)
  {
    if (!pred(*first1, *first2))
      break;
  }
  if (is_ra_it)
  {
    if (first1 == last1)
      return true;
  }
  else
  {
    auto len1 = mystl::distance(first1, last1);
    auto len2 = mystl::distance(first2, last2);
    if (len1 == 0 && len2 == 0)
      return true;
    if (len1 != len2)
      return false;
  }

  // 判断剩余部分
  for (auto i = first1; i != last1; ++i)
  {
    bool is_repeated = false;
    for (auto j = first1; j != i; ++j)
    {
      if (pred(*j, *i))
      {
        is_repeated = true;
        break;
      }
    }

    if (!is_repeated)
    {
      // 计算 *i 在 [first2, last2) 的数目
      auto c2 = 0;
      for (auto j = first2; j != last2; ++j)
      {
        if (pred(*i, *j))
          ++c2;
      }
      if (c2 == 0)
        return false;

      // 计算 *i 在 [first1, last1) 的数目
      auto c1 = 1;
      auto j = i;
      for (++j; j != last1; ++j)
      {
        if (pred(*i, *j))
          ++c1;
      }
      if (c1 != c2)
        return false;
    }
  }
  return true;
}

template <class ForwardIter1, class ForwardIter2, class BinaryPred>
bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2,
                    BinaryPred pred)
{
  return is_permutation_aux(first1, last1, first2, last2, pred);
}

template <class ForwardIter1, class ForwardIter2>
bool is_permutation(ForwardIter1 first1, ForwardIter1 last1,
                    ForwardIter2 first2, ForwardIter2 last2)
{
  typedef typename iterator_traits<ForwardIter1>::value_type v1;
  typedef typename iterator_traits<ForwardIter2>::value_type v2;
  static_assert(std::is_same<v1, v2>::value,
                "the type should be same in mystl::is_permutation");
  return is_permutation_aux(first1, last1, first2, last2,
                            mystl::equal_to<v1>());
}







/*****************************************************************************************/
// next_permutation
// 取得[first, last)所标示序列的下一个排列组合，如果没有下一个排序组合，返回 false，否则返回 true
/*****************************************************************************************/
template <class BidirectionalIter>
bool next_permutation(BidirectionalIter first, BidirectionalIter last)
{
  auto i = last;
  if(first == last || first == --i) // 只有 0 或者 1 个元素
    return false;
  for(;;)
  { 
    auto ii = i; // 令 ii 保持为 i 后面的一个元素
    // 找到第一对小于关系的元素
    if(*--i < *ii)
    {
      auto j = last;
      while(!(*i) < *--j) {} // 从后向前寻找第一个大于等于i的元素， 记为j
      mystl::iter_swap(i,j); // 交换i, j 所指元素
      mystl::reverse(ii, last); // 将ii之后的所有元素反转
      return true;
    }
    // 没找到小于关系的元素, 返回false
    if(i == last)
    {
      mystl::reverse(first, last);
      return false;
    }
  }
}

// 重载版本使用函数对象 comp 代替比较操作
template <class BidirectionalIter, class Compared>
bool next_permutation(BidirectionalIter first, BidirectionalIter last, Compared comp)
{
  auto i = last;
  if (first == last || first == --i)
    return false;
  for (;;)
  {
    auto ii = i;
    if (comp(*--i, *ii))
    {
      auto j = last;
      while (!comp(*i, *--j)) {}
      mystl::iter_swap(i, j);       // 交换 i，j 所指元素
      mystl::reverse(ii, last);     // 将 ii 之后的所有元素反转
      return true;
    }
    if (i == first)
    {
      mystl::reverse(first, last);
      return false;
    }
  }
}

/*****************************************************************************************/
// prev_permutation
// 取得[first, last)所标示序列的上一个排列组合，如果没有上一个排序组合，返回 false，否则返回 true
/*****************************************************************************************/
template <class BidirectionalIter>
bool prev_permutation(BidirectionalIter first, BidirectionalIter last)
{
  auto i = last;
  if(first == last || first == --i) // 只有 0 或者 1 个元素
    return false;
  for(;;)
  { 
    auto ii = i; // 令 ii 保持为 i 后面的一个元素
    // 找到第一对大于关系的元素
    if(*--i > *ii)
    {
      auto j = last;
      while(!(*i) > *--j) {} // 从后向前寻找第一个大于等于i的元素， 记为j
      mystl::iter_swap(i,j); // 交换i, j 所指元素
      mystl::reverse(ii, last); // 将ii之后的所有元素反转
      return true;
    }
    // 没找到大于关系的元素, 返回false
    if(i == last)
    {
      mystl::reverse(first, last);
      return false;
    }
  }
}

// 重载版本使用函数对象 comp 代替比较操作
template <class BidirectionalIter, class Compared>
bool prev_permutation(BidirectionalIter first, BidirectionalIter last, Compared comp)
{
  auto i = last;
  if (first == last || first == --i)
    return false;
  for (;;)
  {
    auto ii = i;
    if (comp(*ii, *--i))
    {
      auto j = last;
      while (!comp(*--j, *i)) {}
      mystl::iter_swap(i, j);       // 交换i，j
      mystl::reverse(ii, last);     // 将 ii 之后的所有元素反转
      return true;
    }
    if (i == first)
    {
      mystl::reverse(first, last);
      return false;
    }
  }
}

//! merge 一系列函数没写

















/*****************************************************************************************/
// partial_sort
// 对整个序列做部分排序，保证较小的 N 个元素以递增顺序置于[first, first + N)中
/*****************************************************************************************/
template <class RandomIter>
void partial_sort(RandomIter first, RandomIter middle, RandomIter last)
{ // 先对[first, middle) 的元素生成一个最大堆
  mystl::make_heap(first, middle);
  for(auto i = middle; i < last; ++i)
  { // 如果middle 后面有更小的元素， 则进行替换
    if(*i < *first)
    {
      mystl::pop_heap_aux(first, middle, i, *i, distance_type(first));
    }
    // 再经过while循环以后， 排列中的所有较小元素形成了最大堆， 最后对这个最大堆进行排序即可
    mystl::sort_heap(first, middle);
  }
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class Compared>
void partial_sort(RandomIter first, RandomIter middle,
                  RandomIter last, Compared comp)
{
  mystl::make_heap(first, middle, comp);
  for (auto i = middle; i < last; ++i)
  {
    if (comp(*i, *first))
    {
      mystl::pop_heap_aux(first, middle, i, *i, distance_type(first), comp);
    }
  }
  mystl::sort_heap(first, middle, comp);
}

/*****************************************************************************************/
// partial_sort_copy
// 行为与 partial_sort 类似，不同的是把排序结果复制到 result 容器中
/*****************************************************************************************/
template <class InputIter, class RandomIter, class Distance>
RandomIter
psort_copy_aux(InputIter first, InputIter last,
               RandomIter result_first, RandomIter result_last,
               Distance*)
{
  if (result_first == result_last)
    return result_last;
  auto result_iter = result_first;
  while (first != last && result_iter != result_last)
  {
    *result_iter = *first;
    ++result_iter;
    ++first;
  }
  mystl::make_heap(result_first, result_iter);
  while (first != last)
  {
    if (*first < *result_first)
    {
      mystl::adjust_heap(result_first, static_cast<Distance>(0),
                           result_iter - result_first, *first);
    }
    ++first;
  }
  mystl::sort_heap(result_first, result_iter);
  return result_iter;
}

template <class InputIter, class RandomIter>
RandomIter
partial_sort_copy(InputIter first, InputIter last,
                  RandomIter result_first, RandomIter result_last)
{
  return mystl::psort_copy_aux(first, last, result_first, result_last,
                               distance_type(result_first));
}

// 重载版本使用函数对象 comp 代替比较操作
template <class InputIter, class RandomIter, class Distance, class Compared>
RandomIter
psort_copy_aux(InputIter first, InputIter last,
               RandomIter result_first, RandomIter result_last,
               Distance*, Compared comp)
{
  if (result_first == result_last)
    return result_last;
  auto result_iter = result_first;
  while (first != last && result_iter != result_last)
  {
    *result_iter = *first;
    ++result_iter;
    ++first;
  }
  mystl::make_heap(result_first, result_iter, comp);
  while (first != last)
  {
    if (comp(*first, *result_first))
    {
      mystl::adjust_heap(result_first, static_cast<Distance>(0),
                           result_iter - result_first, *first, comp);
    }
    ++first;
  }
  mystl::sort_heap(result_first, result_iter, comp);
  return result_iter;
}

template <class InputIter, class RandomIter, class Compared>
RandomIter
partial_sort_copy(InputIter first, InputIter last,
                  RandomIter result_first, RandomIter result_last,
                  Compared comp)
{
  return mystl::psort_copy_aux(first, last, result_first, result_last,
                               distance_type(result_first), comp);
}

/*****************************************************************************************/
// partition
// 对区间内的元素重排，被一元条件运算判定为 true 的元素会放到区间的前段
// 该函数不保证元素的原始相对位置
/*****************************************************************************************/
template <class BidirectionalIter, class UnaryPredicate>
BidirectionalIter
partition(BidirectionalIter first, BidirectionalIter last,
          UnaryPredicate unary_pred)
{
  // 类似于快排思想
  while(true)
  { // 从左到右找到第一个不满足条件的元素
    while(first != last && unary_pred(*first))
    {
      ++first;
    }
    if(first == last)
      break;
    --last;
    // 从右向左找到第一个满足条件的元素
    while(first != last && !unary_pred(*last))
    {
      --last;
    }
    if(first == last)
      break;
    // 交换两个元素
    mystl::iter_swap(first, last);
    ++first;
  }
  return first;
}

/*****************************************************************************************/
// partition_copy
// 行为与 partition 类似，不同的是，将被一元操作符判定为 true 的放到 result_true 的输出区间
// 其余放到 result_false 的输出区间，并返回一个 mystl::pair 指向这两个区间的尾部
/*****************************************************************************************/
template <class InputIter, class OutputIter1, class OutputIter2, class UnaryPredicate>
mystl::pair<OutputIter1, OutputIter2>
partition_copy(InputIter first, InputIter last,
               OutputIter1 result_true, OutputIter2 result_false,
               UnaryPredicate unary_pred)
{
  for (; first != last; ++first)
  {
    if (unary_pred(*first))
    {
      *result_true++ = *first;
    }
    else
    {
      *result_false++ = *first;
    }
  }
  return mystl::pair<OutputIter1, OutputIter2>(result_true, result_false);
}

/*****************************************************************************************/
// sort
// 将[first, last)内的元素以递增的方式排序
/*****************************************************************************************/
constexpr static size_t kSmallSectionSize = 128;  // 小型区间的大小，在这个大小内采用插入排序

// 找出 lgk <= n 的 k 的最大值, 用于计算快速排序的递归次数
template <class Size>
Size slg2(Size n)
{
  Size k = 0;
  for(; n > 1; n >>= 1)
    ++k;
  return k;
}

// 一次快速排序， 不含有递归过程
// 保证枢纽左边的值都小于枢纽， 枢纽右边的值都大于枢纽
template <class RandomIter, class T>
RandomIter
unchecked_partition(RandomIter first, RandomIter last, const T& pivot)
{
  while(true)
  {
    while(*first < pivot)
      ++first;
    --last;
    while(*last > pivot)
      --last;
    if(!(first < last))
      return first;
    mystl::iter_swap(first, last);
    ++first;
  }
}


// 内省式排序， 先进行quick_sort, 当分割行为有恶化倾向时(递归次数过多)， 改用heap_sort
template <class RandomIter, class Size>
void intro_sort(RandomIter first, RandomIter last, Size depth_limit)
{
  while(static_cast<size_t>(last - first) > kSmallSectionSize)
  { 
    // 当递归次数达到最大限制
    if(depth_limit == 0)
    { // 改用heap_sort
      mystl::partial_sort(first, last, last); 
      return;
    }
    // 下面是快速排序(quick_sort)
    --depth_limit;
    // 选择快速排序的枢纽，三个位置的中间值
    auto mid = mystl::median(*(first), *(first + (last - first) / 2), *(last - 1));
    // 按照枢纽， 执行一次快速排序， 保证枢纽左边的值都小于枢纽， 枢纽右边的值都大于枢纽
    auto cut = mystl::unchecked_partition(first, last, mid);
    // 对右半边递归的进行内省排序
    mystl::intro_sort(cut, last, depth_limit);
    // 下一次 while 循环， 对左半边进行内省排序， 因为将 cur 赋值给last, 下一次循环排序的就是[first, last) 
    last = cut;
  }
}

// 插入排序辅助函数 unchecked_linear_insert
template <class RandomIter, class T>
void unchecked_linear_insert(RandomIter last, const T& value)
{
  auto next = last;
  --next;
  // 从这到函数结尾， 包括while后的一句， 完成了一次插入操作
  while (value < *next)
  {
    *last = *next;
    last = next;
    --next;
  }
  *last = value;
}







// 插入排序函数 insertion_sort
template <class RandomIter>
void insertion_sort(RandomIter first, RandomIter last)
{
  if(first == last)
    return;
  for(auto i = first + 1; i != last; ++i)
  { // 当前值
    auto value = *i;
    // 当前值 value 小于第一个元素， 直接向前插入
    if(value < *first)
    { // 将[first, i)向后移动一个位置
      mystl::copy_backward(first, i, i + 1);
      *first = value;
    }
    // 当前值大于等于第一个元素
    else{
      mystl::unchecked_linear_insert(i, value);
    }
  }
}

// 插入排序函数 unchecked_insertion_sort
template <class RandomIter>
void unchecked_insertion_sort(RandomIter first, RandomIter last)
{
  for (auto i = first; i != last; ++i)
  {
    mystl::unchecked_linear_insert(i, *i);
  }
}

// 最终插入排序函数 final_insertion_sort
template <class RandomIter>
void final_insertion_sort(RandomIter first, RandomIter last)
{     
  if (static_cast<size_t>(last - first) > kSmallSectionSize)
  {
    mystl::insertion_sort(first, first + kSmallSectionSize);
    mystl::unchecked_insertion_sort(first + kSmallSectionSize, last);
  }
  // 数量小于阈值， 直接执行插入排序，时间复杂度o(n^2)
  else 
  {
    mystl::insertion_sort(first, last);
  }
}


template <class RandomIter>
void sort(RandomIter first, RandomIter last)
{
  if(first != last)
  {
    // 内省式排序，将区间分为一个个小区间，然后对整体进行插入排序，
    // slg2(last - first) 是最大递归层次，如果递归次数过多，快排会退化成O(n^2);
    mystl::intro_sort(first, last, slg2(last - first) * 2);
    // 当元素数量小于阈值的时候， 会直接从内省排序的 while 跳出， 执行下面的插入排序
    mystl::final_insertion_sort(first, last);
  }
}

// 重载版本使用函数对象 comp 代替比较操作
// 分割函数 unchecked_partition
template <class RandomIter, class T, class Compared>
RandomIter
unchecked_partition(RandomIter first, RandomIter last,
                    const T& pivot, Compared comp)
{
  while (true)
  {
    while (comp(*first, pivot))
      ++first;
    --last;
    while (comp(pivot, *last))
      --last;
    if (!(first < last))
      return first;
    mystl::iter_swap(first, last);
    ++first;
  }
}

// 内省式排序，先进行 quick sort，当分割行为有恶化倾向时，改用 heap sort
template <class RandomIter, class Size, class Compared>
void intro_sort(RandomIter first, RandomIter last,
                Size depth_limit, Compared comp)
{
  while (static_cast<size_t>(last - first) > kSmallSectionSize)
  {
    if (depth_limit == 0)
    {                            // 到达最大分割深度限制
      mystl::partial_sort(first, last, last, comp);  // 改用 heap_sort
      return;
    }
    --depth_limit;
    auto mid = mystl::median(*(first), *(first + (last - first) / 2), *(last - 1));
    auto cut = mystl::unchecked_partition(first, last, mid, comp);
    mystl::intro_sort(cut, last, depth_limit, comp);
    last = cut;
  }
}

// 插入排序辅助函数 unchecked_linear_insert
template <class RandomIter, class T, class Compared>
void unchecked_linear_insert(RandomIter last, const T& value, Compared comp)
{
  auto next = last;
  --next;
  while (comp(value, *next))
  {  // 从尾部开始寻找第一个可插入位置
    *last = *next;
    last = next;
    --next;
  }
  *last = value;
}

// 插入排序函数 unchecked_insertion_sort
template <class RandomIter, class Compared>
void unchecked_insertion_sort(RandomIter first, RandomIter last,
                              Compared comp)
{
  for (auto i = first; i != last; ++i)
  {
    mystl::unchecked_linear_insert(i, *i, comp);
  }
}

// 插入排序函数 insertion_sort
template <class RandomIter, class Compared>
void insertion_sort(RandomIter first, RandomIter last, Compared comp)
{
  if (first == last)
    return;
  for (auto i = first + 1; i != last; ++i)
  {
    auto value = *i;
    if (comp(value, *first))
    {
      mystl::copy_backward(first, i, i + 1);
      *first = value;
    }
    else
    {
      mystl::unchecked_linear_insert(i, value, comp);
    }
  }
}

// 最终插入排序函数 final_insertion_sort
template <class RandomIter, class Compared>
void final_insertion_sort(RandomIter first, RandomIter last, Compared comp)
{
  if (static_cast<size_t>(last - first) > kSmallSectionSize)
  {
    mystl::insertion_sort(first, first + kSmallSectionSize, comp);
    mystl::unchecked_insertion_sort(first + kSmallSectionSize, last, comp);
  }
  else
  {
    mystl::insertion_sort(first, last, comp);
  }
}

template <class RandomIter, class Compared>
void sort(RandomIter first, RandomIter last, Compared comp)
{
  if (first != last)
  {
    // 内省式排序，将区间分为一个个小区间，然后对整体进行插入排序
    mystl::intro_sort(first, last, slg2(last - first) * 2, comp);
    mystl::final_insertion_sort(first, last, comp);
  }
}

/*****************************************************************************************/
// nth_element
// 对序列重排，使得所有小于第 n 个元素的元素出现在它的前面，大于它的出现在它的后面
/*****************************************************************************************/
template <class RandomIter>
void nth_element(RandomIter first, RandomIter nth, RandomIter last)
{
  if(nth == last)
    return;
  while(last - first > 3)
  { // 做一次快速排序， 并返回枢纽位置的迭代器
    auto cut = mystl::unchecked_partition(first, last, mystl::median(*first,
                                          *(first + (last - first) / 2),
                                          *(last - 1)));
    if (cut <= nth)  // 如果 nth 位于右段
      first = cut;   // 对右段进行分割
    else
      last = cut;    // 对左段进行分割
  }
  mystl::insertion_sort(first, last);
}

// 重载版本使用函数对象 comp 代替比较操作
template <class RandomIter, class Compared>
void nth_element(RandomIter first, RandomIter nth,
                 RandomIter last, Compared comp)
{
  if (nth == last)
    return;
  while (last - first > 3)
  {
    auto cut = mystl::unchecked_partition(first, last, mystl::median(*first, 
										  *(first + (last - first) / 2),
										  *(last - 1)), comp);
    if (cut <= nth)  // 如果 nth 位于右段
      first = cut;   // 对右段进行分割
    else
      last = cut;    // 对左段进行分割
  }
  mystl::insertion_sort(first, last, comp);
}

/*****************************************************************************************/
// unique_copy
// 从[first, last)中将元素复制到 result 上，序列必须有序，如果有重复的元素，只会复制一次
/*****************************************************************************************/
// unique_copy_dispatch 的 forward_iterator_tag 版本
template <class InputIter, class ForwardIter>
ForwardIter
unique_copy_dispatch(InputIter first, InputIter last,
                     ForwardIter result, forward_iterator_tag)
{
  *result = *first;
  while (++first != last)
  { // 两个不相等的值才会赋值，如果遇到相等的则会直接跳过
    if (*result != *first)
      *++result = *first;
  }
  return ++result;
}

// unique_copy_dispatch 的 output_iterator_tag 版本
// 由于 output iterator 只能进行只读操作，所以不能有 *result != *first 这样的判断
template <class InputIter, class OutputIter>
OutputIter
unique_copy_dispatch(InputIter first, InputIter last,
                     OutputIter result, output_iterator_tag)
{
  auto value = *first;
  *result = value;
  while (++first != last)
  {
    if (value != *first)
    {
      value = *first;
      *++result = value;
    }
  }
  return ++result;
}

template <class InputIter, class OutputIter>
OutputIter
unique_copy(InputIter first, InputIter last, OutputIter result)
{
  if (first == last)
    return result;
  return mystl::unique_copy_dispatch(first, last, result, iterator_category(result));
}

// 重载版本使用函数对象 comp 代替比较操作
// unique_copy_dispatch 的 forward_iterator_tag 版本
template <class InputIter, class ForwardIter, class Compared>
ForwardIter
unique_copy_dispatch(InputIter first, InputIter last,
                     ForwardIter result, forward_iterator_tag, Compared comp)
{
  *result = *first;
  while (++first != last)
  {
    if (!comp(*result, *first))
      *++result = *first;
  }
  return ++result;
}

// unique_copy_dispatch 的 output_iterator_tag 版本
// 由于 output iterator 只能进行只读操作，所以不能有 *result != *first 这样的判断
template <class InputIter, class OutputIter, class Compared>
OutputIter
unique_copy_dispatch(InputIter first, InputIter last,
                     OutputIter result, output_iterator_tag, Compared comp)
{
  auto value = *first;
  *result = value;
  while (++first != last)
  {
    if (!comp(value, *first))
    {
      value = *first;
      *++result = value;
    }
  }
  return ++result;
}

template <class InputIter, class OutputIter, class Compared>
OutputIter
unique_copy(InputIter first, InputIter last, OutputIter result, Compared comp)
{
  if (first == last)
    return result;
  return mystl::unique_copy_dispatch(first, last, result, iterator_category(result), comp);
}

/*****************************************************************************************/
// unique
// 移除[first, last)内重复的元素，序列必须有序，和 remove 类似，它也不能真正的删除重复元素
/*****************************************************************************************/
template <class ForwardIter>
ForwardIter unique(ForwardIter first, ForwardIter last)
{ // 找出第一个相邻且相等的元素
  first = mystl::adjacent_find(first, last);
  return mystl::unique_copy(first, last, first);
}

// 重载版本使用函数对象 comp 代替比较操作
template <class ForwardIter, class Compared>
ForwardIter unique(ForwardIter first, ForwardIter last, Compared comp)
{
  first = mystl::adjacent_find(first, last, comp);
  return mystl::unique_copy(first, last, first, comp);
}





}





#endif