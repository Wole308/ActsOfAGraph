############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
## vivado_hls -f script.tcl
############################################################

# faketime -f "-1y" vitis_hls -f script.tcl

# open_project acts_vitishls
open_project acts_vitishls3
# set_top TOPP0_topkernelP1
# set_top TOPP0_topkernelP9
# set_top TOPP0_topkernelP6
set_top TOPP0_topkernelP1
add_files acts/acts_all.cpp
open_solution "solution1"
set_part {xcu280-fsvh2892-2L-e}
create_clock -period 10 -name default
#source "./jp_hw/solution1/directives.tcl"
#csim_design
csynth_design
#cosim_design
export_design -format ip_catalog
