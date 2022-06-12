import snap

nodes = 32*1024*1024
edges = 512*1024*1024
G2 = snap.GenRndGnm(snap.TNGraph, nodes, edges)
print("%d %d %d" % (nodes, nodes, edges))
for EI in G2.Edges():
    print("%d %d" % (EI.GetDstNId(), EI.GetSrcNId()))




