import numpy as np
import matplotlib.pyplot as plt

fn = "prompt_gamma_simulation.txt"  # 确保在此脚本同目录或改为绝对路径

# 直接用 numpy 读取（跳过 header）
# 数据列顺序: eventID, energy_MeV, x_mm, y_mm, z_mm
print("Loading full data with numpy.loadtxt (this may use significant memory)...")
data = np.loadtxt(fn, skiprows=1)  # 返回 shape=(N,5)

eventID = data[:,0]
energy = data[:,1]
x = data[:,2]
y = data[:,3]
z = data[:,4]

# 可选过滤：只保留能量>0.1 MeV 的光子
mask = (energy > 4.3) & (energy < 4.5)
x = x[mask]; y = y[mask]; z = z[mask]; energy = energy[mask]

plt.hist(x, bins=1000, range=(-80, 80))

# XY heatmap
plt.figure(figsize=(7,6))
plt.hist2d(x, y, bins=(200, 200), cmap='inferno')
plt.colorbar(label='counts')
plt.xlabel('x (mm)'); plt.ylabel('y (mm)')
plt.title('Prompt gamma XY heatmap')

plt.show()
plt.close()
