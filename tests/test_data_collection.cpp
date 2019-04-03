/**
 * @file test_data_collection.cpp
 * @author Maximilien Naveau (maximilien.naveau@gmail.com)
 * @brief This is a list of unittests responsible for the data collection behavior.
 * @version 0.1
 * @date 2019-04-03
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <gtest/gtest.h>
#include <fstream>
#include "real_time_tools/data_collection/out_string_stream.hpp"
#include "real_time_tools/data_collection/data_stream.hpp"

// We use this in the unnittest for code simplicity
using namespace real_time_tools;

/**
 * @brief The DISABLED_TestOutStringStream class is used to disable test.
 */
class DISABLED_TestOutStringStream : public ::testing::Test {};

/**
 * @brief The TestOutStringStream class: test suit template for setting up
 * the unit tests for the Device.
 */
class TestOutStringStream : public ::testing::Test {
public:
  /**
   * @brief Construct a new TestOutStringStream object
   */
  TestOutStringStream(): ::testing::Test(){}
protected:
  /**
   * @brief SetUp, is executed before the unit tests
   */
  void SetUp() {}
  /**
   * @brief TearDown, is executed after teh unit tests
   */
  void TearDown() {}
};

/**
 * @brief The DISABLED_TestDataStream class is used to disable test.
 */
class DISABLED_TestDataStream : public ::testing::Test {};

/**
 * @brief The TestDataStream class: test suit template for setting up
 * the unit tests for the Device.
 */
class TestDataStream : public ::testing::Test {
public:
  /**
   * @brief Construct a new TestDataStream object
   */
  TestDataStream(): ::testing::Test(){}
protected:
  /**
   * @brief SetUp, is executed before the unit tests
   */
  void SetUp() {}
  /**
   * @brief TearDown, is executed after teh unit tests
   */
  void TearDown() {}
};

/**
 * @brief test_timer_constructor tests the constructor of the class
 */
TEST_F(TestOutStringStream, test_constructor)
{
  
}


