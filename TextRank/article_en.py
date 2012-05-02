# -*- coding: utf-8 -*-
import nltk
import urllib
import codecs
from readability.readability import Document
from lxml import etree

def run(index):
	print "\nIndex %d" % index

	dirname = "data/%04d" % index

	file_url = dirname + "/url_en.txt"
	url = open(file_url).read()
	html = urllib.urlopen(url).read()
	document = Document(html)
	
	title = unicode(document.title()).encode('utf-8')
	file_title = dirname + "/title.txt"
	output = open(file_title, 'w')
	output.write(title)
	output.close()

	print title

	article = process_replace(nltk.clean_html(document.summary()))
	file_article = dirname + "/article_en.txt"
	output = codecs.open(file_article, 'w', 'utf-8')
	output.write(article)
	output.close()

	# parse html
	dom = etree.HTML(html)
	links = dom.xpath("//div[@class='postcats']/ul/li/a")
	tags = ", ".join([link.text for link in links])

	file_tags = dirname + "/tags_org.txt"
	output = open(file_tags, "w")
	output.write(tags)
	output.close()

def process_replace(text):
	return text.replace("&#13;", "")

#for index in range(966, 1000):
#	run(index)

#run(0)

'''
for index in range(1000):
	try:
		print "Index " + str(index)

		filename = "data/%04d/article_en.txt" % index
		input = open(filename)
		article = input.read()
		input.close()

		article = process_replace(article)
		
		output = open(filename, "w")
		output.write(article)
		output.close()
	except IOError:
		pass
'''
