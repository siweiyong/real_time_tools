/**
 * @file threadsafe_timeseries.hxx
 * @author Manuel Wuthrich (manuel.wuthrich@gmail.com)
 * @author Maximilien Naveau (maximilien.naveau@gmail.com)
 * @brief This file contains the implementation of the class in 
 * threadsafe_timeseries.hpp
 * @version 0.1
 * @date 2018-11-27
 * 
 * @copyright Copyright (c) 2018
 * 
 */

#pragma once

#include "real_time_tools/threadsafe/threadsafe_timeseries.hpp"

namespace real_time_tools{

template<typename Type>
ThreadsafeTimeseries<Type>::ThreadsafeTimeseries(
   size_t max_length, Index start_timeindex)
{
    oldest_timeindex_ = start_timeindex;
    newest_timeindex_ = oldest_timeindex_ - 1;

    tagged_timeindex_ = newest_timeindex_;

    history_elements_ = std::make_shared<std::vector<Type>>(max_length);
    history_timestamps_ = std::make_shared<std::vector<Timestamp>>(max_length);

    condition_ = std::make_shared<std::condition_variable>();
    mutex_ = std::make_shared<std::mutex>();
}

template<typename Type>
void ThreadsafeTimeseries<Type>::tag(const Index& timeindex)
{
    std::unique_lock<std::mutex> lock(*mutex_);
    tagged_timeindex_ = timeindex;
}

template<typename Type>
bool ThreadsafeTimeseries<Type>::has_changed_since_tag() const
{
    std::unique_lock<std::mutex> lock(*mutex_);
    return tagged_timeindex_ != newest_timeindex_;
}

template<typename Type> typename ThreadsafeTimeseries<Type>::Index
ThreadsafeTimeseries<Type>::newest_timeindex() const
{
    std::unique_lock<std::mutex> lock(*mutex_);
    while(newest_timeindex_ < oldest_timeindex_)
    {
        condition_->wait(lock);
    }

    return newest_timeindex_;
}

template<typename Type> Type
ThreadsafeTimeseries<Type>::newest_element() const
{
    Index timeindex = newest_timeindex();
    return (*this)[timeindex];
}

template<typename Type> Type
ThreadsafeTimeseries<Type>::operator[](const Index& timeindex) const
{
    std::unique_lock<std::mutex> lock(*mutex_);

    if(timeindex < oldest_timeindex_)
    {
        throw std::invalid_argument(
            "you tried to access timeseries element which is too old.");
    }

    while(newest_timeindex_ < timeindex)
    {
        condition_->wait(lock);
    }

    Type element = (*history_elements_)[timeindex % history_elements_->size()];

    return element;
}

template<typename Type> typename ThreadsafeTimeseries<Type>::Timestamp
ThreadsafeTimeseries<Type>::timestamp_ms(const Index& timeindex) const
{
    std::unique_lock<std::mutex> lock(*mutex_);

    if(timeindex < oldest_timeindex_)
    {
        throw std::invalid_argument(
            "you tried to access timeseries element which is too old.");
    }

    while(newest_timeindex_ < timeindex)
    {
        condition_->wait(lock);
    }

    Timestamp timestamp
            = (*history_timestamps_)[timeindex % history_timestamps_->size()];

    return timestamp;
}

template<typename Type>
void ThreadsafeTimeseries<Type>::append(const Type& element)
{
    {
        std::unique_lock<std::mutex> lock(*mutex_);
        newest_timeindex_++;
        if(newest_timeindex_ - oldest_timeindex_ + 1
                > history_elements_->size())
        {
            oldest_timeindex_++;
        }
        Index history_index = newest_timeindex_ % history_elements_->size();
        (*history_elements_)[history_index] = element;
        (*history_timestamps_)[history_index] = 
        real_time_tools::Timer::get_current_time_ms();
    }
    condition_->notify_all();
}

template<typename Type>
size_t ThreadsafeTimeseries<Type>::length() const
{
    std::unique_lock<std::mutex> lock(*mutex_);
    return newest_timeindex_ - oldest_timeindex_ + 1;
}

} //namespace blmc_drivers