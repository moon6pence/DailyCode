#include <iostream>
#include <vector>

using namespace std;

class Grid
{
public:
	Grid(int n);

	int _index(int x, int y) { return x + y * _n; }
	int _x(int index) { return index % _n; }
	int _y(int index) { return index / _n; }

	void do_union(int index1, int index2);
	int find(int index);
	void mark(int index);
	int size(int index);

	void print();

public:
	int _n;
	vector<int> _s;
};

Grid::Grid(int n) : 
	_n(n), 
	_s(n * n)
{
	for (size_t i = 0; i < _s.size(); i++)
		_s[i] = -1; // root of one-element tree
}

void Grid::do_union(int index1, int index2)
{
	int root1 = find(index1);
	int root2 = find(index2);

	if (root1 == root2)
		return;

	if (_s[root2] < _s[root1])
	{
		// root2 is larger -> root2 becomes new root
		_s[root2] += _s[root1];
		_s[root1] = root2;
	}
	else
	{
		// root1 is larger -> root1 becomes new root
		_s[root1] += _s[root2];
		_s[root2] = root1;
	}
}

int Grid::find(int index)
{
	if (_s[index] < 0)
		return index;
	else
	{
		int s = find(_s[index]);
		_s[index] = s; // path compression
		return s;
	}
}

void Grid::mark(int index)
{
	int x = _x(index), y = _y(index);

	// x - 1, y
	if (x - 1 >= 0) 
		do_union(_index(x, y), _index(x - 1, y));

	// x + 1, y
	if (x + 1 < _n) 
		do_union(_index(x, y), _index(x + 1, y));

	// x, y - 1
	if (y - 1 >= 0) 
		do_union(_index(x, y), _index(x, y - 1));

	// x, y + 1
	if (y + 1 < _n) 
		do_union(_index(x, y), _index(x, y + 1));
}

int Grid::size(int index)
{
	int root = find(index);
	return -_s[root];
}

void Grid::print()
{
	for (int y = 0; y < _n; y++) {
		for (int x = 0; x < _n; x++)
			if (size(_index(x, y)) > 1)
				printf("[%3d]", find(_index(x, y)));
			else
				printf("[   ]");

		printf("\n");
	}

	printf("\n");
}

