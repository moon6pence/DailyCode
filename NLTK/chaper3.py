import nltk, re

def page99():
	wordlist = [w for w in nltk.corpus.words.words('en') if w.islower()]

	# Print words that can be typed with num pad 6, mno
	print [w for w in wordlist if re.search('^[mno]+$', w)]

def page106():
	from nltk.corpus import brown
	hobbies_learned = nltk.Text(brown.words(categories=['hobbies', 'learned']))
	# hobbies_learned.findall(r"<\w*> <and> <other> <\w*s>")
	hobbies_learned.findall(r"<as> <\w*> <as> <\w*>")

def exercise21():
	import urllib
	words = nltk.corpus.words.words('en')

	def unknown(url):
		html = urllib.urlopen(url).read()
		raw = nltk.clean_html(html)
		tokens = set(nltk.word_tokenize(raw))
		return [word.lower() for word in tokens if word.isalpha() and word.lower() not in words]

	print unknown("http://ilens.kaist.ac.kr/index.php/members?category=prof")

def exercise25():
	def pigLatin(word):
		regex = r"^qu|^y|^[^aeiouy]+"
		return ''.join(re.split(regex, word) + re.findall(regex, word)) + "ay"

	def pigLatinText(text):
		return ' '.join([pigLatin(word) for word in text.split()])

	print pigLatin("string")
	print pigLatin("idle")
	print pigLatin("quite")
	print pigLatin("yellow")
	print pigLatin("style")

def exercise37():
	def clean_html(html):
		return re.sub("<[^<>]+>", "", html)

	import urllib
	url = "http://ilens.kaist.ac.kr/index.php/members?category=prof"
	html = urllib.urlopen(url).read()
	print clean_html(html)

def exercise38():
	print re.sub("-\n", "", "long-\nterm")

def exercise41():
	words = ['attribution', 'confabulation', 'elocution', 'sequoia', 'tenacious', 'unidirectional']

	print sorted([''.join(re.findall("[aeiou]", word)) for word in words])

exercise41()
