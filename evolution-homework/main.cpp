#include <iostream>
#include <time.h>
#include <random>
#include <vector>
#include <thread>
#include <Windows.h>

#include "SafeQueue.h"

void dataGenerator(SafeQueue<std::vector<int>>& data);
void dataProcesser(SafeQueue<std::vector<int>>& data, SafeQueue<double>& averages);
void dataAggregator(SafeQueue<double>& averages);

int main()
{
	SafeQueue<std::vector<int>> generatedData;
	SafeQueue<double> averages;

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

void dataGenerator(SafeQueue<std::vector<int>>& data)
{
	time_t start_time = clock();	// time when function has started
	time_t temp_time = start_time;	// time of each loop

	double time_taken = 0.0;	// total time that function has run

	srand((unsigned)time(NULL));	// initialise a seed for rng

	while (time_taken <= 10.0)	//  runs for 10 seconds
	{
		temp_time = clock();
		time_taken = (temp_time - start_time) / double(CLOCKS_PER_SEC);	// check how much time has passed

		std::vector<int> generatedVector;
		int size = rand() % 100 + 1;	// size of the vector between 1 and 100

		for (int i = 0; i < size; i++)
		{
			int num = rand() % 100 + 1;	// number between 1 and 100
			generatedVector.push_back(num);	// pushing the number into the vector
		}

		data.push(generatedVector);	// pushing the vector into the queue of vectors

		std::this_thread::sleep_for(std::chrono::milliseconds(200));	// wait for 0.2 secodns before adding a new vector 
	}
}

void dataProcesser(SafeQueue<std::vector<int>>& data, SafeQueue<double>& averages)
{
	std::vector<int> temp;
	double average = 0;
	int sum = 0;

	bool success = true;

	while (true)
	{
		success = data.pop(temp);	// pop the first value and assign it to temp

		if (!success)	// if there is no value to pop anymore then quit the loop
		{
			break;
		}

		// calculating the sum of the ints in the vector
		for (int i = 0; i < temp.size(); i++)
		{
			sum += temp.at(i);
		}

		// calculating the average of the ints in the vector
		average = (double)sum / temp.size();

		averages.push(average);	// push the average to the queue of averages

		// reset sum and average
		sum = 0;
		average = 0;
	}
}

void dataAggregator(SafeQueue<double>& averages)
{
	double temp = 0;
	double sum = 0;
	int count = 0;

	bool success = true;

	while (true)
	{
		success = averages.pop(temp);
		std::this_thread::sleep_for(std::chrono::milliseconds(20));	// works best with a slight delay

		if (!success)	// if there is no value to pop anymore then quit the loop
		{
			break;
		}

		count++;
		sum += temp;
	}

	double answer = sum / count;

	std::cout << "The average of averages is: " << answer << "!\n";
}