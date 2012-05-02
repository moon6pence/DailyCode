# reference: https://gist.github.com/1646117

import nltk
import itertools
from operator import itemgetter

from pygraph.classes.graph import graph
from pygraph.classes.digraph import digraph
from pygraph.algorithms.pagerank import pagerank
from pygraph.classes.exceptions import AdditionError

def textrank(text):
	def filter_for_tags(tagged, tags=['NN', 'JJ', 'NNP']):
		return [item for item in tagged if item[1] in tags]

	def normalize(tagged):
		return [(item[0].replace('.', ''), item[1]) for item in tagged]

	def unique_everseen(iterable, key=None):
		seen = set()
		seen_add = seen.add
		if key is None:
			for element in itertools.ifilterfalse(seen.__contains__, iterable):
				seen_add(element)
				yield element
		else:
			for element in iterable:
				k = key(element)
				if k not in seen:
					seen_add(k)
					yield element

	text = nltk.word_tokenize(text)
	tagged = nltk.pos_tag(text)
	tagged = filter_for_tags(tagged)
	tagged = normalize(tagged)

	unique_word_set = unique_everseen([x[0] for x in tagged])

	graph = digraph()
	graph.add_nodes(list(unique_word_set))

	window_start = 0
	window_end = 2

	while 1:
		window_words = tagged[window_start:window_end]
		if len(window_words) == 2:
			# print window_words
			try:
				graph.add_edge((window_words[0][0], window_words[1][0]))
			except AdditionError, e:
				# print 'already added %s, %s' % ((window_words[0][0], window_words[1][0]))
				pass
		else:
			break

		window_start += 1
		window_end += 1

	calculated_page_rank = pagerank(graph)
	di = sorted(calculated_page_rank.iteritems(), key=itemgetter(1), reverse=1)

	log = "\n".join([str(item[0]) + " : " + str(item[1]) for item in di])
	tags = ", ".join([str(item[0]) for item in di][:5])

	return log, tags

def run(index):
	print "\nIndex " + str(index)

	dirname = "data/%04d" % index

	# read english article
	try:
		text = open(dirname + "/article_en.txt").read()
	except IOError:
		print "No article file"
		return

	# run textrank
	log, tags = textrank(text)
	print tags

	# write log
	output = open(dirname + "/log.txt", 'w')
	output.write(log)
	output.close()

	# write tags
	output = open(dirname + "/tags.txt", 'w')
	output.write(tags)
	output.close()

if __name__ == '__main__':
	for index in range(100, 1000):
		run(index)

	#run(0)
