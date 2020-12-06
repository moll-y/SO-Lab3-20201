#!/bin/python
import matplotlib.pyplot as plt 
import numpy as np

x, y = np.loadtxt('data.txt', usecols=(2, 4), unpack=True)
y_avgr = []
n = 0

for i in range(len(y)):
    n += y[i]
    if i != 0 and (i+1) % 20 == 0:
        avgr = n / 20
        y_avgr.append(avgr)
        n = 0
    
plt.xticks(fontsize=14)
plt.style.use('ggplot')
plt.figure(figsize=(7,7))

plt.plot(x, y, 'ro') 
plt.plot([1,2,4,8], y_avgr, label='average', color='blue', marker='+') 

plt.legend(loc='upper left', frameon=True)

plt.xlabel('Number of cores/threads') 
plt.ylabel('Time (ms)') 
plt.title('Parallel Execution') 

plt.savefig('parallel.png', dpi=400)


#plt.clf()


#xx, yy = np.loadtxt('seq.txt', usecols=(2, 4), unpack=True)

#plt.plot(xx, yy, 'ro') 

#plt.xlabel('threads') 
#plt.ylabel('time (ms)') 
#plt.title('sequential execution') 

#plt.savefig("sequential.png")
