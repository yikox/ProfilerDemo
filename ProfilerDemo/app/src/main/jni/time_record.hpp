#ifndef __TIME_RECORD__
#define __TIME_RECORD__

#include<sys/time.h>
#include<stdio.h>

class time_record
{
private:
    /* data */
    struct timeval _begin;
    struct timeval _end;
    long sum;
    short run_times;
public:
    time_record(){
	    this->sum = 0;
	    this->run_times = 0;
	    gettimeofday(&this->_begin,NULL);
	    gettimeofday(&this->_end,NULL);
		}
    ~time_record(){}

    void record_begin(){
			gettimeofday(&this->_begin,NULL);
		}
    void record_end(){
			gettimeofday(&this->_end,NULL);
	    this->sum += (_end.tv_sec - _begin.tv_sec) * 1000000 + (_end.tv_usec - _begin.tv_usec);
	    this->run_times++;
		}

    void record_reset(){
			this->sum = 0;
	    this->run_times = 0;
	    gettimeofday(&this->_begin,NULL);
	    gettimeofday(&this->_end,NULL);
		}

    void record_printf(const char * arg){
			if(this->sum > 1000000)
				printf("[%s]:alltime=%lfs;runtime=%d;avg=%lfs\n",arg,(double)this->sum/1000000,this->run_times,(double)this->sum/1000000/this->run_times);
			else 
				printf("[%s]:alltime=%lfms;runtime=%d;avg=%lfms\n",arg,(double)this->sum/1000,this->run_times,(double)this->sum/1000/this->run_times);
		}
	double record_getTimeMs()
	{
    	return (double)this->sum/1000/this->run_times;
	}
};

#endif