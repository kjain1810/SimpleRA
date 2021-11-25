import sys
from random import randint
import os

os.chdir("../data")


orgstdout = sys.stdout
t1 = open("T1.csv","w")
t2 = open("T2.csv","w")
sys.stdout = t1

num_of_columns_t1 = randint(4,10)
num_of_rows = randint(2800,4000)

for i in range(num_of_columns_t1):
    if i > 0:
        print(",",end="")
    print(chr(97 + i),end="")
print()

for i in range(num_of_rows):
    for j in range(num_of_columns_t1):
        if j > 0:
            print(",",end="")
        print(randint(10,50),end="")
    print()

sys.stdout = t2

num_of_columns = randint(7,15)
num_of_rows = randint(2800,4000)

for i in range(num_of_columns):
    if i > 0:
        print(",",end="")
    print(chr(97 + num_of_columns_t1 + i),end="")
print()

for i in range(num_of_rows):
    for j in range(num_of_columns):
        if j > 0:
            print(",",end="")
        print(randint(10,100),end="")
    print()

sys.stdout = orgstdout