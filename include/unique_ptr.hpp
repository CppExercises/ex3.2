#include "default_deleter.hpp"
#include <utility>

// Impelement your unique_ptr here

// for Task1 the declaration should look like this:
// template <typename T> class unique_ptr;

// for Task2 the declaration shoud be modified to look like this:
// template <typename T, typename DELETER = default_deleter<T>> class unique_ptr;


template <typename T> class unique_ptr
{

public:
  using element_type = T;
  using pointer = T *;
  

private:
  T *ptr = nullptr;

  void swap(unique_ptr &other)
  {
    std::swap(ptr, other.ptr);
  }

public:

  // The action relevant with COPY must be deleted. The kernel idea of unique_ptr
  // TestA 37-45 lines actually doesn't work even if we comment these two lines copy func, there still no any error.
  unique_ptr(unique_ptr &other) = delete;
  unique_ptr &operator=(unique_ptr &other) = delete;

  // constructs empty
  unique_ptr() = default;

  // destructor
  ~unique_ptr()
  {
    delete ptr;
  }

  // construction from raw ptr
  unique_ptr(T *ptr)
  {
    this->ptr = ptr;
  }

  // move construction
  // Actually we should release the old one after copy construction but
  // only one of (this and other) would be non-nullptr, the other one must be nullptr, so we can
  // swap them directly and don't need to care about the pointer release.
  unique_ptr(unique_ptr &&other)
  {
    swap(other);
  }

  // move assignment

  unique_ptr &operator=(unique_ptr &&other)
  {
    swap(other);
    return *this;
  }

  // The real implementation is at the end
  /*
  bool operator==(unique_ptr &other)
  {
    return ptr == other.ptr;
  }

  bool operator!=(unique_ptr &other)
  {
    return ptr != other.ptr;
  }

  bool operator==(std::nullptr_t)
  {
    return ptr == nullptr;
  }

  bool operator!=(std::nullptr_t)
  {
    return ptr != nullptr;
  }
  */

  



  // get func
  T * get()
  {
    return this->ptr;
  }

  void reset(T *ptr)
  {
    unique_ptr(ptr).swap(*this);
  }
  
  void reset()
  {
    unique_ptr().swap(*this);
  }

  T* release()
  {
    // std::exchange(A, B)
    // give B to A and return A, which perfectly meet our requirements.
    return std::exchange(this->ptr, nullptr);
  }

  

  

};

// Initially I wrote these inside the class defination as a member function
// then I found there is test for nullptr == p only p == nullptr
// I started to think how to implement nullptr == p
// I tried much times the error is I can not overload the operator == with 2 arguments
// So finally I wrote them here outside of the class.

// unique p == nullptr
template <typename T>
bool operator==(unique_ptr<T> &other, std::nullptr_t)
{
  return other.get() == nullptr;
}

// unique p != nullptr
template <typename T>
bool operator!=(unique_ptr<T> &other, std::nullptr_t)
{
  return other.get() != nullptr;
}


// nullptr == unique p
template <typename T>
bool operator==(std::nullptr_t, unique_ptr<T> &other)
{
  return other.get() == nullptr;
}

// nullptr != unique p
template <typename T>
bool operator!=(std::nullptr_t, unique_ptr<T> &other)
{
  return other.get() != nullptr;
}



template <typename T>
bool operator==(unique_ptr<T> &other1, unique_ptr<T> &other2)
{
  return other1.get() == other2.get();
}
template <typename T>
bool operator!=(unique_ptr<T> &other1, unique_ptr<T> &other2)
{
  return other1.get() != other2.get();
}

