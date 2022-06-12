import snap

# python generate_snap_graph.py > /home/oj2zf/dataset/rmat_28m_128m.mtx
nodes = 28*1024*1024
edges = 128*1024*1024
G2 = snap.GenRndGnm(snap.TNGraph, nodes, edges)
print("%d %d %d" % (nodes, nodes, edges))
for EI in G2.Edges():
    print("%d %d" % (EI.GetDstNId(), EI.GetSrcNId()))




