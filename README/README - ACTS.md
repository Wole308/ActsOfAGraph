xilinx19sdx
XRT
./evaluate.sh

vivado_hls -f script.tcl 

/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 

make all TARGET=sw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
make all TARGET=hw_emu DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
XCL_EMULATION_MODE=sw_emu ./host xclbin/krnl_vaddmul.sw_emu.xilinx_u280_xdma_201910_1.xclbin

nohup make all DEVICE=/opt/xilinx/platforms/xilinx_u280_xdma_201910_1/xilinx_u280_xdma_201910_1.xpfm 
./host xclbin/krnl_vaddmul.sw_emu.xilinx_u280_xdma_201910_1.xclbin

/tools/Xilinx/SDx/2019.1/bin/xcpp -I../../..//libs/xcl2 -I/opt/xilinx/xrt/include/ -I/tools/Xilinx/Vivado/2019.1/include/ -Wall -O0 -g -std=c++14 -fmessage-length=0 ../../..//libs/xcl2/xcl2.cpp src/host.cpp  -o 'host'  -L/opt/xilinx/xrt/lib/ -lOpenCL -lpthread -lrt -lstdc++ 

source /tools/Xilinx/Vivado/2020.1/settings64.sh
source /tools/Xilinx/Vitis/2020.1/settings64.sh
tools/Xilinx/Vitis/2020.1/samples/vadd
/tools/Xilinx/Vitis/2020.1/bin/v++ -t sw_emu --platform xilinx_u280_xdma_201920_3 -c -k krnl_vadd -I'./src' -o'vadd.sw_emu.xo' ./src/krnl_vadd.cpp
switching from SDx to Vitis: https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/buildingdevicebinary.html#zab1528399121187

vi ~/.bashrc

Next Paper: Disk, Insider

vitis tweaking flow URL (timing versus resources constraint), avoiding congestion: 
https://www.xilinx.com/html_docs/xilinx2021_1/vitis_doc/buildingdevicebinary.html#zab1528399121187
https://forums.xilinx.com/t5/Implementation/run-congestion-command/td-p/910438
--xp (sdaccel command line utility webpage)
https://www.xilinx.com/html_docs/xilinx2019_1/sdaccel_doc/wrj1504034328013.html#uno1528577884586
https://www.xilinx.com/support/answers/66314.html
https://forums.xilinx.com/t5/Implementation/implementation-error-due-to-routing-congestion/td-p/474578
https://forums.xilinx.com/t5/Alveo-Accelerator-Cards/How-to-use-vivado-prop/td-p/1089904
https://forums.xilinx.com/t5/Implementation/What-does-Spread-Logic-mean/td-p/673141
https://www.xilinx.com/developer/articles/using-configuration-files-to-control-vitis-compilation.html
https://forums.xilinx.com/t5/Vivado-TCL-Community/how-to-get-the-list-of-all-possible-strategies-with-tcl-commands/td-p/913712 *
https://forums.xilinx.com/t5/Vitis-Acceleration-SDAccel-SDSoC/How-to-set-route-directives-in-Vitis-2019-2/td-p/1221096 *
https://forums.xilinx.com/t5/Vitis-Acceleration-SDAccel-SDSoC/Vivado-implementation-strategies-in-Vitis/td-p/1046022 *


XOCC := $(XILINX_VITIS)/bin/v++
CLFLAGS += -t $(TARGET) --platform $(DEVICE) --save-temps
TARGETS := hw
TARGET := $(TARGETS)
DEVICEPATH=/opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm
$(XOCC) $(CLFLAGS) --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelproc) -l $(LDCLFLAGS_PROC) --nk topkernelproc:4 -o'$@' $(+)

$(XILINX_VITIS)/bin/v++ -t hw --platform $(DEVICE) --save-temps --kernel_frequency $(SYNFREQUENCY) --temp_dir $(BUILD_DIR_topkernelproc) -l $(LDCLFLAGS_PROC) --nk topkernelproc:4 -o'$@' $(+)

/tools/Xilinx/Vitis/2020.1/bin/v++ -t hw --platform /opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm --save-temps --kernel_frequency 300 --temp_dir ./_x.hw.xilinx_u280_xdma_201920_3/topkernelproc -c -k topkernelproc -I'acts/acts' -I'acts/actsutility/' -o'xclbin/topkernelproc.hw.xilinx_u280_xdma_201920_3.xo' acts/acts/actsproc.cpp acts/actsutility/actsutility.cpp			
/tools/Xilinx/Vitis/2020.1/bin/v++ -t hw --platform /opt/xilinx/platforms/xilinx_u280_xdma_201920_3/xilinx_u280_xdma_201920_3.xpfm --save-temps --kernel_frequency 300 --temp_dir ./_x.hw.xilinx_u280_xdma_201920_3/topkernelproc -l --sp topkernelproc_1.m_axi_gmem0:HBM[0]  --sp topkernelproc_2.m_axi_gmem0:HBM[1]  --sp topkernelproc_3.m_axi_gmem0:HBM[2]  --sp topkernelproc_4.m_axi_gmem0:HBM[3] --nk topkernelproc:4 -o'xclbin/topkernelproc.hw.xilinx_u280_xdma_201920_3.xclbin' xclbin/topkernelproc.hw.xilinx_u280_xdma_201920_3.xo		
v++ -t sw_emu --platform xilinx_u200_xdma_201830_2 -c -k vadd -I'./src' -o'vadd.sw_emu.xo' ./src/vadd.cpp

source /tools/Xilinx/SDx/2019.1/settings64.sh
export XILINX_VIVADO=/tools/Xilinx/Vivado/2019.1
export XILINX_VIVADO=/tools/Xilinx/Vivado/2019.2
export XILINX_VIVADO=/tools/Xilinx/Vivado/2020.1
printenv

##################
LC_ALL=C
LD_LIBRARY_PATH=/tools/Xilinx/SDx/2019.1/lib/lnx64.o/Ubuntu/18:/tools/Xilinx/SDx/2019.1/lib/lnx64.o/Ubuntu:/tools/Xilinx/SDx/2019.1/lib/lnx64.o:/tools/Xilinx/SDx/2019.1/runtime/lib/x86_64:/opt/xilinx/xrt/lib::/usr/local/cuda/lib64
LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:
XILINX_VITIS=/tools/Xilinx/Vitis/2020.1
SSH_CONNECTION=128.143.0.10 59548 128.143.69.191 22
LESSCLOSE=/usr/bin/lesspipe %s %s
LANG=en_US.UTF-8
OLDPWD=/home/oj2zf
XILINX_XRT=/opt/xilinx/xrt
XILINX_VIVADO=/tools/Xilinx/Vivado/2020.1
XILINX_SDX=/tools/Xilinx/SDx/2019.1
XDG_SESSION_ID=999
USER=oj2zf
XILINXD_LICENSE_FILE=2100@license2.ece.virginia.edu
PWD=/home/oj2zf/Documents/SDAccel_Examples-master/SDAccel_Examples-master/getting_started/host/mult_compute_units
HOME=/home/oj2zf
SSH_CLIENT=128.143.0.10 59548 22
XDG_DATA_DIRS=/usr/local/share:/usr/share:/var/lib/snapd/desktop
SSH_TTY=/dev/pts/10
MAIL=/var/mail/oj2zf
TERM=xterm
SHELL=/bin/bash
SHLVL=1
PYTHONPATH=/opt/xilinx/xrt/python:
LOGNAME=oj2zf
DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1007/bus
XDG_RUNTIME_DIR=/run/user/1007
PATH=/tools/Xilinx/SDK/2019.1/bin:/tools/Xilinx/SDK/2019.1/gnu/microblaze/lin/bin:/tools/Xilinx/SDK/2019.1/gnu/arm/lin/bin:/tools/Xilinx/SDK/2019.1/gnu/microblaze/linux_toolchain/lin64_le/bin:/tools/Xilinx/SDK/2019.1/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin:/tools/Xilinx/SDK/2019.1/gnu/aarch32/lin/gcc-arm-none-eabi/bin:/tools/Xilinx/SDK/2019.1/gnu/aarch64/lin/aarch64-linux/bin:/tools/Xilinx/SDK/2019.1/gnu/aarch64/lin/aarch64-none/bin:/tools/Xilinx/SDK/2019.1/gnu/armr5/lin/gcc-arm-none-eabi/bin:/tools/Xilinx/SDK/2019.1/tps/lnx64/cmake-3.3.2/bin:/tools/Xilinx/SDx/2019.1/bin:/tools/Xilinx/Vivado/2019.1/bin:/home/oj2zf/.local/bin:/opt/xilinx/xrt/bin:/tools/Xilinx/Vitis/2020.1/bin:/tools/Xilinx/Vitis/2020.1/gnu/microblaze/lin/bin:/tools/Xilinx/Vitis/2020.1/gnu/arm/lin/bin:/tools/Xilinx/Vitis/2020.1/gnu/microblaze/linux_toolchain/lin64_le/bin:/tools/Xilinx/Vitis/2020.1/gnu/aarch32/lin/gcc-arm-linux-gnueabi/bin:/tools/Xilinx/Vitis/2020.1/gnu/aarch32/lin/gcc-arm-none-eabi/bin:/tools/Xilinx/Vitis/2020.1/gnu/aarch64/lin/aarch64-linux/bin:/tools/Xilinx/Vitis/2020.1/gnu/aarch64/lin/aarch64-none/bin:/tools/Xilinx/Vitis/2020.1/gnu/armr5/lin/gcc-arm-none-eabi/bin:/tools/Xilinx/Vitis/2020.1/tps/lnx64/cmake-3.3.2/bin:/tools/Xilinx/Vitis/2020.1/cardano/bin:/tools/Xilinx/Vivado/2020.1/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/usr/local/cuda/bin:/usr/local/go/bin
LESSOPEN=| /usr/bin/lesspipe %s
_=/usr/bin/printenv

https://support.xilinx.com/s/article/69355?language=en_US
export LIBRARY_PATH=/usr/lib/x86_64-linux-gnu  // sh
setenv LIBRARY_PATH /usr/lib/x86_64-linux-gnu  // csh
##################

4W: 275MHz
6W: 156MHz
8W: 210MHz
10W: 162MHz
12W: