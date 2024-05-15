import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

import csv

a = 0.9

t = [] # column 0
data1 = [] # column 1
data2 = [] # column 1

filename = input("Enter filename: ")
avg_val = 0

with open(filename+'.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column

        avg_val = a * avg_val + (1 - a) * float(row[1])
        data2.append(avg_val)



plt.plot(t, data1,'-b', label='Raw')
plt.plot(t, data2,'-r', label='Filtered')
plt.xlabel('Time [s]')
plt.ylabel('Signal')
plt.title('IIR of ' + filename + ' (A = '+ str(a) +', B = '+ str(1-a) +')')
plt.show()

