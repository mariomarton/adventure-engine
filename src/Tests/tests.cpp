#include <iostream>
#include <cstring>
#include <vector>
#include <chrono>
#include <thread>
#include <random>
#include "../Util/Reader.h"
#include "tests.h"

TEST_CASE(TestConfigReaderInvalidTypes) {
    std::string path = "../Tests/TestExamples/InvalidTypes";
    Game game(path);
    auto reader = Reader(path, game);

    ASSERT(!reader.readEverything());
}

TEST_CASE(TestConfigReaderInvalidCommand) {
    std::string path = "../Tests/TestExamples/InvalidCommand";
    Game game(path);
    auto reader = Reader(path, game);

    ASSERT(!reader.readEverything());
}

TEST_CASE(TestConfigReaderInvalidFileMissing) {
    std::string path = "../Tests/TestExamples/InvalidFileMissing";
    Game game(path);
    auto reader = Reader(path, game);

    ASSERT(!reader.readEverything());
}

TEST_CASE(TestConfigReaderValidInput) {
    std::string path = "../Tests/TestExamples/valid";
    Game game(path);
    auto reader = Reader(path, game);

    ASSERT(reader.readEverything());
}

TEST_CASE(TestGameRandInput) {
    std::string path = "../Tests/TestExamples/valid";
    Game game(path);
    auto reader = Reader(path, game);
    ASSERT(reader.readEverything());

    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dist(1, 100);

    auto start = std::chrono::steady_clock::now();
    const auto timeLimit = std::chrono::seconds(5);

    try {
        game.startGame();

        while (std::chrono::steady_clock::now() - start < timeLimit) {
            // Simulate random input
            int randomInput = dist(rng);
            std::cin.ignore();
            std::cin.clear();
            std::cin.str(std::to_string(randomInput));  // Set random input

            // sleep a bit between inputs
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    } catch (const std::runtime_error& e) {
        ASSERT(false);  // fail the test if an exception is thrown
    }
}

void runTests() {
    // Define an array of test instances
    Test tests[] = {
            // List your test instances here
            TestConfigReaderValidInput_test_instance,
            TestConfigReaderInvalidCommand_test_instance,
            TestConfigReaderInvalidFileMissing_test_instance,
            TestConfigReaderInvalidTypes_test_instance
            //TestGameRandInput
    };

    int numTests = sizeof(tests) / sizeof(Test);
    int numPassed = 0;

    // Run each test case
    for (int i = 0; i < numTests; i++) {
        std::cout << "Running test: " << tests[i].name << std::endl;

        try {
            tests[i].function();
            std::cout << "Test passed." << std::endl;
            numPassed++;
        } catch (const std::exception& e) {
            std::cerr << "Test failed: " << e.what() << std::endl;
        }
    }

    std::cout << "Tests completed. Passed: " << numPassed << " / " << numTests << "\n\n\n\n";
}