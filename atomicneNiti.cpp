#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> brojac(0);

void povecaj_brojac(){
	for(int i=0; i<100000; i++){
		brojac++;
	}
}

int main(){
	std::thread nit1(povecaj_brojac);
	
	std::thread nit2(povecaj_brojac);

	nit1.join();
	nit2.join();
	
	std::cout << "Vrijednost brojaca: " << brojac << std::endl;
	return 0;
}