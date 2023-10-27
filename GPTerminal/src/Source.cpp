#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <chrono>

#include "PowerTUI.h"


class ThreadsClass
{
public:
	ThreadsClass() {};
	~ThreadsClass() {};

	void setVar(bool classBool) {
		this->runLoop = classBool;
	}

	void classLoop() {
		int n = 0;
		while (runLoop) {
			n++;
			std::cout << n << " ";
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}

private:

	bool runLoop = true;

};



int main(){

	ThreadsClass myThreadsClass = ThreadsClass();

	std::thread firstThread(&ThreadsClass::classLoop, &myThreadsClass);


	std::this_thread::sleep_for(std::chrono::seconds(5));

	myThreadsClass.setVar(false);

	firstThread.join();

    return 0;
}