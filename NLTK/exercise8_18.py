import nltk
from time import time

grammar1 = nltk.parse_cfg(""" 
S -> NP VP
VP -> V NP | V NP PP
PP -> P NP
V -> "saw" | "ate" | "walked"
NP -> "John" | "Mary" | "Bob" | Det N | Det N PP 
Det -> "a" | "an" | "the" | "my"
N -> "man" | "dog" | "cat" | "telescope" | "park" 
P -> "in" | "on" | "by" | "with"
""")

sent_list = [
	'Mary saw a dog'.split(), 
	'John walked the park with the dog'.split(),
	'Bob saw Mary with a telescope'.split()]

parser_list = [
	nltk.RecursiveDescentParser(grammar1), 
	nltk.ShiftReduceParser(grammar1), 
	nltk.ChartParser(grammar1)]

for parser in parser_list:
	for sent in sent_list:
		t0 = time()
		for i in range(0, 100):
			parser.nbest_parse(sent)
		t1 = time()
		print t1 - t0,
	print
