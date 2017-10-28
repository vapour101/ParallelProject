import glob
import numpy as np
import matplotlib.pyplot as plt

def plot_all(filestring="data:*"):
    files=glob.glob(filestring)
    matrix_list=[np.loadtxt(f, delimiter=',') for f in files]
    for i in range(1, len(matrix_list)+1):
        raw_data=matrix_list[i-1]
        sorted = raw_data[raw_data[:, 3].argsort()]
        plt.figure(i)
        plt.plot(sorted[:, 0], 'ro')
        plt.plot(sorted[:, 1], 'b+')
        plt.savefig(filestring+files[i-1])

def plot_better():
    a=np.loadtxt("full_sorted", delimiter=',')
    x=np.arange(2, 26, 1)
    y=2000*np.exp(0.6*x)
    plt.semilogy(a[:, 1], a[:, 0], 'r+', label="Serial")
    plt.semilogy(x,y)
    plt.xlabel("Nodes searched")
    plt.ylabel("Time (nanoseconds)")
    plt.legend(loc='upper left')
    plt.savefig("better_graph")

plot_better()

