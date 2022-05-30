#!/bin/bash
#!/bin/bash

DEVICEPATH=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm

./evaluate.sh
faketime -f "-1y" make hw PLATFORM=$DEVICEPATH SYNFREQUENCY=300



