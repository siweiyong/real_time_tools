#include "real_time_tools/spinner.hpp"
#include "real_time_tools/realtime_thread_creation.hpp"
#include "real_time_tools/realtime_check.hpp"
#include "real_time_tools/timer.hpp"


void* thread_function(void*)
{

    double frequency = 1000.0;

    real_time_tools::Spinner spinner;
    spinner.set_frequency(frequency);

    real_time_tools::Timer timer;

    spinner.spin();

    for(int i=0;i<3;i++){
        timer.tic();
        spinner.spin();
        timer.tac();
    }

    timer.print_statistics();
}



int main(int , char* []) {

    real_time_tools::RealTimeThread thread;
    real_time_tools::block_memory();
    real_time_tools::create_realtime_thread(thread, thread_function);
    real_time_tools::join_thread(thread);

}
