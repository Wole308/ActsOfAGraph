Using username "wole.j".

wole.j@105.128.45.129's password:

Last login: Tue Aug 20 11:48:18 2019 from 105.128.234.22

Not sourcing xilinx

[12:33:40]wole.j@jh-wole:~$ssh wole.j@msl-dpe-d3

wole.j@msl-dpe-d3's password:

Last login: Tue Aug 20 12:06:14 2019 from 10.1.51.129

XILINX_XRT      : /opt/XRT/build/Release/opt/xilinx/xrt

PATH            : /opt/XRT/build/Release/opt/xilinx/xrt/bin:/opt/Xilinx/SDK/2018                                                                                                 .2/bin:/opt/Xilinx/SDK/2018.2/gnu/microblaze/lin/bin:/opt/Xilinx/SDK/2018.2/gnu/                                                                                                 arm/lin/bin:/opt/Xilinx/SDK/2018.2/gnu/microblaze/linux_toolchain/lin64_le/bin:/                                                                                                 opt/Xilinx/SDK/2018.2/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin:/opt/Xilinx/SDK/                                                                                                 2018.2/gnu/aarch32/lin/gcc-arm-none-eabi/bin:/opt/Xilinx/SDK/2018.2/gnu/aarch64/                                                                                                 lin/aarch64-linux/bin:/opt/Xilinx/SDK/2018.2/gnu/aarch64/lin/aarch64-none/bin:/o                                                                                                 pt/Xilinx/SDK/2018.2/gnu/armr5/lin/gcc-arm-none-eabi/bin:/opt/Xilinx/SDK/2018.2/                                                                                                 tps/lnx64/cmake-3.3.2/bin:/opt/Xilinx/Vivado/2018.2/bin:/opt/Xilinx/DocNav:/opt/                                                                                                 Xilinx/SDx/2018.2/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/b                                                                                                 in:/usr/games:/usr/local/games:/snap/bin

LD_LIBRARY_PATH : /opt/XRT/build/Release/opt/xilinx/xrt/lib:/opt/Xilinx/SDx/2018                                                                                                 .2/lib/lnx64.o/Ubuntu:/opt/Xilinx/SDx/2018.2/lib/lnx64.o:/opt/Xilinx/SDx/2018.2/                                                                                                 runtime/lib/x86_64

PYTHON_PATH     :

success on sourcing

[12:51:36]wole.j@msl-dpe-d3:~$cd /home/wole.j/Documents/Summer_Projects/acts

[12:51:43]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$clear

[12:51:44]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$make cleanall

*** started! ***

cleanall successful

rm -rf vadd.xo xocc_kernel* _x pfm_top_* kernel.xo kernel.xclbin host.exe

[12:51:47]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$make hwemu

*** started! ***

cleanall successful

rm -rf vadd.xo xocc_kernel* _x pfm_top_* kernel.xo kernel.xclbin host.exe

/opt/Xilinx/SDx/2018.2/bin/xocc -c --target hw_emu --platform xilinx_vcu1525_dynamic_5_1 src/kernel.cpp src/array/array_read.cpp src/array/array_print.cpp src/array/array_init.cpp src/array/array_write.cpp src/array/array.cpp src/cal/cal.cpp src/cal/cal_read.cpp src/cal/cal_write.cpp src/cal/cal_utility.cpp src/cal/cal_init.cpp src/cal/cal_print.cpp src/cal/cal_reset.cpp src/partitioner/partitioner.cpp src/functions/partitionarray.cpp src/functions/partitionLL.cpp src/functions/naive_partition_array.cpp src/functions/partitionbuffer.cpp src/functions/computeandpartitionbuffer.cpp src/functions/partitiongraph.cpp src/functions/partitionCALtoT.cpp src/functions/storecalstossd.cpp src/functions/applyLL.cpp src/functions/partitionarraytoT.cpp src/functions/partitionCAL_ATOB.cpp src/functions/partitionCAL_BTOA.cpp src/functions/apply_partitions.cpp src/functions/apply_vertexupdates.cpp src/functions/process_graph.cpp                                         src/memoryallocator/memoryallocator.cpp src/calstatemanager/calstatemanager.cpp src/debugger/debugger.cpp src/generalutility/generalutility.cpp src/printer/printer.cpp src/initializer/initializer.cpp src/initializer/initialize_baseaddrs.cpp src/initializer/initialize_brams.cpp src/initializer/initialize_calstategens.cpp src/initializer/initialize_calstatetrackers.cpp src/initializer/initialize_destCALs.cpp src/initializer/initialize_memoryallocs.cpp src/initializer/initialize_sourcearrays.cpp src/initializer/initialize_sourcegraphs.cpp src/initializer/initialize_workloadinfo.cpp src/initializer/initialize_srcrange.cpp        --kernel kernel -o kernel.xo



****** xocc v2018.2 (64-bit)

  **** SW Build 2258646 on Thu Jun 14 20:02:38 MDT 2018

    ** Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.



Attempting to get a license: ap_opencl

Feature available: ap_opencl

INFO: [XOCC 60-585] Compiling for hardware emulation target

Running SDx Rule Check Server on port:34845

ERROR: [XOCC 60-705] No platform was found that matches 'xilinx_vcu1525_dynamic_5_1'. Make sure that the platform is specified correctly and that valid license is installed. The valid platforms supported by the license found are:

xilinx_kcu1500_dynamic_5_0

xilinx_vcu1525_dynamic_5_0

zc702

zc706

zcu102

zcu104

zcu106

zed



ERROR: [XOCC 60-587] Failed to add a platform: specified platform xilinx_vcu1525_dynamic_5_1 is not found

ERROR: [XOCC 60-600] Kernel compile setup failed to complete

ERROR: [XOCC 60-592] Failed to finish compilation

Makefile:92: recipe for target 'build_xo_hwemu' failed

make: *** [build_xo_hwemu] Error 1

[12:53:22]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$vim ~./bashrc

[12:55:04]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$vim ./~bashrc

[12:55:16]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$vim .~/bashrc

[12:55:26]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$vim ~/.bashrc

[12:56:22]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$make hwemu

*** started! ***

cleanall successful

rm -rf vadd.xo xocc_kernel* _x pfm_top_* kernel.xo kernel.xclbin host.exe

/opt/Xilinx/SDx/2018.2/bin/xocc -c --target hw_emu --platform xilinx_vcu1525_dynamic_5_1 src/kernel.cpp src/array/array_read.cpp src/array/array_print.cpp src/array/array_init.cpp src/array/array_write.cpp src/array/array.cpp src/cal/cal.cpp src/cal/cal_read.cpp src/cal/cal_write.cpp src/cal/cal_utility.cpp src/cal/cal_init.cpp src/cal/cal_print.cpp src/cal/cal_reset.cpp src/partitioner/partitioner.cpp src/functions/partitionarray.cpp src/functions/partitionLL.cpp src/functions/naive_partition_array.cpp src/functions/partitionbuffer.cpp src/functions/computeandpartitionbuffer.cpp src/functions/partitiongraph.cpp src/functions/partitionCALtoT.cpp src/functions/storecalstossd.cpp src/functions/applyLL.cpp src/functions/partitionarraytoT.cpp src/functions/partitionCAL_ATOB.cpp src/functions/partitionCAL_BTOA.cpp src/functions/apply_partitions.cpp src/functions/apply_vertexupdates.cpp src/functions/process_graph.cpp                                         src/memoryallocator/memoryallocator.cpp src/calstatemanager/calstatemanager.cpp src/debugger/debugger.cpp src/generalutility/generalutility.cpp src/printer/printer.cpp src/initializer/initializer.cpp src/initializer/initialize_baseaddrs.cpp src/initializer/initialize_brams.cpp src/initializer/initialize_calstategens.cpp src/initializer/initialize_calstatetrackers.cpp src/initializer/initialize_destCALs.cpp src/initializer/initialize_memoryallocs.cpp src/initializer/initialize_sourcearrays.cpp src/initializer/initialize_sourcegraphs.cpp src/initializer/initialize_workloadinfo.cpp src/initializer/initialize_srcrange.cpp        --kernel kernel -o kernel.xo



****** xocc v2018.2 (64-bit)

  **** SW Build 2258646 on Thu Jun 14 20:02:38 MDT 2018

    ** Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.



Attempting to get a license: ap_opencl

Feature available: ap_opencl

INFO: [XOCC 60-585] Compiling for hardware emulation target

Running SDx Rule Check Server on port:35935

ERROR: [XOCC 60-705] No platform was found that matches 'xilinx_vcu1525_dynamic_5_1'. Make sure that the platform is specified correctly and that valid license is installed. The valid platforms supported by the license found are:

xilinx_kcu1500_dynamic_5_0

xilinx_vcu1525_dynamic_5_0

zc702

zc706

zcu102

zcu104

zcu106

zed



ERROR: [XOCC 60-587] Failed to add a platform: specified platform xilinx_vcu1525_dynamic_5_1 is not found

ERROR: [XOCC 60-600] Kernel compile setup failed to complete

ERROR: [XOCC 60-592] Failed to finish compilation

Makefile:92: recipe for target 'build_xo_hwemu' failed

make: *** [build_xo_hwemu] Error 1

[12:56:31]wole.j@msl-dpe-d3:~/Documents/Summer_Projects/acts$cd /opt/Xilinx/

[12:57:18]wole.j@msl-dpe-d3:/opt/Xilinx$ls

DocNav  SDK  SDx  Vivado  xic  xilinx_vcu1525_dynamic_5_1

[12:57:18]wole.j@msl-dpe-d3:/opt/Xilinx$sudo mv ./SDx/2018.2/platforms/xilinx_vcu1525_dynamic_5_0/ .

[sudo] password for wole.j:

[12:57:43]wole.j@msl-dpe-d3:/opt/Xilinx$ls

DocNav  SDK  SDx  Vivado  xic  xilinx_vcu1525_dynamic_5_0  xilinx_vcu1525_dynamic_5_1

[12:57:46]wole.j@msl-dpe-d3:/opt/Xilinx$sudo cp -a ./xilinx_vcu1525_dynamic_5_1/ ./SDx/2018.2/platforms/

[12:58:07]wole.j@msl-dpe-d3:/opt/Xilinx$xocc

ERROR: No Xilinx Device (--xdevice) or Platform (--platform/-f) option specified.

Allowed options:

  --clkid arg                  Select index available from SoC platform. Each

                               index has a different default clock frequency

  -c [ --compile ]             Run a compile mode, generate a XO file

  --custom_script arg          <kernel_name:HLS_script> or

                               <data_file_for_Vivado_scripts> Import HLS

                               script(s) or a data file containing paths to

                               customized Vivado run scripts

  -g [ --debug ]               Generate code for debugging

  -D [ --define ] arg          <name=definition> Predefine name as a macro with

                               definition. This option is passed to the openCL

                               preprocessor.

  --dk arg                     <[protocol|chipscope|list_ports]:<compute_unit_n

                               ame>:<interface_name>> Enable debug IP core

                               insertion

  --export_script              Export HLS script(s) or a data file containing

                               paths to generated Vivado run scripts

  -h [ --help ]                Print usage message

  -I [ --include ] arg         Add the directory to the list of directories to

                               be searched for header files. This option is

                               passed to the openCL preprocessor

  --ini_file arg               Specify a file containing XP switch values in

                               xocc.ini format

  --input_files arg            Specify input files

  --interactive arg            [impl] Launch Vivado with underlying project

                               loaded

  -j [ --jobs ] arg            Set the number of jobs

  -k [ --kernel ] arg          Specify a kernel to compile or link

  --kernel_frequency arg       Set a user defined clock frequency in MHz for

                               kernel, overriding a default value from DSA

  -l [ --link ]                Run a link mode, generate a XCLBIN file

  --log_dir arg                Specify a directory to copy internally generated

                               log files to

  --lsf arg                    Specify a bsub command to submit a job to LSF

  --max_memory_ports arg       [all|kernel_name] Set the maximum memory ports

                               for all or given kernel name. Valid for OpenCL

                               kernels

  --memory_port_data_width arg [all|kernel_name:number] Set the memory port

                               data width to the number for all or given kernel

                               name. Valid for OpenCL kernels

  --message-rules arg          Optional input file containing rules controlling

                               xocc's messaging

  --nk arg                     <kernel_name:number(:compute_unit_name1.compute_

                               unit_name2...)> Set the number of compute units

                               per kernel name. Default is one compute unit per

                               kernel. Optional compute unit names can be

                               specified using a period delimiter

  --no_ip_cache                Turn off IP cache for Vivado Synthesis

  -O [ --optimize ] arg        Optimize level: [0-3,s,quick]; default is 0

  -o [ --output ] arg          Set output file name. Default: a.xclbin (link,

                               build), a.xo (compile)

  -f [ --platform ] arg        Specify a Platform

  --profile_kernel arg         <data:[kernel_name|all]:[compute_unit_name|all]:

                               [interface_name|all](:[counters|all])>

                               <[stall|exec]:[kernel_name|all]:[compute_unit_na

                               me|all](:[counters|all])> Enable profiling DDR

                               memory traffic for kernel and host

  --remote_ip_cache arg        Specify remote IP cache directory for Vivado

                               Synthesis

  -R [ --report_level ] arg    Report level: [0-2,estimate]; default is 0

  --report_dir arg             Specify a directory to copy report files to

  --reuse_synth arg            <synthesized DCP> Import an external synthesized

                               DCP and only run the Vivado implementation and

                               XCLBIN packaging

  --reuse_impl arg             <implemented DCP> Import an external implemented

                               DCP and only run the XCLBIN packaging

  -s [ --save-temps ]          Do not delete intermediate files

  --sp arg                     <compute_unit_name>.<port_name>:<system_port>

                               Specify system port mapping for compute unit

  --sys_config arg             Specify a system configuration setting for SoC

                               platforms. Valid value is ocl

  -t [ --target ] arg          Specify a compile target: [sw_emu|hw_emu|hw];

                               default is hw

  --temp_dir arg               Specify a directory to use for temporary files

                               and directories

  --user_ip_repo_paths arg     Specify existing user IP repository. This value

                               will be prepended to ip_repo_paths

  -v [ --version ]             Print version information

  --xp arg                     Specify advanced parameters or properties for

                               kernel compilation



[12:58:11]wole.j@msl-dpe-d3:/opt/Xilinx$xocc --list

xilinx_kcu1500_dynamic_5_0

xilinx_vcu1525_dynamic_5_1

zc702

zc706

zcu102

zcu104

zcu106

zed

[12:58:16]wole.j@msl-dpe-d3:/opt/Xilinx$xocc

ERROR: No Xilinx Device (--xdevice) or Platform (--platform/-f) option specified.

Allowed options:

  --clkid arg                  Select index available from SoC platform. Each

                               index has a different default clock frequency

  -c [ --compile ]             Run a compile mode, generate a XO file

  --custom_script arg          <kernel_name:HLS_script> or

                               <data_file_for_Vivado_scripts> Import HLS

                               script(s) or a data file containing paths to

                               customized Vivado run scripts

  -g [ --debug ]               Generate code for debugging

  -D [ --define ] arg          <name=definition> Predefine name as a macro with

                               definition. This option is passed to the openCL

                               preprocessor.

  --dk arg                     <[protocol|chipscope|list_ports]:<compute_unit_n

                               ame>:<interface_name>> Enable debug IP core

                               insertion

  --export_script              Export HLS script(s) or a data file containing

                               paths to generated Vivado run scripts

  -h [ --help ]                Print usage message

  -I [ --include ] arg         Add the directory to the list of directories to

                               be searched for header files. This option is

                               passed to the openCL preprocessor

  --ini_file arg               Specify a file containing XP switch values in

                               xocc.ini format

  --input_files arg            Specify input files

  --interactive arg            [impl] Launch Vivado with underlying project

                               loaded

  -j [ --jobs ] arg            Set the number of jobs

  -k [ --kernel ] arg          Specify a kernel to compile or link

  --kernel_frequency arg       Set a user defined clock frequency in MHz for

                               kernel, overriding a default value from DSA

  -l [ --link ]                Run a link mode, generate a XCLBIN file

  --log_dir arg                Specify a directory to copy internally generated

                               log files to

  --lsf arg                    Specify a bsub command to submit a job to LSF

  --max_memory_ports arg       [all|kernel_name] Set the maximum memory ports

                               for all or given kernel name. Valid for OpenCL

                               kernels

  --memory_port_data_width arg [all|kernel_name:number] Set the memory port

                               data width to the number for all or given kernel

                               name. Valid for OpenCL kernels

  --message-rules arg          Optional input file containing rules controlling

                               xocc's messaging

  --nk arg                     <kernel_name:number(:compute_unit_name1.compute_

                               unit_name2...)> Set the number of compute units

                               per kernel name. Default is one compute unit per

                               kernel. Optional compute unit names can be

                               specified using a period delimiter

  --no_ip_cache                Turn off IP cache for Vivado Synthesis

  -O [ --optimize ] arg        Optimize level: [0-3,s,quick]; default is 0

  -o [ --output ] arg          Set output file name. Default: a.xclbin (link,

                               build), a.xo (compile)

  -f [ --platform ] arg        Specify a Platform

  --profile_kernel arg         <data:[kernel_name|all]:[compute_unit_name|all]:

                               [interface_name|all](:[counters|all])>

                               <[stall|exec]:[kernel_name|all]:[compute_unit_na

                               me|all](:[counters|all])> Enable profiling DDR

                               memory traffic for kernel and host

  --remote_ip_cache arg        Specify remote IP cache directory for Vivado

                               Synthesis

  -R [ --report_level ] arg    Report level: [0-2,estimate]; default is 0

  --report_dir arg             Specify a directory to copy report files to

  --reuse_synth arg            <synthesized DCP> Import an external synthesized

                               DCP and only run the Vivado implementation and

                               XCLBIN packaging

  --reuse_impl arg             <implemented DCP> Import an external implemented

                               DCP and only run the XCLBIN packaging

  -s [ --save-temps ]          Do not delete intermediate files

  --sp arg                     <compute_unit_name>.<port_name>:<system_port>

                               Specify system port mapping for compute unit

  --sys_config arg             Specify a system configuration setting for SoC

                               platforms. Valid value is ocl

  -t [ --target ] arg          Specify a compile target: [sw_emu|hw_emu|hw];

                               default is hw

  --temp_dir arg               Specify a directory to use for temporary files

                               and directories

  --user_ip_repo_paths arg     Specify existing user IP repository. This value

                               will be prepended to ip_repo_paths

  -v [ --version ]             Print version information

  --xp arg                     Specify advanced parameters or properties for

                               kernel compilation



[12:58:31]wole.j@msl-dpe-d3:/opt/Xilinx$ls /opt/Xilinx/SDx/2018.2/platforms/

xilinx_kcu1500_dynamic_5_0  xilinx_vcu1525_dynamic_5_1  zc702  zc706  zcu102  zcu104  zcu106  zed

[12:58:44]wole.j@msl-dpe-d3:/opt/Xilinx$xocc --list

xilinx_kcu1500_dynamic_5_0

xilinx_vcu1525_dynamic_5_1

zc702

zc706

zcu102

zcu104

zcu106

zed

[12:59:16]wole.j@msl-dpe-d3:/opt/Xilinx$

