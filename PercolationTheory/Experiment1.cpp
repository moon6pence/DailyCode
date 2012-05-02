#include "Grid.cpp"
#include "RandomSequence.cpp"

int n_trials = 10;
int N = 100;

void experiment1(int N);

int main(int argc, char *argv[])
{
	// randomize seed
	srand(time(0));

	if (argc > 1)
		N = atoi(argv[1]);

	if (argc > 2)
		n_trials = atoi(argv[2]);

	printf("%d\n", N);

	// experiment 1
	for (int trial = 0; trial < n_trials; trial++)
	{
		printf("%d ", trial);
		experiment1(N);
	}

	return 0;
}

void experiment1(int N)
{
	Grid grid(N);
	vector<int> random_sequence = generateRandomSequence(N * N);

	// increasing p
	for (int p = 0; p < random_sequence.size(); p++)
	{
		// grid.print();

		// pick next random sequence
		int index = random_sequence[p];
		grid.mark(index);

		// check if connected with left side
		bool connected_left = false;
		for (int y = 0; y < grid._n; y++)
			if (grid.find(index) == grid.find(grid._index(0, y)))
			{
				connected_left = true;
				break;
			}

		if (!connected_left) continue;

		// check if connected with right side
		bool connected_right = false;
		for (int y = 0; y < grid._n; y++)
			if (grid.find(index) == grid.find(grid._index(grid._n - 1, y)))
			{
				connected_right = true;
				break;
			}

		if (!connected_right) continue;

		// left side and right side is connected!
		// grid.print();

		// print p and size of 'giant component' 
		printf("%d %d\n", p, grid.size(index));
		return;
	}
}
