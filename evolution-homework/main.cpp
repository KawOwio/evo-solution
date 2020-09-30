#include <iostream>
#include <time.h>
#include <random>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <Windows.h>

std::mutex mtx;	//mutex

void dataGenerator(std::queue<std::vector<int>>& data);
void dataProcesser(std::queue<std::vector<int>>& data, std::queue<int>& averages);
void dataAggregator(std::queue<int>& averages);

int main()
{
	std::queue<std::vector<int>> generatedData;
	std::queue<int> averages;

	// create threads
	std::thread generator(dataGenerator, std::ref(generatedData));
	std::thread processer(dataProcesser, std::ref(generatedData), std::ref(averages));
	std::thread aggregator(dataAggregator, std::ref(averages));

	// wait until all threads finish
	generator.join();
	processer.join();
	aggregator.join();

	system("pause");
	return 0;
}

void dataGenerator(std::queue<std::vector<int>>& data)
{
	time_t start_time = clock();	// time when function has started
	time_t temp_time = start_time;	// time of each loop

	double time_taken = 0.0;	// total time that function has run
	
	srand((unsigned)time(NULL));

	while (time_taken <= 10.0)
	{
		temp_time = clock();
		time_taken = (temp_time - start_time) / double(CLOCKS_PER_SEC);	// check how much time has passed

		std::vector<int> generatedVector;
		int size = rand() % 100 + 1;	// size of the vector between 1 and 100

		for (int i = 0; i < size; i++)
		{
			int num = rand() % 100 + 1;	// number between 1 and 100
			generatedVector.push_back(num);
		}

		mtx.lock();	// preventing multiple threads accessing the queue at the same time
		data.push(generatedVector);
		mtx.unlock();

		Sleep(200);	// wait for 0.2 secodns before adding a new vector 
		std::cout << "Time: " << time_taken << "\n";
	}
}

void dataProcesser(std::queue<std::vector<int>>& data, std::queue<int>& averages)
{
	//calculates the average and sum for each vectors. 
	//passes to thee aggregator
}

void dataAggregator(std::queue<int>& averages)
{
	//calculates average of averages
	//prints out the result 
}