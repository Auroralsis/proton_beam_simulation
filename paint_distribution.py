import numpy as np
import matplotlib.pyplot as plt

fn = "Prompt_gamma.txt"  # 确保在此脚本同目录或改为绝对路径

# 直接用 numpy 读取（跳过 header）
# 数据列顺序: eventID, energy_MeV, x_mm, y_mm, z_mm
print("Loading full data with numpy.loadtxt (this may use significant memory)...")
data = np.loadtxt(fn, skiprows=1)  # 返回 shape=(N,5)

eventID = data[:,0]
energy = data[:,1]
x = data[:,2]
y = data[:,3]
z = data[:,4]

# 可选过滤：只保留能量在4.4MeV左右的瞬发光子
mask = (energy > 4.2) & (energy < 4.6)
print(mask.shape)
x = x[mask]; y = y[mask]; z = z[mask]; energy = energy[mask]

# 直接绘制光子在x轴上的分布
plt.hist(x, bins=1000, range=(-70, 70))

# XY heatmap
plt.figure(figsize=(7,6))
plt.hist2d(x, y, bins=(256, 256), cmap='inferno')
plt.colorbar(label='counts')
plt.xlabel('x (mm)'); plt.ylabel('y (mm)')
plt.title('Prompt gamma XY heatmap')

plt.show()
