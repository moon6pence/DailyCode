# -*- coding: utf-8 -*-
import nltk
import urllib
import codecs
from readability.readability import Document
from lxml import etree

def run(index):
	print "\nIndex %d" % index

	dirname = "data/%04d" % index

	file_url = dirname + "/url_ko.txt"
	url = open(file_url).read()
	html = urllib.urlopen(url).read()
	document = Document(html)
	
	article = nltk.clean_html(document.summary())
	file_article = dirname + "/article_ko.txt"
	output = codecs.open(file_article, 'w', 'utf-8')
	output.write(article)
	output.close()

	print "Length: %d" % len(article)

for index in range(0, 1000):
	run(index)

#run(0)
