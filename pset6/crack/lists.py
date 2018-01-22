'''your_list = 'abcdefghijklmnopqrstuvwxyz'
complete_list = []
for current in xrange(10):
    a = [i for i in your_list]
    for y in xrange(current):
        a = [x+i for i in your_list for x in a]
    complete_list = complete_list+a
'''

'''
i for i in your_list
is like doing
for i in your_list:
    ~get i~
'''

your_list = 'abc'
complete_list = [] # goal is to build this by concatonating chunks
for current in range(3): # we're doing this 5 times
    letrs = [ch for ch in your_list] # turn the charset into a list
    for y in range(current): # turn the count into its own iteration
        letrs = [substr+i for i in your_list for substr in letrs]
    complete_list = complete_list+letrs

for thing in complete_list:
    print(thing)