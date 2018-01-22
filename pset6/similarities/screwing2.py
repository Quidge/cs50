a = [
    ("this", 1),
    ("that", 7),
    ("theother", 0),
    ("whatsworse", 22),
    ("elsenot", 7)
    ]

b = sorted(a, key=lambda elem: elem[1])

print(a)
print(b)