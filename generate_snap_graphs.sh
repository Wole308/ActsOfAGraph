#!/bin/bash
#!/bin/bash

# python generate_snap_graph_12by128.py > /home/oj2zf/dataset/rmat_12m_128m.mtx
# python generate_snap_graph_16by128.py > /home/oj2zf/dataset/rmat_16m_128m.mtx
# python generate_snap_graph_20by128.py > /home/oj2zf/dataset/rmat_20m_128m.mtx
# python generate_snap_graph_24by128.py > /home/oj2zf/dataset/rmat_24m_128m.mtx
# python generate_snap_graph_28by128.py > /home/oj2zf/dataset/rmat_28m_128m.mtx

python generate_snap_graph_16by256.py > /home/oj2zf/dataset/rmat_16m_256m.mtx
python generate_snap_graph_32by512.py > /home/oj2zf/dataset/rmat_32m_512m.mtx



