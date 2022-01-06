#include <unique_ptr.hpp>

#include "testasserts.hpp"

struct Widget {
  double m = 42;
};

int main() {

  using T = Widget;

  { // reset(arg): should delete 'old' managed resource and start to manage arg
    auto up = unique_ptr(new T{});
    auto *ptr = new T{};
    up.reset(ptr);
    TEST_ASSERT(up.get() == ptr, "reset(ptr) NOT resetting resource to ptr");
    up.reset();
    TEST_ASSERT(up.get() == nullptr, "reset() NOT resetting to nullptr");
  }

  { // release(): should release (not delete) managed resource
    auto *ptr1 = new T{};
    {
      auto up = unique_ptr(ptr1);
      auto *ptr2 = up.release();
      TEST_ASSERT(up.get() == nullptr, "release() NOT resetting to nullptr");
      TEST_ASSERT(ptr1 == ptr2,
                  "release() NOT returning previously managed resource");
    }
    // need to delete original ressource because it was released from unique_ptr
    delete ptr1;
  }

  { // assign nullptr
    auto up = unique_ptr(new T{});
    up = nullptr;
    TEST_ASSERT(up.get() == nullptr, "object which was assigned using a "
                                     "nullptr does not point to nullptr");
  }

  return 0;
}