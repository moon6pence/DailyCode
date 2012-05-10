import sys

if len(sys.argv) > 1:
	filename = sys.argv[1]
else:
	filename = 'data/exp1_10.txt'

input = open(filename)

N = int(input.readline())
print N

data = [0] * (N**2)
n_trials = 0

for line in input:
	trial, p, size = [int(token) for token in line.split()]
	print trial, p, size

	for index in range(p, N**2):
		data[index] += 1

	n_trials += 1

output = open("data/plot_%d.txt" % N, 'w')
for index, value in enumerate(data):
	p = float(index) / float(N**2)
	probability = float(value) / float(n_trials)
	output.write("%f %f\n" % (p, probability))
output.close()
