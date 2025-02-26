#ifndef SRC_TESTS_H
#define SRC_TESTS_H

#include <functional>

struct Test {
    const char* name;
    std::function<void()> function;
};

#define TEST_CASE(name)                                          \
    void name##_test();                                           \
    Test name##_test_instance = {#name, name##_test};              \
    void name##_test()

#define ASSERT(condition)                                         \
    if (!(condition)) {                                            \
        std::cerr << "Assertion failed: " << #condition << std::endl; \
        throw std::runtime_error("Test assertion failed.");        \
    }

void runTests();

#endif //SRC_TESTS_H
