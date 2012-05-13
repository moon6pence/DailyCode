# -*- coding: utf-8 -*-
import re
import os
import nltk

import urllib
import codecs
from readability.readability import Document
from lxml import etree

input = open('urllist.txt')
urllist = [url for url in input]

def run(index):
	print "Index %d" % index

	# make directories
	dir_name = "data/%04d" % index
	if not os.path.exists(dir_name):
		os.mkdir(dir_name)

	# korean article url
	url_ko = urllist[index]

	# write url_ko.txt
	output = open(dir_name + "/url_ko.txt", 'w')
	output.write(url_ko)
	output.close()
	
	# load html
	html = urllib.urlopen(url_ko).read()

	# apply readability
	document = Document(html)
	article = nltk.clean_html(document.summary())

	# replace some characters
	article = re.sub('&#13;', '\n', article)	

	# write article_ko.txt
	output = codecs.open(dir_name + "/article_ko.txt", 'w', 'utf-8')
	output.write(article)
	output.close()

	# parse tags in html
	dom = etree.HTML(html)
	tags = dom.xpath("//div[@class='postcats']/ul/li/a")
	text_tags = ', '.join([tag.text for tag in tags])

	# write tags_org.txt
	output = codecs.open(dir_name + "/tags_org.txt", 'w', 'utf-8')
	output.write(text_tags)
	output.close()

	# parse english article url
	links = dom.xpath("//div[@class='articlePage']/p/a")

	url_en = ""
	for link in links:
		if link.attrib.has_key('href'):
			url_en = link.attrib['href']

	# write url_en.txt
	output = open(dir_name + "/url_en.txt", 'w')
	output.write(url_en)
	output.close()

	# parse article category
	navItems = dom.xpath("//li[contains(@class, 'navItem-selected')]/a")
	if len(navItems) > 0:
		category = navItems[0].text
	else:
		category = u'None'

	print category.encode('utf-8')

	# write category.txt
	output = codecs.open(dir_name + "/category.txt", 'w', 'utf-8')
	output.write(category)
	output.close()

#run(0)

for index in range(0, 1000):
	try:
		run(index)
	except Exception:
		try:
			run(index)
		except Exception:
			print "----- Failed"
