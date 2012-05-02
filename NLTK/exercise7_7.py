import nltk
from nltk.corpus import conll2000
test_sents = conll2000.chunked_sents('test.txt', chunk_types=['NP'])[:100]

grammar = r"""
NP: {<(DT)|(PRP$)>?(<JJ.*>|<VBG>|(<$>?<CD>))*<NN.*>+((<VBG><NN.*>?)|(<CC><NN.*>))*}
NP: {<PRP>|<WP>}
"""
gerunds_chunker = nltk.RegexpParser(grammar)

score = gerunds_chunker.evaluate(test_sents)
print score
