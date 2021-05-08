#ifndef MYSTL_HASHTABLE_H
#define MYSTL_HASHTABLE_H

// 头文件包含了一个模板类，hashtable
// hashtable: 哈希表， 使用开链法处理哈希冲突


#include <initializer_list>

#include "algo.h"
#include "functional.h"
#include "memory.h"
#include "vector.h"
#include "util.h"
#include "exceptdef.h"

namespace mystl
{

// hashtable 上的节点
// 链表上的节点
template <class T>
struct hashtable_node
{
    hashtable_node* next;   // 指向下一个节点
    T               value;  // 存储实值
    hashtable_node() = default;
    hashtable_node(const T& n)
        :next(nullptr),
        value(n)
    {
    }
    // 拷贝构造函数
    hashtable_node(const hashtable_node& node)
        : next(node.next),
        value(node.value)
    {
    }
    // 移动构造函数
    hashtable_node(hashtable_node&& node)
        :next(node.next),
        value(mystl::move(value))
    {
    }
};

template <class T, bool>
struct ht_value_traits_imp
{   
    // 对于set 来说， key_type 和 mapped_type 是相同类型
    typedef T key_type;
    typedef T mapped_type;
    typedef T value_type;

    // 外面对象是个set的话， key 和 value 返回相同的值
    template <class Ty>
    static const key_type& get_key(const Ty& value)
    {
        return value;
    }

    template <class Ty>
    static const value_type& get_value(const Ty& value)
    {
        return value;
    }
};

template <class T>
struct ht_value_traits_imp<T, true>
{   
    // 对于map 来说，value.first 为key, value.second 为对应的值
    typedef typename std::remove_cv<typename T::first_type>::type   key_type;
    typedef typename T::second_type                                 mapped_type;
    typedef T                                                       value_type;

    // 外面对象是个set的话， key 和 value 返回相同的值
    template <class Ty>
    static const key_type& get_key(const Ty& value)
    {
        return value.first;
    }

    template <class Ty>
    static const value_type& get_value(const Ty& value)
    {
        return value;
    }
};


// ht_value_traits 只是为了推导出模板出的某个具体参数， 而实际的工作全部在 ht_value_traits_imp 中完成 
template <class T>
struct ht_value_traits
{   // 判断 value 是否为一个 pair(键值对)， 如果是说明外面的对象是一个map, 不是说明外面的对象是一个set,
    // 然后再取调用相应的泛化版本 或者特化版本
    static constexpr bool is_map = mystl::is_pair<T>::value;

    typedef ht_value_traits_imp<T,is_map>   value_traits_type;

    typedef typename value_traits_type::key_type      key_type;
    typedef typename value_traits_type::mapped_type   mapped_type;
    typedef typename value_traits_type::value_type    value_type; 

    template <class Ty>
    static const key_type& get_key(const Ty& value)
    {
        return value_traits_type::get_key(value);
    }

    template <class Ty>
    static const value_type& get_value(const Ty& value)
    {
        return value_traits_type::get_value(value);
    }
};


// 声明
template <class T, class HashFun, class KeyEqual>
class hashtable;

template <class T, class HashFun, class KeyEqual>
class ht_iterator;


template <class T, class HashFun, class KeyEqual>
class ht_const_iterator;

template <class T>
struct ht_local_iterator;

template <class T>
struct ht_const_local_iterator;

// ht_iterator

template <class T, class Hash, class KeyEqual>
struct ht_iterator_base :public mystl::iterator<mystl::forward_iterator_tag, T>
{ // 内嵌型别
  typedef mystl::hashtable<T, Hash, KeyEqual>         hashtable;
  typedef ht_iterator_base<T, Hash, KeyEqual>         base;
  typedef mystl::ht_iterator<T, Hash, KeyEqual>       iterator;
  typedef mystl::ht_const_iterator<T, Hash, KeyEqual> const_iterator;
  typedef hashtable_node<T>*                          node_ptr;
  typedef hashtable*                                  contain_ptr;
  typedef const node_ptr                              const_node_ptr;
  typedef const contain_ptr                           const_contain_ptr;

  typedef size_t                                      size_type;
  typedef ptrdiff_t                                   difference_type;

  node_ptr    node;  // 迭代器当前所指节点
  contain_ptr ht;    // 保持与容器的连结

  ht_iterator_base() = default;

  bool operator==(const base& rhs) const { return node == rhs.node; }
  bool operator!=(const base& rhs) const { return node != rhs.node; }
};

template <class T, class Hash, class KeyEqual>
struct ht_iterator: public ht_iterator_base<T, Hash, KeyEqual>
{
    typedef ht_iterator_base<T, Hash, KeyEqual> base;
    typedef typename base::hashtable            hashtable;
    typedef typename base::iterator             iterator;
    typedef typename base::const_iterator       const_iterator;
    typedef typename base::node_ptr             node_ptr;
    typedef typename base::contain_ptr          contain_ptr;

    typedef ht_value_traits<T>                  value_traits;
    typedef T                                   value_type;
    typedef value_type*                         pointer;
    typedef value_type&                         reference;     

    // 在子类中引用基类的成员要使用using
    using base::node;
    using base::ht;
    // 默认构造函数
    ht_iterator() = default;
    ht_iterator(node_ptr n, contain_ptr t)
    {
        node = n;
        ht = t;
    }
    // 拷贝构造函数
    ht_iterator(const iterator& rhs)
    {
        node = rhs.node;
        ht = rhs.ht;
    }
    ht_iterator(const const_iterator& rhs)
    {
        node = rhs.node;
        ht = rhs.ht;
    }
    iterator& operator=(const iterator& rhs)
    {
        if (this != &rhs)
        {
            node = rhs.node;
            ht = rhs.ht; 
        }
        return *this;
    }
    iterator& operator=(const const_iterator& rhs)
    {
        if (this != &rhs)
        {
            node = rhs.node;
            ht = rhs.ht; 
        }
        return *this;
    }

    reference  operator*()  const { return node->value; }
    pointer    operator->() const { return &(operator*()); }

    iterator& operator++()
    {
        assert(node != nullptr);
        const node_ptr old = node;
        // 同一个 bucket上 链表的下一个位置
        node = node->next;
        if(node == nullptr)
        {   // 如果下一个位置为空，跳到下一个bucket的起始处
            // index 是当前元素利用哈希函数计算出来的索引， 即第几个bucket
            auto index = ht->hash(value_traits::get_key(old->value));
            // node是vector中的节点，即每个链表的表头
            // 直到找到一个非空的链表
            while(!node && ++index < ht->bucket_size_)
                node = ht->buckets_[index];
        }
        return *this;
    }
    // 后置++ 会调用前置++， 并且产生临时对象
    iterator operator++(int)
    {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }
};

template <class T, class Hash, class KeyEqual>
struct ht_const_iterator :public ht_iterator_base<T, Hash, KeyEqual>
{
  typedef ht_iterator_base<T, Hash, KeyEqual> base;
  typedef typename base::hashtable            hashtable;
  typedef typename base::iterator             iterator;
  typedef typename base::const_iterator       const_iterator;
  typedef typename base::const_node_ptr       node_ptr;
  typedef typename base::const_contain_ptr    contain_ptr;

  typedef ht_value_traits<T>                  value_traits;
  typedef T                                   value_type;
  typedef const value_type*                   pointer;
  typedef const value_type&                   reference;

  using base::node;
  using base::ht;

  ht_const_iterator() = default;
  ht_const_iterator(node_ptr n, contain_ptr t)
  {
    node = n;
    ht = t;
  }
  ht_const_iterator(const iterator& rhs)
  {
    node = rhs.node;
    ht = rhs.ht;
  }
  ht_const_iterator(const const_iterator& rhs)
  {
    node = rhs.node;
    ht = rhs.ht;
  }
  const_iterator& operator=(const iterator& rhs)
  {
    if (this != &rhs)
    {
      node = rhs.node;
      ht = rhs.ht;
    }
    return *this;
  }
  const_iterator& operator=(const const_iterator& rhs)
  {
    if (this != &rhs)
    {
      node = rhs.node;
      ht = rhs.ht;
    }
    return *this;
  }

  // 重载操作符
  // 取内容
  reference operator*()  const { return node->value; }
  // 取内容的地址
  pointer   operator->() const { return &(operator*()); }

  const_iterator& operator++()
  {
    MYSTL_DEBUG(node != nullptr);
    const node_ptr old = node;
    node = node->next;
    if (node == nullptr)
    { // 如果下一个位置为空，跳到下一个 bucket 的起始处
      auto index = ht->hash(value_traits::get_key(old->value));
      while (!node && ++index < ht->bucket_size_)
      {
        node = ht->buckets_[index];
      }
    }
    return *this;
  }
  const_iterator operator++(int)
  {
    const_iterator tmp = *this;
    ++*this;
    return tmp;
  }
};

// local iterator
// 在同一个bucket, 即同一个链表上的迭代器
template <class T>
struct ht_local_iterator :public mystl::iterator<mystl::forward_iterator_tag, T>
{
  typedef T                          value_type;
  typedef value_type*                pointer;
  typedef value_type&                reference;
  typedef size_t                     size_type;
  typedef ptrdiff_t                  difference_type;
  typedef hashtable_node<T>*         node_ptr;

  typedef ht_local_iterator<T>       self;
  typedef ht_local_iterator<T>       local_iterator;
  typedef ht_const_local_iterator<T> const_local_iterator;
  node_ptr node;

  ht_local_iterator(node_ptr n)
    :node(n)
  {
  }
  ht_local_iterator(const local_iterator& rhs)
    :node(rhs.node)
  {
  }
  ht_local_iterator(const const_local_iterator& rhs)
    :node(rhs.node)
  {
  }

  reference operator*()  const { return node->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    MYSTL_DEBUG(node != nullptr);
    node = node->next;
    return *this;
  }
  
  self operator++(int)
  {
    self tmp(*this);
    ++*this;
    return tmp;
  }

  bool operator==(const self& other) const { return node == other.node; }
  bool operator!=(const self& other) const { return node != other.node; }
};

template <class T>
struct ht_const_local_iterator :public mystl::iterator<mystl::forward_iterator_tag, T>
{
  typedef T                          value_type;
  typedef const value_type*          pointer;
  typedef const value_type&          reference;
  typedef size_t                     size_type;
  typedef ptrdiff_t                  difference_type;
  typedef const hashtable_node<T>*   node_ptr;

  typedef ht_const_local_iterator<T> self;
  typedef ht_local_iterator<T>       local_iterator;
  typedef ht_const_local_iterator<T> const_local_iterator;

  node_ptr node;

  ht_const_local_iterator(node_ptr n)
    :node(n)
  {
  }
  ht_const_local_iterator(const local_iterator& rhs)
    :node(rhs.node)
  {
  }
  ht_const_local_iterator(const const_local_iterator& rhs)
    :node(rhs.node)
  {
  }

  reference operator*()  const { return node->value; }
  pointer   operator->() const { return &(operator*()); }

  self& operator++()
  {
    MYSTL_DEBUG(node != nullptr);
    node = node->next;
    return *this;
  }

  self operator++(int)
  {
    self tmp(*this);
    ++*this;
    return tmp;
  }

  bool operator==(const self& other) const { return node == other.node; }
  bool operator!=(const self& other) const { return node != other.node; }
};


#define PRIME_NUM 99

// 1. start with p = 101
// 2. p = next_prime(p * 1.7)
// 3. if p < (2 << 63), go to step 2, otherwise, go to step 4
// 4. end with p = prev_prime(2 << 63 - 1)
// bucket 的数量
static constexpr size_t ht_prime_list[] = {
  101ull, 173ull, 263ull, 397ull, 599ull, 907ull, 1361ull, 2053ull, 3083ull,
  4637ull, 6959ull, 10453ull, 15683ull, 23531ull, 35311ull, 52967ull, 79451ull,
  119179ull, 178781ull, 268189ull, 402299ull, 603457ull, 905189ull, 1357787ull,
  2036687ull, 3055043ull, 4582577ull, 6873871ull, 10310819ull, 15466229ull,
  23199347ull, 34799021ull, 52198537ull, 78297827ull, 117446801ull, 176170229ull,
  264255353ull, 396383041ull, 594574583ull, 891861923ull, 1337792887ull,
  2006689337ull, 3010034021ull, 4515051137ull, 6772576709ull, 10158865069ull,
  15238297621ull, 22857446471ull, 34286169707ull, 51429254599ull, 77143881917ull,
  115715822899ull, 173573734363ull, 260360601547ull, 390540902329ull, 
  585811353559ull, 878717030339ull, 1318075545511ull, 1977113318311ull, 
  2965669977497ull, 4448504966249ull, 6672757449409ull, 10009136174239ull,
  15013704261371ull, 22520556392057ull, 33780834588157ull, 50671251882247ull,
  76006877823377ull, 114010316735089ull, 171015475102649ull, 256523212653977ull,
  384784818980971ull, 577177228471507ull, 865765842707309ull, 1298648764060979ull,
  1947973146091477ull, 2921959719137273ull, 4382939578705967ull, 6574409368058969ull,
  9861614052088471ull, 14792421078132871ull, 22188631617199337ull, 33282947425799017ull,
  49924421138698549ull, 74886631708047827ull, 112329947562071807ull, 168494921343107851ull,
  252742382014661767ull, 379113573021992729ull, 568670359532989111ull, 853005539299483657ull,
  1279508308949225477ull, 1919262463423838231ull, 2878893695135757317ull, 4318340542703636011ull,
  6477510814055453699ull, 9716266221083181299ull, 14574399331624771603ull, 18446744073709551557ull
};

// 找出最接近并大于等于 n 的那个质数
inline size_t ht_next_prime(size_t n)
{
    const size_t* first = ht_prime_list;
    const size_t* last = ht_prime_list + PRIME_NUM;
    const size_t* pos = mystl::lower_bound(first, last, n);
    return pos == last ? (*last - 1) : *pos;
}

// 模板类 hashtable
// 参数一代表数据类型，参数二代表哈希函数， 参数三代表键值比较相等的函数
template <class T, class Hash, class KeyEqual>
class hashtable
{
    friend struct mystl::ht_iterator<T, Hash, KeyEqual>;
    friend struct mystl::ht_const_iterator<T, Hash, KeyEqual>;

public:
    // hashtable 的型别定义
    typedef ht_value_traits<T>                          value_traits;
    typedef typename value_traits::key_type             key_type;
    typedef typename value_traits::mapped_type          mapped_type;
    typedef typename value_traits::value_type           value_type;
    typedef Hash                                        hasher;
    typedef KeyEqual                                    key_equal;

    typedef hashtable_node<T>                           node_type;
    typedef node_type*                                  node_ptr;
    typedef mystl::vector<node_ptr>                     bucket_type;

    typedef mystl::allocator<T>                         allocator_type;
    typedef mystl::allocator<T>                         data_allocator;
    typedef mystl::allocator<node_type>                 node_allocator;

    typedef typename allocator_type::pointer            pointer;
    typedef typename allocator_type::const_pointer      const_pointer;
    typedef typename allocator_type::reference          reference;
    typedef typename allocator_type::const_reference    const_reference;
    typedef typename allocator_type::size_type          size_type;
    typedef typename allocator_type::difference_type    difference_type;

    typedef mystl::ht_iterator<T, Hash, KeyEqual>       iterator;
    typedef mystl::ht_const_iterator<T, Hash, KeyEqual> const_iterator;
    typedef mystl::ht_local_iterator<T>                 local_iterator;
    typedef mystl::ht_const_local_iterator<T>           const_local_iterator;

    allocator_type get_allocator() const { return allocator_type(); }

private:
    // 用以下六个参数来表现 hashtable
    bucket_type buckets_;       // vector<node>
    size_type   bucket_size_;   // bucket 的数量
    size_type   size_;          // 总的节点数量
    float       mlf_;           // 平均每个bucket 所挂载的节点数量
    hasher      hash_;          // 哈希函数
    key_equal   equal_;          // 键值比较相等的函数


private:
    bool is_equal(const key_type& key1, const key_type& key2)
    {
        return equal_(key1, key2);
    }

    bool is_equal(const key_type& key1, const key_type& key2)  const
    {
        return equal_(key1, key2);
    }

    // 根据节点，返回对应的const 迭代器
    const_iterator M_cit(node_ptr node) const noexcept
    {
        return const_iterator(node, const_cast<hashtable*>(this));
    }

    iterator M_begin() noexcept
    {
        for(size_type n = 0; n < bucket_size_; ++n)
        {   // buckets_[n] 一定是链表的头结点
            if(buckets_[n])  // 找到第一个有节点的位置就返回
                return iterator(buckets_[n], this);
        }
        return iterator(nullptr, this);
    }
    const_iterator M_begin() const noexcept
    {
        for(size_type n = 0; n < bucket_size_; ++n)
        {   // buckets_[n] 一定是链表的头结点
            if(buckets_[n])  // 找到第一个有节点的位置就返回
                return M_cit(buckets_[n], this); // M_cit 返回const 迭代器
        }
        return M_cit(nullptr, this);
    }

    public:
    // 构造函数
    explicit hashtable(size_type bucket_count,
                        const Hash& hash = Hash(),
                        const KeyEqual& equal = KeyEqual())
        :size_(0), mlf_(1.0f), hash_(hash), equal_(equal)
    {
        init(bucket_count);
    }

    template <class Iter, typename std::enable_if<
    mystl::is_input_iterator<Iter>::value, int>::type = 0>
    hashtable(Iter first, Iter last,
              size_type bucket_count,
              const Hash& hash = Hash(),
              const KeyEqual& equal = KeyEqual())
    :size_(mystl::distance(first, last)), mlf_(1.0f), hash_(hash), equal_(equal)
    {
        init(mystl::max(bucket_count, static_cast<size_type>(mystl::distance(first, last))));
    }

    // 拷贝构造函数
    hashtable(const hashtable& rhs)
        : hash_(rhs.hash_), equal_(rhs.equal_)
    {
        copy_init(rhs);
    }

    // 移动构造函数
    hashtable(hashtable&& rhs) noexcept
        : bucket_size_(rhs.bucket_size_),
        size_(rhs.size_),
        mlf_(rhs.mlf_),
        hash_(rhs.hash_),
        equal_(rhs.equal_)
    {
        buckets_ = mystl::move(rhs.buckets_);
        rhs.bucket_size_ = 0;
        rhs.size_ = 0;
        rhs.mlf_ = 0.0f;
    }
    // hashtable(hashtable&& rhs) noexcept
    //   : bucket_size_(rhs.bucket_size_), 
    //   size_(rhs.size_),
    //   mlf_(rhs.mlf_),
    //   hash_(rhs.hash_),
    //   equal_(rhs.equal_)
    // {
    //   buckets_ = mystl::move(rhs.buckets_);
    //   rhs.bucket_size_ = 0;
    //   rhs.size_ = 0;
    //   rhs.mlf_ = 0.0f;
    // }

    // 拷贝赋值 / 移动赋值
    hashtable& operator=(const hashtable& rhs);
    hashtable& operator=(hashtable&& rhs) noexcept;

    ~hashtable(){ clear(); }

    // 迭代器相关操作
    // 迭代器只能单向移动，所以不需要反向迭代器
    iterator       begin()        noexcept
    { return M_begin(); }
    const_iterator begin()  const noexcept
    { return M_begin(); }
    iterator       end()          noexcept
    { return iterator(nullptr, this); }
    const_iterator end()    const noexcept
    { return M_cit(nullptr); }
    
    const_iterator cbegin() const noexcept
    { return begin(); }
    const_iterator cend()   const noexcept
    { return end(); }

    // 容量的相关操作
    bool        empty()    const  noexcept { return size_ == 0; }
    size_type   size()     const  noexcept { return size_; }
    size_type   max_size() const  noexcept { return static_cast<size_type>(-1); }

    // 修改容器相关操作
    template <class ...Args>
    iterator emplace_multi(Args&& ... args);

    template <class ...Args>
    pair<iterator, bool> emplace_unique(Args&& ...args);

    // [note]: hint 对于 hash_table 其实没有意义，因为即使提供了 hint，也要做一次 hash，
    // 来确保 hash_table 的性质，所以选择忽略它
    template <class ...Args>
    iterator emplace_multi_use_hint(const_iterator /*hint*/, Args&& ...args)
    { return emplace_multi(mystl::forward<Args>(args)...); }

    template <class ...Args>
    iterator emplace_unique_use_hint(const_iterator /*hint*/, Args&& ...args)
    { return emplace_unique(mystl::forward<Args>(args)...).first; }

    //insert 
    // 不改变bucket数量的基础上，插入元素
    iterator                insert_multi_noresize(const value_type& value);
    pair<iterator, bool>    insert_unique_noresize(const value_type& value);

    iterator    insert_multi(const value_type& value)
    {
        rehash_if_need(1);
        return insert_multi_noresize(value);
    }

    iterator    insert_multi(value_type&& value)
    {
        return emplace_multi(mystl::move(value));
    }

    pair<iterator, bool>    insert_unique(const value_type& value)
    {
        rehash_if_need(1);
        return insert_unique_noresize(value);
    }

    pair<iterator, bool>    insert_unique(value_type&& value)
    {
        return emplace_unique(value);
    }

      // [note]: 同 emplace_hint
    iterator insert_multi_use_hint(const_iterator /*hint*/, const value_type& value)
    { return insert_multi(value); }
    iterator insert_multi_use_hint(const_iterator /*hint*/, value_type&& value)
    { return emplace_multi(mystl::move(value)); }

    iterator insert_unique_use_hint(const_iterator /*hint*/, const value_type& value)
    { return insert_unique(value).first; }
    iterator insert_unique_use_hint(const_iterator /*hint*/, value_type&& value)
    { return emplace_unique(mystl::move(value)); }

    template <class InputIter>
    void    insert_multi(InputIter first, InputIter last)
    {
        copy_insert_multi(first,last,iterator_category(first));
    }
    template <class InputIter>
    void    insert_unique(InputIter first, InputIter last)
    {
        copy_insert_unique(first,last,iterator_category(first));
    }

    //erase / clear
    void    erase(const_iterator position);
    void    erase(const_iterator first, const_iterator last);

    size_type erase_multi(const key_type& key);
    size_type erase_unique(const key_type& key);

    void clear();
    void swap(hashtable& rhs) noexcept;

    // 查找相关操作
    size_type       count(const key_type& key) const;

    iterator        find(const key_type& key);
    const_iterator  find(const key_type& key) const;

    pair<iterator, iterator> equal_range_multi(const key_type& key);
    pair<const_iterator, const_iterator> equal_range_multi(const key_type& key) const;

    pair<iterator, iterator> equal_range_unique(const key_type& key);
    pair<const_iterator, const_iterator> equal_range_unique(const key_type& key) const;

    local_iterator       begin(size_type n)        noexcept
    { 
        MYSTL_DEBUG(n < size_);
        return buckets_[n];   
    }
    const_local_iterator begin(size_type n)  const noexcept
    { 
        MYSTL_DEBUG(n < size_);
        return buckets_[n];
    }
    const_local_iterator cbegin(size_type n) const noexcept
    { 
        MYSTL_DEBUG(n < size_);
        return buckets_[n];
    }

    local_iterator       end(size_type n)          noexcept
    { 
        MYSTL_DEBUG(n < size_);
        return nullptr; 
    }
    const_local_iterator end(size_type n)    const noexcept
    { 
        MYSTL_DEBUG(n < size_);
        return nullptr; 
    }
    const_local_iterator cend(size_type n)   const noexcept
    {
        MYSTL_DEBUG(n < size_);
        return nullptr; 
    }

    size_type bucket_count()                 const noexcept
    { return bucket_size_; }
    size_type max_bucket_count()             const noexcept
    { return ht_prime_list[PRIME_NUM - 1]; }

    size_type bucket_size(size_type n)       const noexcept;
    size_type bucket(const key_type& key)    const
    { return hash(key); }   // bucket 就是利用哈希函数 根据键值计算实际的位置

    // hash policy
    // 每个bucket上挂载的节点数量
    float load_factor() const noexcept
    {
        return bucket_size_ != 0 ? (float)size_ / bucket_size_ : 0.0f;
    }
    float max_load_factor() const noexcept
    {
        return mlf_;
    }
    // 设置mlf
    void max_load_factor(float ml)
    {
        THROW_OUT_OF_RANGE_IF(ml != ml || ml < 0, "invalid hash load factor");
        mlf_ = ml;
    } 
    void rehash(size_type count);

    void reserve(size_type count)
    { rehash(static_cast<size_type>((float)count / max_load_factor() + 0.5f)); }

    hasher  hash_fcn() const { return hash_; }
    key_equal key_eq() const { return equal_; }

private:
    // init
    void    init(size_type n);
    void    copy_init(const hashtable& ht);

    // node
    template <class ...Args>
    node_ptr create_node(Args&& ...args);
    void     destroy_node(node_ptr n);

    // hash
    size_type next_size(size_type n) const;
    size_type hash(const key_type& key, size_type n) const;
    size_type hash(const key_type& key) const;
    void      rehash_if_need(size_type n);

    // insert
    template <class InputIter>
    void copy_insert_multi(InputIter first, InputIter last, mystl::input_iterator_tag);
    template <class ForwardIter>
    void copy_insert_multi(ForwardIter first, ForwardIter last, mystl::forward_iterator_tag);
    template <class InputIter>
    void copy_insert_unique(InputIter first, InputIter last, mystl::input_iterator_tag);
    template <class ForwardIter>
    void copy_insert_unique(ForwardIter first, ForwardIter last, mystl::forward_iterator_tag);

    // insert node
    pair<iterator, bool> insert_node_unique(node_ptr np);
    iterator             insert_node_multi(node_ptr np);

    // bucket operator
    void replace_bucket(size_type bucket_count);
    void erase_bucket(size_type n, node_ptr first, node_ptr last);
    void erase_bucket(size_type n, node_ptr last);

    // comparision
    bool equal_to_multi(const hashtable& other);
    bool equal_to_unique(const hashtable& other);
};
//************************************************************************************************

// 拷贝赋值
template <class T, class Hash, class KeyEqual>
hashtable<T, Hash, KeyEqual>&
hashtable<T, Hash, KeyEqual>::
operator=(const hashtable& rhs)
{
  if (this != &rhs)
  {
    hashtable tmp(rhs);  // const 变量不允许修改，所以只能构造临时变量后再交换
    swap(tmp);
  }
  return *this;
}

// 移动赋值
template <class T, class Hash, class KeyEqual>
hashtable<T, Hash, KeyEqual>&
hashtable<T, Hash, KeyEqual>::
operator=(hashtable&& rhs) noexcept
{
    hashtable tmp(mystl::move(rhs));
    swap(tmp);
    return *this;
}

template <class T, class Hash, class keyEqual>
template <class ...Args>
typename hashtable<T,Hash,keyEqual>::iterator
hashtable<T,Hash,keyEqual>::
emplace_multi(Args&& ...args)
{   // 创建要插入的节点
    auto np = create_node(mystl::forward<Args>(args)...);
    try
    {
        if((float)(size_) + 1 > (float)bucket_size_ * max_load_factor())
            rehash(size_ + 1);
    }
    catch(...)
    {
        destroy_node(np);
        throw;
    }
    // 最后，执行插入动作
    return insert_node_multi(np);
}


template <class T, class Hash, class keyEqual>
template <class ...Args>
pair<typename hashtable<T,Hash,keyEqual>::iterator,bool>
hashtable<T,Hash,keyEqual>::
emplace_unique(Args&& ...args)
{   // 创建要插入的节点
    auto np = create_node(mystl::forward<Args>(args)...);
    try
    {
        if((float)(size_) + 1 > (float)bucket_size_ * max_load_factor())
            rehash(size_ + 1);
    }
    catch(...)
    {
        destroy_node(np);
        throw;
    }
    // 最后，执行插入动作
    return insert_node_unique(np);
}

// 在不需要重新创建表格的情况下插入新节点，键值不允许重复
template <class T, class Hash, class keyEqual>
pair<typename hashtable<T,Hash,keyEqual>::iterator,bool>
hashtable<T,Hash,keyEqual>::
insert_unique_noresize(const value_type& value)
{
    const auto n = hash(value_traits::get_key(value));
    //  first 为该节点在bucket 中的位置
    auto first = buckets_[n];
    for(auto cur = first; cur; cur = cur->next)
    {   // 如果当前链表上有和当前值相等的元素， 直接插入失败
        if(is_equal(value_traits::get_key(cur->value), value_traits::get_key(value)))
            return mystl::make_pair(iterator(cur,this),false);
    }
    // 让新节点成为链表的第一个节点
    auto tmp = create_node(value);
    tmp->next = first;
    buckets_[n] = tmp;  // 把这个节点添加到vector中
    ++size_;
    return mystl::make_pair(iterator(tmp,this),false);
}


// 在不需要重新创建表格的情况下插入新节点，键值不允许重复
template <class T, class Hash, class keyEqual>
typename hashtable<T,Hash,keyEqual>::iterator
hashtable<T,Hash,keyEqual>::
insert_multi_noresize(const value_type& value)
{
    const auto n = hash(value_traits::get_key(value));
    //  first 为该节点在bucket 中的位置
    auto first = buckets_[n];
    auto tmp = create_node(value);
    
    for(auto cur = first; cur; cur = cur->next)
    {   // 如果当前链表上有和当前值相等的元素， 直接插入失败
        if(is_equal(value_traits::get_key(cur->value), value_traits::get_key(value)))
        {
            tmp->next = cur->next;
            cur->next = tmp;
            ++size_;
            return iterator(tmp, this);
        }
            
    }
    // 否则插入在链表头部
    tmp->next = first;
    buckets_[n] = tmp;  // 把这个节点添加到vector中
    ++size_;
    return iterator(tmp,this);
}

// 删除迭代器所指向的节点
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
erase(const_iterator position)
{
    auto p = position.node;
    if(p)
    {
        const auto n = hash(value_traits::get_key(p->value));
        // cur 为postion 所在链表的头结点
        auto cur = buckets_[n];
        if(cur == p)
        {// 如果 p 位于链表头部， 则需要更换vector中的节点
            buckets_[n] = cur->next;
            destroy_node(cur);
            --size_;
        }
        // p 不是位于链表头部
        else{
            auto next = cur->next;
            while(next)
            {   // 直到遇到要删除的节点为止
                if(next == p)
                {
                    cur->next = next->next;
                    destroy_node(next);
                    --size_;
                    break;
                }
                // 没有遇到要删除的节点
                else
                {
                    cur = next;
                    next = cur->next;
                }
            }
        }
    }
}

// 删除[first, last) 内的节点
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
erase(const_iterator first, const_iterator last)
{
    if(first.node == last.node)
        return;
    auto first_bucket = first.node
        ? hash(value_traits::get_key(first.node->value))
        :bucket_size_;
    auto last_bucket = last.node
        ? hash(value_traits::get_key(last.node->value))
        :bucket_size_;

    if(first_bucket == last_bucket)
    {   // 如果在 同一个bucket 内
        erase_bucket(first_bucket, first.node, last.node);
    }
    // 不在同一个bucket 内
    else{
        // 先删除first_bucket 内所有的节点
        erase_bucket(first_bucket, first.node, nullptr);
        // 再删除[first+1, last) 所有的bucket
        for(auto n = first_bucket + 1; n < last_bucket; ++n)
        {
            if(buckets_[n] != nullptr)
                erase_bucket(n, nullptr);
        }
        // 如果last_bucket 不是最后一个bucket，将其部分节点删除
        if(last_bucket != bucket_size_)
            erase_bucket(last_bucket, last.node);
    }
}

// 删除键值为 key 的节点
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
erase_multi(const key_type& key)
{   // p 是一个pair, 即等于键值的范围的前后迭代器
    auto p = equal_range_multi(key);
    if(p.first.node != nullptr)
    {
        erase(p.first, p.second);
        // 返回被删除的节点数量
        return mystl::distance(p.first, p.second);
    }
    // 说明没找到
    return 0; 
}


template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
erase_unique(const key_type& key)
{
    const auto n = hash(key);
    auto first = buckets_[n];

    if(first)
    {
        // key 为链表的第一个节点
        if(is_equal(value_traits::get_key(first->value), key))
        {
            buckets_[n] = first->next;
            destroy_node(first);
            --size_;
            return 1;
        }
        // key 不是链表的第一个节点
        else{
            auto next = first->next;
            while(next)
            {   // 直到在该链表上找到对应的键值
                if(is_equal(value_traits::get_key(next->value), key))
                {
                    first->next = next->next;
                    destroy_node(next);
                    --size_;
                    return 1;
                }
                // 如果没找到，first 和 next 正常向下移动
                first = next;
                next = first->next;
            }
        }
    }
}

// 清空 hash_table
// 将所有链表清空， 保留vector
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
clear()
{
    if(size_ != 0)
    {
        for(size_type i = 0; i < bucket_size_; i++)
        {
            node_ptr cur = buckets_[i];
            while(cur != nullptr)
            {
                node_ptr next  = cur->next;
                destroy_node(cur);
                cur = next;
            }
            buckets_[i] = nullptr;
        }
        size_ = 0;
    }
}

// 在某个bucket 节点的数量
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
bucket_size(size_type n) const noexcept
{
    size_type result = 0;
    for(auto cur = buckets_[n]; cur; cur = cur->next)
    {
        ++result;
    }
    return result;
}


// 重新对元素进行一遍哈希， 插入到新的位置
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
rehash(size_type count)
{   // 新的bucket 数量
    auto n = ht_next_prime(count);
    // 扩大bucket数量
    if(n > bucket_size_)
    {
        replace_bucket(n);
    }
    else{
        // 减少 bucket 数量(前提是值得减少)
        if ((float)size_ / (float)n < max_load_factor() - 0.25f &&
            (float)n < (float)bucket_size_ * 0.75)  // worth rehash
        {
            replace_bucket(n);
        }
    }
}

// 查找键值为 key 的节点， 返回迭代器
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::iterator
hashtable<T, Hash, KeyEqual>::
find(const key_type& key)
{
    const auto n = hash(key);
    node_ptr first = buckets_[n];
    for(; first && !is_equal(value_traits::get_key(first->value),key); first = first->next){}
    return iterator(first, this);
}

template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::const_iterator
hashtable<T, Hash, KeyEqual>::
find(const key_type& key) const
{
  const auto n = hash(key);
  node_ptr first = buckets_[n];
  for (; first && !is_equal(value_traits::get_key(first->value), key); first = first->next) {}
  return M_cit(first);
}

// 查找键值为 Key 出现的次数
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
count(const key_type& key) const
{
    const auto n = hash(key);
    size_type result = 0;
    for(auto cur = buckets_[n]; cur; cur = cur->next)
    {   
        if(is_equal(value_traits::get_key(cur->value), key))
            ++result;
    }
    return result;
}

// 查找与键值 key 相等的区间， 返回一个pair, 指向相等的区间
template <class T, class Hash, class KeyEqual>
pair<typename hashtable<T, Hash, KeyEqual>::iterator,
    typename hashtable<T, Hash, KeyEqual>::iterator>
hashtable<T, Hash, KeyEqual>::
equal_range_multi(const key_type& key)
{
    const auto n = hash(key);
    for(node_ptr first = buckets_[n]; first; first = first->next)
    {   // 出现相等的键值
        if(is_equal(value_traits::get_key(first->value), key))
        {
            for(node_ptr second = first->next; second; second = second->next)
            {
                if(!is_equal(value_traits::get_key(second->value), key))
                    return mystl::make_pair(iterator(first, this), iterator(second, this));
            }
            // 整条链表都相等， 查找下一个链表
            for(auto m = n + 1; m < bucket_size_; ++m)
            {
                if(buckets_[m])
                    return mystl::make_pair(iterator(first, this), iterator(buckets_[m], this));
            }
            // 直到整个哈希表的末尾都相等
            return mystl::make_pair(iterator(first, this), end());
        }
    }
    // 找不到相等的则返回end()
    return mystl::make_pair(end(), end());
}

template <class T, class Hash, class KeyEqual>
pair<typename hashtable<T, Hash, KeyEqual>::const_iterator,
  typename hashtable<T, Hash, KeyEqual>::const_iterator>
hashtable<T, Hash, KeyEqual>::
equal_range_multi(const key_type& key) const
{
  const auto n = hash(key);
  for (node_ptr first = buckets_[n]; first; first = first->next)
  {
    if (is_equal(value_traits::get_key(first->value), key))
    {
      for (node_ptr second = first->next; second; second = second->next)
      {
        if (!is_equal(value_traits::get_key(second->value), key))
          return mystl::make_pair(M_cit(first), M_cit(second));
      }
      for (auto m = n + 1; m < bucket_size_; ++m)
      { // 整个链表都相等，查找下一个链表出现的位置
        if (buckets_[m])
          return mystl::make_pair(M_cit(first), M_cit(buckets_[m]));
      }
      return mystl::make_pair(M_cit(first), cend());
    }
  }
  return mystl::make_pair(cend(), cend());
}

template <class T, class Hash, class KeyEqual>
pair<typename hashtable<T, Hash, KeyEqual>::iterator,
  typename hashtable<T, Hash, KeyEqual>::iterator>
hashtable<T, Hash, KeyEqual>::
equal_range_unique(const key_type& key)
{
  const auto n = hash(key);
  for (node_ptr first = buckets_[n]; first; first = first->next)
  {
    if (is_equal(value_traits::get_key(first->value), key))
    {
      if (first->next)
        return mystl::make_pair(iterator(first, this), iterator(first->next, this));
      for (auto m = n + 1; m < bucket_size_; ++m)
      { // 整个链表都相等，查找下一个链表出现的位置
        if (buckets_[m])
          return mystl::make_pair(iterator(first, this), iterator(buckets_[m], this));
      }
      return mystl::make_pair(iterator(first, this), end());
    }
  }
  return mystl::make_pair(end(), end());
}

template <class T, class Hash, class KeyEqual>
pair<typename hashtable<T, Hash, KeyEqual>::const_iterator,
  typename hashtable<T, Hash, KeyEqual>::const_iterator>
hashtable<T, Hash, KeyEqual>::
equal_range_unique(const key_type& key) const
{
  const auto n = hash(key);
  for (node_ptr first = buckets_[n]; first; first = first->next)
  {
    if (is_equal(value_traits::get_key(first->value), key))
    {
      if (first->next)
        return mystl::make_pair(M_cit(first), M_cit(first->next));
      for (auto m = n + 1; m < bucket_size_; ++m)
      { // 整个链表都相等，查找下一个链表出现的位置
        if (buckets_[m])
          return mystl::make_pair(M_cit(first), M_cit(buckets_[m]));
      }
      return mystl::make_pair(M_cit(first), cend());
    }
  }
  return mystl::make_pair(cend(), cend());
}

template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
swap(hashtable& rhs) noexcept
{
    if (this != &rhs)
    {
        buckets_.swap(rhs.buckets_);
        mystl::swap(bucket_size_, rhs.bucket_size_);
        mystl::swap(size_, rhs.size_);
        mystl::swap(mlf_, rhs.mlf_);
        mystl::swap(hash_, rhs.hash_);
        mystl::swap(equal_, rhs.equal_);
    }
}

//*****************************************************************************************************
// helper function
// init 函数
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
init(size_type n)
{   // bucket 的数量
    const auto bucket_nums = next_size(n);
    try
    {   // 申请一个容量为 bucket_nums 的vector, 并且将其中所有元素全部初始化为nullptr
        buckets_.reserve(bucket_nums);
        buckets_.assign(bucket_nums, nullptr);
    }
    catch(...)
    {
        bucket_size_ = 0;
        size_ = 0;
        throw;
    }
    bucket_size_ = buckets_.size();
}

template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
copy_init(const hashtable& ht)
{
    bucket_size_ = 0;
    buckets_.reserve(ht.bucket_size_);
    buckets_.assign(ht.bucket_size_, nullptr);
    try
    {
        for(size_type i = 0; i < ht.bucket_size_; ++i)
        {   
            node_ptr cur = buckets_[i];
            if(cur)
            {
                auto copy = create_node(cur->value);
                buckets_[i] = copy;
                for(auto next = cur->next; next; cur = next, next = cur->next)
                {
                    copy->next = create_node(next->value);
                    copy = copy->next;
                }
                copy->next = nullptr;
            }
        }
        bucket_size_ = ht.bucket_size_;
        mlf_ = ht.mlf_;
        size_ = ht.size_;
    }
    catch(...)
    {
        clear();
    }
}

// create_node 函数
template <class T, class Hash, class KeyEqual>
template <class ...Args>
typename hashtable<T, Hash, KeyEqual>::node_ptr
hashtable<T, Hash, KeyEqual>::
create_node(Args&& ...args)
{
  node_ptr tmp = node_allocator::allocate(1);
  try
  {
    data_allocator::construct(mystl::address_of(tmp->value), mystl::forward<Args>(args)...);
    tmp->next = nullptr;
  }
  catch (...)
  {
    node_allocator::deallocate(tmp);
    throw;
  }
  return tmp;
}

// destroy_node 函数
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
destroy_node(node_ptr node)
{ // 析构对象
  data_allocator::destroy(mystl::address_of(node->value));
  // 释放内存
  node_allocator::deallocate(node);
  node = nullptr;
}

// next_size 函数
// 返回合适的bucket 数量
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::next_size(size_type n) const
{
  return ht_next_prime(n);
}

// hash 函数
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
hash(const key_type& key, size_type n) const
{
    return hash_(key) % n;
}

template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::size_type
hashtable<T, Hash, KeyEqual>::
hash(const key_type& key) const
{
    return hash_(key) % bucket_size_;
}

// rehash_if_need 函数
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
rehash_if_need(size_type n)
{
  if (static_cast<float>(size_ + n) > (float)bucket_size_ * max_load_factor())
    rehash(size_ + n);
}

// copy_insert
template <class T, class Hash, class KeyEqual>
template <class InputIter>
void hashtable<T, Hash, KeyEqual>::
copy_insert_multi(InputIter first, InputIter last, mystl::input_iterator_tag)
{
  rehash_if_need(mystl::distance(first, last));
  for (; first != last; ++first)
    insert_multi_noresize(*first);
}

template <class T, class Hash, class KeyEqual>
template <class ForwardIter>
void hashtable<T, Hash, KeyEqual>::
copy_insert_multi(ForwardIter first, ForwardIter last, mystl::forward_iterator_tag)
{
  size_type n = mystl::distance(first, last);
  rehash_if_need(n);
  for (; n > 0; --n, ++first)
    insert_multi_noresize(*first);
}

template <class T, class Hash, class KeyEqual>
template <class InputIter>
void hashtable<T, Hash, KeyEqual>::
copy_insert_unique(InputIter first, InputIter last, mystl::input_iterator_tag)
{
  rehash_if_need(mystl::distance(first, last));
  for (; first != last; ++first)
    insert_unique_noresize(*first);
}

template <class T, class Hash, class KeyEqual>
template <class ForwardIter>
void hashtable<T, Hash, KeyEqual>::
copy_insert_unique(ForwardIter first, ForwardIter last, mystl::forward_iterator_tag)
{
  size_type n = mystl::distance(first, last);
  rehash_if_need(n);
  for (; n > 0; --n, ++first)
    insert_unique_noresize(*first);
}

// insert_node 函数
template <class T, class Hash, class KeyEqual>
typename hashtable<T, Hash, KeyEqual>::iterator
hashtable<T, Hash, KeyEqual>::
insert_node_multi(node_ptr np)
{
    const auto n = hash(value_traits::get_key(np->value));
    auto cur = buckets_[n];
    // 如果计算出来的bucket 上没有链表，则直接插入
    if(cur == nullptr)
    {
        buckets_[n] = np;
        ++size_;
        return iterator(np, this);
    }
    for(; cur; cur = cur->next)
    {// 如果遇到相等的值
        if(is_equal(value_traits::get_key(cur->value), value_traits::get_key(np->value)))
        {   // 在cur 后面插入np
            np->next = cur->next;
            cur->next = np;
            ++size_;
            return iterator(np, this);
        }
    }
    // 如果没遇到相等的节点， 将该节点挂在链表的首位
    np->next = buckets_[n];
    buckets_[n] = np;
    ++size_;
    return iterator(np, this);
}

// insert_node_unique 函数
template <class T, class Hash, class KeyEqual>
pair<typename hashtable<T, Hash, KeyEqual>::iterator, bool>
hashtable<T, Hash, KeyEqual>::
insert_node_unique(node_ptr np)
{
  const auto n = hash(value_traits::get_key(np->value));
  auto cur = buckets_[n];
  if (cur == nullptr)
  {
    buckets_[n] = np;
    ++size_;
    return mystl::make_pair(iterator(np, this), true);
  }
  for (; cur; cur = cur->next)
  {
    if (is_equal(value_traits::get_key(cur->value), value_traits::get_key(np->value)))
    { // 如果遇到相等的节点， 则直接插入失败
      return mystl::make_pair(iterator(cur, this), false);
    }
  }
  np->next = buckets_[n];
  buckets_[n] = np;
  ++size_;
  return mystl::make_pair(iterator(np, this), true);
}

// replace_bucket 函数
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
replace_bucket(size_type bucket_count)
{ // 创建bucket
  bucket_type bucket(bucket_count);
  if (size_ != 0)
  { // 遍历每一个bucket
    for (size_type i = 0; i < bucket_size_; ++i)
    { // 遍历 bucket 上的链表
      for (auto first = buckets_[i]; first; first = first->next)
      {                       // first 指向原来的元素
        auto tmp = create_node(first->value);
        const auto n = hash(value_traits::get_key(first->value), bucket_count);
        auto f = bucket[n];
        bool is_inserted = false;
        //!  这个for 循环没看懂，在每个元素内为什么还要进行for循环
        for (auto cur = f; cur; cur = cur->next)
        {
          if (is_equal(value_traits::get_key(cur->value), value_traits::get_key(first->value)))
          {
            tmp->next = cur->next;
            cur->next = tmp;
            is_inserted = true;
            break;
          }
        }
        if (!is_inserted)
        {
          tmp->next = f;
          bucket[n] = tmp;
        }
      }
    }
  }
  buckets_.swap(bucket);
  bucket_size_ = buckets_.size();
}

// erase_bucket 函数
// 在第 n 个bucket内， 删除[first, last) 的节点
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
erase_bucket(size_type n, node_ptr first, node_ptr last)
{
    auto cur = buckets_[n];
    // first 为头结点
    if(cur == first)
    {
        erase_bucket(n, last);
    }
    else
    {
        node_ptr next = cur->next;
        // for循环， 用于找到first
        for(; next != first; cur = next, next = cur->next) {}
        while(next != last)
        {
            cur->next = next->next;
            destroy_node(next);
            next = cur->next;
            --size_;
        }
    }
}

// erase_bucket 函数
// 在第 n 个bucket 内， 删除[buckets_[n], last) 的节点
template <class T, class Hash, class KeyEqual>
void hashtable<T, Hash, KeyEqual>::
erase_bucket(size_type n, node_ptr last)
{
    auto cur = buckets_[n];
    while(cur != last)
    {
        auto next = cur->next;
        destroy_node(cur);
        cur = next;
        --size_;
    }
    buckets_[n] = last;
}

// equal_to 函数
// 判断两个hashtable 是否相等
template <class T, class Hash, class KeyEqual>
bool hashtable<T, Hash, KeyEqual>::equal_to_multi(const hashtable& other)
{
  if (size_ != other.size_)
    return false;
  for (auto f = begin(), l = end(); f != l;)
  {
    auto p1 = equal_range_multi(value_traits::get_key(*f));
    auto p2 = other.equal_range_multi(value_traits::get_key(*f));
    if (mystl::distance(p1.first, p1.last) != mystl::distance(p2.first, p2.last) ||
        !mystl::is_permutation(p1.first, p2.last, p2.first, p2.last))
      return false;
    f = p1.last;
  }
  return true;
}

template <class T, class Hash, class KeyEqual>
bool hashtable<T, Hash, KeyEqual>::equal_to_unique(const hashtable& other)
{
  if (size_ != other.size_)
    return false;
  for (auto f = begin(), l = end(); f != l; ++f)
  {
    auto res = other.find(value_traits::get_key(*f));
    if (res.node == nullptr || *res != *f)
      return false;
  }
  return true;
}

// 重载 mystl 的 swap
template <class T, class Hash, class KeyEqual>
void swap(hashtable<T, Hash, KeyEqual>& lhs,
          hashtable<T, Hash, KeyEqual>& rhs) noexcept
{
  lhs.swap(rhs);
}

}
#endif