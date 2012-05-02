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
	for (int trial = 0; trial < n_trials; trial++)
		experiment2(N);

	return 0;
}

void experiment2(int N)
{
	Grid grid(N);
	vector<int> random_sequence = generateRandomSequence(N * N);

	int largest_component_size = 1;

	// increasing p
	for (int p = 0; p < random_sequence.size(); p++)
	{
		// pick next random sequence
		int index = random_sequence[p];
		grid.mark(index);

		int new_component_size = grid.size(index);
		if (new_component_size > largest_component_size)
			largest_component_size = new_component_size;

		printf("%d %d\n", p, largest_component_size);
	}
}
