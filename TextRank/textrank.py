# reference: https://gist.github.com/1646117

import nltk
import itertools
from operator import itemgetter

from pygraph.classes.graph import graph
from pygraph.classes.digraph import digraph
from pygraph.algorithms.pagerank import pagerank
from pygraph.classes.exceptions import AdditionError

import urllib
from readability.readability import Document

def get_article(url):
	html = urllib.urlopen(url).read()
	document = Document(html)
	readable_article = document.summary()
	return nltk.clean_html(readable_article)

def filter_for_tags(tagged, tags=['NN', 'JJ', 'NNP']):
    return [item for item in tagged if item[1] in tags]

def normalize(tagged):
    return [(item[0].replace('.', ''), item[1]) for item in tagged]

def unique_everseen(iterable, key=None):
    "List unique elements, preserving order. Remember all elements ever seen."
    # unique_everseen('AAAABBBCCDAABBB') --> A B C D
    # unique_everseen('ABBCcAD', str.lower) --> A B C D
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

url = "http://online.wsj.com/article/SB10001424052702303772904577333392094289110.html?mod=WSJASIA_hps_MIDDLESixthNews"

text = get_article(url)
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
        print window_words
        try:
            graph.add_edge((window_words[0][0], window_words[1][0]))
        except AdditionError, e:
            print 'already added %s, %s' % ((window_words[0][0], window_words[1][0]))
    else:
        break

    window_start += 1
    window_end += 1

calculated_page_rank = pagerank(graph)
di = sorted(calculated_page_rank.iteritems(), key=itemgetter(1))
for k, g in itertools.groupby(di, key=itemgetter(1)):
    print k, map(itemgetter(0), g)
