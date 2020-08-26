### compile FPGA kernel on F1 instance 
source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
source /opt/xilinx/xrt/setup.sh 
source ~/src/project_data/aws-fpga/sdaccel_setup.sh 
cp -rf kernel.xclbin /home/centos/src/project_data/aws-fpga/SDAccel/tools/build/
cd /home/centos/src/project_data/aws-fpga/SDAccel/tools/build
cd build/
rm -rf to_aws
../create_sdaccel_afi.sh -xclbin=kernel.xclbin -o=kernel -s3_bucket=alifahmed -s3_dcp_key=dcp -s3_logs_key=logs
aws ec2 describe-fpga-images --fpga-image-id afi-03131e5ff3a58bb62 (get id (e.g. 03131e5ff3a58bb62) from /home/centos/src/project_data/aws-fpga/SDAccel/tools/build/20_03_05-193942_afi_id.txt)
	aws ec2 describe-fpga-images --fpga-image-id afi-002978ab433de38e1
cp -rf /home/centos/src/project_data/aws-fpga/SDAccel/tools/build/kernel.awsxclbin /home/centos/src/project_data/oj2zf/ActsOfAGraph/kernel.awsxclbin

### run FPGA kernel on F1 instance 
sudo su
source /opt/xilinx/xrt/setup.sh 
source /opt/Xilinx/SDx/2019.1.op2552052/settings64.sh 
./host kernel.awsxclbin

### delete FPGA AFI
aws ec2 delete-fpga-image --fpga-image-id afi-06b12350a123fbabc