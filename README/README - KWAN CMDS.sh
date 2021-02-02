partitionarray:: range for each linked-list in dest: 65536

KERNEL FINISHED SUCCESSFULLY



EXECUTE_KERNEL COMPLETED SUCCESSFULLY!

execute_write_AU:: HOST -> FPGA(host BO) -> FPGA(p2p BO) -> SSD

EXECUTE_WRITE COMPLETED SUCCESSFULLY!

APPLY UPDATES (AU) KERNEL EXECUTED SUCCESSFULLY!

FREEING MEMORY

host:: SYSTEMSHRINKFACTOR: 16

host:: MAXDRAMKVCAPACITY: 3145728

host:: MAXDRAMVERTEXCAPACITY: 1048576

host:: MAX TREE DEPTH: 3

host:: NUM_PARTITIONS: 16

host:: ALLOWANCEFACTOR: 100

host:: AVERAGE_EDGE_VERTEX_RATIO: 2

host:: KVDRAMWORKASZ (keyvalue_t): 3211264

host:: KVDRAMWORKBSZ (keyvalue_t): 3211264

host:: KVDRAMWORKASZ_KVS (keyvalueset_t): 200704

host:: KVDRAMWORKBSZ_KVS (keyvalueset_t): 200704

host:: EDGEDRAMSZ (edge_t): 2097152

host:: EDGEDRAMSZ_ES (edgeset_t): 131072

host:: VERTEXDRAMSZ (vertex_t): 1048576

host:: VERTEXDRAMSZ_VS (vertexset_t): 131072

host:: WORKACALSTATESDRAMSZ: 262144

host:: WORKBCALSTATESDRAMSZ: 262144

host:: _KVBUFFERSZ: 8192

host:: _KVBUFFERSZ_KVS: 512

host:: _VERTEXBUFFERSZ (vertex_t): 256

host:: _VERTEXBUFFERSZ_VS (vertexset_t): 32

host:: _EDGEBUFFERSZ (edge_t): 256

host:: _EDGEBUFFERSZ_ES (edgeset_t): 16

host:: _VERTEXUPDATESBUFFER: 32768

host:: _VERTEXUPDATESBUFFER_KVS: 2048

host:: OBJECTBLOCKOFFSETSSZ: 1024

host:: SMALLCALBLOCKSZ: 1024

host:: LARGECALBLOCKSZ: 50176

host:: _KVPARTITIONSBUFFERSZ: 1024

host:: _KVPARTITIONSBUFFERSZ_KVS: 64

host:: _MAXBLOCKRAMRANGE: 4096

host:: CALWORKSPACEFACTOR: 3

WARNING: Profiling may contain incomplete information. Please ensure all OpenCL objects are released by your host code (e.g., clReleaseProgram()).

[12:18:11]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$make cleanall

*** started! ***

cleanall successful

rm -rf vadd.xo xocc_kernel* _x pfm_top_* kernel.xo kernel.xclbin host.exe

[12:18:55]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$clear

[12:19:22]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$source /opt/dsa/xilinx_vcu1525_dynamic_5_0_p2p/xbinst/setup.sh

[12:19:32]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$./host.exe

host:: SYSTEMSHRINKFACTOR: 16

host:: MAXDRAMKVCAPACITY: 3145728

host:: MAXDRAMVERTEXCAPACITY: 1048576

host:: MAX TREE DEPTH: 3

host:: NUM_PARTITIONS: 16

host:: ALLOWANCEFACTOR: 100

host:: AVERAGE_EDGE_VERTEX_RATIO: 2

host:: KVDRAMWORKASZ (keyvalue_t): 3211264

host:: KVDRAMWORKBSZ (keyvalue_t): 3211264

host:: KVDRAMWORKASZ_KVS (keyvalueset_t): 200704

host:: KVDRAMWORKBSZ_KVS (keyvalueset_t): 200704

host:: EDGEDRAMSZ (edge_t): 2097152

host:: EDGEDRAMSZ_ES (edgeset_t): 131072

host:: VERTEXDRAMSZ (vertex_t): 1048576

host:: VERTEXDRAMSZ_VS (vertexset_t): 131072

host:: WORKACALSTATESDRAMSZ: 262144

host:: WORKBCALSTATESDRAMSZ: 262144

host:: _KVBUFFERSZ: 8192

host:: _KVBUFFERSZ_KVS: 512

host:: _VERTEXBUFFERSZ (vertex_t): 256

host:: _VERTEXBUFFERSZ_VS (vertexset_t): 32

host:: _EDGEBUFFERSZ (edge_t): 256

host:: _EDGEBUFFERSZ_ES (edgeset_t): 16

host:: _VERTEXUPDATESBUFFER: 32768

host:: _VERTEXUPDATESBUFFER_KVS: 2048

host:: OBJECTBLOCKOFFSETSSZ: 1024

host:: SMALLCALBLOCKSZ: 1024

host:: LARGECALBLOCKSZ: 50176

host:: _KVPARTITIONSBUFFERSZ: 1024

host:: _KVPARTITIONSBUFFERSZ_KVS: 64

host:: _MAXBLOCKRAMRANGE: 4096

host:: CALWORKSPACEFACTOR: 3

host:: seen here 7

Host:: No exception generated from system parameters. TreeDepth is an Integer

MAXDRAMVERTEXCAPACITY_POW: 20

_VERTEXBUFFERSZ_POW: 8

NUM_PARTITIONS_POW: 4

INFO: Successfully opened NVME SSD (vertices) /home/wole.j/Documents/Summer_Projects/mockssd/vertices.graph

INFO: Successfully opened NVME SSD (edges) /home/wole.j/Documents/Summer_Projects/mockssd/edges.graph

INFO: Successfully opened NVME SSD (workA) /home/wole.j/Documents/Summer_Projects/mockssd/keyvalues_backup.graph

host:: seen here 1

xcl.c:: seen here 1

xcl.c:: seen here 2

xcl.c:: seen here 7

WARNING: The setting device_profile will be deprecated after 2018.2. Please use data_transfer_trace.

xcl.c:: seen here 8

host:: seen here 2

INFO: Importing ./kernel.xclbin

INFO: Loaded file

INFO: Created Binary

INFO: Built Program

host:: seen here 3

host:: seen here 4

MEMORY FOR ALL SSD KV CONTAINERS ALLOCATED SUCCESSFULLY!

64 SSD KV CONTAINERS CREATED SUCCESSFULLY!

MEMORY FOR ALL APPLY UPDATES CHUNKS ALLOCATED SUCCESSFULLY!

16 APPLY UPDATES CHUNKS CREATED SUCCESSFULLY!

execute_read_AU:: SSD -> FPGA(p2p BO) -> FPGA(host BO) -> HOST

EXECUTE_READ COMPLETED SUCCESSFULLY!

host:: about to launch kernel

^C

[12:24:51]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$xbutil query

ERROR: No card found

[12:24:56]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$source /opt/

containerd/         dsa/                ldap_client_ubuntu/ Xilinx/             XRT/

[12:24:56]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$xbsak

Running xbsak for 4.0+ DSA's



Usage: xbsak <command> [options]



Command and option summary:

  boot    [-d device]

  clock   [-d device] [-r region] [-f clock1_freq_MHz] [-g clock2_freq_MHz]

  dmatest [-d device] [-b [0x]block_size_KB]

  mem     --read [-d device] [-a [0x]start_addr] [-i size_bytes] [-o output filename]

  mem     --write [-d device] [-a [0x]start_addr] [-i size_bytes] [-e pattern_byte]

  flash   [-d device] -m primary_mcs [-n secondary_mcs] [-o bpi|spi]

  flash   scan

  help

  list

  scan

  program [-d device] [-r region] -p xclbin

  query   [-d device [-r region]]

  reset   [-d device] [-h | -r region]

  status  [--debug_ip_name]



Examples:

List all devices

  xbsak list

Scan for Xilinx PCIe device(s) & associated drivers (if any) and relevant system information

  xbsak scan

Boot device 1 from PROM and retrain the PCIe link without rebooting the host

  xbsak boot -d 1

Change the clock frequency of region 0 in device 0 to 100 MHz

  xbsak clock -f 100

For device 0 which supports multiple clocks, change the clock 1 to 200MHz and clock 2 to 250MHz

  xbsak clock -f 200 -g 250

Download the accelerator program for device 2

  xbsak program -d 2 -p a.xclbin

Run DMA test on device 1 with 32 KB blocks of buffer

  xbsak dmatest -d 1 -b 0x2000

Read 256 bytes from DDR starting at 0x1000 into file read.out

  xbsak mem --read -a 0x1000 -i 256 -o read.out

  Default values for address is 0x0, size is DDR size and file is memread.out

Write 256 bytes to DDR starting at 0x1000 with byte 0xaa

  xbsak mem --write -a 0x1000 -i 256 -e 0xaa

  Default values for address is 0x0, size is DDR size and pattern is 0x0

List the debug IPs available on the platform

  xbsak status

[12:25:31]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$xbsak query

INFO: Found 1 device(s)

DSA name:       xilinx_vcu1525_dynamic_5_0

Vendor:         10ee

Device:         6a8f

SDevice:        4350

SVendor:        10ee

DDR size:       0x4000000 KB

DDR count:      4

OnChip Temp:    34 C

Power(Beta):    **Unable to estimate power**

OCL Frequency:

        0:      150 MHz

        1:      500 MHz

PCIe:           GEN3 x 16

DMA bi-directional threads:    2

MIG Calibrated: true



Total DMA Transfer Metrics:

  Chan[0].h2c:  0xa61d KB

  Chan[0].c2h:  0x10 KB

  Chan[1].h2c:  0xa624 KB

  Chan[1].c2h:  0x10 KB



Firewall Last Error Status:

        0:      0x0 (GOOD)

        1:      0x0 (GOOD)

        2:      0x0 (GOOD)



Xclbin ID:  0x5d5c1a7d



Mem Topology:

     Tag       Type          Base Address  Size (KB)

 [0] bank0     MEM_DDR4      0x0           0x1000000

 [1] bank2     **UNUSED**    0x800000000   0x1000000

 [2] bank3     **UNUSED**    0xc00000000   0x1000000

 [3] bank1     **UNUSED**    0x400000000   0x1000000



Compute Unit Status:

  CU[0]: kernel:kernel_1@0x1800000 (START)



Device Memory Usage:

  Bank[0].mem:  0x0 KB

  Bank[0].bo:   0

  Bank[1].mem:  0x0 KB

  Bank[1].bo:   0

  Bank[2].mem:  0x0 KB

  Bank[2].bo:   0

  Bank[3].mem:  0x0 KB

  Bank[3].bo:   0

INFO: xbsak query successful.

[12:25:35]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$

[12:26:14]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$

[12:26:14]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$ls

description.json  kernel.xclbin  lessons_learned.txt  qor.json   sdaccel.ini                  src       vhls_proj  xcl.h

host.exe          kernel.xo      Makefile             README.md  sdaccel_profile_summary.csv  utils.mk  xcl.c      xrt.ini

[12:26:15]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$vi Makefile

[12:27:46]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$cd /opt/Xilinx/

[12:27:50]wole.j@msl-dpe-d3:/opt/Xilinx$ls

DocNav  SDK  SDx  Vivado  xic  xilinx_vcu1525_dynamic_5_1

[12:27:50]wole.j@msl-dpe-d3:/opt/Xilinx$cd SDx/

[12:27:58]wole.j@msl-dpe-d3:/opt/Xilinx/SDx$ls

2018.2

[12:27:59]wole.j@msl-dpe-d3:/opt/Xilinx/SDx$cd 2018.2/

[12:28:01]wole.j@msl-dpe-d3:/opt/Xilinx/SDx/2018.2$ls

bin     data  docs     examples  gnu      lib         lnx64      runtime  scripts         settings64.sh  strategies  templates

common  doc   eclipse  ext       include  llvm-clang  platforms  samples  settings64.csh  sim            target      tps

[12:28:01]wole.j@msl-dpe-d3:/opt/Xilinx/SDx/2018.2$cd platforms/

[12:28:03]wole.j@msl-dpe-d3:/opt/Xilinx/SDx/2018.2/platforms$ls

xilinx_kcu1500_dynamic_5_0  xilinx_vcu1525_dynamic_5_0  zc702  zc706  zcu102  zcu104  zcu106  zed

[12:28:04]wole.j@msl-dpe-d3:/opt/Xilinx/SDx/2018.2/platforms$cd ..

[12:28:07]wole.j@msl-dpe-d3:/opt/Xilinx/SDx/2018.2$ls

bin     data  docs     examples  gnu      lib         lnx64      runtime  scripts         settings64.sh  strategies  templates

common  doc   eclipse  ext       include  llvm-clang  platforms  samples  settings64.csh  sim            target      tps

[12:28:08]wole.j@msl-dpe-d3:/opt/Xilinx/SDx/2018.2$cd ..

[12:28:09]wole.j@msl-dpe-d3:/opt/Xilinx/SDx$ls

2018.2

[12:28:09]wole.j@msl-dpe-d3:/opt/Xilinx/SDx$cd ..

[12:28:13]wole.j@msl-dpe-d3:/opt/Xilinx$ls

DocNav  SDK  SDx  Vivado  xic  xilinx_vcu1525_dynamic_5_1

[12:28:13]wole.j@msl-dpe-d3:/opt/Xilinx$cd xi

xic/                        xilinx_vcu1525_dynamic_5_1/

[12:28:13]wole.j@msl-dpe-d3:/opt/Xilinx$cd xilinx_vcu1525_dynamic_5_1/

[12:28:20]wole.j@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1$ls

hw  sw  test  xbinst  xilinx_vcu1525_dynamic_5_1.xpfm

[12:28:20]wole.j@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1$cd xbinst/

[12:28:24]wole.j@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst$ls

firmware  install.sh  runtime  test

[12:28:24]wole.j@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst$cd firmware/

[12:28:32]wole.j@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware$ls

10ee-6a90-4351-000000005aace1cf.dsabin  xilinx_vcu1525_dynamic_5_1.bit  xilinx_vcu1525_dynamic_5_1.mcs  xilinx_vcu1525_dynamic_5_1.prm

[12:28:32]wole.j@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware$xbsak query

INFO: Found 1 device(s)

DSA name:       xilinx_vcu1525_dynamic_5_0

Vendor:         10ee

Device:         6a8f

SDevice:        4350

SVendor:        10ee

DDR size:       0x4000000 KB

DDR count:      4

OnChip Temp:    36 C

Power(Beta):    **Unable to estimate power**

OCL Frequency:

        0:      150 MHz

        1:      500 MHz

PCIe:           GEN3 x 16

DMA bi-directional threads:    2

MIG Calibrated: true



Total DMA Transfer Metrics:

  Chan[0].h2c:  0xa61d KB

  Chan[0].c2h:  0x10 KB

  Chan[1].h2c:  0xa624 KB

  Chan[1].c2h:  0x10 KB



Firewall Last Error Status:

        0:      0x0 (GOOD)

        1:      0x0 (GOOD)

        2:      0x0 (GOOD)



Xclbin ID:  0x5d5c1a7d



Mem Topology:

     Tag       Type          Base Address  Size (KB)

 [0] bank0     MEM_DDR4      0x0           0x1000000

 [1] bank2     **UNUSED**    0x800000000   0x1000000

 [2] bank3     **UNUSED**    0xc00000000   0x1000000

 [3] bank1     **UNUSED**    0x400000000   0x1000000



Compute Unit Status:

  CU[0]: kernel:kernel_1@0x1800000 (START)



Device Memory Usage:

  Bank[0].mem:  0x0 KB

  Bank[0].bo:   0

  Bank[1].mem:  0x0 KB

  Bank[1].bo:   0

  Bank[2].mem:  0x0 KB

  Bank[2].bo:   0

  Bank[3].mem:  0x0 KB

  Bank[3].bo:   0

INFO: xbsak query successful.

[12:28:36]wole.j@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware$sudo su

[sudo] password for wole.j:

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware# ls

10ee-6a90-4351-000000005aace1cf.dsabin  xilinx_vcu1525_dynamic_5_1.bit  xilinx_vcu1525_dynamic_5_1.mcs  xilinx_vcu1525_dynamic_5_1.prm

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware# cp ./10ee-6a90-4351-000000005aace1cf.dsabin ./xilinx_vcu1525_dynamic_5_1.bit /lib/firmware/xilinx/

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware# ls

10ee-6a90-4351-000000005aace1cf.dsabin  xilinx_vcu1525_dynamic_5_1.bit  xilinx_vcu1525_dynamic_5_1.mcs  xilinx_vcu1525_dynamic_5_1.prm

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware# xbsak list

xbsak: command not found

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware# xbsak scan

xbsak: command not found

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware# ls

10ee-6a90-4351-000000005aace1cf.dsabin  xilinx_vcu1525_dynamic_5_1.bit  xilinx_vcu1525_dynamic_5_1.mcs  xilinx_vcu1525_dynamic_5_1.prm

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware# source ../runtime/

bin/       lib/       platforms/

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware# ls

10ee-6a90-4351-000000005aace1cf.dsabin  xilinx_vcu1525_dynamic_5_1.bit  xilinx_vcu1525_dynamic_5_1.mcs  xilinx_vcu1525_dynamic_5_1.prm

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/firmware# cd ..

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst# ls

firmware  install.sh  runtime  test

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst# cd runtime/

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime# ls

bin  lib  platforms

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime# cd bin/

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime/bin# ls

xbflash  xbrecover  xbsak  xclbincat  xclbinsplit

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime/bin# ./xbsak

Running xbsak for 4.0+ DSA's



Usage: ./xbsak <command> [options]



Command and option summary:

  boot    [-d device]

  clock   [-d device] [-r region] [-f clock1_freq_MHz] [-g clock2_freq_MHz]

  dmatest [-d device] [-b [0x]block_size_KB]

  mem     --read [-d device] [-a [0x]start_addr] [-i size_bytes] [-o output filename]

  mem     --write [-d device] [-a [0x]start_addr] [-i size_bytes] [-e pattern_byte]

  flash   [-d device] -m primary_mcs [-n secondary_mcs] [-o bpi|spi]

  flash   scan

  help

  list

  scan

  program [-d device] [-r region] -p xclbin

  query   [-d device [-r region]]

  reset   [-d device] [-h | -r region]

  status  [--debug_ip_name]



Examples:

List all devices

  ./xbsak list

Scan for Xilinx PCIe device(s) & associated drivers (if any) and relevant system information

  ./xbsak scan

Boot device 1 from PROM and retrain the PCIe link without rebooting the host

  ./xbsak boot -d 1

Change the clock frequency of region 0 in device 0 to 100 MHz

  ./xbsak clock -f 100

For device 0 which supports multiple clocks, change the clock 1 to 200MHz and clock 2 to 250MHz

  ./xbsak clock -f 200 -g 250

Download the accelerator program for device 2

  ./xbsak program -d 2 -p a.xclbin

Run DMA test on device 1 with 32 KB blocks of buffer

  ./xbsak dmatest -d 1 -b 0x2000

Read 256 bytes from DDR starting at 0x1000 into file read.out

  ./xbsak mem --read -a 0x1000 -i 256 -o read.out

  Default values for address is 0x0, size is DDR size and file is memread.out

Write 256 bytes to DDR starting at 0x1000 with byte 0xaa

  ./xbsak mem --write -a 0x1000 -i 256 -e 0xaa

  Default values for address is 0x0, size is DDR size and pattern is 0x0

List the debug IPs available on the platform

  ./xbsak status

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime/bin# ls

xbflash  xbrecover  xbsak  xclbincat  xclbinsplit

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime/bin# ./xbflash

XBFLASH -- Xilinx Board Flash Utility

Help: Try 'xbflash [-d device] -m primary_mcs [-n secondary_mcs] [-o spi|bpi]'.

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime/bin# ./xbflash -m ../../../

hw/                              test/                            .Xil/

sw/                              xbinst/                          xilinx_vcu1525_dynamic_5_1.xpfm

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime/bin# ./xbflash -m ../../

firmware/   install.sh  runtime/    test/

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime/bin# ./xbflash -m ../../firmware/

10ee-6a90-4351-000000005aace1cf.dsabin  xilinx_vcu1525_dynamic_5_1.bit          xilinx_vcu1525_dynamic_5_1.mcs          xilinx_vcu1525_dynamic_5_1.prm

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime/bin# ./xbflash -m ../../firmware/xilinx_vcu1525_dynamic_5_1.mcs

XBFLASH -- Xilinx Board Flash Utility

    primary mcs: ../../firmware/xilinx_vcu1525_dynamic_5_1.mcs

INFO: Parsing file ../../firmware/xilinx_vcu1525_dynamic_5_1.mcs

INFO: Found 587 ELA Records

Idcode byte[0] ff

Idcode byte[1] 20

Idcode byte[2] bb

Idcode byte[3] 21

Idcode byte[4] 10

.............................

XBFLASH completed succesfully. Please cold reboot machine to force load the updated flash image on the FPGA.

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime/bin# cd ..

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime# ls

bin  lib  platforms

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst/runtime# cd ..

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst# ls

firmware  install.sh  runtime  test

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst# ./install.sh

INFO: Installing firmware for FPGA devices

Removing existing xclmgmt

Removing existing xocl

Found hal zip..ignoring

/tmp/8896/0

Archive:  xocl.zip

  inflating: driver/xclng/drm/xocl/mgmtpf/mgmt-core.c

  inflating: driver/xclng/drm/xocl/mgmtpf/mgmt-cw.c

  inflating: driver/xclng/drm/xocl/mgmtpf/mgmt-utils.c

  inflating: driver/xclng/drm/xocl/mgmtpf/mgmt-ioctl.c

  inflating: driver/xclng/drm/xocl/mgmtpf/mgmt-sysfs.c

  inflating: driver/xclng/drm/xocl/mgmtpf/mgmt-core.h

  inflating: driver/xclng/drm/xocl/mgmtpf/10-xclmgmt.rules

  inflating: driver/xclng/drm/xocl/mgmtpf/Makefile

  inflating: driver/xclng/drm/xocl/xocl_drv.h

  inflating: driver/xclng/drm/xocl/xocl_subdev.c

  inflating: driver/xclng/drm/xocl/xocl_subdev.h

  inflating: driver/xclng/drm/xocl/xocl_ctx.c

  inflating: driver/xclng/drm/xocl/xocl_thread.c

  inflating: driver/xclng/drm/xocl/xocl_test.c

  inflating: driver/xclng/drm/xocl/userpf/xdma.c

  inflating: driver/xclng/drm/xocl/userpf/qdma.c

  inflating: driver/xclng/drm/xocl/userpf/common.h

  inflating: driver/xclng/drm/xocl/userpf/xocl_bo.c

  inflating: driver/xclng/drm/xocl/userpf/xocl_bo.h

  inflating: driver/xclng/drm/xocl/userpf/xocl_drm.c

  inflating: driver/xclng/drm/xocl/userpf/xocl_drm.h

  inflating: driver/xclng/drm/xocl/userpf/xocl_ioctl.c

  inflating: driver/xclng/drm/xocl/userpf/xocl_sysfs.c

  inflating: driver/xclng/drm/xocl/userpf/xocl_drv.c

  inflating: driver/xclng/drm/xocl/userpf/10-xocl.rules

  inflating: driver/xclng/drm/xocl/userpf/Makefile

  inflating: driver/xclng/drm/xocl/lib/libxdma.c

  inflating: driver/xclng/drm/xocl/lib/libxdma.h

  inflating: driver/xclng/drm/xocl/lib/cdev_sgdma.h

  inflating: driver/xclng/drm/xocl/lib/libxdma_api.h

  inflating: driver/xclng/drm/xocl/lib/Makefile.in

  inflating: driver/xclng/drm/xocl/lib/libqdma/libqdma_export.h

  inflating: driver/xclng/drm/xocl/lib/libqdma/libqdma_export.c

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_descq.c

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_descq.h

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_device.c

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_device.h

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_intr.c

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_intr.h

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_context.c

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_context.h

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_mbox.c

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_mbox.h

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_sriov.c

  inflating: driver/xclng/drm/xocl/lib/libqdma/thread.c

  inflating: driver/xclng/drm/xocl/lib/libqdma/thread.h

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_thread.c

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_thread.h

  inflating: driver/xclng/drm/xocl/lib/libqdma/version.h

  inflating: driver/xclng/drm/xocl/lib/libqdma/xdev.h

  inflating: driver/xclng/drm/xocl/lib/libqdma/xdev.c

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_regs.h

  inflating: driver/xclng/drm/xocl/lib/libqdma/qdma_regs.c

  inflating: driver/xclng/drm/xocl/subdev/feature_rom.c

  inflating: driver/xclng/drm/xocl/subdev/mm_xdma.c

  inflating: driver/xclng/drm/xocl/subdev/mm_qdma.c

  inflating: driver/xclng/drm/xocl/subdev/mb_scheduler.c

  inflating: driver/xclng/drm/xocl/subdev/xvc.c

  inflating: driver/xclng/drm/xocl/subdev/sysmon.c

  inflating: driver/xclng/drm/xocl/subdev/firewall.c

  inflating: driver/xclng/drm/xocl/subdev/microblaze.c

  inflating: driver/xclng/drm/xocl/subdev/xiic.c

  inflating: driver/xclng/drm/xocl/subdev/mailbox.c

  inflating: driver/xclng/drm/xocl/subdev/icap.c

  inflating: driver/xclng/drm/xocl/subdev/str_qdma.c

  inflating: driver/xclng/drm/.dir-locals.el

  inflating: driver/xclng/include/xocl_ioctl.h

  inflating: driver/xclng/include/mgmt-reg.h

  inflating: driver/xclng/include/mgmt-ioctl.h

  inflating: driver/xclng/include/qdma_ioctl.h

  inflating: driver/xclng/include/drm/drm.h

  inflating: driver/xclng/include/drm/drm_mode.h

  inflating: driver/include/ert.h

  inflating: driver/include/xclfeatures.h

  inflating: driver/include/xclbin.h

  inflating: driver/include/xclerr.h

INFO: building kernel mode driver

echo /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf

/tmp/8896/0/driver/xclng/drm/xocl/mgmtpf

make -C /lib/modules/4.13.0-36-generic/build M=/tmp/8896/0/driver/xclng/drm/xocl/mgmtpf modules

make[1]: Entering directory '/usr/src/linux-headers-4.13.0-36-generic'

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../xocl_subdev.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../xocl_ctx.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../xocl_thread.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../subdev/sysmon.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../subdev/feature_rom.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../subdev/microblaze.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../subdev/firewall.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../subdev/xvc.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../subdev/xiic.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../subdev/mailbox.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../subdev/icap.o

/tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../subdev/icap.c: In function ‘icap_unlock_bitstream’:

/tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/../subdev/icap.c:1746:6: warning: assignment discards ‘const’ qualifier from pointer target type [-Wdiscarded-qualifiers]

   id = &uuid_null;

      ^

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/mgmt-core.o

/tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/mgmt-core.c: In function ‘xclmgmt_setup_msix’:

/tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/mgmt-core.c:541:17: warning: unused variable ‘i’ [-Wunused-variable]

  int total, rv, i;

                 ^

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/mgmt-cw.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/mgmt-utils.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/mgmt-ioctl.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/mgmt-sysfs.o

  LD [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/xclmgmt.o

  Building modules, stage 2.

  MODPOST 1 modules

  CC      /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/xclmgmt.mod.o

  LD [M]  /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/xclmgmt.ko

make[1]: Leaving directory '/usr/src/linux-headers-4.13.0-36-generic'

INFO: Installing new kernel mode driver xclmgmt version 2018.2.2

echo /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf

/tmp/8896/0/driver/xclng/drm/xocl/mgmtpf

make -C /lib/modules/4.13.0-36-generic/build M=/tmp/8896/0/driver/xclng/drm/xocl/mgmtpf modules

make[1]: Entering directory '/usr/src/linux-headers-4.13.0-36-generic'

  Building modules, stage 2.

  MODPOST 1 modules

make[1]: Leaving directory '/usr/src/linux-headers-4.13.0-36-generic'

make -C /lib/modules/4.13.0-36-generic/build M=/tmp/8896/0/driver/xclng/drm/xocl/mgmtpf modules_install

make[1]: Entering directory '/usr/src/linux-headers-4.13.0-36-generic'

  INSTALL /tmp/8896/0/driver/xclng/drm/xocl/mgmtpf/xclmgmt.ko

At main.c:160:

- SSL error:02001002:system library:fopen:No such file or directory: bss_file.c:175

- SSL error:2006D080:BIO routines:BIO_new_file:no such file: bss_file.c:178

sign-file: certs/signing_key.pem: No such file or directory

  DEPMOD  4.13.0-36-generic

make[1]: Leaving directory '/usr/src/linux-headers-4.13.0-36-generic'

depmod -a

install -m 644 10-xclmgmt.rules /etc/udev/rules.d

rmmod -s xclmgmt || true

modprobe xclmgmt

INFO: building kernel mode driver

echo /tmp/8896/0/driver/xclng/drm/xocl/userpf

/tmp/8896/0/driver/xclng/drm/xocl/userpf

make -C /lib/modules/4.13.0-36-generic/build M=/tmp/8896/0/driver/xclng/drm/xocl/userpf modules

make[1]: Entering directory '/usr/src/linux-headers-4.13.0-36-generic'

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../xocl_subdev.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../xocl_ctx.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../xocl_thread.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../subdev/mm_xdma.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../subdev/feature_rom.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../subdev/mm_qdma.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../subdev/mb_scheduler.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../subdev/mailbox.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../subdev/xvc.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../subdev/icap.o

/tmp/8896/0/driver/xclng/drm/xocl/userpf/../subdev/icap.c: In function ‘icap_unlock_bitstream’:

/tmp/8896/0/driver/xclng/drm/xocl/userpf/../subdev/icap.c:1746:6: warning: assignment discards ‘const’ qualifier from pointer target type [-Wdiscarded-qualifiers]

   id = &uuid_null;

      ^

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../subdev/str_qdma.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libxdma.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libqdma/qdma_descq.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libqdma/qdma_device.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libqdma/qdma_intr.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libqdma/qdma_regs.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libqdma/qdma_thread.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libqdma/qdma_context.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libqdma/qdma_mbox.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libqdma/qdma_sriov.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libqdma/thread.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libqdma/xdev.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/../lib/libqdma/libqdma_export.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/xdma.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/qdma.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/xocl_drv.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/xocl_bo.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/xocl_drm.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/xocl_ioctl.o

  CC [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/xocl_sysfs.o

  LD [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/xocl.o

  Building modules, stage 2.

  MODPOST 1 modules

  CC      /tmp/8896/0/driver/xclng/drm/xocl/userpf/xocl.mod.o

  LD [M]  /tmp/8896/0/driver/xclng/drm/xocl/userpf/xocl.ko

make[1]: Leaving directory '/usr/src/linux-headers-4.13.0-36-generic'

INFO: Installing new kernel mode driver xocl version 2018.2.7

echo /tmp/8896/0/driver/xclng/drm/xocl/userpf

/tmp/8896/0/driver/xclng/drm/xocl/userpf

make -C /lib/modules/4.13.0-36-generic/build M=/tmp/8896/0/driver/xclng/drm/xocl/userpf modules

make[1]: Entering directory '/usr/src/linux-headers-4.13.0-36-generic'

  Building modules, stage 2.

  MODPOST 1 modules

make[1]: Leaving directory '/usr/src/linux-headers-4.13.0-36-generic'

make -C /lib/modules/4.13.0-36-generic/build M=/tmp/8896/0/driver/xclng/drm/xocl/userpf modules_install

make[1]: Entering directory '/usr/src/linux-headers-4.13.0-36-generic'

  INSTALL /tmp/8896/0/driver/xclng/drm/xocl/userpf/xocl.ko

At main.c:160:

- SSL error:02001002:system library:fopen:No such file or directory: bss_file.c:175

- SSL error:2006D080:BIO routines:BIO_new_file:no such file: bss_file.c:178

sign-file: certs/signing_key.pem: No such file or directory

  DEPMOD  4.13.0-36-generic

make[1]: Leaving directory '/usr/src/linux-headers-4.13.0-36-generic'

depmod -a

install -m 644 10-xocl.rules /etc/udev/rules.d

rmmod -s xocl || true

rmmod -s xdma || true

modprobe xocl

Generating SDAccel runtime environment setup script, setup.sh for bash

Generating SDAccel runtime environment setup script, setup.csh for (t)csh

Runtime drivers are installed/upgraded successfully

root@msl-dpe-d3:/opt/Xilinx/xilinx_vcu1525_dynamic_5_1/xbinst#

