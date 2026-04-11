import subprocess
import csv
import io
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

result = subprocess.run(["/home/ivan/code/prog_2025/10_05"], capture_output=True, text=True)

reader = csv.DictReader(io.StringIO(result.stdout))
rows = list(reader)

names = reader.fieldnames[1:]
ns = [int(row["n"]) for row in rows]

colors = [
    "#3266ad", "#e05c2a", "#2e9e75", "#9b59b6",
    "#c0392b", "#e67e22", "#d4ac0d", "#e91e8c", "#73726c",
]

dashes = [
    (None, None), (6, 3), (None, None), (6, 3),
    (None, None), (4, 4), (None, None), (8, 3), (3, 3),
]

fig, ax = plt.subplots(figsize=(10, 6))

for i, name in enumerate(names):
    ys = [int(row[name]) for row in rows]
    ls = "--" if dashes[i][0] else "-"
    ax.plot(ns, ys, label=name, color=colors[i], linestyle=ls, linewidth=2, marker="o", markersize=4)

ax.set_xlabel("Число строк", fontsize=12)
ax.set_ylabel("Коллизии", fontsize=12)
ax.set_title("Коллизии хеш-функций", fontsize=13)
ax.legend(ncol=3, fontsize=10)
ax.grid(True, linestyle="--", alpha=0.4)
fig.tight_layout()
plt.savefig("collisions.png", dpi=150)
