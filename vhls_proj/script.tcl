############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
## vivado_hls -f script.tcl
############################################################

open_project actsofagraph
set_top topkernel

add_files ../kernels/acts.cpp

open_solution "solution1"
# set_part {xcvu9p-fsgd2104-3-e}
set_part {xcvu9p-fsgd2104-2-i}
create_clock -period 10 -name default
#source "./jp_hw/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -format ip_catalog
