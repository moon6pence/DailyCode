#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <map>

using namespace std;

// bfs recursion
void run();
void pull_water(vector<int>& a, int from, int to);

void read_vector(vector<int>& v, int k);
void print_vector(vector<int>& v);

int k;
vector<int> capacity;
vector<int> initial_amount;
vector<int> target_amount;

int main()
{
	while (true)
	{
		cin >> k;
		if (k == 0) break;

		// read input
		read_vector(capacity, k);
		read_vector(initial_amount, k);
		read_vector(target_amount, k);

		run();
	}

	return 0;
}

void run()
{
	// use vector of integers as node
	typedef vector<int> node_t;

	set<node_t> nodes;
	queue<node_t> queue;
	map<node_t, node_t> prev;

	// start node
	nodes.insert(initial_amount);
	queue.push(initial_amount);

	// run bfs
	while (!queue.empty())
	{
		// pop node
		node_t node(queue.front());
		queue.pop();

		// check if node is final state
		bool is_final_state = true;

		for (int i = 0; i < k; i++)
			if (target_amount[i] != 0 && target_amount[i] != node[i])
			{
				is_final_state = false;
				break;
			}

		if (is_final_state)
		{
			// print nodes in reverse order
			while (node != initial_amount)
			{
				print_vector(node);
				node = prev[node];
			}
			print_vector(node);

			// stop
			return;
		}

		// proceed bfs for k(k - 1) way of pouring water
		for (int i = 0; i < k; i++)
			for (int j = 0; j < k; j++)
			{
				// skip same container
				if (i == j) continue;

				// yeah! pull the water : from i to j
				node_t next_node(node);
				pull_water(next_node, i, j);

				// if new state is found, keep recursive with new node
				if (nodes.find(next_node) == nodes.end())
				{
					nodes.insert(next_node); 
					queue.push(next_node);

					prev[next_node] = node;
				}
			}
	}

	cout << "NO" << endl;
}

void pull_water(vector<int>& a, int from, int to)
{
	// if there's enough capacity
	if (a[from] + a[to] <= capacity[to])
	{
		a[to] = a[to] + a[from];
		a[from] = 0;
	}
	else
	{
		a[from] = a[to] + a[from] - capacity[to];
		a[to] = capacity[to];
	}
}

void read_vector(vector<int>& v, int k)
{
	v.clear();

	for (int i = 0; i < k; i++)
	{
		int n;
		cin >> n;
		v.push_back(n);
	}
}

void print_vector(vector<int>& v)
{
	for (vector<int>::iterator i = v.begin(); i != v.end(); ++i)
		cout << *i << " ";

	cout << endl;
}
