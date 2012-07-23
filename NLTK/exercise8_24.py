import sys
import nltk
from nltk.corpus import treebank

t = treebank.parsed_sents('wsj_0001.mrg')[0]
print t

def tree_string(node):
	str = ""
	if len(node) > 1:
		str += "[" + " ".join([tree_string(child) for child in node]) + "]" 

	str += node.node

	return str

print tree_string(t)
