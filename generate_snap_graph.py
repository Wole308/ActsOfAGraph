import snap

# nodes = 100
# edges = 1000

# nodes = 1*1024*1024
# edges = 64*1024*1024

# nodes = 16*1024*1024
# edges = 128*1024*1024

# https://snap.stanford.edu/snappy/index.html
# create a directed random graph on 100 nodes and 1k edges
# G2 = snap.GenRndGnm(snap.TNGraph, nodes, edges)

# traverse the nodes
# for NI in G2.Nodes():
    # print("node id %d with out-degree %d and in-degree %d" % (
        # NI.GetId(), NI.GetOutDeg(), NI.GetInDeg()))
        
# traverse the edges
# for EI in G2.Edges():
    # print("edge (%d, %d)" % (EI.GetSrcNId(), EI.GetDstNId()))
    
# traverse the edges by nodes
# for NI in G2.Nodes():
    # for Id in NI.GetOutEdges():
        # print("edge (%d %d)" % (NI.GetId(), Id))
   
# python generate_snap_graph.py > /home/oj2zf/dataset/rmat_12m_128m.mtx
# nodes = 12*1024*1024
# edges = 128*1024*1024
# G2 = snap.GenRndGnm(snap.TNGraph, nodes, edges)
# print("%d %d %d" % (nodes, nodes, edges))
# for EI in G2.Edges():
    # print("%d %d" % (EI.GetDstNId(), EI.GetSrcNId()))
    
# python generate_snap_graph.py > /home/oj2zf/dataset/rmat_16m_128m.mtx
# nodes = 16*1024*1024
# edges = 128*1024*1024
# G2 = snap.GenRndGnm(snap.TNGraph, nodes, edges)
# print("%d %d %d" % (nodes, nodes, edges))
# for EI in G2.Edges():
    # print("%d %d" % (EI.GetDstNId(), EI.GetSrcNId()))

# python generate_snap_graph.py > /home/oj2zf/dataset/rmat_20m_128m.mtx
nodes = 20*1024*1024
edges = 128*1024*1024
G2 = snap.GenRndGnm(snap.TNGraph, nodes, edges)
print("%d %d %d" % (nodes, nodes, edges))
for EI in G2.Edges():
    print("%d %d" % (EI.GetDstNId(), EI.GetSrcNId()))

# python generate_snap_graph.py > /home/oj2zf/dataset/rmat_24m_128m.mtx
# nodes = 24*1024*1024
# edges = 128*1024*1024
# G2 = snap.GenRndGnm(snap.TNGraph, nodes, edges)
# print("%d %d %d" % (nodes, nodes, edges))
# for EI in G2.Edges():
    # print("%d %d" % (EI.GetDstNId(), EI.GetSrcNId()))

# python generate_snap_graph.py > /home/oj2zf/dataset/rmat_28m_128m.mtx
# nodes = 28*1024*1024
# edges = 128*1024*1024
# G2 = snap.GenRndGnm(snap.TNGraph, nodes, edges)
# print("%d %d %d" % (nodes, nodes, edges))
# for EI in G2.Edges():
    # print("%d %d" % (EI.GetDstNId(), EI.GetSrcNId()))




