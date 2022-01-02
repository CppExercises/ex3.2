#pragma once

template<class T>
struct default_deleter {
  void operator()(T* p) { delete p; }
};
