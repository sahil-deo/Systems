#include <cassert>
#include <iostream>
#include <string>
#include <vector>
#include "darr.h"  


// ---------- Simple test harness ----------
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define RUN_TEST(fn)                                                        \
    do {                                                                    \
        std::cout << "Running " #fn "... ";                                \
        ++g_tests_run;                                                     \
        try {                                                               \
            fn();                                                           \
            std::cout << "PASSED\n";                                        \
        } catch (const std::exception& e) {                                 \
            ++g_tests_failed;                                               \
            std::cout << "FAILED (exception: " << e.what() << ")\n";        \
        } catch (...) {                                                     \
            ++g_tests_failed;                                               \
            std::cout << "FAILED (unknown exception)\n";                    \
        }                                                                   \
    } while (0)

#define CHECK(cond)                                                         \
    do {                                                                    \
        if (!(cond)) {                                                      \
            throw std::runtime_error("CHECK failed: " #cond " at line "     \
                                      + std::to_string(__LINE__));           \
        }                                                                    \
    } while (0)

#define CHECK_THROWS(expr)                                                  \
    do {                                                                    \
        bool threw = false;                                                 \
        try { (expr); } catch (...) { threw = true; }                       \
        if (!threw) {                                                       \
            throw std::runtime_error("Expected throw for: " #expr           \
                                      " at line " + std::to_string(__LINE__));\
        }                                                                    \
    } while (0)

// ---------- Tests ----------

// 1. Default construction
void test_default_constructor() {
    DArr<int> arr;
    CHECK(arr.size() == 0);
}

// 2. Construction with capacity (size should still be 0 unless your
//    semantics say capacity == initial size — adjust based on your impl)
void test_capacity_constructor_size_is_zero() {
    DArr<int> arr(10);
    CHECK(arr.size() == 0); // capacity != size, unless your DArr treats it as pre-filled
}

// 2b. If capacity constructor actually pre-fills (uncomment/adjust if true)
// void test_capacity_constructor_prefilled() {
//     DArr<int> arr(5);
//     CHECK(arr.size() == 5);
// }

// 3. Construction with capacity + default value
void test_default_value_constructor() {
    int defval = 42;
    DArr<int> arr(5, defval);
    CHECK(arr.size() == 5); // assumes this variant *does* fill 'size' elements
    for (size_t i = 0; i < arr.size(); ++i) {
        CHECK(arr.get(i) == 42);
    }
}

// 4. push_back increases size and stores value
void test_push_back_basic() {
    DArr<int> arr;
    int a = 1, b = 2, c = 3;
    arr.push_back(a);
    arr.push_back(b);
    arr.push_back(c);
    CHECK(arr.size() == 3);
    CHECK(arr.get(0) == 1);
    CHECK(arr.get(1) == 2);
    CHECK(arr.get(2) == 3);
}

// 5. push_back triggers growth beyond initial capacity
void test_push_back_growth() {
    DArr<int> arr(2); // small initial capacity
    for (int i = 0; i < 100; ++i) {
        int v = i;
        arr.push_back(v);
    }
    CHECK(arr.size() == 100);
    for (int i = 0; i < 100; ++i) {
        CHECK(arr.get(i) == i);
    }
}

// 6. pop_back decreases size
void test_pop_back_basic() {
    DArr<int> arr;
    int a = 1, b = 2;
    arr.push_back(a);
    arr.push_back(b);
    CHECK(arr.size() == 2);
    arr.pop_back();
    CHECK(arr.size() == 1);
    CHECK(arr.get(0) == 1);
    arr.pop_back();
    CHECK(arr.size() == 0);
}

// 7. pop_back on empty container should not crash / should be well-defined
//    (throw, assert, or no-op — adjust CHECK_THROWS if your impl silently no-ops)
void test_pop_back_empty() {
    DArr<int> arr;
    CHECK_THROWS(arr.pop_back());
}

// 8. get() with valid indices
void test_get_valid() {
    DArr<int> arr;
    for (int i = 0; i < 10; ++i) {
        int v = i * i;
        arr.push_back(v);
    }
    for (int i = 0; i < 10; ++i) {
        CHECK(arr.get(i) == i * i);
    }
}

// 9. get() with out-of-bounds index should throw (adjust if your impl asserts/UB)
void test_get_out_of_bounds() {
    DArr<int> arr;
    int a = 5;
    arr.push_back(a);
    CHECK_THROWS(arr.get(1));
    CHECK_THROWS(arr.get(100));
}

// 10. set() updates existing element
void test_set_basic() {
    DArr<int> arr;
    int a = 1, b = 2;
    arr.push_back(a);
    arr.push_back(b);
    int newval = 99;
    arr.set(0, newval);
    CHECK(arr.get(0) == 99);
    CHECK(arr.get(1) == 2);
}

// 11. set() out-of-bounds should throw
void test_set_out_of_bounds() {
    DArr<int> arr;
    int a = 1;
    arr.push_back(a);
    int v = 10;
    CHECK_THROWS(arr.set(5, v));
}

// 12. size() reflects sequence of operations correctly
void test_size_tracking() {
    DArr<int> arr;
    CHECK(arr.size() == 0);
    int v = 1;
    arr.push_back(v);
    arr.push_back(v);
    arr.push_back(v);
    CHECK(arr.size() == 3);
    arr.pop_back();
    CHECK(arr.size() == 2);
    arr.push_back(v);
    CHECK(arr.size() == 3);
}

// 13. Works with non-trivial types (std::string) — checks copy semantics
void test_non_trivial_type() {
    DArr<std::string> arr;
    std::string s1 = "hello";
    std::string s2 = "world";
    arr.push_back(s1);
    arr.push_back(s2);
    CHECK(arr.get(0) == "hello");
    CHECK(arr.get(1) == "world");

    // mutate original after push_back — should NOT affect stored copy
    // (only valid if push_back takes by value/copies; if it stores a reference,
    // this test will need to change)
    s1 = "mutated";
    CHECK(arr.get(0) == "hello");
}

// 14. get() returns a reference that can mutate internal storage
void test_get_reference_mutation() {
    DArr<int> arr;
    int v = 10;
    arr.push_back(v);
    arr.get(0) = 20;
    CHECK(arr.get(0) == 20);
}

// 15. Large-scale stress test: push then pop everything
void test_stress_push_pop() {
    DArr<int> arr;
    const int N = 10000;
    for (int i = 0; i < N; ++i) {
        int v = i;
        arr.push_back(v);
    }
    CHECK(arr.size() == static_cast<size_t>(N));
    for (int i = N - 1; i >= 0; --i) {
        CHECK(arr.get(static_cast<size_t>(i)) == i);
        arr.pop_back();
    }
    CHECK(arr.size() == 0);
}

// 16. Copy construction / assignment (only if DArr supports it — remove if not)
void test_copy_semantics() {
    DArr<int> arr;
    int a = 1, b = 2, c = 3;
    arr.push_back(a);
    arr.push_back(b);
    arr.push_back(c);

    DArr<int> copy = arr; // copy constructor
    CHECK(copy.size() == arr.size());
    for (size_t i = 0; i < arr.size(); ++i) {
        CHECK(copy.get(i) == arr.get(i));
    }

    // mutate copy, ensure original unaffected (deep copy check)
    int newval = 999;
    copy.set(0, newval);
    CHECK(arr.get(0) == 1);
    CHECK(copy.get(0) == 999);
}

// 17. Zero-capacity construction should behave like default
void test_zero_capacity_constructor() {
    DArr<int> arr(0);
    CHECK(arr.size() == 0);
    int v = 1;
    arr.push_back(v); // should still work, growing from 0
    CHECK(arr.size() == 1);
    CHECK(arr.get(0) == 1);
}

// 18. Interleaved push/pop/set/get sequence — general correctness sanity check
void test_interleaved_operations() {
    DArr<int> arr;
    int v;

    v = 10; arr.push_back(v);
    v = 20; arr.push_back(v);
    v = 30; arr.push_back(v);
    CHECK(arr.size() == 3);

    arr.pop_back();
    CHECK(arr.size() == 2);

    v = 99;
    arr.set(0, v);
    CHECK(arr.get(0) == 99);
    CHECK(arr.get(1) == 20);

    v = 40;
    arr.push_back(v);
    CHECK(arr.size() == 3);
    CHECK(arr.get(2) == 40);
}

// ---------- test ----------
int main() {
    RUN_TEST(test_default_constructor);
    RUN_TEST(test_capacity_constructor_size_is_zero);
    RUN_TEST(test_default_value_constructor);
    RUN_TEST(test_push_back_basic);
    RUN_TEST(test_push_back_growth);
    RUN_TEST(test_pop_back_basic);
    RUN_TEST(test_pop_back_empty);
    RUN_TEST(test_get_valid);
    RUN_TEST(test_get_out_of_bounds);
    RUN_TEST(test_set_basic);
    RUN_TEST(test_set_out_of_bounds);
    RUN_TEST(test_size_tracking);
    RUN_TEST(test_non_trivial_type);
    RUN_TEST(test_get_reference_mutation);
    RUN_TEST(test_stress_push_pop);
    RUN_TEST(test_copy_semantics);
    RUN_TEST(test_zero_capacity_constructor);
    RUN_TEST(test_interleaved_operations);

    std::cout << "\n" << g_tests_run << " tests run, "
              << g_tests_failed << " failed.\n";
    return g_tests_failed == 0 ? 0 : 1;
}