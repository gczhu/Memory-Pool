#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "MemoryPool.h"
#include <cstddef>
using namespace std;

template <class T>
class MyAllocator {
public:
    // member types
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef true_type propagate_on_container_move_assignment;
    typedef true_type is_always_equal;
    // convert allocator<T> to allocator<U>
    template<typename U> struct rebind { typedef MyAllocator<U> other; };

    // member functions
    MyAllocator() noexcept = default;		                                               // create a new allocator instance
    ~MyAllocator() noexcept = default;		                                               // destruct an allocator instance
    template<typename U>
    MyAllocator(MyAllocator<U> const&) noexcept {}									       // create an allocator of type U

    pointer address(reference _Val) const noexcept { return &_Val; }				       // obtain the address of an object
    const_pointer address(const_reference _Val) const noexcept { return &_Val; }	       // obtain the address of an object

    pointer allocate(size_type _Count) {                                                   // allocate uninitialized storage
        return (pointer)(MemoryPool::getInstance()->allocate(_Count * sizeof(value_type)));
    }

    void deallocate(pointer _Ptr, size_type _Count) {                                      // deallocate storage
        MemoryPool::getInstance()->deallocate((unsigned char*)_Ptr);
    }

    size_type max_size() const noexcept {											       // return the largest supported allocation size
        return numeric_limits<size_type>::max() / sizeof(value_type);
    }

    // template<typename _Objty, typename... _Types>
    // void construct(_Objty* _Ptr, _Types&&... _Args) {								       // construct an object in allocated storage
    //     ::new ((void*)_Ptr) _Objty(std::forward<_Types>(_Args)...);
    // }

    template<typename _Uty>
    void destroy(_Uty* _Ptr) {														       // destruct an object in allocated storage
        _Ptr->~_Uty();
    }
};

// non-member functions
// compare two allocator instances
template <class T1, class T2>
bool operator==(const MyAllocator<T1>& _Obj1, const MyAllocator<T2>& _Obj2) noexcept{
    return true;
}

template <class T1, class T2>
bool operator!=(const MyAllocator<T1>& _Obj1, const MyAllocator<T2>& _Obj2) noexcept{
    return false;
}

#endif