import pandas as pd
import os

os.chdir("../data")

t2 = pd.read_csv("result2.csv")
t3 = pd.read_csv("result3.csv")
t2.columns = t2.columns.str.replace(' ', '')
t3.columns = t3.columns.str.replace(' ', '')
t1 = pd.merge(pd.read_csv("T1.csv"),pd.read_csv("T2.csv"),left_on="a",right_on="k")

arr = t1.columns.values.tolist()

t1 = t1.sort_values(by=arr).reset_index(drop=True)
t2 = t2.sort_values(by=arr).reset_index(drop=True)
t3 = t3.sort_values(by=arr).reset_index(drop=True)

if(t1.equals(t2)):
    print("NESTED IS CORRECT :)")
else:
    print("PROBLEM WITH NESTED")

if(t1.equals(t3)):
    print("PARTHASH IS CORRECT :)")
else:
    print("PROBLEM WITH PARTHASH")