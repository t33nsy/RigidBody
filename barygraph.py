import pandas as pd
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

df = pd.read_csv("./build/bary.csv", header=None, names=["X", "Y", "Z"])

fig = plt.figure(figsize=(8, 6))
ax = fig.add_subplot(111, projection="3d")

sc = ax.scatter(df["X"], df["Y"], df["Z"], c=df["Z"], cmap="coolwarm", marker="o")

plt.colorbar(sc, ax=ax, label="Z value")

ax.set_label("BARYCENTER")
ax.set_xlabel("X")
ax.set_ylabel("Y")
ax.set_zlabel("Z")
ax.set_title("3D Scatter Plot from CSV")

plt.show()
