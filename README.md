# Exercise EX3.2

This exercise is part **two** of three parts of EX3. See also [Item 000](https://cppitems.github.io/#/item/000) for an overview and deadlines. The submission deadline for EX3 (all three parts) is **Mo	10.01.2021, 4pm**. The sources related to EX3.2 are available at https://github.com/cppitems/ex3.2.

## Task1: Simplified `unique_ptr`

In this exercise it is your task to implement a *smart pointer* class in the header `include/unique_ptr.hpp`. 
Your implementation `unique_ptr` will be a simplified version of `std::unique_ptr` as it always uses `delete` to dealloacte the managed resource.

One noticeable simplification is that your class does not support a 'deleter deduction' for array types automatically (you will add a *custom deleter* later in Task2). This allows implicit type deduction on construction for your type (which is not available for `std::unique_ptr` due to the fact that the returned type from a `new T` is indistinguishable from the returned type from `new T[]`).

### Task1: Details ands tests
Your implementation should provide the following functionality (checked by the test below):

- **TestA_SMF_and_get** checks: 
  - nested types `::pointer` and `::element_type` reflecting the type of the managed resource
  - `get()` to return a raw pointer to the managed resource
  - construction from raw pointer
  - move construction from other `unique_ptr`
  - move assignment from other `unique_ptr`
  - disabled copy-construction and copy-assign
  - Note: There is no need to implement conversion constructors or conversion assignments between `unique_ptr`s of different template type `T`.
- **TestB_assign_reset_release** checks:
  - `reset(arg)` to delete the currently managed resource and start managing the resource passed via a pointer (`arg`)
  - `reset()` to delete the currently managed resource (i.e., behave the same as `reset(nullptr)`)
  - assignment from `nullptr`, which should have the same behaviour as `reset()`  
  - `release()` to stop management of the currently wrapped resource and return a pointer to the resource
- **TestC_compare** checks:
  - comparison operators `==` and `!=` between two `unique_ptr`s of the same type (should behave like the equivalent raw pointer comparison)
  - comparison operators `==` and `!=` between a `unique_ptr` and a `nullptr` (should behave like a raw pointer comparison with `nullptr`) 

## Task2: Custom deleter

After implementing all of the functionality above, you are required to add support for a *custom deleter*  which is declared as `void deleter(T*)`

Git-Note: To be able to revert to the state of your implementation w/o a custom deleter you could issue a `git commit` before starting to implement a custom deleter.

Similarly to the standard library, the type of this deleter should be introduced as a second template parameter of the `unique_ptr` class. A deleter object should then be stored as a non-reference class member. The deleter is a callable and has to be used when the managed resource is to be deleted.

If no deleter is supplied, the default deleter `default_deleter` in `include/default_deleter.hpp` should be used (via a default for the template parameter). 

This custom deleter not allows to handle custom deletion tasks, for example, you can now manage a raw array with your class if you provide a the fitting `delete[]` via a custom deleter. 

**Hint**: Try to make all previous tests (A-C) work again once you implemented the default deleter as the default template parameter before moving on to the tests for a custom deleter below.

Note: Since here in this exercise a *deleter* must be copyable, this implementation it will not support the use of raw *function pointers* passed as deleters.

### Task2: Details and tests 
Your implementation of a custom deleter should provide the following functionaltiy:

- **TestD_custom_deleter** checks: 
  - construction using custom deleter arguments (std::function and a lambda)
  - invocation of custom deleters to on deletion
  - move construction and move assignment when a custom deleter is in use
- **TestE_custom_deleter_examples**: checks 
  - a use case where a raw array (to be deleted via `delete[]`) is wrapped using `unique_ptr`
  - a use case where `FILE` handles are wrapped using `unique_ptr` and a custom deleter is used to 'close' the handle on the ´FILE´

Hint: Be aware that the deleter also has to be moved to the new object in a move constructor (see next hint for move assignment). 

Hint: Move assignment between objects with different deleters is **NOT** allowed and must therefore not be supported (it is also not tested). The reason is that lambda functions (which are predominantly used as deleters) do not support copy assignment or move assignment until C++20. They do support copy and move construction in C++17 already and therefore the move constructor still has to be implemented to correctly (see hint above).

Hint: Be aware that constructors with a single argument are allowed to act implicitly as *converting constructors* (which can be disabled via the `explicit` keyword).

  ## References
- `std::unique_ptr`: https://en.cppreference.com/w/cpp/memory/unique_ptr
