"""save_ntuple_pyroot.py
Robustly find TTree objects in a ROOT file and export branches to text/CSV.
"""
from ROOT import TFile
import csv
import sys

fname = "simulation.root"
treename1 = "Detection"  # 可改为实际 ntuple 名称，或设为 None 来选择第一个 TTree
treename2 = "Prompt gamma"
out_txt1 = "Detection.txt"
out_txt2 = "Prompt_gamma.txt"

f = TFile.Open(fname)
if not f or f.IsZombie():
    print("Cannot open file:", fname)
    sys.exit(1)

def find_tree(rootfile, name=None):
    # 尝试按名字直接获取且为 TTree
    if name:
        obj = rootfile.Get(name)
        if obj and obj.InheritsFrom("TTree"):
            return obj
    # 遍历 keys，返回第一个 TTree 或匹配 name 的 TTree
    for key in rootfile.GetListOfKeys():
        kname = key.GetName()
        robj = rootfile.Get(kname)
        if robj and robj.InheritsFrom("TTree"):
            if (name is None) or (kname == name):
                return robj
    return None

tree1 = find_tree(f, treename1)
tree2 = find_tree(f, treename2)

if tree1 is None and tree2 is None:
    print("No TTrees found in", fname)
    f.ls()
    sys.exit(1)

def get_branches(tree):
    return [b.GetName() for b in tree.GetListOfBranches()]

if tree1:
    branches1 = get_branches(tree1)
    with open(out_txt1, "w", newline='') as csvfile:
        writer = csv.writer(csvfile, delimiter=" ")
        writer.writerow(branches1)
        for entry in tree1:
            row = [getattr(entry, name) for name in branches1]
            writer.writerow(row)

if tree2:
    branches2 = get_branches(tree2)
    with open(out_txt2, "w", newline='') as csvfile:
        writer = csv.writer(csvfile, delimiter=" ")
        writer.writerow(branches2)
        for entry in tree2:
            row = [getattr(entry, name) for name in branches2]
            writer.writerow(row)

print("Wrote", out_txt1, out_txt2)