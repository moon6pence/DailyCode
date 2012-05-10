#include "Grid.cpp"
#include "RandomSequence.cpp"

int n_trials = 10;
int N = 100;

void experiment2(int N);

int main(int argc, char *argv[])
{
	// randomize seed
	srand(time(0));

	if (argc > 1)
		N = atoi(argv[1]);

	if (argc > 2)
		n_trials = atoi(argv[2]);

	printf("%d\n", N);

	// experiment 2
	vector<double> average;
	for (int p = 0; p < N * N; p++)
		average.push_back(0.0);

	for (int count = 0; count < n_trials; count++)
	{
		Grid grid(N);
		vector<int> random_sequence = generateRandomSequence(N * N);

		int largest_component_size = 1;

		// increasing p
		for (int p = 0; p < N * N; p++)
		{
			// pick next random sequence
			int index = random_sequence[p];
			grid.mark(index);

			int new_component_size = grid.size(index);
			if (new_component_size > largest_component_size)
				largest_component_size = new_component_size;

			average[p] = double(average[p] * count + largest_component_size) / (count + 1);
		}
	}

	for (int p = 0; p < N * N; p++)
		printf("%d %lf\n", p, average[p]);

	return 0;
}
