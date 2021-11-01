from scipy.sparse import coo_matrix
import pandas as pd
import numpy as np
import time


df = pd.read_csv("belgium_osm.mtx", sep=' ')
df.x = df.x - 1
df.y = df.y - 1
data = [1] * len(df.x)
start = time.time()
mtx = coo_matrix((data, (df.x, df.y)), shape=(1441295, 1441295))
#mtx = coo_matrix(([1]*12, ([0, 0, 0, 1, 1, 1, 2, 2, 2,3,3,3], [1,2,3,0,2,3,0,1,3,0,1,2])), shape=(1441296, 1441296))
a_3 = (coo_matrix(mtx).multiply(coo_matrix(mtx*mtx)))
end = time.time()
print(a_3.sum()/2,end-start)

