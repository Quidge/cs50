import pickle
from time import gmtime, strftime

time = gmtime()
format = strftime("%Y-%m-%d %H:%M:%S", gmtime())
print(time)
print(format)

data = {"whee": ["i'm a field", "So am I", 2, 24], "whee": []}

listnums = []
pickled = pickle.dumps(data)
print(pickled)
print(pickle.loads(pickled))
#print(sum(listnums))
