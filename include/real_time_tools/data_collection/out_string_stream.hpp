/**
 * @file out_string_stream.hpp
 * @author Maximilien Naveau (maximilien.naveau@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef OUT_STRING_STREAM_HPP
#define OUT_STRING_STREAM_HPP

#include <iomanip>
#include <sstream>

namespace real_time_tools
{
/**
 * @brief Stream of data that store data without memory allocation.
 */
class OutStringStream : public std::ostringstream
{
public:
  /**
   * @brief Construct a new OutStringStream object
   */
  OutStringStream  ();
  /**
   * @brief Destroy the OutStringStream object
   */
  ~OutStringStream  ();

  /**
   * @brief Resize the inner character buffer
   * 
   * @param size is the new buffer size
   */
  void resize (const std::streamsize& size);

  /**
   * @brief This append a new data to the inner buffer.
   * 
   * @param data is the data formatted as char*.
   * @param size is the number of character of the data.
   * @return true if everything is alright.
   * @return false if the buffer is full.
   */
  bool add_data (const char * data, const std::streamoff& size);

  /**
   * @brief This method allows you to dump the current buffer in a file.
   * 
   * @param os is the out stream corresponding to the file.
   */
  void dump (std::ostream& os);

  /**
   * @brief clear the inner buffer.
   */
  void empty ();

  /**
   * @brief Get the buffer_copy object
   * 
   * @return std::string 
   */
  std::string get_buffer_copy()
  {
    return std::string(buffer_);
  }

private:
  /**
   * @brief Inner buffer of data.
   */
  char* buffer_;

  /**
   * @brief Current index in the buffer. Also correspond the current quantity of
   * time interval stored.
   */
  std::streamsize index_;

  /**
   * @brief This is the buffer maximum size.
   */
  std::streamsize buffer_size_;

  /**
   * @brief Is the current buffer full?
   */
  bool full_;

  /**
   * @brief This is the given name in order to create the file.
   */
  std::string givenname_;
};

} // namespace real_time_tools

#endif // OUT_STRING_STREAM_HPP