# save_ntuple_pyroot.py
from ROOT import TFile
import csv

fname = "simulation.root"
treename1 = "Detection"  # 用实际名字
treename2 = "Prompt gamma"
out_txt1 = "Detection.txt"  # 或 .csv
out_txt2 = "Prompt_gamma.txt"

f = TFile.Open(fname)
tree1 = f.Get(treename1)
tree2 = f.Get(treename2)

# 获取分支名列表
branches1 = [b.GetName() for b in tree1.GetListOfBranches()]

# 打开 CSV 文件写头和逐条写入（注意性能：PyROOT 可能较慢）
with open(out_txt1, "w", newline='') as csvfile:
    writer = csv.writer(csvfile, delimiter=" ")
    for i, entry in enumerate(tree1):
        row = [getattr(entry, name) for name in branches1]
        writer.writerow(row)

# 获取分支名列表
branches2 = [b.GetName() for b in tree2.GetListOfBranches()]

# 打开 CSV 文件写头和逐条写入（注意性能：PyROOT 可能较慢）
with open(out_txt2, "w", newline='') as csvfile:
    writer = csv.writer(csvfile, delimiter=" ")
    for i, entry in enumerate(tree2):
        row = [getattr(entry, name) for name in branches2]
        writer.writerow(row)

print("Wrote", out_txt1, out_txt2)