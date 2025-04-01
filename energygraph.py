import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("./build/energy.csv", header=None, names=["Energy"])

plt.figure(figsize=(8, 6))

plt.plot(df.index, df["Energy"], marker="o", linestyle="-", color="b")

plt.xlabel("Measurement Number (X)")
plt.ylabel("Energy (Y)")
plt.title("Energy vs Measurement Number")

plt.grid(True)

plt.show()

print(df)