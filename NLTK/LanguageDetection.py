import nltk

def detectLanguage(targetText):
	# source word freq dist
	fileNameList = ['English-Latin1', 'Spanish-Latin1', 'German_Deutsch-Latin1', 'French_Francais-Latin1', 'Italian-Latin1']
	wordsList = [nltk.corpus.udhr.words(fileName) for fileName in fileNameList]
	freqDistList = [nltk.FreqDist(words) for words in wordsList]

	# target word freq dist
	targetWordList = nltk.word_tokenize(targetText)
	targetFreqDist = nltk.FreqDist(targetWordList)

	# compare each source freq dist with target dist
	correlationList = [nltk.spearman_correlation(targetFreqDist, freqDist)
					   for freqDist in freqDistList]

	# return list of pair (language, factor of similarity)
	return zip(fileNameList, correlationList)
	
# Shakespeare - Hamlet
print detectLanguage(nltk.corpus.gutenberg.raw(fileids='shakespeare-hamlet.txt'))

# French
import nltk.corpus.europarl_raw
print detectLanguage(nltk.corpus.europarl_raw.french.raw(fileids='ep-00-01-17.fr'))
