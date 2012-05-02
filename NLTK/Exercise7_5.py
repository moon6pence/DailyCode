import nltk

grammar = "NP: {<(DT)|(PRP)>?<JJ.*|VBG>*<NN.*>+(<VBG><NN.*>?)?}"
gerunds_chunker = nltk.RegexpParser(grammar)

# the receiving end
test1 = [('the', 'DT'), ('receiving', 'VBG'), ('end', 'NN')]

# assistant managing editor
test2 = [('assistant', 'NN'), ('managing', 'VBG'), ('editor', 'NN')]

# khkim working homework
test3 = [('khkim', 'NN'), ('working', 'VBG'), ('homework', 'NN')]

# the wonderful shining gem
test4 = [('the', 'DT'), ('wonderful', 'JJ'), ('shining', 'VBG'), ('gem', 'NN')]

print gerunds_chunker.parse(test1)
print gerunds_chunker.parse(test2)
print gerunds_chunker.parse(test3)
print gerunds_chunker.parse(test4)
