#include <vector>

using namespace std;

vector<int> generateRandomSequence(int size)
{
	vector<int> sequence(size);

	// generate random sequence
	for (int index = 0; index < sequence.size(); index++)
		sequence[index] = index;

	// five times of sequence size is enough to build random sequence
	for (int do_swap = 0; do_swap < sequence.size() * 5; do_swap++)
	{
		// pick 2 index randomly
		int index1 = rand() % sequence.size();
		int index2 = rand() % sequence.size();

		// swap 
		int temp = sequence[index1];
		sequence[index1] = sequence[index2];
		sequence[index2] = temp;
	}

	return sequence;
}

