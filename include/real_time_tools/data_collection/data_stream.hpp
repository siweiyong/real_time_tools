/**
 * @file data_type_interface.hpp
 * @author Maximilien Naveau (maximilien.naveau@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef DATA_STREAM_HPP
#define DATA_STREAM_HPP

#include "real_time_tools/data_collection/out_string_stream.hpp"


namespace real_time_tools{

/**
 * @brief This abstract interface allows us to store any kind of data inside a
 * buffer.
 */
class DataStreamInterface{
  /**
   * @brief Construct a new DataStreamInterface object
   */
  DataStreamInterface(){};

  /**
   * @brief Destroy the DataStreamInterface object
   */
  virtual ~DataStreamInterface(){};

  /**
   * @brief Copy the registered pointer internally
   */
  template<typename TimeType>
  void acquire(TimeType time);

  /**
   * @brief dump the buffer in a file
   */
  virtual void dump(const std::string file_name)=0;

  /**
   * @brief clear the buffer.
   */
  virtual void clear()=0;
};

/**
 * @brief This is the particular daclaration of the DataStreamInterface.
 * This class allows you to store and collect data from any pointer. The
 * object must have an "operator <<".
 * 
 * @tparam T 
 */
template<typename T>
class DataStream: DataStreamInterface{

  /**
   * @brief Construct a new DataStream object
   * 
   * @param data 
   * @param name 
   * @param units 
   */
  DataStream(const T* data, std::string name, std::string units);

  /**
   * @brief Construct a new DataStream object
   * 
   * @param data is the pointer to data to be stored.
   * @param name is gonna be used to define the file name to dump the data
   * @param units is the list of units for every column stored in case the
   * pointer is a vector.
   */
  DataStream(const T* data, std::string name, std::vector<std::string> units);

  /**
   * @brief Copy the registered pointer internally
   */
  virtual void acquire()=0;

  /**
   * @brief dump the buffer in a file
   */
  virtual void dump(const std::string file_name)=0;

  /**
   * @brief clear the buffer.
   */
  virtual void clear()=0;

private:
  /**
   * @brief This is the pointer toward the object to colelct data from.
   */
  const T* data_;
  
  /**
   * @brief This is the data buffer.
   */
  OutStringStream buffer_;

  /**
   * @brief This is the units of the data buffer.
   */
  std::vector<std::string> units_

  /**
   * @brief 
   */
  std::string name_
};


} // namespace real_time_tools

#endif // DATA_STREAM_HPP