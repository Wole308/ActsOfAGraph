
sudo lsblk -o NAME,FSTYPE,SIZE,MOUNTPOINT,LABEL

################# changes you might want to change back ################################
cal_write.cpp: // if(writesz > 0){ checkkvforallzeros_dram(dram, was commented out

partitioner.cpp: hashfunc
################# changes you might want to change back ################################



ssh -X wole.j@msl-dpe-d3
DEVICE=xilinx_vcu1525_dynamic_5_1
xlock
/opt/Xilinx/Vivado/2018.2/bin/vivado_hls
/share/SDAccel_Examples/getting_started/hello_world/
sdx
vivado_hls
https://github.com/Xilinx/SDAccel_Examples
vim
:wq!
:q!

=== WORK DIRECTORY ===
~/Documents/SDAccel_Examples/getting_started/hello_world/naive_pattern_search
make check TARGET=sw_emu DEVICE=xilinx_vcu1525_dynamic_5_1
nohup make all DEVICE=xilinx_vcu1525_dynamic_5_0

vivado_hls -f script.tcl

JumpHost
IP: 105.128.45.150
IP: 105.128.45.129 (latest)
username: wole.j

UVA
username: oj2zf
Host: automata11.cs.virginia.edu

pkill -9 -u `id -u username`

./host -r /nvme/byte_copy_file.txt


################# P2P bytecopy ################################
source /opt/dsa/xilinx_vcu1525_dynamic_5_0_p2p/xbinst/setup.sh
#build host and xclbin scripts
./build_host.sh
./build_xclbin.sh (only works for hw)

source /opt/dsa/xilinx_vcu1525_dynamic_5_0_p2p/xbinst/setup.sh
#run compiled binaries
./host.exe -r /nvme/byte_copy_file.txt
################# P2P bytecopy ################################

################# build and run hw ################################
nohup make all DEVICE=xilinx_vcu1525_dynamic_5_0

now 5_1

# source /opt/dsa/xilinx_vcu1525_dynamic_5_0_p2p/xbinst/setup.sh
source /opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/setup.sh
./host xclbin/vadd.hw.xilinx_vcu1525_dynamic_5_0.xclbin

source /opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/setup.sh
./host.exe kernel.xclbin
################# build and run hw ################################

################# build and run sw_emu ################################
make check TARGET=sw_emu DEVICE=xilinx_vcu1525_dynamic_5_0 
#1
################# build and run sw_emu ################################
source /opt/Xilinx/SDx/2018.2/settings64.sh
source /opt/XRT/build/Release/opt/xilinx/xrt/setup.sh
export XILINX_SDX=/opt/Xilinx/SDx/2018.2

################# bashrc contents ################################

wole.j@msl-dpe-d3

ps faux | grep -v boot

sudo reboot

system name:iDRAC : https://10.1.100.242/restgui/start.html?login
username:dpe 
password:dpe

tar xvzf file.tar.gz

################# FPGA shell / debug commands ################################
xbutil reset 
xbutil query
lspci | grep Xil
xbutil scan 

xbsak reset (xbsak: old version)
xbsak query
lspci | grep Xil
xbsak scan 

################# logging in as root ################################
IP: ssh root@msl-dpe-d3
Password: msl-dpe

################# FPGA shell / debug commands ################################
xbsak 


// changes made to:
apply_vertexupdates.cpp 
partitionarraytoT.cpp
partitionarray.cpp

/opt/Xilinx/SDx/2018.2/bin/xcpp -I /opt/Xilinx/SDx/2018.2/runtime/include/1_2/ -I//opt/Xilinx/SDx/2018.2/Vivado_HLS/include/ -O0 -g -Wall -fmessage-length=0 -std=c++14 -I../../..//libs/xcl2 -I../../..//libs/bitmap src/host.cpp ../../..//libs/xcl2/xcl2.cpp ../../..//libs/bitmap/bitmap.cpp  -o 'host' -lOpenCL -lpthread -lrt -lstdc++ -L/opt/Xilinx/SDx/2018.2/runtime/lib/x86_64

echo $PATH

/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games
export PATH=$PATH:/path/to/my/program
echo $PATH

find /lib* /usr/lib* -name lib*.a

################# SDAccel check timing information ################################
/home/wole.j/Documents/ACTGraph/_x/logs/kernel/topkernel_vivado_hls.log
/home/wole.j/Documents/ACTGraph/_x/reports/link/system_estimate_kernel.xtxt

################# changes ################################
inlined function in partitioner
inlined all functions in array.cpp, cal.cpp, general_utility.cpp
restructured hash function
push_compute.cpp
array.h
memory_allocator.cpp
applyLL.cpp 
partitionLL.cpp

push_compute:
// *edgebufferstatus = EMPTY;
	if((_edgebuffer_tracker->weof - _edgebuffer_tracker->reof) <= (_EDGEBUFFERSZ / 8)){ *edgebufferstatus = EMPTY; } else { *edgebufferstatus = NOT_EMPTY; }

gen.py:
context['MAXDRAMEDGECAPACITY'] = 1073741824 # 268434944

################# ??? ################################
install with apt-get? it should automatically add path to environment variable (refresh cache, etc)
by calling ld_config


################# generate large kronecker graphs ################################
# graphgen -g:e -n:1000 -m:5000 -o:graph.txt
# /home/wole.j/Documents/snap-master/examples/graphgen
./graphgen -g:e -n:100 -m:1700 -o:/projects/DPE/wole/rmat_1B_17B.graph
./graphgen -g:p -n:100 -p:0.1 -o:rmat_1B_17B.graph
./krongen -o:kronecker_graph.txt -m:"0.9 0.6; 0.6 0.1" -i:10

nohup ./graphgen -g:k -n:1000000000 -m:17000000000 -o:/projects/DPE/wole/rmat/rmat_1B_17B.graph
nohup ./graphgen -g:p -n:1000000000 -o:/projects/DPE/wole/rmat/rmat_1B_17B.graph
nohup ./graphgen -g:e -n:1000000000 -m:17000000000 -o:/projects/DPE/wole/rmat/rmat_1B_17B.graph

################# FPGA in crabtree (Sergiu's machine) ################################
### === how to run ===
xilinx19sdx
XRT
make swemu

### === path to relevant information ===
/tools/Xilinx/SDx/2019.1/
/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm

###
make check TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm
make all DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm

### AWS 
source /opt/xilinx/xrt/setup.sh 
source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
FPGA part: xcvu9p-flga2104-1-e

### running FPGA kernel on F1 instance 
cd /home/centos/src/project_data/wole/ACTGraph_Legend
247  python gen.py 
248  history
249  75!
250  source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
251  source /opt/xilinx/xrt/setup.sh 
252  sdx
253  source ~/src/project_data/aws-fpga/sdaccel_setup.sh 
254  history
255  cp -rf kernel.xclbin /home/centos/src/project_data/aws-fpga/SDAccel/tools/build/
256  cd /home/centos/src/project_data/aws-fpga/SDAccel/tools/build
257  cd ..
258  cd build/
259  ../create_sdaccel_afi.sh -xclbin=kernel.xclbin -o=kernel -s3_bucket=alifahmed -s3_dcp_key=dcp -s3_logs_key=logs
260  history
261  aws ec2 describe-fpga-image --fpga-image-id afi--03131e5ff3a58bb62
262  aws ec2 describe-fpga-image --fpga-image-id afi-03131e5ff3a58bb62 (get id (e.g. 03131e5ff3a58bb62) from /home/centos/src/project_data/aws-fpga/SDAccel/tools/build/20_03_05-193942_afi_id.txt)
263  aws ec2 describe-fpga-images --fpga-image-id afi-03131e5ff3a58bb62
264  aws ec2 describe-fpga-images --owners self
265  history
266  history > hist.txt






