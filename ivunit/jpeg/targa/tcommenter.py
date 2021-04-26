txt = open("table.play", "r").read().split('\n')
print(txt[0])
i = 0
new = []
for line in txt: 
    line += "    // idx=" + str(i)
    new.append(line)
    i += 1

txt = '\n'.join(new)
print(txt[0:50])
print(txt)
