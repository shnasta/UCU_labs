import random
import sys

if len(sys.argv) != 2:
    print("Usage: python dataGenerator.py <N>")
    sys.exit()
    
N = int(sys.argv[1])
file_path = 'data/' + 'numbers_' + str(N) + '.txt'

# generate N random positive integers from 0 to 10^8 and write to the file
with open(file_path, 'w') as f:
    for i in range(N):
        number = random.randint(0, 10**8)
        f.write(str(number) + '\n')
