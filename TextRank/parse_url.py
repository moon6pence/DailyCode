import os
import urllib
from lxml import etree

input = open('urllist.txt')
urllist = [url for url in input]

index = 56

while 1:
	url_ko = urllist[index]
	print "\n# Index %d" % index
	print "url : " + url_ko

	# make directories
	dir_name = "data/%04d" % index
	if not os.path.exists(dir_name):
		os.mkdir(dir_name)

	# download korean html
	html = urllib.urlopen(url_ko).read()
	dom = etree.HTML(html)
	links = dom.xpath("//div[@class='articlePage']/p/a")

	# parse english article url
	url_en = ""
	for link in links:
		if link.attrib.has_key('href'):
			url_en = link.attrib['href']

	# write to file
	filename = dir_name + "/url_en.txt"
	output = open(filename, 'w')
	output.write(url_en)
	output.close()

	filename = dir_name + "/url_ko.txt"
	output = open(filename, 'w')
	output.write(url_ko)
	output.close()

	print "Saved: " + url_en

	index = index + 1
