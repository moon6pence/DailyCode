import nltk
from nltk.corpus import conll2000

noun_phases = [phase for phase 
					 in conll2000.chunked_words('train.txt') 
					 if hasattr(phase, 'node') and phase.node == 'NP']

noun_phases = conll2000.chunked_words('train.txt', chunk_types=['NP'])

def tag_sequence_string(phase):
	return " ".join([tagged_word[1] for tagged_word in phase])

tag_sequences = [tag_sequence_string(phase) for phase in noun_phases]
fdist = nltk.FreqDist(tag_sequences)

for key in fdist.keys()[:30]:
	print key, fdist[key]
