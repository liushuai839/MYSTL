#ifndef MYSTL_LIST_H
#define MYSTL_LIST_H

#include "iterator.h"
#include "memory.h"
#include "util.h"
#include "exceptdef.h"
#include "functional.h"


namespace mystl{


template <class T> struct list_node_base;
template <class T> struct list_node;


template <class T> 
struct node_traits{
    typedef list_node_base<T>*    base_ptr;
    typedef list_node<T>*         node_ptr;
};


template <class T>
struct list_node_base
{
  typedef typename node_traits<T>::base_ptr base_ptr;
  typedef typename node_traits<T>::node_ptr node_ptr;

  base_ptr prev;  // 前一节点
  base_ptr next;  // 下一节点

  list_node_base() = default;
  
  //base_node 强转成node
  node_ptr as_node()
  {
    return static_cast<node_ptr>(self());
  }

  void unlink()
  {
    prev = next = self();
  }

  base_ptr self()
  {
    return static_cast<base_ptr>(&*this);
  }
};

// 普通节点， 在根节点的基础上添加了数据域
//! 普通节点， 在根节点的基础上添加了数据域
template <class T>
struct list_node : public list_node_base<T>
{
  typedef typename node_traits<T>::base_ptr base_ptr;
  typedef typename node_traits<T>::node_ptr node_ptr;

  T value;  // 数据域

  list_node() = default;
  list_node(const T& v)
    :value(v)
  {
  }
  list_node(T&& v)
    :value(mystl::move(v))
  {
  }

  // node 强转成base_node
  base_ptr as_base()
  {
    return static_cast<base_ptr>(&*this);
  }
  node_ptr self()
  {
    return static_cast<node_ptr>(&*this);
  }
};

// list 的迭代器
template <class T>
struct list_iterator : public mystl::iterator<mystl::bidirectional_iterator_tag, T>
{
  typedef T                                 value_type;
  typedef T*                                pointer;
  typedef T&                                reference;
  typedef typename node_traits<T>::base_ptr base_ptr;
  typedef typename node_traits<T>::node_ptr node_ptr;
  typedef list_iterator<T>                  self;

  base_ptr node_;  // 指向当前节点

  // 构造函数
  list_iterator() = default;
  list_iterator(base_ptr x)
    :node_(x) {}
  list_iterator(node_ptr x)
    :node_(x->as_base()) {}
  list_iterator(const list_iterator& rhs)
    :node_(rhs.node_) {}

  // 重载操作符  // TODO 没看懂
  reference operator*()  const { return node_->as_node()->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    MYSTL_DEBUG(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }
  self operator++(int)
  {
    self tmp = *this;
    ++*this;
    return tmp;
  }
  self& operator--()
  {
    MYSTL_DEBUG(node_ != nullptr);
    node_ = node_->prev;
    return *this;
  }
  self operator--(int)
  {
    self tmp = *this;
    --*this;
    return tmp;
  }

  // 重载比较操作符
  bool operator==(const self& rhs) const { return node_ == rhs.node_; }
  bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
};


// const_iterator
template <class T>
struct list_const_iterator : public iterator<bidirectional_iterator_tag, T>
{ 
  //! 指针和引用都是const类型
  typedef T                                 value_type;
  typedef const T*                          pointer;    
  typedef const T&                          reference;
  typedef typename node_traits<T>::base_ptr base_ptr;
  typedef typename node_traits<T>::node_ptr node_ptr;
  typedef list_const_iterator<T>            self;

  base_ptr node_;

  list_const_iterator() = default;
  list_const_iterator(base_ptr x)
    :node_(x) {}
  list_const_iterator(node_ptr x)
    :node_(x->as_base()) {}
  list_const_iterator(const list_iterator<T>& rhs)
    :node_(rhs.node_) {}
  list_const_iterator(const list_const_iterator& rhs)
    :node_(rhs.node_) {}

  reference operator*()  const { return node_->as_node()->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    MYSTL_DEBUG(node_ != nullptr);
    node_ = node_->next;
    return *this;
  }
  self operator++(int)
  {
    self tmp = *this;
    ++*this;
    return tmp;
  }
  self& operator--()
  {
    MYSTL_DEBUG(node_ != nullptr);
    node_ = node_->prev;
    return *this;
  }
  self operator--(int)
  {
    self tmp = *this;
    --*this;
    return tmp;
  }

  // 重载比较操作符
  bool operator==(const self& rhs) const { return node_ == rhs.node_; }
  bool operator!=(const self& rhs) const { return node_ != rhs.node_; }
};

template <class T>
class list{

public:
    // list 的嵌套型别定义
    typedef mystl::allocator<T>                      allocator_type;
    typedef mystl::allocator<T>                      data_allocator;
    typedef mystl::allocator<list_node_base<T>>      base_allocator;
    typedef mystl::allocator<list_node<T>>           node_allocator;

    typedef typename allocator_type::value_type      value_type;
    typedef typename allocator_type::pointer         pointer;
    typedef typename allocator_type::const_pointer   const_pointer;
    typedef typename allocator_type::reference       reference;
    typedef typename allocator_type::const_reference const_reference;
    typedef typename allocator_type::size_type       size_type;
    typedef typename allocator_type::difference_type difference_type;

    typedef list_iterator<T>                         iterator;
    typedef list_const_iterator<T>                   const_iterator;
    typedef mystl::reverse_iterator<iterator>        reverse_iterator;
    typedef mystl::reverse_iterator<const_iterator>  const_reverse_iterator;

    typedef typename node_traits<T>::base_ptr        base_ptr;
    typedef typename node_traits<T>::node_ptr        node_ptr;

    allocator_type get_allocator() { return node_allocator(); }

private:
    base_ptr node_; // 指向末尾的节点
    size_type size_; // 大小

public:
    // 构造函数
    list()
    { fill_init(0, value_type()); }
    explicit list(size_type n)
    { fill_init(n, value_type()); }
    list(size_type n, const T& value)
    { fill_init(n, value); }

    template <class Iter, typename std::enable_if<
        mystl::is_input_iterator<Iter>::value, int>::type = 0>
    list(Iter first, Iter last)
    { copy_init(first, last); }

    list(std::initializer_list<T> ilist)
    { copy_init(ilist.begin(), ilist.end()); }

    // 拷贝构造函数
    list(const list& rhs)
    { copy_init(rhs.cbegin(), rhs.cend()); }
    // 移动构造函数
    list(list&& rhs) noexcept
        : node_(rhs.node_), size_(rhs.size_)
    {
        rhs.node_ = nullptr;
        rhs.size_ = 0;
    }

    //拷贝赋值
    list& operator=(const list& rhs)
    {   
        if( this != &rhs)
        {
            assign(rhs.begin(), rhs.end());
        }
        return *this;
    }

    // 移动赋值
    list& operator= (list&& rhs) noexcept
    {
        clear();
        splice(end(), rhs);
        return *this;
    }
    // 列表初始化
    list& operator=(std::initializer_list<T> ilist)
    {
        list tmp(ilist.begin(), ilist.end());
        swap(tmp);
        return *this;
    }

    ~list()
    {
        if(node_)
        {
            clear();
            base_allocator::deallocate(node_);
            node_ = nullptr;
            size_ = 0;
        }
    }

public:
    // 迭代器相关操作
    iterator               begin()         noexcept
    { return node_->next; }
    const_iterator         begin()   const noexcept
    { return node_->next; }
    iterator               end()           noexcept 
    { return node_; }
    const_iterator         end()     const noexcept
    { return node_; }

    reverse_iterator       rbegin()        noexcept
    { return reverse_iterator(end()); }
    const_reverse_iterator rbegin()  const noexcept
    { return reverse_iterator(end()); }
    reverse_iterator       rend()          noexcept
    { return reverse_iterator(begin()); }
    const_reverse_iterator rend()    const noexcept
    { return reverse_iterator(begin()); }

    const_iterator         cbegin()  const noexcept
    { return begin(); }
    const_iterator         cend()    const noexcept
    { return end(); }
    const_reverse_iterator crbegin() const noexcept
    { return rbegin(); }
    const_reverse_iterator crend()   const noexcept
    { return rend(); }

    // 容量相关操作
    bool      empty()   const noexcept
    {   return node_->next == node_; }
    size_type size()    const noexcept
    {   return size_; }
    size_type max_size() const noexcept
    {   return static_cast<size_type>(-1); }

    // 访问元素相关操作
    reference   front()
    {
        assert(!empty());
        return *begin();
    }

    const_reference front() const 
    { 
        MYSTL_DEBUG(!empty()); 
        return *begin(); 
    }

    reference       back() 
    { 
        MYSTL_DEBUG(!empty());
        return *(--end());
    }

    const_reference back()  const 
    { 
        MYSTL_DEBUG(!empty()); 
        return *(--end());
    }

    // 调整容器相关操作


    // assign 
    void    assign(size_type n, const value_type& value)
    { fill_assign(n, value); }
    
    template <class Iter, typename std::enable_if<
        mystl::is_input_iterator<Iter>::value, int>::type = 0>
    void     assign(Iter first, Iter last)
    { copy_assign(first, last); }

    void     assign(std::initializer_list<T> ilist)
    { copy_assign(ilist.begin(), ilist.end()); }

    // emplace_front / emplace_back / emplace
    template <class ... Args>
    void        emplace_front(Args&& ...args)
    {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
        auto link_node = create_node(mystl::forward<Args>(args)...);
        // 相当于添加一个节点
        link_nodes_at_front(link_node->as_base(), link_node->as_node());
        ++size_;
    }

    template <class ... Args>
    void        emplace_back(Args&& ...args)
    {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
        auto link_node = create_node(mystl::forward<Args>(args)...);
        // 相当于添加一个节点
        link_nodes_at_back(link_node->as_base(), link_node->as_node());
        ++size_;
    }

    // 返回添加的位置
    template <class ... Args>
    iterator    emplace(const_iterator pos, Args&& ...args)
    {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
        auto link_node = create_node(mystl::forward<Args>(args)...);
        // 相当于添加一个节点
        link_nodes(pos.node_,link_node->as_base(), link_node->as_node());
        ++size_;
        return iterator(link_node);
    }

    // push_front / push_back
    template <class ... Args>
    void        push_front(const value_type& value)
    {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
        auto link_node = create_node(value);
        // 相当于添加一个节点
        link_nodes_at_front(link_node->as_base(), link_node->as_node());
        ++size_;
    }

    void        push_front(value_type&& value)
    {
        emplace_front(mystl::move(value));
    }

    template <class ... Args>
    void        push_back(const value_type& value)
    {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
        auto link_node = create_node(value);
        // 相当于添加一个节点
        link_nodes_at_back(link_node->as_base(), link_node->as_node());
        ++size_;
    }

    void        push_back(value_type&& value)
    {
        emplace_back(value);
    }

    // pop_front / pop_back
    void pop_front()
    {
        assert(!empty());
        auto n = node_->next;  // node_ 指向末尾节点, node->next 则为开始的节点
        unlink_nodes(n,n);
        destroy_node(n->as_node());
        --size_;
    }

    void pop_back()
    {
        assert(!empty());
        auto n = node_->prev;
        unlink_nodes(n,n);
        destroy_node(n->as_node());
        --size_;
    }

    //insert
    iterator insert(const_iterator pos, const value_type& value)
    {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
        auto link_node = create_node(value);
        ++size_;
        return link_iter_node(pos, link_node->as_base());
    }

    iterator insert(const_iterator pos, value_type&& value)
    {
        THROW_LENGTH_ERROR_IF(size_ > max_size() - 1, "list<T>'s size too big");
        auto link_node = create_node(mystl::move(value));
        ++size_;
        return link_iter_node(pos, link_node->as_base());
    }

    iterator insert(const_iterator pos, size_type n, const value_type& value)
    { 
        THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
        return fill_insert(pos, n, value); 
    }

    template <class Iter, typename std::enable_if<
        mystl::is_input_iterator<Iter>::value, int>::type = 0>
    iterator insert(const_iterator pos, Iter first, Iter last)
    { 
        size_type n = mystl::distance(first, last);
        THROW_LENGTH_ERROR_IF(size_ > max_size() - n, "list<T>'s size too big");
        return copy_insert(pos, n, first); 
    }



    //erase / clear
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    void clear();

    //resize
    void resize(size_type new_size) {resize(new_size, value_type()); }
    void resize(size_type new_size, const value_type& value);

    void swap(list& rhs) noexcept
    {
        mystl::swap(node_, rhs.node_);
        mystl::swap(size_, rhs.size_);
    }

    //reverse
    void    reverse();

    // list相关操作
    void    splice(const_iterator pos, list& other);
    void    splice(const_iterator pos, list& other, const_iterator it);
    void    splice(const_iterator pos, list& other, const_iterator first, const_iterator last);

    void    merge(list& x)
    { merge(x, mystl::less<T>()); }
    template <class Compare>
    void    merge(list& x, Compare comp);

    void    sort()
    { list_sort(begin(), end(), size(), mystl::less<T>()); }

    template <class Compared>
    void    sort(Compared comp)
    { list_sort(begin(), end(), size(), comp); }



private:
    // helper functions
    
    //create_nodes / destroy node
    template <class ...Args>
    node_ptr  create_node(Args&& ...args);
    void      destroy_node(node_ptr p);

    // initialize
    void    fill_init(size_type n, const value_type& value);
    template <class IIter>
    void    copy_init(IIter first, IIter last);

    void    fill_assign(size_type n, const value_type& value);
    template <class IIter>
    void    copy_assign(IIter first, IIter last);

    // link / unlink
    iterator link_iter_node(const_iterator pos, base_ptr node);
    void    link_nodes(base_ptr p, base_ptr first, base_ptr last);
    void    link_nodes_at_front(base_ptr first, base_ptr last);
    void    link_nodes_at_back(base_ptr first, base_ptr last);
    void    unlink_nodes(base_ptr f, base_ptr l);

    // assign
    iterator    fill_insert(const_iterator pos, size_type n, const value_type& value);
    template    <class Iter>
    iterator    copy_insert(const_iterator pos, size_type n, Iter first);

    // sort
    template <class Compared>
    iterator    list_sort(iterator first, iterator last, size_type n, Compared comp);

};

//-----------------------------------------------------------------------------------------------------------

// 删除 pos 处的位置
template <class T>
typename list<T>::iterator
list<T>::erase(const_iterator pos)
{
    assert(pos != cend());
    auto n = pos.node_;
    // 将下一节点直接略过
    auto next = n->next;
    unlink_nodes(n,n);
    destroy_node(n->as_node());
    --size_;
    return iterator(next);
}

// 删除[first, last) 内的元素
template <class T>
typename list<T>::iterator
list<T>::erase(const_iterator first, const_iterator last){
    if(first != last)
    {
        unlink_nodes(first.node_, last.node_->prev);
        while(first != last)
        {
            auto cur = first.node_;
            ++first;
            destroy_node(cur->as_node());
            --size_;
        }
    }
    return iterator(last.node_);
}

// 清空list
template <class T>
void list<T>::clear()
{
    if(size_ != 0)
    {   // 首节点
        auto cur = node_->next;
        for(base_ptr next = cur->next; cur != node_; cur = next, next = cur->next)
        {
            destroy_node(cur->as_node());
        }
    }

    // 保留了一个尾节点
    node_->unlink();
    size_ = 0;
}


// 将 List x 接于 pos 之前
template <class T>
void list<T>::splice(const_iterator pos, list& x)
{
    assert(this != &x);
    if(!x.empty())
    {
        THROW_OUT_OF_RANGE_IF(size_ > max_size() - x.size_, "list<T>'s size too big");
        auto f = x.node_->next;
        auto l = x.node_->prev;
        // 将原来的环形链表释放
        x.unlink_nodes(f,l);
        link_nodes(pos.node_,f,l);
        size_ += x.size_;
        x.size_ = 0;
    }
}

// 将 it 所指的节点 接合于 pos 之前
// 只添加一个节点
template <class T>
void list<T>::splice(const_iterator pos, list& x, const_iterator it)
{
    if(pos.node_ != it.node_ && pos.node_ != it.node_->next)
    {
        THROW_OUT_OF_RANGE_IF(size_ > max_size() - 1, "list<T>'s size too big");
        auto f = it.node_;

        x.unlink_nodes(f,f);
        link_nodes(pos.node_,f,f);

        ++size_;
        --x.size_;
    }
}

// 将list x 的[first, last) 内的节点接合于 pos 之前
template <class T>
void list<T>::splice(const_iterator pos, list& x, const_iterator first, const_iterator last)
{
    if(first != last && this != &x){
        
        size_type n = mystl::distance(first, last);
        THROW_OUT_OF_RANGE_IF(size_ > max_size() - 1, "list<T>'s size too big");
        auto f = first.node_;
        auto l = last.node_->prev;
        
        x.unlink_nodes(f,l);
        link_nodes(pos.node_,f,l);

        size_ += n;
        x.size_ -= n;
    }
}

// 重置容器大小
template <class T>
void list<T>::resize(size_type new_size, const value_type& value)
{
    auto i = begin();
    size_type len = 0;
    while(i != end() && len < new_size){
        ++i;
        ++len;
    }
    // 缩小   
    if(len == new_size){
        erase(i, node_);
    }
    else{
        insert(node_, new_size - len, value);
    }
}

// 与另一个 list 合并
template <class T>
template <class Compare>
void list<T>::merge(list& x, Compare comp)
{
    if(this != &x)
    {
       THROW_LENGTH_ERROR_IF(size_ > max_size() - x.size_, "list<T>'s size too big");

        auto f1 = begin();
        auto l1 = end();
        auto f2 = x.begin();
        auto l2 = x.end();
        
        // 把l2 有序的插入到 l1 中去
        while(f1 != l1 && f2 != l2)
        {
            if(comp(*f2, *f1))
            {
               auto next = f2;
               ++next;
               // 找到f2 中第一个大于当前f1节点的位置，并记录
               for(;next != f2 && comp(*next, *f1); ++next)
                    ;
                auto f = f2.node_;
                auto l = next.node_->prev;
                f2 = next; 

                // 在l2 中unlink 这些节点
                x.unlink_nodes(f,l);
                link_nodes(f1.node_,f,l);
                ++f1;
            }   
            else{
                ++f1;
            }
        }
        // 如果l2 有剩余
        if(f2 != l2)
        {
            auto f = f2.node_;
            auto l = l2.node_->next;
            x.unlink_nodes(f,l);
            link_nodes(l1.node_,f,l);
        }
    size_ += x.size_;
    x.size_ = 0;
    }
}

template <class T>
void list<T>::reverse()
{
    if(size_ <= 1)
        return ;
    auto i = begin();
    auto e = end();
    while(i.node_ != e.node_)
    {
        mystl::swap(i.node_->prev, i.node_->next);
        i.node_ = i.node_->next;
    }
    mystl::swap(e.node_->prev, e.node_->next);
}


//-------------------------------------------------------------------------------------------------
// helper function

// 创建节点
template <class T>
template <class ...Args>
typename list<T>::node_ptr 
list<T>::create_node(Args&& ...args)
{ 
  node_ptr p = node_allocator::allocate(1);
  try
  {
    data_allocator::construct(mystl::address_of(p->value), mystl::forward<Args>(args)...);
    p->prev = nullptr;
    p->next = nullptr;
  }
  catch (...)
  {
    node_allocator::deallocate(p);
    throw;
  }
  return p;
}

// 销毁节点
template <class T>
void list<T>::destroy_node(node_ptr p)
{   // 析构对象
    data_allocator::destroy(mystl::address_of(p->value));
    // 释放空间
    node_allocator::deallocate(p);
}

// 用 n 个元素初始化容器
template <class T>
void list<T>::fill_init(size_type n, const value_type& value)
{ // 创建根节点
  node_ = base_allocator::allocate(1);
  node_->unlink();
  size_ = n;
  try
  {
    for (; n > 0; --n)
    { // 创建节点
      auto node = create_node(value);
      // 连接节点
      link_nodes_at_back(node->as_base(), node->as_base());
    }
  }
  catch (...)
  {
    clear();
    base_allocator::deallocate(node_);
    node_ = nullptr;
    throw;
  }
}

template <class T>
template <class IIter>
void list<T>::copy_init(IIter first, IIter last)
{
    // 创建根节点
    node_ = base_allocator::allocate(1);
    node_->unlink();
    size_type n = mystl::distance(first, last);
    size_ = n;
    try
    {
        for(; n > 0; --n){
            // 创建节点
            auto node = create_node(*first);
            link_nodes_at_back(node->as_base(), node->as_base());
        }
    }
    catch(...)
    {
        clear();
        base_allocator::deallocate(node_);
        node_ = nullptr;
        throw;
    } 
}

// 在 pos 处连接一个节点
template <class T>
typename list<T>::iterator 
list<T>::link_iter_node(const_iterator pos, base_ptr link_node)
{ 
  if (pos == node_->next)
  {
    link_nodes_at_front(link_node, link_node);
  }
  else if (pos == node_)
  {
    link_nodes_at_back(link_node, link_node);
  }
  else
  {
    link_nodes(pos.node_, link_node, link_node);
  }
  return iterator(link_node);
}


// 在pos位置之前插入[first, last]的节点
template <class T>
void list<T>::link_nodes(base_ptr pos, base_ptr first, base_ptr last)
{
    pos->prev->next = first;
    first->prev = pos->prev;
    pos->prev = last;
    last->next = pos;
}
// 在头部连接 [first, last] 结点
template <class T>
void list<T>::link_nodes_at_front(base_ptr first, base_ptr last)
{
  first->prev = node_;
  last->next = node_->next;
  last->next->prev = last;
  node_->next = first;
}

// 在尾部连接 [first, last] 结点
template <class T>
void list<T>::link_nodes_at_back(base_ptr first, base_ptr last)
{
  last->next = node_;
  first->prev = node_->prev;
  first->prev->next = first;
  node_->prev = last;
}

template <class T>
void list<T>::unlink_nodes(base_ptr first, base_ptr last)
{
    first->prev->next = last->next;
    last->next->prev = first->prev;
}

template <class T>
void list<T>::fill_assign(size_type n, const value_type& value)
{
    auto i = begin();
    auto e = end();
    for(; n > 0 && i != e; --n, ++i)
        *i = value;
    if(n > 0)
        insert(e, n, value);
    // 删除原来的list
    else    
        erase(i,e);
    
}

template <class T>
template <class IIter>
void list<T>::copy_assign(IIter f2, IIter l2)
{
  auto f1 = begin();
  auto l1 = end();
  for (; f1 != l1 && f2 != l2; ++f1, ++f2)
  {
    *f1 = *f2;
  }
  if (f2 == l2)
  {
    erase(f1, l1);
  }
  else
  {
    insert(l1, f2, l2);
  }  
}


// 在 pos 处插入n个元素
template <class T>
typename list<T>::iterator
list<T>::fill_insert(const_iterator pos, size_type n, const value_type& value)
{
    iterator r(pos.node_);
    if(n != 0)
    {
        const auto add_size = n;
        auto node = create_node(value);
        node->prev = nullptr;
        // 临时做个标记
        r = iterator(node);
        iterator end = r;
        try
        {
            for(--n; n > 0; --n, ++end)
            {
                auto next = create_node(value);
                end.node_->next = next->as_base();
                next->prev = end.node_;
            }
            size_ += add_size;
        } 
        catch(...)
        {
            auto enode = end.node_;
            while (true)
            {
                auto prev = enode->prev;
                destroy_node(enode->as_node());
                if (prev == nullptr)
                break;
                enode = prev;
            }
            throw;
        }
        link_nodes(pos.node_, r.node_, end.node_);
    }
    return r;
}
// 在 pos 处插入 [first, last) 的元素
// 和fill_insert 思路一样
template <class T>
template <class Iter>
typename list<T>::iterator 
list<T>::copy_insert(const_iterator pos, size_type n, Iter first)
{
  iterator r(pos.node_);
  if (n != 0)
  {
    const auto add_size = n;
    auto node = create_node(*first);
    node->prev = nullptr;
    r = iterator(node);
    iterator end = r;
    try
    {
      for (--n, ++first; n > 0; --n, ++first, ++end)
      {
        auto next = create_node(*first);
        end.node_->next = next->as_base();  // link node
        next->prev = end.node_;
      }
      size_ += add_size;
    }
    catch (...)
    {
      auto enode = end.node_;
      while (true)
      {
        auto prev = enode->prev;
        destroy_node(enode->as_node());
        if (prev == nullptr)
          break;
        enode = prev;
      }
      throw;
    }
    link_nodes(pos.node_, r.node_, end.node_);
  }
  return r;
}

// 对 list 进行归并排序，返回一个迭代器指向区间最小元素的位置
template <class T>
template <class Compared>
typename list<T>::iterator 
list<T>::list_sort(iterator f1, iterator l2, size_type n, Compared comp)
{
  if (n < 2)
    return f1;

  if (n == 2)
  { // 两个元素若相反的话， 则直接交换节点
    if (comp(*--l2, *f1))
    {
      auto ln = l2.node_;
      unlink_nodes(ln, ln);
      link_nodes(f1.node_, ln, ln);
      return l2;
    }
    return f1;
  }

  auto n2 = n / 2;
  auto l1 = f1;
  mystl::advance(l1, n2);
  auto result = f1 = list_sort(f1, l1, n2, comp);  // 前半段的最小位置
  auto f2 = l1 = list_sort(l1, l2, n - n2, comp);  // 后半段的最小位置

  // 把较小的一段区间移到前面
  if (comp(*f2, *f1))
  {
    auto m = f2;
    ++m;
    for (; m != l2 && comp(*m, *f1); ++m)
      ;
    auto f = f2.node_;
    auto l = m.node_->prev;
    result = f2;
    l1 = f2 = m;
    unlink_nodes(f, l);
    m = f1;
    ++m;
    link_nodes(f1.node_, f, l);
    f1 = m;
  }
  else
  {
    ++f1;
  }

  // 合并两段有序区间
  while (f1 != l1 && f2 != l2)
  {
    if (comp(*f2, *f1))
    {
      auto m = f2;
      ++m;
      for (; m != l2 && comp(*m, *f1); ++m)
        ;
      auto f = f2.node_;
      auto l = m.node_->prev;
      if (l1 == f2)
        l1 = m;
      f2 = m;
      unlink_nodes(f, l);
      m = f1;
      ++m;
      link_nodes(f1.node_, f, l);
      f1 = m;
    }
    else
    {
      ++f1;
    }
  }
  return result;
}

// 重载比较操作符
template <class T>
bool operator==(const list<T>& lhs, const list<T>& rhs)
{
  auto f1 = lhs.cbegin();
  auto f2 = rhs.cbegin();
  auto l1 = lhs.cend();
  auto l2 = rhs.cend();
  for (; f1 != l1 && f2 != l2 && *f1 == *f2; ++f1, ++f2)
    ;
  return f1 == l1 && f2 == l2;
}

template <class T>
bool operator<(const list<T>& lhs, const list<T>& rhs)
{
  return mystl::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <class T>
bool operator!=(const list<T>& lhs, const list<T>& rhs)
{
  return !(lhs == rhs);
}

template <class T>
bool operator>(const list<T>& lhs, const list<T>& rhs)
{
  return rhs < lhs;
}

template <class T>
bool operator<=(const list<T>& lhs, const list<T>& rhs)
{
  return !(rhs < lhs);
}

template <class T>
bool operator>=(const list<T>& lhs, const list<T>& rhs)
{
  return !(lhs < rhs);
}

// 重载 mystl 的 swap
template <class T>
void swap(list<T>& lhs, list<T>& rhs) noexcept
{
  lhs.swap(rhs);
}

}
#endif