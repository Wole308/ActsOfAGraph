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
