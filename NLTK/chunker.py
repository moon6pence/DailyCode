import nltk
from nltk.corpus import conll2000
test_sents = conll2000.chunked_sents('test.txt', chunk_types=['NP'])
train_sents = conll2000.chunked_sents('train.txt', chunk_types=['NP'])

grammar = "NP: {(<DT><NN>)|<PRP>|<NNS>|<NNP>|<NN>|(<NNP><NNP>)|(<DT><JJ><NN>)|(<JJ><NNS>)|(<DT><NNS>)|(<JJ><NN>)}"
regexp_chunker = nltk.RegexpParser(grammar)
# print regexp_chunker.evaluate(test_sents)

class UnigramChunker(nltk.ChunkParserI):
	def __init__(self, train_sents):
		train_data = [[(t, c) for w, t, c in nltk.chunk.tree2conlltags(sent)]
					  for sent in train_sents]
		self.tagger = nltk.UnigramTagger(train_data)

	def parse(self, sentence):
		pos_tags = [pos for (word, pos) in sentence]
		tagged_pos_tags = self.tagger.tag(pos_tags)
		chunktags = [chunktag for (pos, chunktag) in tagged_pos_tags]
		conlltags = [(word, pos, chunktag) for ((word, pos), chunktag) in zip(sentence, chunktags)]
		return nltk.chunk.conlltags2tree(conlltags)

# print UnigramChunker(train_sents).evaluate(test_sents)

class BigramChunker(nltk.ChunkParserI):
	def __init__(self, train_sents):
		train_data = [[(t, c) for w, t, c in nltk.chunk.tree2conlltags(sent)]
					  for sent in train_sents]
		self.tagger = nltk.BigramTagger(train_data)

	def parse(self, sentence):
		pos_tags = [pos for (word, pos) in sentence]
		tagged_pos_tags = self.tagger.tag(pos_tags)
		chunktags = [chunktag for (pos, chunktag) in tagged_pos_tags]
		conlltags = [(word, pos, chunktag) for ((word, pos), chunktag) in zip(sentence, chunktags)]
		return nltk.chunk.conlltags2tree(conlltags)

# chunkscore = BigramChunker(train_sents).evaluate(test_sents)

class NgramChunker(nltk.ChunkParserI):
	def __init__(self, train_sents):
		train_data = [[(t, c) for w, t, c in nltk.chunk.tree2conlltags(sent)]
					  for sent in train_sents]
		t1 = nltk.UnigramTagger(train_data)
		t2 = nltk.BigramTagger(train_data, backoff=t1)
		t3 = nltk.TrigramTagger(train_data, backoff=t2)
		self.tagger = t3

	def parse(self, sentence):
		pos_tags = [pos for (word, pos) in sentence]
		tagged_pos_tags = self.tagger.tag(pos_tags)
		chunktags = [chunktag for (pos, chunktag) in tagged_pos_tags]
		conlltags = [(word, pos, chunktag) for ((word, pos), chunktag) in zip(sentence, chunktags)]
		return nltk.chunk.conlltags2tree(conlltags)

# chunkscore = NgramChunker(train_sents).evaluate(test_sents)

# (pos_tag[n - 1] + pos_tag[n] + pos_tag[n + 1], chunk_tag[n])
def train_sent(sent):
	words = []
	for index in range(len(sent)):
		if index > 0: 
			prev = sent[index - 1][1] 
		else: 
			prev = "^"

		if index < len(sent) - 1: 
			next = sent[index + 1][1] 
		else:
			next = "$"

		word = prev + " " + sent[index][1] + " " + next
		tag = sent[index][2]
		words.append((word, tag))

	return words

# pos_tag[n - 1] + pos_tag[n] + pos_tag[n + 1]
def parse_sent(sent):
	words = []

	for index in range(len(sent)):
		if index > 0: 
			prev = sent[index - 1]
		else: 
			prev = "^"

		if index < len(sent) - 1: 
			next = sent[index + 1]
		else:
			next = "$"

		word = prev + " " + sent[index] + " " + next
		words.append(word)

	return words

class FrontBackChunker(nltk.ChunkParserI):
	def __init__(self, train_sents):
		train_data = [train_sent(nltk.chunk.tree2conlltags(sent))
					  for sent in train_sents]

		self.tagger = nltk.UnigramTagger(train_data)

	def parse(self, sentence):
		pos_tags = [pos for (word, pos) in sentence]
		tagged_pos_tags = self.tagger.tag(parse_sent(pos_tags))
		chunktags = [chunktag for (pos, chunktag) in tagged_pos_tags]
		conlltags = [(word, pos, chunktag) for ((word, pos), chunktag) in zip(sentence, chunktags)]
		return nltk.chunk.conlltags2tree(conlltags)

# print FrontBackChunker(train_sents).evaluate(test_sents)

class ConsecutiveNPChunkTagger(nltk.TaggerI):
	def __init__(self, train_sents):
		train_set = []
		for tagged_sent in train_sents:
			untagged_sent = nltk.tag.untag(tagged_sent)
			history = []
			for i, (word, tag) in enumerate(tagged_sent):
				featureset = npchunk_features(untagged_sent, i, history)
				train_set.append( (featureset, tag) )
				history.append(tag)

		self.classifier = nltk.MaxentClassifier.train(
				train_set, algorithm='lbfgsb', trace=0)

	def tag(self, sentence):
		history = []
		for i, word in enumerate(sentence):
			featureset = npchunk_features(sentence, i, history)
			tag = self.classifier.classify(featureset)
			history.append(tag)
		return zip(sentence, history)

class ConsecutiveNPChunker(nltk.ChunkParserI):
	def __init__(self, train_sents):
		tagged_sents = [[((w, t), c) for (w, t, c) 
						 in nltk.chunk.tree2conlltags(sent)]
						for sent in train_sents]

		self.tagger = ConsecutiveNPChunkTagger(tagged_sents)

	def parse(self, sentence):
		tagged_sents = self.tagger.tag(sentence)
		conlltags = [(w, t, c) for ((w, t), c) in tagged_sents]
		return nltk.chunk.conlltags2tree(conlltags)


def npchunk_features(sentence, i, history):
	word, pos = sentence[i]
	return {"pos" : pos}

chunker = ConsecutiveNPChunker(train_sents)
