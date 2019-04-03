/**
 * @file out_string_stream.cpp
 * @author Maximilien Naveau (maximilien.naveau@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include "real_time_tools/data_collection/out_string_stream.hpp"
#include <string.h>

namespace real_time_tools{

OutStringStream::
    OutStringStream()
    : std::ostringstream(), buffer_(0), index_(0), buffer_size_(0), full_(false)
{
}

OutStringStream::
    ~OutStringStream()
{
  delete[] buffer_;
}

void OutStringStream::
    resize(const std::streamsize &size)
{
  index_ = 0;
  buffer_size_ = size;
  full_ = false;

  delete[] buffer_;
  buffer_ = new char[static_cast<size_t>(size)];
}

bool OutStringStream::
    add_data(const char *data, const std::streamoff &size)
{
  std::streamsize towrite = static_cast<std::streamsize>(size);
  if (index_ + towrite > buffer_size_)
  {
    full_ = true;
    return false;
  }
  memcpy(buffer_ + index_, data, static_cast<size_t>(towrite));
  index_ += towrite;
  return true;
}

void OutStringStream::
    dump(std::ostream &os)
{
  os.write(buffer_, index_);
}

void OutStringStream::
    empty()
{
  index_ = 0;
  full_ = false;
}

} // namespace real_time_tools