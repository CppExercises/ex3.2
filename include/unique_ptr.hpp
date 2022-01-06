#include "default_deleter.hpp"
#include <utility>

// Impelement your unique_ptr here

// for Task1 the declaration should look like this:
// template <typename T> class unique_ptr;

// for Task2 the declaration shoud be modified to look like this:
// template <typename T, typename DELETER = default_deleter<T>> class unique_ptr;


template <typename T, typename DELETER = default_deleter<T>> class unique_ptr
{

public:
  using element_type = T;
  using pointer = T *;


private:
  T *ptr = nullptr;

  DELETER dlt = default_deleter<T>();


  // DELETER dlt = default_deleter<T>();
  // DELETER dlt = std::function<void(T *)>(default_deleter<T>());
  // DELETER dlt = std::function<void(T *)>([](auto A) {});

  // Anyone one of the above 3 methods can work but the below two can not.
  // Could I understand that first we have to give a initial value to dlt, if not I would meet error about 
  // deleted function lambda, I guess happend in the process of copy/move assignment? 39 line
  // why I can not do line 38? just give a empty function. Any argument is okay but just can not be empty. 29 line
  // error: no matching function for call to ‘std::function<void(Widget*)>::function(unique_ptr<Widget, std::function<void(Widget*)> >::<lambda()>)’


  // DELETER dlt = std::function<void(T *)>([]() {});
  // DELETER dlt;

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
    dlt(ptr);
  }

  // construction from raw ptr
  
  /*
  unique_ptr(T *ptr)
  {
    this->ptr = ptr;
  }
  */


  // very important
  /*
  We receive a DELETER type which could be a lambda function. When we try to call this construction, it has to initialize the DELETER dlt first,
  which is executed by DELETER default constructor. But lambda function types have no default constructor. It is deleted by definition.

  Remember, objects have to be fully constructed before the code in the constructor can be executed.
  If you need to do initialization of members before the object is fully constructed, you need to do this in the member initialization list.
  */
  unique_ptr(T *ptr, DELETER dlt): ptr(ptr),dlt(dlt)
  {
    
    /*
/home/wang/Documents/ex3.2/tests/TestD_custom_deleter.cpp:38:50:   required from here
/home/wang/Documents/ex3.2/include/unique_ptr.hpp:58:3: error: use of deleted function ‘<lambda(auto:1*)>::<lambda>()’
   {
   ^
/home/wang/Documents/ex3.2/tests/TestD_custom_deleter.cpp:28:24: note: a lambda closure type has a deleted default constructor
 auto deleter_lambda = [](auto *ptr) {
                        ^
In file included from /home/wang/Documents/ex3.2/tests/TestD_custom_deleter.cpp:5:0:
/home/wang/Documents/ex3.2/include/unique_ptr.hpp:60:15: error: use of deleted function ‘<lambda(auto:1*)>&<lambda(auto:1*)>::operator=(const<lambda(auto:1*)>&)’
     this->dlt = dlt;
     ~~~~~~~~~~^~~~~
/home/wang/Documents/ex3.2/tests/TestD_custom_deleter.cpp:28:24: note: a lambda closure type has a deleted copy assignment operator
 auto deleter_lambda = [](auto *ptr) {

    */


    // this->dlt = dlt;
    // this way can not work but the member initialization list which now is using can work.
    // Is there any solution for this way?
    // whether we give a initial value to dlt, if without initial value, the error is above, if with initial value the error is below.
    // could not convert ‘default_deleter<Widget>()’ from ‘default_deleter<Widget>’ to ‘<lambda(auto:1*)>’
    // means our dlt has different type between argument dlt(the lambda func case)?



  }
  

  // move construction
  // Actually we should release the old one after copy construction but
  // only one of (this and other) would be non-nullptr, the other one must be nullptr, so we can
  // swap them directly and don't need to care about the pointer release.
  unique_ptr(unique_ptr &&other)
  {
    swap(other);
    std::swap(dlt, other.dlt);
  }

  // move assignment

  unique_ptr &operator=(unique_ptr &&other)
  {
    swap(other);
    // std::swap(dlt, other.dlt); why line 121 can work but here not, what is the difference.
    // error: no matching function for call to ‘swap(<lambda(auto:1*)>&, <lambda(auto:1*)>&)’
    // why I can not exchange two lambda function? The problem is the std::swap does not support? Do I understand correctly?
    // If want to swap have to implement it ourself?
    // but I can still pass the TestD without exchanging the member DELETER.
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
template <typename T, typename DELETER = default_deleter<T>>
bool operator==(unique_ptr<T, DELETER> &other, std::nullptr_t)
{
  return other.get() == nullptr;
}

// unique p != nullptr
template <typename T, typename DELETER = default_deleter<T>>
bool operator!=(unique_ptr<T, DELETER> &other, std::nullptr_t)
{
  return other.get() != nullptr;
}


// nullptr == unique p
template <typename T, typename DELETER = default_deleter<T>>
bool operator==(std::nullptr_t, unique_ptr<T, DELETER> &other)
{
  return other.get() == nullptr;
}

// nullptr != unique p
template <typename T, typename DELETER = default_deleter<T>>
bool operator!=(std::nullptr_t, unique_ptr<T, DELETER> &other)
{
  return other.get() != nullptr;
}



template <typename T, typename DELETER = default_deleter<T>>
bool operator==(unique_ptr<T, DELETER> &other1, unique_ptr<T, DELETER> &other2)
{
  return other1.get() == other2.get();
}

template <typename T, typename DELETER = default_deleter<T>>
bool operator!=(unique_ptr<T, DELETER> &other1, unique_ptr<T, DELETER> &other2)
{
  return other1.get() != other2.get();
}