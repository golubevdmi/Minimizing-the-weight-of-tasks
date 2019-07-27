#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>


struct Task
{
	Task(std::uint32_t duration = 0, 
		std::uint32_t directive_period = 0,
		std::uint32_t weight = 0) :

		id(counter++),
		duration(duration),
		directive_period(directive_period),
		weight(weight),
		x_start(0),
		y_end(0)
	{}

	std::uint32_t id;

	std::uint32_t duration;
	std::uint32_t directive_period;
	std::uint32_t weight;

	// Start execution
	std::uint32_t x_start;
	// Completion of execution
	std::uint32_t y_end;


	void show()
	{
		std::cout << "| " << id
			      << "\t| duration: " << duration
				  << "\t| directive period: " << directive_period
				  << "\t| weight: " << weight
				  << "\t| x_start: " << x_start
				  << "\t| y_end: " << y_end 
				  << "\t| valid: " << ( y_end <= directive_period ) << std::endl;
	}

private:
	static std::uint32_t counter;
};


std::vector<Task> LexicographicBruteForce(const std::vector<Task> &tasks);
std::vector<Task> Greedy(const std::vector<Task> &tasks);
std::vector<Task> SortingByDuration(const std::vector<Task> &tasks);
std::vector<Task> SortingByWeight(const std::vector<Task> &tasks);
std::vector<Task> HillClimbing(const std::vector<Task> &tasks, std::uint32_t number_of_iter = 10000);
std::vector<Task> SimulatedAnnealing(const std::vector<Task> &tasks, std::double_t primary_temperature = 1000.0, std::double_t final_temperature = 1.0);


std::vector<Task> GenerateTasks(std::uint32_t number_of_tasks = 0, std::uint32_t max_duration = 10, std::uint32_t max_directive_period = 100, std::uint32_t max_weight = 20);
std::vector<Task> GenerateTasksTest();

// Energy
std::uint64_t FitnessFunction(std::vector<Task> &tasks);
std::uint64_t FitnessFunction(const std::vector<Task> &tasks);

void showTasks(const std::vector<Task> &tasks);