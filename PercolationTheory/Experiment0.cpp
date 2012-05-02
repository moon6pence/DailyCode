#include "Grid.cpp"
#include "RandomSequence.cpp"

const int N = 10;

int main()
{
	// randomize seed
	srand(time(0));

	Grid grid(N);
	vector<int> random_sequence = generateRandomSequence(N * N);

	for (int p = 0; p < random_sequence.size(); p++)
	{
		int index = random_sequence[p];
		grid.mark(index);

		grid.print();
	}

	return 0;
}

