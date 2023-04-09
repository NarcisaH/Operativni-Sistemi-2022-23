#include <thread>
#include <atomic>

struct Mutex {
	std::atomic<bool> locked = false;
	
	void lock(){
		bool expected = false;
		while(!locked.compare_exchange_strong(expected, true, std::memory_order_aquire)){
			expected = false;
			std::this_thread::yield();
		}
	}
}

	void unlock(){
		locked.store(false, std::memory_order_release)
	}

};
