// This tests whether the custom deleter functionality is implemented as
// expected

#include <functional>
#include <unique_ptr.hpp>

#include "testasserts.hpp"

struct Widget {
  double m = 42;
};

using T = Widget;
using UP = unique_ptr<T>;

// global counters
int count_deleter_function_calls = 0;
int count_deleter_lambda_calls = 0;

void deleter_function(T *ptr) {
  delete ptr;
  ++count_deleter_function_calls;
}

// need to wrap in a callable object, so it can be default constructed
auto deleter_callable = std::function<void(T *)>(deleter_function);

auto deleter_lambda = [](auto *ptr) {
  delete ptr;
  ++count_deleter_lambda_calls;
};

int main() {

  // custom deleter construction using deleter function and deleter lambda
  {
    auto up1 = unique_ptr(new T{}, deleter_callable);
    auto up2 = unique_ptr(new T{}, deleter_lambda);
  }
  TEST_ASSERT(count_deleter_function_calls == 1,
              "expected one call to deleter_function");
  TEST_ASSERT(count_deleter_lambda_calls == 1,
              "expected one call to deleter_lambda");

  { // deleter function + move construction
    auto up1 = unique_ptr(new T{}, deleter_callable);
    auto up2 = std::move(up1);
  }
  TEST_ASSERT(count_deleter_function_calls == 2,
              "expected two calls to deleter_function");
  TEST_ASSERT(count_deleter_lambda_calls == 1,
              "expected one call to deleter_lambda");

  // deleter lambda + move assignment
  {
    auto up1 = unique_ptr(new T{}, deleter_lambda);
    auto up2 = unique_ptr(new T{}, deleter_lambda);
    up1 = std::move(up2);
  }
  TEST_ASSERT(count_deleter_function_calls == 2,
              "expected no additional calls to deleter_function");
  TEST_ASSERT(count_deleter_lambda_calls == 3,
              "expected 3 calls to deleter_lambda");

  return 0;
}