#!/bin/bash
#!/bin/bash

./evaluate.sh
# make host

# ./evaluate.sh
make actsobj
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/soc-orkut.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/soc-LiveJournal1.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/sx-stackoverflow.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/ljournal-2008.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/soc-Pokec.mtx" "NAp"

# ./actsobj "nap" "bfs" 8 1 "/home/oj2zf/dataset/soc-orkut.mtx" "NAp"
# ./actsobj "nap" "pr" 8 1 "/home/oj2zf/dataset/rmat_33m_134m.mtx" "NAp"
# gdb ./actsobj

./actsobj "nap" "hits" 1 0 "/home/oj2zf/dataset/rmat_1m_134m.mtx" "NAp"
./actsobj "nap" "hits" 1 0 "/home/oj2zf/dataset/rmat_2m_134m.mtx" "NAp"
./actsobj "nap" "hits" 1 0 "/home/oj2zf/dataset/rmat_4m_134m.mtx" "NAp"
./actsobj "nap" "hits" 1 0 "/home/oj2zf/dataset/rmat_8m_134m.mtx" "NAp"
./actsobj "nap" "hits" 1 0 "/home/oj2zf/dataset/rmat_16m_134m.mtx" "NAp"
./actsobj "nap" "hits" 1 0 "/home/oj2zf/dataset/rmat_33m_134m.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/rmat_67m_134m.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/rmat_134m_134m.mtx" "NAp"

# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/rmat_1m_268m.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/rmat_2m_268m.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/rmat_4m_268m.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/rmat_8m_268m.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/rmat_16m_268m.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/rmat_33m_268m.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/rmat_67m_268m.mtx" "NAp"
# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/rmat_134m_268m.mtx" "NAp"

# ./actsobj "nap" "pr" 1 0 "/home/oj2zf/dataset/amazon-2008.mtx" "NAp"




