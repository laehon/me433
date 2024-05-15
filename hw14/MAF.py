import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function

import csv

n = 20


t = [] # column 0
data1 = [] # column 1
data2 = [] # column 1
filter = [0] * n

filename = input("Enter filename: ")
count = 0

with open(filename+'.csv') as f:
    # open the csv file
    reader = csv.reader(f)
    for row in reader:
        # read the rows 1 one by one
        t.append(float(row[0])) # leftmost column
        data1.append(float(row[1])) # second column

        filter[count % n] = float(row[1])
        val = sum(filter) / n
        data2.append(val)
        count = count + 1



plt.plot(t, data1,'-b', label='Raw')
plt.plot(t, data2,'-r', label='Filtered')
plt.xlabel('Time [s]')
plt.ylabel('Signal')
plt.title('MAF of '+ filename +' (Averaging '+ str(n) +' data points)')
plt.show()

