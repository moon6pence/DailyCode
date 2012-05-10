import sys

list_N = [10, 20, 40, 80, 160, 320, 640, 1280]
plot = {}

for N in list_N:
	filename = "data/exp1_%d.txt" % N
	input = open(filename)

	N_in_file = int(input.readline())

	sum_size = 0
	n_trials = 0

	for line in input:
		trial, p, size = [int(token) for token in line.split()]
		# print trial, p, size

		sum_size += size
		n_trials += 1
		
	print N, float(sum_size) / float(n_trials)
