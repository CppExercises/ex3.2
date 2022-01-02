#include <unique_ptr.hpp>
#include "testasserts.hpp"

struct Widget {
  double m = 42;
};

int main() {

  using T = Widget;
  using UP = unique_ptr<T>;

  { // nested types
    using expected_pointer = T *;
    using expected_element = T;
    static_assert(
        std::is_same<UP::pointer, expected_pointer>::value,
        "Typedef \"pointer\" not defined in unique_ptr (or unexpected type)");
    static_assert(std::is_same<UP::element_type, expected_element>::value,
                  "Typedef \"element_type\" not defined in unique_ptr (or "
                  "unexpected type)");
  }

  { // .get() member
    static_assert(
        std::is_same<decltype(std::declval<UP>().get()), UP::pointer>::value,
        ".get() did not return expected type");
  }

  { // constructor from raw pointer
    const auto ptr1 = new T{};
    auto up1 = unique_ptr(ptr1);
    const auto ptr2 = up1.get();
    TEST_ASSERT(ptr1 == ptr2, "unique pointer not constructed from raw pointer")
  }

  { // copy constructor
    static_assert(!std::is_copy_constructible<UP>::value,
                  "copy construction is NOT disabled");
  }

  { // copy assignment
    static_assert(!std::is_copy_assignable<UP>::value,
                  "copy assignment is NOT disabled");
  }

  { // move constructor
    auto up1 = unique_ptr(new T{});
    const auto ptr1 = up1.get();
    auto up2 = unique_ptr(std::move(up1));
    TEST_ASSERT(up1.get() == nullptr,
                "moved-from object not pointing to nullptr");
    TEST_ASSERT(ptr1 == up2.get(),
                "move-destination not pointing to moved-from resource");
  }

  { // move assignment
    auto up1 = unique_ptr(new T{});
    auto up2 = unique_ptr(new T{});
    const auto ptr1 = up1.get();
    const auto ptr2 = up2.get();

    up2 = std::move(up1);

    TEST_ASSERT(up2.get() != nullptr, "move-assigned object points to nullptr");
    TEST_ASSERT(up2.get() == ptr1,
                "move-assigned object does not point to moved-from object");
    TEST_ASSERT(up1.get() == nullptr || up1.get() == ptr2,
                "moved-from object is neither nullptr nor moved-to ptr");
  }

  return 0;
}