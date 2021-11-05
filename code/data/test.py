import pandas as pd

df = pd.read_csv("lp1.mtx", sep=" ", header=None,names=["x", "y"])
#print(df)
for i in range(0,len(df.x)):
    if(df.x[i]==df.y[i]):
    	print("ok")

