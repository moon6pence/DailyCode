# -*- coding: utf-8 -*-
import urllib
from readability.readability import Document
import nltk
import codecs
import re

def run(index):
	print "Index %d" % index
	dirname = "data/%04d" % index

	# url of english article
	url = open(dirname + "/url_en.txt").read()

	# download html
	html = urllib.urlopen(url).read().decode('latin-1')

	# apply readability
	document = Document(html)
	article = document.summary()
	article = nltk.clean_html(article)

	# replace latin characters
	article = re.sub(u'&#13;', u'\n', article)
	article = re.sub(u'\x92', u'`', article)
	article = re.sub(u'\x96', u'-', article)

	# article_en.txt
	output = codecs.open(dirname + "/article_en.txt", 'w', encoding='ascii', errors='ignore')
	output.write(article)
	output.close()

	# title.txt
	output = codecs.open(dirname + "/title.txt", 'w', encoding='ascii', errors='ignore')
	output.write(document.title())
	output.close()

#run(949)

for index in range(0, 1000):
	try:
		run(index)
	except Exception:
		try:
			run(index)
		except Exception:
			print "----- Failed"
