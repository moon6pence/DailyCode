# read tags_in_categories
categories = ['Business', 'International', 'IT', 'Lifestyle', 'NorthKorea', 'Person', 'Politics']
tags_in_categories = {}

for category in categories:
	input = open("data/" + category + ".txt")
	tags_in_categories[category] = [line.strip() for line in input]

def determine_category(tags):
	# print tags
	count_category = [0] * len(categories)

	max_count = -1
	max_category = None
	for i, category in enumerate(categories):
		count = 0
		for tag in tags:
			if tag in tags_in_categories[category]:
				count += 1

		# print category, count

		if count > max_count:
			max_count = count
			max_category = category

	return max_category
