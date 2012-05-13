# -*- coding: utf-8 -*-
import codecs

N = 900

category_list = [None] * N
tags_list = [None] * N

# read corpus
for index in range(N):
	dir_name = "data/%04d" % index

	input = codecs.open(dir_name + "/category.txt", 'r', 'utf-8')
	category = input.read()

	if category == u'국제':
		category_list[index] = 'International'
	elif category == u'인물':
		category_list[index] = 'Person'
	elif category == u'경제&비지니스':
		category_list[index] = 'Business'
	elif category == u'정치':
		category_list[index] = 'Politics'
	elif category == u'북한':
		category_list[index] = 'NorthKorea'
	elif category == u'IT':
		category_list[index] = 'IT'
	elif category == u'라이프스타일 & 문화':
		category_list[index] = 'Lifestyle'
	elif category == u'None':
		category_list[index] = None
	else:
		print u"Exception: " + category

	tags_list[index] = open(dir_name + "/tags.txt").read().split(', ')

# tags in categories
tags_in_category = {}

for i, tags in enumerate(tags_list):
	category = category_list[i]

	if not tags_in_category.has_key(category):
		tags_in_category[category] = set()

	tags_in_category[category] = tags_in_category[category].union(set(tags))

# save to file
for category in tags_in_category.keys():
	if category == None:
		continue

	output = open("data/" + category + ".txt", 'w')

	for tag in tags_in_category[category]:
		output.write(tag + '\n')

	output.close()
