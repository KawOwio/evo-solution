#include <iostream>
#include <time.h>
#include <vector>
#include <thread>
#include <mutex>
#include <Windows.h>

std::mutex mtx;	//mutex

void dataGenerator(std::vector<std::vector<int>>& vec);
void dataProcesser(std::vector<std::vector<int>>& vec, std::vector<int>& vecOfAverages);
void dataAggregator(std::vector<int>& vecOfAverages);

int main()
{
	std::vector<std::vector<int>> vec;	//TODO: change to queue
	std::vector<int> averages;	//TODO: change to queue

	std::thread generator(dataGenerator, std::ref(vec));
	std::thread processer(dataProcesser, std::ref(vec), std::ref(averages));
	std::thread aggregator(dataAggregator, std::ref(averages));

	generator.join();
	processer.join();
	aggregator.join();

	system("pause");
	return 0;
}

void dataGenerator(std::vector<std::vector<int>>& vec)
{
	time_t start_time = clock();	// time when function has started
	time_t temp_time = start_time;	// time of each loop

	double time = 0.0;	// total time that function has run
	
	while (time <= 10.0)
	{
		temp_time = clock();
		time = (temp_time - start_time) / double(CLOCKS_PER_SEC);	// check how much time has passed

		std::vector<int> vec1 = { 1, 2, 3, 4, 5 };	//TODO: change to random size and random vars
		vec.push_back(vec1);	//TODO: change to queue; add mutex

		Sleep(200);	// wait for 0.2 secodns before adding a new vector 
		std::cout << "Time: " << time << "\n";
	}
}

void dataProcesser(std::vector<std::vector<int>>& vec, std::vector<int>& vecOfAverages)
{
	//calculates the average and sum for each vectors. 
	//passes to thee aggregator
}

void dataAggregator(std::vector<int>& vecOfAverages)
{
	//calculates average of averages
	//prints out the result 
}