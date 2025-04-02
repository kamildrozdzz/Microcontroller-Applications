#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "word_utilities.h"

void TEST_CASE_1(void) {
    // Test for a standard string with multiple words
    CU_ASSERT(7 == GetLongestWordLen("What is the longest word?"));
}

void TEST_CASE_2(void) {
    // Test for an empty string
    CU_ASSERT(0 == GetLongestWordLen(""));
}

void TEST_CASE_3(void) {
    // Test for a string containing a single word
    CU_ASSERT(5 == GetLongestWordLen("Hello"));
}

void TEST_CASE_4(void) {
    // Test for a string with multiple short words
    CU_ASSERT(4 == GetLongestWordLen("This is a test"));
}

void TEST_CASE_5(void) {
    // Test for a string containing only spaces
    CU_ASSERT(0 == GetLongestWordLen("     "));
}

void TEST_CASE_6(void) {
    // Test for a string with mixed-case words and punctuation
    CU_ASSERT(8 == GetLongestWordLen("Mixed-case WORDS and punctuation!"));
}

void TEST_CASE_7(void) {
    // Test for a string with numbers as part of words
    CU_ASSERT(6 == GetLongestWordLen("abc123 def45"));
}

void TEST_CASE_8(void) {
    // Test for a string containing special characters
    CU_ASSERT(7 == GetLongestWordLen("Special#chars_in_word!"));
}

void TEST_CASE_9(void) {
    // Test for a string with a very long word
    CU_ASSERT(20 == GetLongestWordLen("Thisisaverylongword followed by short"));
}

void TEST_CASE_10(void) {
    // Test for a string in a different language locale
    CU_ASSERT(9 == GetLongestWordLen("Czy to najdłuższe słowo?"));
}

void unit_testing_example(void) {
    CU_pSuite myTestSuite;
    // Initialize CUnit
    CU_initialize_registry();

    // Create a new test suite
    myTestSuite = CU_add_suite("my test suite", NULL, NULL);

    // Add tests to the test suite
    CU_add_test(myTestSuite, "Test case 1", TEST_CASE_1);
    CU_add_test(myTestSuite, "Test case 2", TEST_CASE_2);
    CU_add_test(myTestSuite, "Test case 3", TEST_CASE_3);
    CU_add_test(myTestSuite, "Test case 4", TEST_CASE_4);
    CU_add_test(myTestSuite, "Test case 5", TEST_CASE_5);
    CU_add_test(myTestSuite, "Test case 6", TEST_CASE_6);
    CU_add_test(myTestSuite, "Test case 7", TEST_CASE_7);
    CU_add_test(myTestSuite, "Test case 8", TEST_CASE_8);
    CU_add_test(myTestSuite, "Test case 9", TEST_CASE_9);
    CU_add_test(myTestSuite, "Test case 10", TEST_CASE_10);

    // Set verbose mode
    CU_basic_set_mode(CU_BRM_VERBOSE);
    // Run all tests
    CU_basic_run_tests();
    // Print results
    CU_basic_show_failures(CU_get_failure_list());
    // Deinitialize CUnit
    CU_cleanup_registry();
}
