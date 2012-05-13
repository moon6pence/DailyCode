import nltk
import itertools
from operator import itemgetter

import networkx as nx
import matplotlib.pyplot as plt

import plural
from determine_category import determine_category

def pagerank(graph, damping_factor=0.85, max_iterations=100, min_delta=0.00001):
	nodes = graph.nodes()
	graph_size = len(nodes)
	if graph_size == 0:
		return {}

	# value for nodes without inbound links
	min_value = (1.0-damping_factor) / graph_size 

	# itialize the page rank dict with 1/N for all nodes
	pagerank = dict.fromkeys(nodes, 1.0 / graph_size)
	
	for i in range(max_iterations):
		# total difference compared to last iteraction
		diff = 0 

		# computes each node PageRank based on inbound links
		for node in nodes:
			rank = min_value
			for referring_page in graph.predecessors(node):
				weight = 0 # Sum of out weight
				for links in graph.neighbors(referring_page):	
					weight += graph[referring_page][links]['weight']

				rank += damping_factor * graph[node][referring_page]['weight'] * pagerank[referring_page] / weight
	
			diff += abs(pagerank[node] - rank)
			pagerank[node] = rank

		# stop if PageRank has converged
		if diff < min_delta:
			break

	return pagerank

def textrank(sentence_list):
	def preprocess(tagged_sentence):
		def filter_for_cdnn(tagged_sentence):
			result_list = []
			if len(tagged_sentence) > 0:
				result_list.append(tagged_sentence[0])
				for i in range(1,len(tagged_sentence)):
					if tagged_sentence[i-1][1] == 'CD' and tagged_sentence[i][1].startswith('NN'):
						continue
					else:
						result_list.append(tagged_sentence[i])

			return result_list
		
		def filter_for_tags(tagged_sentence):
			return [item for item in tagged_sentence if item[1].startswith('NN') or item[1].startswith('JJ')]

		def delete_point(tagged_sentence):
			return [[item[0].replace('.', ''), item[1]] for item in tagged_sentence]	

		def filter_for_words(tagged_sentence):
			words_to_delete = ["'",'"',":",";","?",",",".","!","(",")","%","$","&","*","#","@", "|", "m", "s", "Mr.", "Mrs.", "Ms.", "Dr.", "Co.", "Lt.", "Inc."]
			return [item for item in tagged_sentence if item[0] not in words_to_delete]

		def singularize(tagged_sentence):
			for t in tagged_sentence:
				if t[1] == "NNS":
					t[0] = plural.singularize(t[0])

			return tagged_sentence

		return singularize(filter_for_tags(filter_for_cdnn(delete_point(filter_for_words(tagged_sentence)))))
	
	def add_edge(t1, t2):
		node1 = (t1[0].lower(), t1[1][:2])		
		node2 = (t2[0].lower(), t2[1][:2])

		edge = (node1, node2)

		if not graph.has_edge(node1, node2):
			graph.add_edge(node1, node2, weight = 1)
		else:
			weight = graph[node1][node2]['weight']
			graph[node1][node2]['weight'] = weight + 1
		
	def forward_add_edge(tagged_bigram):
		add_edge(tagged_bigram[0], tagged_bigram[1])
			
	def backward_add_edge(tagged_bigram):
		add_edge(tagged_bigram[1], tagged_bigram[0])
			
	def double_add_edge(tagged_bigram):
		forward_add_edge(tagged_bigram)
		backward_add_edge(tagged_bigram)

	# preprocess
	sentence_list = [preprocess(sentence) for sentence in sentence_list]
	
	# build nodes
	whole_sentences = []
	for sentence in sentence_list:
		whole_sentences += sentence

	unique_word_set = list(set([(w[0].lower(), w[1][:2]) for w in whole_sentences]))

	graph = nx.DiGraph()
	graph.add_nodes_from(unique_word_set)

	# build links
	for sentence in sentence_list:
		bigram = nltk.bigrams(sentence)
	
		for t in bigram:
			double_add_edge(t)

	calculated_page_rank = pagerank(graph)
	di = sorted(calculated_page_rank.iteritems(), key=itemgetter(1), reverse=1)
	
	result = [(item[0], item[1] * len(graph.nodes())) for item in di]
	return result, graph

def select_result(result):
	def delete_stopwords(item_list):
		stopwords = nltk.corpus.stopwords.words('english')
		return [item for item in item_list if item[0][0].lower() not in stopwords]
	
	def delete_jj(item_list):
		return [item for item in item_list if not item[0][1].startswith('JJ')]

	result = delete_jj(result)
	result = delete_stopwords(result)

	return result[:5]

def do_show_graph(selected_result, graph):
	selected_nodes = [item[0] for item in selected_result]

	G = nx.DiGraph()

	for node in selected_nodes:
		G.add_node(node[0])
		G.add_nodes_from([neighbor[0] for neighbor in graph.neighbors(node)])

	for edge in graph.edges():
		if edge[0][0] in G.nodes() and edge[1][0] in G.nodes():
			G.add_edge(edge[0][0], edge[1][0])

	adjust_size = {}
	for node in selected_nodes:
		adjust_size[node[0]] = 1.5
	
	nx.draw(G, width=0.05, node_size=0, adjust_size=adjust_size)
	plt.show()

def run(index, show_graph=False):
	print "\nIndex " + str(index)

	dirname = "data/%04d" % index

	# read english article
	try:
		text = open(dirname + "/pos_tag.txt")
	except IOError:
		print "No article file"
		return

	lines = text.readlines()
	tokens_list = [line.split(' ') for line in lines]
	tags_list = [[list(token.split('//')) for token in tokens] for tokens in tokens_list]

	# run textrank
	result, graph = textrank(tags_list)
	selected_result = select_result(result)
	
	log = "\n".join([item[0][0] + ", " + item[0][1] + " : " + str(item[1]) for item in result])
	tags = [item[0][0] for item in selected_result]

	#print log
	#print tags

	tags_for_category = [item[0][0] for item in result[:10]]
	category = determine_category(tags_for_category)
	tags_string = '[' + category + '] ' + ", ".join(tags)
	print tags_string

	# show graph
	if show_graph:
		do_show_graph(selected_result, graph)

	# write log
	output = open(dirname + "/log.txt", 'w')
	output.write(log)
	output.close()

	# write tags
	output = open(dirname + "/tags.txt", 'w')
	output.write(tags_string)
	output.close()

if __name__ == '__main__':
	#run(0)
	#run(798, show_graph=True)

	for index in range(901, 1000):
		run(index)
