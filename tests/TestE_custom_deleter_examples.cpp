#include "testasserts.hpp"
#include <unique_ptr.hpp>

// global count
int count_closer_calls = 0;
int delete_array_calls = 0;

void write(FILE *handle) {
  fprintf(handle, "%s", "{\n  \"TestF\": \"success\"\n}\n");
}

auto open(const char *name, const char *mode) {
  FILE *ptr = std::fopen(name, mode);
  return unique_ptr(ptr, [](FILE *handle) {
    std::fclose(handle);
    ++count_closer_calls;
  });
}

int main() {
  { // custom deleter for a raw array
    auto up = unique_ptr(new double[10], [](double *ptr) {
      ++delete_array_calls;
      delete[] ptr;
    });
  }
  TEST_ASSERT(delete_array_calls == 1,
              "delete[] not called after unique_ptr went out of scope");

  { // custom "closer" as deleter
    auto filename = "data.json";
    auto mode = "w+";
    auto filePtr = open(filename, mode);
    TEST_ASSERT(filePtr != nullptr, "filePtr is nullptr after opening")
    write(filePtr.get());
  }
  TEST_ASSERT(count_closer_calls == 1,
              "closer not called after unique_ptr went out of scope");

  return 0;
}