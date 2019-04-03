/*!
 * @file
 * this file contains the class NonRtDataCollector to collect data
 * in a non real time thread inside a specific library and dump it
 * in the SL format
 *
 * @authors Maximilien Naveau
*/

#ifndef NON_RT_DATA_COLLECTOR_HPP
#define NON_RT_DATA_COLLECTOR_HPP

#include <string>
#include <deque>
#include <Eigen/Eigen>

#include "real_time_tools/data_collection/data_stream.hpp"

namespace real_time_tools
{

class DataCollector
{
public:
  /**
   * @brief Construct a new DataCollector object
   */
  DataCollector() {}

  /**
   * @brief Destroy the DataCollector object
   */
  virtual ~DataCollector() {}

  /**
   * @brief Register a data stream.
   * 
   * @param data_stream 
   */
  void add_data_stream(const DataStreamInterface* data_stream);

  /**
   * @brief Remove all tracked variable, purge the current buffers and close the
   * files.
   */
  void clear();

  /**
   * @brief This function append the current data to the buffer if the data
   * collection has been started.
   */
  virtual void update_data_collection();

  /**
   * @brief The update_data_collection does not register anything.
   */
  virtual void stop_data_collection();

  /**
   * @brief The update_data_collection collect the data from the registered
   * pointers
   */
  virtual void start_data_collection();

  //! 
  /**
   * @brief virtual function to check whether data collection has been completed
   * 
   * @return true: the internal buffers are full and the data collection is
   *               stopped
   * @return false: the internal buffers are not full and the data collection
   *                is running
   */
  virtual bool is_data_collection_done();

private:

  /**
   * @brief Manage if the collection of the data is in process or not.
   */
  bool running_;

  /**
   * @brief This is the list of all the c++ variables we are currently tracking.
   */
  std::list< const DataStreamInterface* > data_stream_list;
};

} // end namespace real_time_tools

#endif // NON_RT_DATA_COLLECTOR_HPP
