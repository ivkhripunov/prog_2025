import numpy as np
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker

CSV_PATH = "/home/ivan/code/prog_2025/code/sem_2/task_18/10_05/collisions.csv"
PNG_PATH = "/home/ivan/code/prog_2025/code/sem_2/task_18/10_05/collisions.png"

data  = np.genfromtxt(CSV_PATH, delimiter=",", names=True, dtype=None, encoding="utf-8")
names = list(data.dtype.names[1:])  # skip "n"
ns    = data["n"]

colors = [
    "#3266ad",  # RS
    "#e05c2a",  # JS
    "#2e9e75",  # PJW
    "#9b59b6",  # ELF
    "#c0392b",  # BKDR
    "#e67e22",  # SDBM
    "#d4ac0d",  # DJB
    "#e91e8c",  # DEK
    "#73726c",  # AP
]

linestyles = ["-", "--", "-", "--", "-", "--", "-", "--", "-"]

fig, ax = plt.subplots(figsize=(11, 6))

ns_theory = np.linspace(0, ns[-1], 500)
ax.plot(
    ns_theory,
    ns_theory ** 2 / (2 * 2 ** 32),
    color="black",
    linestyle=":",
    linewidth=1.5,
    label="Theory N²/2M",
    )

for i, name in enumerate(names):
    #if name in ['PJW', 'ELF']: continue
    ax.plot(
        ns, data[name],
        label=name,
        color=colors[i],
        linestyle=linestyles[i],
        linewidth=2,
        marker="o",
        markersize=3,
        markevery=50,
    )

ax.set_xlabel("Число строк (N)", fontsize=12)
ax.set_ylabel("Коллизии", fontsize=12)
ax.set_title("Коллизии хеш-функций. Случайные строки длины 50", fontsize=13)
ax.legend(ncol=3, fontsize=10)
ax.grid(True, linestyle="--", alpha=0.4)
ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, _: f"{int(x):,}"))
fig.tight_layout()
plt.savefig(PNG_PATH, dpi=150)
print(f"Saved {PNG_PATH}")