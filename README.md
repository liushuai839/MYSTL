# MYSTL

## 项目概况：

本项目主要利用 c++ 复现STL基本的容器和算法, 包括三个文件夹, MYSTL 下面存放所有容器和算法具体实现的文件, TEST 下面存放所有容器和算法的测试文件, bin 下面存放可执行文件

参考: https://github.com/Alinshans/MyTinySTL

## 系统环境：

Ubuntu18.04

g++ 7.5.0

## **容器**  

(1) **序列式容器**：vector, deque, stack, queue, list

(2) **关联式容器**：map, multimap, set, multiset, unordered_map, unordered_set, unordered_multimap, unordered_multiset

## **算法**

(1) **基本算法** ：copy, copy_if,  copy_n,  move, move_backward, equal, fill, fill_n, iter_swap, lexicographical_compare, max, min, mismatch,

(2) **关于堆的算法** ：make_heap, pop_heap, push_heap, sort_heap,

(3) **常规算法** : adjacent_find,  all_of,  any_of, binary_search, count, count_if, equal_range, find, find_first_of, find_if, find_if_not, for_each, generate_n,
                                     includes,  //is_heap,  is_sorted,lower_bound,  max_elememt, median, min_elememt,  is_permutation, // next_permutation, none_of,
                                      nth_element, partial_sort, partial_sort_copy, partition, partition_copy,  // prev_permutation, random_shuffle, remove, remove_copy,   				    
                                     remove_copy_if, remove_if, replace, replace_copy, replace_copy_if, replace_if,  reverse,  reverse_copy, search, search_n, sort,  swap_ranges,
  					                          transform, unique, unique_copy, upper_bound

## 实验结果：

**算法测试实例：**

https://github.com/liushuai839/MYSTL/blob/master/image-20210509094033057.png

**容器测试示例：**

下面测试的是项目实现的容器添加大量元素和c++ 标准库中容器的时间对比

https://github.com/liushuai839/MYSTL/blob/master/image-20210509094812942.png
https://github.com/liushuai839/MYSTL/blob/master/image-20210509094850793.png
https://github.com/liushuai839/MYSTL/blob/master/image-20210509094927445.png
