s = input('string?')

T = s.split('b')
g = int(T[0])
mok = int(T[1])
M = int(T[2])

t = T[3]

res = ''
for i in range(0,2*M,2):
    I = int((((i/2)*mok)%M)*2)
    print(I)
    val = ((ord(t[I])-ord('A'))*16+(ord(t[I+1])-ord('A')) - g) % 255
    res+=(chr(val))

print(res)