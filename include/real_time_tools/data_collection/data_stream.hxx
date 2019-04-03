/**
 * @file data_stream.cpp
 * @author Maximilien Naveau (maximilien.naveau@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2019-04-02
 * 
 * @copyright Copyright (c) 2019
 * 
 */

 
 template <class Serializable>
Exchange_manager_consumer<Serializable>::Exchange_manager_consumer(std::string segment_id,
								   std::string object_id,
								   int max_exchange_size)
  : items_(max_exchange_size) {

  segment_id_ = segment_id;
  object_id_consumer_ = object_id+"_consumer";
  object_id_producer_ = object_id+"_producer";
  object_id_reset_ = object_id+"_reset";
  previous_producer_id_=-1;
  ready_to_consume_ = true;
  nb_consumed_ = 0;

  // init of shared memory
  double foo[2];
  foo[0]=static_cast<double>(0); 
  foo[1]=static_cast<double>(0);
  double* data = items_.get_data();
  data[0]=-1;
  data[1]=0;
  shared_memory::set(segment_id_,object_id_consumer_,foo,2);
  shared_memory::set(segment_id_,object_id_producer_,
		     data,items_.get_data_size());
  shared_memory::set(segment_id_,object_id_reset_,false);
		     
}