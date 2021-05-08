#ifndef MYSTL_STACK_H
#define MYSTL_STACK_H

#include "deque.h"
namespace mystl
{


template <class T, class Container = mystl::deque<T>>
class stack{

public:
    // 底层容器的型别
    typedef Container                       container_type;

    typedef typename Container::value_type      value_type;
    typedef typename Container::size_type       size_type;
    typedef typename Container::reference       reference;
    typedef typename Container::const_reference const_reference;
private:
    container_type c_;  // 底层容器

public:
    // 默认构造函数
    stack() = default;
    explicit stack(size_type n)
        :c_(n)
    {
    }
    stack(size_type n, const value_type& value) 
        :c_(n, value)
    {
    }
    template <class IIter>
    stack(IIter first, IIter last)
        : c_(first, last)
    {
    }
    // 参数列表初始化
    stack(std::initializer_list<T> ilist) 
        :c_(ilist.begin(), ilist.end())
    {
    }
    // 拷贝构造函数
    stack(const Container& c)
        :c_(c)
    {
    }
    // 移动构造函数
    stack(Container&& c) noexcept(std::is_nothrow_move_constructible<Container>::value)
        :c_(mystl::move(c)) 
    {
    }
    // 拷贝构造函数
    stack(const stack& rhs) 
        :c_(rhs.c_) 
    {
    }
    // 移动构造函数
    stack(stack&& rhs) noexcept(std::is_nothrow_move_constructible<Container>::value)
        :c_(mystl::move(rhs.c_))
    {
    }

    stack& operator=(const stack& rhs)
    {
        c_ = rhs.c_;
        return *this;
    }
    stack& operator=(stack&& rhs) noexcept(std::is_nothrow_move_assignable<Container>::value)
    { 
    c_ = mystl::move(rhs.c_); 
    return *this;
    }

    stack& operator=(std::initializer_list<T> ilist) 
    {
        c_ = ilist; 
        return *this;
    }

     ~stack() = default;

    // 访问元素相关操作
    reference           top()        {return c_.back();}
    reference           top() const  {return c_.back();}

    // 容量相关操作
    bool        empty()  const noexcept {return c_.empty();}
    size_type   size()   const noexcept {return c_.size();}

    // 修改容器相关操作
    template <class... Args>
    void emplace(Args&& ...args)
    { c_.emplace_back(mystl::forward<Args>(args)...); }

    void push(const value_type& value)
    { c_.push_back(value); }
    void push(value_type&& value)      
    { c_.push_back(mystl::move(value)); }

    void pop()
    { c_.pop_back(); }

    void clear()
    {
        while(!empty())
            pop();
    }

    void swap(stack& rhs) noexcept(noexcept(mystl::swap(c_, rhs.c_)))
    { mystl::swap(c_, rhs.c_); }

    public:
    friend bool operator==(const stack& lhs, const stack& rhs) { return lhs.c_ == rhs.c_; }
    friend bool operator< (const stack& lhs, const stack& rhs) { return lhs.c_ <  rhs.c_; }
};



// 重载比较操作符
template <class T, class Container>
bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
  return lhs == rhs;
}

template <class T, class Container>
bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
  return lhs < rhs;
}

template <class T, class Container>
bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
  return !(lhs == rhs);
}

template <class T, class Container>
bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
  return rhs < lhs;
}

template <class T, class Container>
bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
  return !(rhs < lhs);
}

template <class T, class Container>
bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs)
{
  return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class T, class Container>
void swap(stack<T, Container>& lhs, stack<T, Container>& rhs) noexcept(noexcept(lhs.swap(rhs)))
{
  lhs.swap(rhs);
}


}
#endif