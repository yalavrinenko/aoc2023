T = [55, 82, 64, 90]
H = [246, 1441, 1012, 1111]

prod = 1
for i in [0, 1, 2, 3]:
    D = T[i] ** 2 - 4 * H[i]
    x1 = (T[i] + D ** 0.5) / 2
    x2 = (T[i] - D ** 0.5) / 2
    mtime = min(x1, x2)
    Mtime = max(x1, x2)
    print(mtime, Mtime)
    prod *= (int(Mtime) - int(mtime))

print("Result 1", prod)

T = 55826490
H = 246144110121111

D = T ** 2 - 4 * H
x1 = (T + D ** 0.5) / 2
x2 = (T - D ** 0.5) / 2
mtime = min(x1, x2)
Mtime = max(x1, x2)
print(mtime, Mtime)

print("Result 2", int(Mtime) - int(mtime))
