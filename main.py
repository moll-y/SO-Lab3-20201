import matplotlib.pyplot as plt 
import numpy as np

x, y = np.loadtxt('test.txt', usecols=(2, 4), unpack=True)

plt.plot(x, y, 'ro') 

plt.xlabel('threads') 
plt.ylabel('time (ms)') 
plt.title('parallel execution') 

plt.savefig("parallel.png")
plt.clf()


xx, yy = np.loadtxt('seq.txt', usecols=(2, 4), unpack=True)

plt.plot(xx, yy, 'ro') 

plt.xlabel('threads') 
plt.ylabel('time (ms)') 
plt.title('sequential execution') 

plt.savefig("sequential.png")
