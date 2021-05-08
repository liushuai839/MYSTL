#include "vector_test.h"
#include "deque_test.h"
#include "stack_test.h"
#include "queue_test.h"
#include "list_test.h"
#include "map_test.h"
#include "set_test.h"
#include "unordered_map_test.h"
#include "unordered_set_test.h"
#include "algorithm_test.h"
using namespace mystl;


int main(){

    mystl::algorithm_test();
    mystl::vector_test();
    mystl::deque_test();
    mystl::stack_test();
    mystl::queue_test();
    mystl::list_test();
    mystl::map_test();
    mystl::multimap_test();
    mystl::set_test();
    mystl::multiset_test();
    mystl::unordered_map_test();
    mystl::unordered_multimap_test();
    mystl::unordered_set_test();
    mystl::unordered_multiset_test();
}

