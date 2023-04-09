#include <iostream>
#include <thread>
#include <atomic>
#include <vector>

struct Semaphore {
	std::atomic<int> count;
	
	Semaphore(int initial_count=1) : count(initial_count){}
	
	void wait(){
		int expected = 1;
		while(!count.compare_exchange_strong(expected, 0)){
			exxpected = 1;
			std::this_thread_yield();
		}
	}
	
	void signal(){
		count.store(1);
	}
	
	locked.store(false,std::memory_order_release);

};
