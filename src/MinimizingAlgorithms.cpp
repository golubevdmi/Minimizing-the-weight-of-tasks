#include "MinimizingAlgorithms.h"


#define GREEDY_THRESHOLD 0.7
#define GREEDY_COEFF_WEIGHT    0.35
#define GREEDY_COEFF_DURATION  0.30
#define GREEDY_COEFF_DIRECTIVE 0.35



// Task counter
std::uint32_t Task::counter = 0;

// Generate primary tasks
std::vector<Task> GenerateTasks(std::uint32_t number_of_tasks, std::uint32_t max_duration, std::uint32_t max_directive_period, std::uint32_t max_weight)
{
	std::vector<Task> tasks;
	for (std::uint32_t i = 0; i < number_of_tasks; i++)
		tasks.push_back(Task(rand() % max_duration,
			5 * i + 1,
			1 + rand() % max_weight));

	return tasks;
}
std::vector<Task> GenerateTasksTest()
{
	std::vector<Task> tasks;

	tasks.push_back(Task(35, 36, 1));
	tasks.push_back(Task(14, 93, 4));
	tasks.push_back(Task(12, 120, 8));
	tasks.push_back(Task(32, 69, 12));
	tasks.push_back(Task(8, 78, 5));


	return tasks;
}

// Allows you to assess the degree of fitness of specific tasks
// Energy in SA
std::uint64_t FitnessFunction(std::vector<Task> &tasks)
{
	std::uint64_t sum = 0;
	std::uint32_t time = 0;

	for (auto &task : tasks)
	{
		task.x_start = time;
		time += task.duration;
		task.y_end = time;


		if (task.directive_period < task.y_end)
			sum += task.weight;
	}

	return sum;
}
std::uint64_t FitnessFunction(const std::vector<Task> &tasks)
{
	std::uint64_t sum = 0;
	for (auto &task : tasks)
	{
		if (task.directive_period < task.y_end)
			sum += task.weight;
	}

	return sum;
}

std::uint64_t getWeightsOfUnsolvedTasks(const std::vector<Task> &tasks)
{
	std::uint64_t sum = 0;

	for (auto &task : tasks)
		if (task.directive_period < task.y_end)				//вычисление штрафа(если нарушен директивный срок)
			sum += task.weight;

	return sum;
}
std::uint64_t getSumOfWeights(const std::vector<Task> &tasks)
{
	std::uint64_t sum = 0;

	for (auto task : tasks)
		sum += task.weight;

	return sum;
}
void showTasks(const std::vector<Task> &tasks)
{
	std::cout << "__________________________________________________\n";
	for (auto task : tasks)
		task.show();

	std::cout << ">> Sum of weights: " << getSumOfWeights(tasks)
		<< "\n>> Sum of unresolved tasks weights: " << getWeightsOfUnsolvedTasks(tasks) << std::endl;
}

//
// Brute Force
std::vector<Task> LexicographicBruteForce(const std::vector<Task> &tasks)
{
	std::vector<Task> tasks_min;
	std::vector<Task> tasks_tmp;
	tasks_tmp = tasks_min = tasks;

	while (true)
	{
		// Find the largest i-number such that ai < ai + 1
		std::int32_t i = static_cast<std::int32_t>(tasks_tmp.size()) - 2;
		for (; 
			i >= 0 && tasks_tmp[i + 1].id < tasks_tmp[i].id;
			i--);

		if (i == -1) 
			break;


		// Find such a j-number for which aj > ai.
		std::int32_t j = static_cast<std::int32_t>(tasks_tmp.size()) - 1;
		for (;
			j >= 0 && tasks_tmp[j].id < tasks_tmp[i].id;
			j--);

		std::swap(tasks_tmp[i], tasks_tmp[j]);
		std::reverse(tasks_tmp.begin() + (i + 1), tasks_tmp.end());

		if (FitnessFunction(tasks_min) > FitnessFunction(tasks_tmp))
			tasks_min = tasks_tmp;
	}

	return tasks_min;
}

// Search tasks for more weight, shorter duration
// and directive period
std::vector<Task> Greedy(const std::vector<Task> &tasks)
{
	std::vector<Task> tasks_min = tasks;

	std::sort(tasks_min.begin(), tasks_min.end(), [](const Task &object1, const Task &object2) -> bool
	{
		if (object1.duration > object1.directive_period)
			return false;
		if (object2.duration > object2.directive_period)
			return true;

		if (!object1.duration && !object2.duration)
			return object1.id < object2.id;
		if (!object1.duration)
			return true;

		if (!object1.weight && !object2.weight)
			return object1.id > object2.id;
		else if (!object1.weight)
			return false;

		return ((object1.duration < object2.duration) * GREEDY_COEFF_DURATION
			+ (object1.weight > object2.weight) * GREEDY_COEFF_WEIGHT
			+ (object1.directive_period < object2.directive_period) * GREEDY_COEFF_DIRECTIVE
			> GREEDY_THRESHOLD);
	});

	FitnessFunction(tasks_min);

	return tasks_min;
}

//
// Sorting
std::vector<Task> SortingByDuration(const std::vector<Task> &tasks)
{
	std::vector<Task> tasks_min = tasks;

	std::sort(tasks_min.begin(), tasks_min.end(), [](const Task &object1, const Task &object2) -> bool
	{
		return object1.duration < object2.duration;
	});

	FitnessFunction(tasks_min);

	return tasks_min;
}
std::vector<Task> SortingByWeight(const std::vector<Task> &tasks)
{
	std::vector<Task> tasks_min = tasks;

	std::sort(tasks_min.begin(), tasks_min.end(), [](const Task &object1, const Task &object2) -> bool
	{
		return object1.weight > object2.weight;
	});

	FitnessFunction(tasks_min);

	return tasks_min;
}

// Mathematical optimization technique which belongs to the family of local search
// It is an iterative algorithm that starts with an arbitrary solution to a problem, then attempts to
// find a better solution by making an incremental change to the solution. If the change produces a better solution,
// another incremental change is made to the new solution, and so on until no further improvements can be found.
std::vector<Task> HillClimbing(const std::vector<Task> &tasks, std::uint32_t number_of_iter)
{
	std::vector<Task> tasks_min, tasks_tmp;
	tasks_tmp = tasks_min = tasks;


	std::vector<Task> tasks_random = tasks;
	for (std::size_t i = 0; i < number_of_iter; i++)
	{
		// Get a random array of tasks
		std::random_shuffle(tasks_random.begin(), tasks_random.end());

		// Search. Based on an inspection of the surroundings, by replacing the
		// first element with all the following ones. If a new minimum is found, search by new area
		std::vector<Task> tasks_local_min = tasks_random;
		for (std::size_t j = 1; j < tasks_tmp.size(); j++)
		{
			tasks_tmp = tasks_local_min;
			std::swap(tasks_tmp[0], tasks_tmp[j]);

			if (FitnessFunction(tasks_local_min) > FitnessFunction(tasks_tmp))
			{
				tasks_local_min = tasks_tmp;
				j = 1;
			}
		}

		if (FitnessFunction(tasks_min) > FitnessFunction(tasks_local_min))
			tasks_min = tasks_local_min;
	}

	return tasks_min;
}

//
// Probabilistic technique for approximating the global optimum of a given function.
std::vector<Task> SimulatedAnnealing(const std::vector<Task> &tasks, std::double_t primary_temperature, std::double_t final_temperature)
{
	std::vector<Task> tasks_candidate, tasks_min;
	tasks_candidate = tasks_min = tasks;

	// Get a primary candidate
	std::random_shuffle(tasks_candidate.begin(), tasks_candidate.end());

	for (std::double_t cur_temperature = primary_temperature; cur_temperature > final_temperature; cur_temperature -= 0.1)
	{
		std::vector<Task> tasks_tmp = tasks_candidate;
		std::swap(tasks_tmp[rand() % tasks_tmp.size()], tasks_tmp[rand() % tasks_tmp.size()]);

		if (FitnessFunction(tasks_tmp) <= FitnessFunction(tasks_min))
		{
			tasks_candidate = tasks_min = tasks_tmp;
		}
		else
		{
			// Calculate transition probability
			std::double_t probability = exp((-1) * (FitnessFunction(tasks_tmp) - FitnessFunction(tasks_min)) / cur_temperature);
			
			// More - new candidate, less - old
			if (probability > ( static_cast<double_t>(rand() % 100) / 100.0) )
				tasks_candidate = tasks_tmp;
		}
	}

	return tasks_min;
}