Hi Wole,

Please follow the steps in this link to mount a filesystem on the device:

Run lsblk to know which nvme drive is there. You’ll see something like /dev/nvme1n1

Make a directory: 

$$ mkdir /mnt/data


Make a filesystem for that drive:

$$ mkfs -t ext4 /dev/nvme1n1

Mount the filesystem:

$$mount -t ext4 /dev/nvme1n1 /mnt/data

Add this line to the end of /etc/fstab file:

/dev/nvme1n1   /mnt/data           ext4       defaults                1             2



Please try this and let me know if it works.

Thanks,
Nima


[15:51:14]wole.j@msl-dpe-d3:~/Documents/ACTGraph$lsblk
NAME    MAJ:MIN RM   SIZE RO TYPE MOUNTPOINT
sda       8:0    0 223.5G  0 disk
├─sda2    8:2    0   3.7G  0 part [SWAP]
└─sda1    8:1    0 219.8G  0 part /
nvme0n1 259:0    0 447.1G  0 disk /nvme
[15:51:17]wole.j@msl-dpe-d3:~/Documents/ACTGraph$nvme list
/dev/nvme0n1: Permission denied
[15:51:37]wole.j@msl-dpe-d3:~/Documents/ACTGraph$sudo nvme list
[sudo] password for wole.j:
Node             SN                   Model                                    Namespace Usage                      Format           FW Rev
---------------- -------------------- ---------------------------------------- --------- -------------------------- ---------------- --------
/dev/nvme0n1     S23YNAAH200786       SAMSUNG MZQLV480HCGR-00003               1           8.72  GB / 480.10  GB    512   B +  0 B   BXV5401Q
[15:51:47]wole.j@msl-dpe-d3:~/Documents/ACTGraph$ls /dev/
autofs           ecryptfs   ipmi0     loop-control        port    sda2      tty10  tty22  tty34  tty46  tty58      ttyS10  ttyS22  ttyS6    vboxdrvu    vcsa3
block            fb0        kmsg      mapper              ppp     serial    tty11  tty23  tty35  tty47  tty59      ttyS11  ttyS23  ttyS7    vboxnetctl  vcsa4
bsg              fd         kvm       mcelog              psaux   sg0       tty12  tty24  tty36  tty48  tty6       ttyS12  ttyS24  ttyS8    vboxusb     vcsa5
btrfs-control    full       lightnvm  mei0                ptmx    sg1       tty13  tty25  tty37  tty49  tty60      ttyS13  ttyS25  ttyS9    vcs         vcsa6
bus              fuse       log       mem                 ptp0    shm       tty14  tty26  tty38  tty5   tty61      ttyS14  ttyS26  ttyUSB0  vcs1        vfio
char             hidraw0    loop0     memory_bandwidth    ptp1    snapshot  tty15  tty27  tty39  tty50  tty62      ttyS15  ttyS27  ttyUSB1  vcs2        vga_arbiter
console          hidraw1    loop1     mqueue              pts     snd       tty16  tty28  tty4   tty51  tty63      ttyS16  ttyS28  ttyUSB2  vcs3        vhci
core             hpet       loop2     net                 random  stderr    tty17  tty29  tty40  tty52  tty7       ttyS17  ttyS29  ttyUSB3  vcs4        vhost-net
cpu              hugepages  loop3     network_latency     rfkill  stdin     tty18  tty3   tty41  tty53  tty8       ttyS18  ttyS3   uhid     vcs5        vhost-vsock
cpu_dma_latency  hwrng      loop4     network_throughput  rtc     stdout    tty19  tty30  tty42  tty54  tty9       ttyS19  ttyS30  uinput   vcs6        xclmgmt44801
cuse             i2c-0      loop5     null                rtc0    tty       tty2   tty31  tty43  tty55  ttyprintk  ttyS2   ttyS31  urandom  vcsa        xvc_pub.m44801
disk             initctl    loop6     nvme0               sda     tty0      tty20  tty32  tty44  tty56  ttyS0      ttyS20  ttyS4   userio   vcsa1       zero
dri              input      loop7     nvme0n1             sda1    tty1      tty21  tty33  tty45  tty57  ttyS1      ttyS21  ttyS5   vboxdrv  vcsa2
[15:55:44]wole.j@msl-dpe-d3:~/Documents/ACTGraph$ls /dev/ssd0