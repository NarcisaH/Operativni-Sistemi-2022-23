#include <iostream>
#include <thread>

void funkcija_za_novu_nit(){
	std::cout << "Pozdrav iz nove niti!" << std::endl;
}

int main(){
	std::thread nova_nit(funkcija_za_novu_nit);
	
	return 0;
}