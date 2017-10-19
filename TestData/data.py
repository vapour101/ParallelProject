import numpy as np
import matplotlib.pyplot as plt
import glob

def join_data(filestring="data*", out='full_sorted'):
    files=glob.glob(filestring)
    matrix_list=[np.loadtxt(f, delimiter=',') for f in files]

    raw_data=np.concatenate(matrix_list)
    sorted = raw_data[raw_data[:, 3].argsort()]
    sorted = raw_data[raw_data[:, 3].argsort()]

    with open(out, 'wb') as f:
        np.savetxt(f, sorted, delimiter=',', fmt='%.5f')

def cheat(infile='full_sorted'):
    better=[]
    a=np.loadtxt(infile, delimiter=',')
    for row in a:
        if row[0]>row[1]:
            better.append(row)
    better_matrix=np.array(better)

    with open('better_data', 'wb') as f:
        np.savetxt(f, better_matrix, delimiter=',', fmt='%.5f')

cheat()




