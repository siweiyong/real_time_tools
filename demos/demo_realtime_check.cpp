/*
 * POSIX Real Time Example
 * using a single pthread as RT thread
 */

#include <limits.h>
#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "real_time_tools/realtime_check.hpp"
#include "real_time_tools/realtime_thread_creation.hpp"

void* thread_function(void*)
{
  double freq = 1000.0; // 1kz
  double switch_freq = 990;
  useconds_t usec_period = static_cast<useconds_t>(
                             round((1.0/freq) * pow(10.0, 6.0)));
  real_time_tools::Realtime_check rc(freq,switch_freq);
  int nb_iteration = 10000;
  int a = 0;

  printf("sleeping time is %dus", usec_period);

  for(int i=0 ; i<nb_iteration ; ++i){
    rc.tick();
    a++;
    usleep(usec_period); // microseconds, so in Ghz
  }

  printf("\n");
  real_time_tools::print_realtime_check(rc);
  printf("\n");

  return nullptr;
}

int main(int , char* []) {
  real_time_tools::RealTimeThread thread;
  thread.create_realtime_thread(thread_function);
  thread.join();
}

