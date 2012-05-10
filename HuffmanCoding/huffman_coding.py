from heapq import *

def huffmanCoding(frequency):
	heap = list(frequency)
	heapify(heap)
	while len(heap) > 1:
		right, left = heappop(heap), heappop(heap)
		parent = (left[0] + right[0], left, right)
		heappush(heap, parent)

	return heap[0]

sum = 0

def printTree(tree, prefix = ''):
	global sum
	if len(tree) == 2: # is leaf
		print tree[1], prefix
		sum = sum + (float(len(prefix)) * tree[0] / 100)
	else:
		printTree(tree[1], prefix + '0')
		printTree(tree[2], prefix + '1')

# frequency = [(70, 'A'), (3, 'B'), (20, 'C'), (37, 'D')]
frequency = [ (18.3, ' '), (10.2, 'e'), (7.7, 't'), (6.8, 'a'), (5.9, 'o'), (5.8, 'i'), (5.5, 'n'), (5.1, 's'), (4.9, 'h'), (4.8, 'r'), (3.5, 'd'), (3.4, 'l'), (2.6, 'c'), (2.4, 'u'), (2.1, 'm'), (1.9, 'w'), (1.8, 'f'), (1.7, 'g'), (1.6, 'y'), (1.6, 'p'), (1.3, 'b'), (0.9, 'v'), (0.6, 'k'), (0.2, 'j'), (0.2, 'x'), (0.1, 'q'), (0.1, 'z') ]

print "(a) Huffman code"
printTree(huffmanCoding(frequency))
print "(b) Average length of bit for 1 character: %f" % sum
