import glob
import numpy as np
import matplotlib.pyplot as plt

def plot_all(filestring="*data"):
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
    a=np.loadtxt("better_data", delimiter=',')
    plt.semilogy(a[0:100, 0], 'ro')
    plt.semilogy(a[0:100, 1], 'b+')
    plt.show()
    #plt.savefig("better_graph")


plot_better()