import urllib
from lxml import etree

def getPostLinks(url):
	html = urllib.urlopen(url).read()
	tree = etree.HTML(html)

	postLinks = tree.xpath("//h2[@class='postTitle']/a")
	return [postLink.attrib['href'] for postLink in postLinks]

for i in range(100):
	url = 'http://realtime.wsj.com/korea/page/%d/' % i
	print '\n'.join(getPostLinks(url))
