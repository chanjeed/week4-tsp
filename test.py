child = []
childP1 = [6,7,8]
childP2 = [9,5,4,3,2,1]
countAdd = 0
iP1 = 0
iP2 = 0
while countAdd < 9:
    if countAdd >= 5 and countAdd < 8:
        child.append(childP1[iP1])
        iP1 += 1
    else:
        child.append(childP2[iP2])
        iP2 += 1
    countAdd += 1
print(child)
