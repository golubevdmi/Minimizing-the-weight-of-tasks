#include <iostream>
#include <fstream>

#include <conio.h>

#include "MinimizingAlgorithms.h"



void ComparisonOfAlgorithms(std::int32_t number_of_tasks, std::uint32_t number_of_iter = 100);



int main(int argc, const char* argv[])
{
	std::srand(static_cast<std::int32_t>(time(NULL)));

	std::cout << ">> Sample" << std::endl;


	std::cout << "Primary" << std::endl;
	std::vector<Task> tasks = GenerateTasksTest();
	FitnessFunction(tasks);
	showTasks(tasks);

	if (tasks.size() < 11)
	{
		std::cout << "\n\n-- Lexicographic Brute Force" << std::endl;
		showTasks(LexicographicBruteForce(tasks));
	}

	std::cout << "\n\n-- Sorting by Duration" << std::endl;
	showTasks(SortingByDuration(tasks));

	std::cout << "\n\n-- Sorting by Weight" << std::endl;
	showTasks(SortingByWeight(tasks));

	std::cout << "\n\n-- Greedy" << std::endl;
	showTasks(Greedy(tasks));

	std::cout << "\n\n-- Hill Climbing" << std::endl;
	showTasks(HillClimbing(tasks));

	std::cout << "\n\n-- Simulated Annealing" << std::endl;
	showTasks(SimulatedAnnealing(tasks));


	std::cout << "Press any key to continue" << std::endl;
	_getch();

	ComparisonOfAlgorithms(10);

	std::cout << "Press any key for Exit" << std::endl;
	_getch();
	return 0;
}


void ComparisonOfAlgorithms(std::int32_t number_of_tasks, std::uint32_t number_of_iter)
{
	std::ofstream out("ComparisonOfAlgorithms.txt", std::ios::out);
	if (!out.is_open())
		return;

	std::cout << "\n>> Comparison of Algorithms"
		<< "\n>> Number of tasks: " << number_of_tasks
		<< "\n>> Number of iterations: " << number_of_iter << "\n" << std::endl;

	out << "Size" << "\t" << number_of_tasks
		<< "\n"
		<< "N_Iter" << "\t" << number_of_iter
		<< "\n";

	out << "\t" << "Primary"
		<< "\t" << "Brute Force"
		<< "\t" << "Sort Dur" 
		<< "\t" << "Sort Weight" 
		<< "\t" << "Greedy" 
		<< "\t" << "HillClimbing" 
		<< "\t" << "SimAnn"
		<< "\n";

	for (std::uint32_t i = 0; i < number_of_iter; i++)
	{
		std::cout << "iteration number: " << i << "\r";

		out << i;
		std::vector<Task> tasks = GenerateTasks(number_of_tasks);
		
		out << "\t" << FitnessFunction(tasks);

		if (tasks.size() < 12)
			out << "\t" << FitnessFunction(LexicographicBruteForce(tasks));
		else
			out << "\t" << "N / A";

		out << "\t" << FitnessFunction(SortingByDuration(tasks))
			<< "\t" << FitnessFunction(SortingByWeight(tasks))
			<< "\t" << FitnessFunction(Greedy(tasks))
			<< "\t" << FitnessFunction(HillClimbing(tasks))
			<< "\t" << FitnessFunction(SimulatedAnnealing(tasks))
			<< "\n";
	}

	out.close();

	std::cout << "\n\n>> Complete" << std::endl;
}