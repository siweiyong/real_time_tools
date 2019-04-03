/*!
 * @file
 * this file contains the implementation of the NonRtDataCollector class
 *
 * @authors Ludovic Righetti
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <assert.h>
#include "real_time_tools/data_collection/data_collector.hpp"

namespace real_time_tools
{

void DataCollector::addVector(const Eigen::Ref<const Eigen::VectorXd> &data,
                              const std::vector<std::string> &name,
                              const std::vector<std::string> &units)
{
  assert(data.size() <= name.size());
  assert(data.size() <= units.size());

  for (int i = 0; i < data.size(); ++i)
  {
    addVariable((double *)&(data.data()[i]), name[i], units[i]);
  }
}

void DataCollector::addVector(const Eigen::Ref<const Eigen::VectorXd> &data,
                              const std::string &name,
                              const std::vector<std::string> &extension,
                              const std::vector<std::string> &units)
{
  std::vector<std::string> full_names;
  for (int i = 0; i < (int)extension.size(); ++i)
  {
    std::stringstream ss;
    ss << name << '_' << extension[i];
    full_names.push_back(ss.str());
  }
  addVector(data, full_names, units);
}

void DataCollector::addVector(const Eigen::Ref<const Eigen::VectorXd> &data,
                              const std::string &name,
                              const std::string &unit)
{
  std::vector<std::string> full_names;
  for (int i = 0; i < (int)data.size(); ++i)
  {
    std::stringstream ss;
    ss << name << '_' << i;
    full_names.push_back(ss.str());
  }
  addVector(data, full_names, std::vector<std::string>(data.size(), unit));
}

void DataCollector::addVector3d(const Eigen::Ref<const Eigen::Vector3d> &data,
                                const std::string &name,
                                const std::string &units)
{
  std::vector<std::string> varnames;
  varnames.push_back(name + "_x");
  varnames.push_back(name + "_y");
  varnames.push_back(name + "_z");
  std::vector<std::string> un;
  un.resize(3, units);
  addVector(data, varnames, un);
}

void DataCollector::addVector3d(const Eigen::Ref<const Eigen::Vector3d> &data,
                                const std::string &name,
                                const std::string &units,
                                const std::string &extension)
{
  std::vector<std::string> varnames;
  varnames.push_back(name + "_x" + extension);
  varnames.push_back(name + "_y" + extension);
  varnames.push_back(name + "_z" + extension);
  std::vector<std::string> un;
  un.resize(3, units);
  addVector(data, varnames, un);
}

void DataCollector::addQuaternion(const Eigen::Ref<const Eigen::Vector4d> &data,
                                  const std::string &name)
{
  std::vector<std::string> varnames;
  varnames.push_back(name + "_q0");
  varnames.push_back(name + "_q1");
  varnames.push_back(name + "_q2");
  varnames.push_back(name + "_q3");
  std::vector<std::string> un;
  un.resize(4, "-");
  addVector(data, varnames, un);
}

void DataCollector::addVector6d(const Eigen::Ref<const Eigen::Matrix<double, 6, 1>> &data,
                                const std::string &name,
                                const std::string &units)
{
  std::vector<std::string> varnames;
  varnames.push_back(name + "_x");
  varnames.push_back(name + "_y");
  varnames.push_back(name + "_z");
  varnames.push_back(name + "_a");
  varnames.push_back(name + "_b");
  varnames.push_back(name + "_g");
  std::vector<std::string> un;
  un.resize(6, units);
  addVector(data, varnames, un);
}

/* void DataCollector::addMatrix4x4(const Eigen::Ref<const Eigen::Matrix<double, 4, 4> >& data,
                                  const std::string& name,
                                  const std::string& units) {
  int i = 0;
  addVariable((double *)&(data.data()[i++]), name + "_1_1", units);
  addVariable((double *)&(data.data()[i++]), name + "_2_1", units);
  addVariable((double *)&(data.data()[i++]), name + "_3_1", units);
  addVariable((double *)&(data.data()[i++]), name + "_4_1", units);

  addVariable((double *)&(data.data()[i++]), name + "_1_2", units);
  addVariable((double *)&(data.data()[i++]), name + "_2_2", units);
  addVariable((double *)&(data.data()[i++]), name + "_3_2", units);
  addVariable((double *)&(data.data()[i++]), name + "_4_2", units);

  addVariable((double *)&(data.data()[i++]), name + "_1_3", units);
  addVariable((double *)&(data.data()[i++]), name + "_2_3", units);
  addVariable((double *)&(data.data()[i++]), name + "_3_3", units);
  addVariable((double *)&(data.data()[i++]), name + "_4_3", units);

  addVariable((double *)&(data.data()[i++]), name + "_1_4", units);
  addVariable((double *)&(data.data()[i++]), name + "_2_4", units);
  addVariable((double *)&(data.data()[i++]), name + "_3_4", units);
  addVariable((double *)&(data.data()[i++]), name + "_4_4", units);
}
  */

void DataCollector::addMatrix(const Eigen::Ref<const Eigen::MatrixXd> &data,
                              const std::string &name,
                              const std::string &unit)
{
  std::vector<std::string> full_names;
  for (int i = 0; i < (int)data.cols(); ++i)
  {
    for (int j = 0; j < (int)data.rows(); ++j)
    {
      std::stringstream ss;
      ss << name << '_' << j + 1 << '_' << i + 1;
      addVariable((double *)&(data.data()[i * (int)data.rows() + j]), ss.str(), unit);
    }
  }
}

void NonRtDataCollector::addVariable(
    const double *data,
    const std::string &name,
    const std::string &units)
{
  if (running_)
    stopDataCollection();

  if (std::find(double_names_.begin(), double_names_.end(), name) == double_names_.end())
  {
    double_ptr_.push_back(data);
    double_names_.push_back(name);
    double_units_.push_back(units);
    double_data_.push_back(std::deque<double>());
  }
}

void NonRtDataCollector::addVariable(
    const float *data,
    const std::string &name,
    const std::string &units)
{
  if (running_)
    stopDataCollection();

  if (std::find(float_names_.begin(), float_names_.end(), name) == float_names_.end())
  {
    float_ptr_.push_back(data);
    float_names_.push_back(name);
    float_units_.push_back(units);
    float_data_.push_back(std::deque<float>());
  }
}

void NonRtDataCollector::addVariable(
    const int *data,
    const std::string &name,
    const std::string &units)
{
  if (running_)
    stopDataCollection();

  if (std::find(int_names_.begin(), int_names_.end(), name) == int_names_.end())
  {
    int_ptr_.push_back(data);
    int_names_.push_back(name);
    int_units_.push_back(units);
    int_data_.push_back(std::deque<int>());
  }
}

void NonRtDataCollector::addVariable(
    const bool *data,
    const std::string &name,
    const std::string &units)
{
  if (running_)
    stopDataCollection();

  if (std::find(bool_names_.begin(), bool_names_.end(), name) == bool_names_.end())
  {
    bool_ptr_.push_back(data);
    bool_names_.push_back(name);
    bool_units_.push_back(units);
    bool_data_.push_back(std::deque<bool>());
  }
}

//! virtual function to update the collection with the recently added variables
void DataCollector::updateDataCollection()
{
  if (!running_)
  {
    return;
  }
  unsigned index = 0;
  for (unsigned i = 0; i < double_ptr_.size(); ++i)
  {
    double_data_[i].push_back(*double_ptr_[i]);
  }
  index += double_ptr_.size();
  for (unsigned i = 0; i < float_ptr_.size(); ++i)
  {
    float_data_[i].push_back(double(*float_ptr_[i]));
  }
  index += float_ptr_.size();
  for (unsigned i = 0; i < int_ptr_.size(); ++i)
  {
    int_data_[i].push_back(double(*int_ptr_[i]));
  }
  index += int_ptr_.size();
  for (unsigned i = 0; i < bool_ptr_.size(); ++i)
  {
    bool_data_[i].push_back(double(*bool_ptr_[i]));
  }
}

void DataCollector::stopDataCollection()
{
  running_ = false;
}

void DataCollector::startDataCollection()
{
  running_ = true;
}

//! virtual function to check whether data collection has completed:
bool DataCollector::isDataCollectionDone()
{
  return !running_;
}

// dump the file in the SL format
void DataCollector::dump(std::string path)
{
  // std::cout << "compute the buffer size" << std:: endl;
  int buffer_size = 0;
  int nb_rows = 0;
  int nb_cols = 0;
  nb_cols = double_ptr_.size() +
            float_ptr_.size() +
            int_ptr_.size() +
            bool_ptr_.size();
  if (double_data_.size() > 0)
  {
    nb_rows = double_data_[0].size();
  }
  else if (float_data_.size() > 0)
  {
    nb_rows = float_data_[0].size();
  }
  else if (int_data_.size() > 0)
  {
    nb_rows = int_data_[0].size();
  }
  else if (bool_data_.size() > 0)
  {
    nb_rows = bool_data_[0].size();
  }
  else
  {
    std::cout << "nothing to dump, nothing done" << std::endl;
  }
  buffer_size = nb_cols * nb_rows;

  // std::cout << "create the buffer of data" << std:: endl;
  std::vector<float> buff(buffer_size);
  float *buff_ptr = &buff[0];

  // std::cout << "fill the buffer" << std:: endl;
  unsigned index = 0;
  for (unsigned i = 0; i < double_ptr_.size(); ++i)
  {
    for (unsigned j = 0; j < double_data_[i].size(); ++j)
    {
      buff_ptr[(index + i) + j * nb_cols] = float(double_data_[i][j]);
    }
  }
  index += double_ptr_.size();
  for (unsigned i = 0; i < float_ptr_.size(); ++i)
  {
    for (unsigned j = 0; j < float_data_[i].size(); ++j)
    {
      buff_ptr[(index + i) + j * nb_cols] = float(float_data_[i][j]);
    }
  }
  index += float_ptr_.size();
  for (unsigned i = 0; i < int_ptr_.size(); ++i)
  {
    for (unsigned j = 0; j < int_data_[i].size(); ++j)
    {
      buff_ptr[(index + i) + j * nb_cols] = float(int_data_[i][j]);
    }
  }
  index += int_ptr_.size();
  for (unsigned i = 0; i < bool_ptr_.size(); ++i)
  {
    for (unsigned j = 0; j < bool_data_[i].size(); ++j)
    {
      buff_ptr[(index + i) + j * nb_cols] = float(bool_data_[i][j]);
    }
  }
  index += bool_ptr_.size();
  assert((index)*nb_rows == buffer_size);

  // std::cout << "convert the buffer to little endian" << std:: endl;
  for (int i = 0; i < buffer_size; ++i)
  {
    buff_ptr[i] = reverseFloat(buff_ptr[i]);
  }

  // std::cout << "open the file" << std:: endl;
  std::ofstream data_file;
  data_file.open(path.c_str(), std::ofstream::out);

  // std::cout << "dump the header" << std:: endl;
  double frequence = 0.001;
  data_file << buffer_size << " "
            << nb_cols << " "
            << nb_rows << " "
            << frequence << " ";
  data_file << std::endl;

  for (unsigned i = 0; i < double_ptr_.size(); ++i)
  {
    data_file << double_names_[i] << " " << double_units_[i] << " ";
  }
  for (unsigned i = 0; i < float_ptr_.size(); ++i)
  {
    data_file << float_names_[i] << " " << float_units_[i] << " ";
  }
  for (unsigned i = 0; i < int_ptr_.size(); ++i)
  {
    data_file << int_names_[i] << " " << int_units_[i] << " ";
  }
  for (unsigned i = 0; i < bool_ptr_.size(); ++i)
  {
    data_file << bool_names_[i] << " " << bool_units_[i] << " ";
  }
  // I am not sure why I should add 3 characters here
  data_file << std::endl;

  // std::cout << "dump the data" << std:: endl;
  data_file.write(reinterpret_cast<char *>(buff_ptr),
                  sizeof(float) * buffer_size);

  data_file.close();
}

float DataCollector::reverseFloat(const float inFloat)
{
  float retVal;
  char *floatToConvert = (char *)&inFloat;
  char *returnFloat = (char *)&retVal;

  // swap the bytes into a temporary buffer
  returnFloat[0] = floatToConvert[3];
  returnFloat[1] = floatToConvert[2];
  returnFloat[2] = floatToConvert[1];
  returnFloat[3] = floatToConvert[0];

  return retVal;
}

} // end namespace real_time_tools
