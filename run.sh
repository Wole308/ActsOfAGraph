#!/bin/bash
#!/bin/bash

# "USAGE: ./host [--algo] [--rootvid] [--direction] [--numiterations] [--XCLBIN] [--graph_path]"

# ./evaluate_datasets.sh 0 2 
# ./host pr 1 0 1 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/kron_g500-logn20.mtx
./evaluate_datasets.sh 0 2 
./host pr 1 0 1 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_16m_256m.mtx
# ./evaluate_datasets.sh 0 8 
# ./host pr 1 0 1 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/twitter7/twitter7.mtx
# ./evaluate_datasets.sh 0 8 
# ./host pr 1 0 1 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_16m_1024m/rmat_16m_1024m.mtx [FIXME]
# ./evaluate_datasets.sh 0 8 
# ./host pr 1 0 1 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_32m_2048m/rmat_32m_2048m.mtx
# ./evaluate_datasets.sh 0 8 
# ./host pr 1 0 1 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/sk-2005/sk-2005.mtx [FIXME]
# ./evaluate_datasets.sh 0 8 
# ./host pr 1 0 1 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/uk-2005/uk-2005.mtx
# ./evaluate_datasets.sh 0 8 
# ./host pr 1 0 1 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/com-Friendster/com-Friendster.mtx

# ./evaluate_datasets.sh 0 2 
# ./host sssp 1 0 16 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/kron_g500-logn20.mtx
# ./evaluate_datasets.sh 0 2 
# ./host sssp 1 0 16 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_16m_256m.mtx
# ./evaluate_datasets.sh 0 8 
# ./host sssp 1 0 16 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/twitter7/twitter7.mtx
# ./evaluate_datasets.sh 0 8 
# ./host sssp 1 0 16 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_16m_1024m/rmat_16m_1024m.mtx [FIXME]
# ./evaluate_datasets.sh 0 8 
# ./host sssp 1 0 16 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/rmat_32m_2048m/rmat_32m_2048m.mtx
# ./evaluate_datasets.sh 0 8 
# ./host sssp 1 0 16 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/sk-2005/sk-2005.mtx [FIXME]
# ./evaluate_datasets.sh 0 8 
# ./host sssp 1 0 16 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/uk-2005/uk-2005.mtx
# ./evaluate_datasets.sh 0 8 
# ./host sssp 1 0 16 /home/oj2zf/Documents/acts-clusterscale/outputs/vector_addition.xclbin /home/oj2zf/Documents/dataset/com-Friendster/com-Friendster.mtx
