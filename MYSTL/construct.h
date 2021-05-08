#ifndef MYTINYSTL_CONSTRUCT_H_
#define MYTINYSTL_CONSTRUCT_H_

// 这个头文件包含两个函数 construct，destroy
// construct : 负责对象的构造
// destroy   : 负责对象的析构

#include <new>
// #include "head_file.h"
#include "util.h"
#include "type_traits.h"
#include "iterator.h"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4100)  // unused parameter
#endif // _MSC_VER

namespace mystl
{

// construct 构造对象

template <class Ty>
void construct(Ty* ptr)
{
  ::new ((void*)ptr) Ty();    //! 利用new构造一个对象， new 类名，只不过将他强转成void*类型
}

template <class Ty1, class Ty2>
void construct(Ty1* ptr, const Ty2& value) //! 重载
{
  ::new ((void*)ptr) Ty1(value);
}

template <class Ty, class... Args>
void construct(Ty* ptr, Args&&... args)   //! 变参列表的重载
{
  ::new ((void*)ptr) Ty(mystl::forward<Args>(args)...);
}

// destroy 将对象析构

template <class Ty>
void destroy_one(Ty*, std::true_type) {}



/*
  析构函数的特点：
  1. 析构函数的函数名和类名相同，与构造函数用～进行区分
  2. 析构函数没有返回类型，也不能指定参数，因此析构函数只能有一个，不能被重载
  3. 当对象被撤销的时候，析构函数自动的被调用，与构造函数不同的是，析构函数也可以被显示的调用，以释放动态内存
*/
template <class Ty>
void destroy_one(Ty* pointer, std::false_type)
{
  if (pointer != nullptr)
  {
    pointer->~Ty();
  }
}

template <class ForwardIter>
void destroy_cat(ForwardIter , ForwardIter , std::true_type) {}

template <class ForwardIter>
void destroy_cat(ForwardIter first, ForwardIter last, std::false_type)
{ //! 释放多个对象
  for (; first != last; ++first)
    destroy(&*first);
}

template <class Ty>
void destroy(Ty* pointer)
{                           // 查询这个类是否为普通易损坏类型，返回true/false
  destroy_one(pointer, std::is_trivially_destructible<Ty>{});
}

template <class ForwardIter>
void destroy(ForwardIter first, ForwardIter last)
{
  destroy_cat(first, last, std::is_trivially_destructible<
              typename iterator_traits<ForwardIter>::value_type>{});
}

} // namespace mystl

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#endif // !MYTINYSTL_CONSTRUCT_H_

