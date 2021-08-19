# gunrock
GPU available on Automata20 (ssh oj2zf@automata20.cs.virginia.edu, P5vTuiH4)
Titan GPU in Automata 20 does not have HBM2 memories. Lynx05, lynx06, and lynx 07 have HBM2 memory.

reset GPU: (nvidia-smi --gpu-reset)?

Bash Scripts:
 #cuda="cuda9"
 cuda="cuda10
if [ $cuda = "cuda9" ]
then
	module load cuda-toolkit-9.0
	module load cmake-3.10
	module load boost
	module load openmpi4.0.0
	export LD_LIBRARY_PATH="/sw/centos/cuda/9.2/lib64:$LD_LIBRARY_PATH"
	export PATH="/sw/centos/cuda/9.2/bin:$PATH"
else
	#for gunrock changed from 10 to 10.1
	module load cuda-toolkit-10.1
	module load cmake-3.10
	module load boost
	module load openmpi4.0.0
	export LD_LIBRARY_PATH="/sw/centos/cuda/10.1/lib64:$LD_LIBRARY_PATH"
	export LD_LIBRARY_PATH="/sw/centos/cuda/10.1/extras/CUPTI/lib64:$LD_LIBRARY_PATH"
	export PATH="/sw/centos/cuda/10.1/bin:/sw/centos/cuda/10.1/NsightCompute-1.0:$PATH"
fi

Building Gunrock:
https://gunrock.github.io/docs/#/gunrock/building_gunrock

Running Gunrock:
/net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market /net/bigtemp/oj2zf/gunrock_wole/dataset/small/bips98_606.mtx --normalized --compensate --undirected
/net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market /net/bigtemp/oj2zf/gunrock_wole/dataset/large/soc-orkut/soc-orkut.mtx --normalized --compensate --undirected				
/net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market /net/bigtemp/oj2zf/gunrock_wole/dataset/large/kron_g500-logn21/kron_g500-logn21.mtx --normalized --compensate --undirected				
/net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market /net/bigtemp/oj2zf/gunrock_wole/dataset/large/hollywood-2009/hollywood-2009.mtx --normalized --compensate --undirected
/net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market /net/bigtemp/oj2zf/gunrock_wole/dataset/large/road_usa/road_usa.mtx --normalized --compensate --undirected
/net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market /net/bigtemp/oj2zf/gunrock_wole/dataset/large/soc-twitter-2010/soc-twitter-2010.mtx --normalized --compensate --undirected

/net/bigtemp/oj2zf/gunrock_wole/build/bin/bfs market /net/bigtemp/oj2zf/gunrock_wole/dataset/small/bips98_606.mtx --normalized --compensate --undirected
/net/bigtemp/oj2zf/gunrock_wole/build/bin/bfs market /net/bigtemp/oj2zf/gunrock_wole/dataset/large/soc-orkut/soc-orkut.mtx --normalized --compensate --undirected				
/net/bigtemp/oj2zf/gunrock_wole/build/bin/bfs market /net/bigtemp/oj2zf/gunrock_wole/dataset/large/kron_g500-logn21/kron_g500-logn21.mtx --normalized --compensate --undirected				

Running Gunrock on Slurm:
srun -w lynx05 -p gpu --reservation=oj2zf_34 --gres=gpu:4 --pty bash -i -l -
srun --exclusive -w lynx05 -p gpu --gres=gpu:4 --pty bash -i -l 
srun --exclusive -w lynx06 -p gpu --gres=gpu:4 --pty bash -i -l -
srun --exclusive -w lynx05 -p gpu --gres=gpu:4 --pty bash -i -l /net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market /net/bigtemp/oj2zf/gunrock_wole/dataset/small/bips98_606.mtx --normalized --compensate --undirected 					
srun --exclusive -w lynx06 -p gpu --gres=gpu:4 --pty bash -i -l /net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market /net/bigtemp/oj2zf/gunrock_wole/dataset/small/bips98_606.mtx --normalized --compensate --undirected
srun -w lynx06 -p gpu --gres=gpu:4 /net/bigtemp/oj2zf/gunrock_wole/build/bin/pr market /net/bigtemp/oj2zf/gunrock_wole/dataset/small/bips98_606.mtx --normalized --compensate --undirected  
srun --exclusive -w lynx06 -p gpu --gres=gpu:4 --pty bash -i -l submit.sh
/net/bigtemp/oj2zf/gunrock_wole/examples/pr/run.sh contains examples
/net/bigtemp/oj2zf/gunrock_wole/examples/BaseMakefile.mk
./bin/test_pr_10.1_x86_64 market /net/bigtemp/oj2zf/gunrock_wole/dataset/small/bips98_606.mtx --partition-method=random --compensate --undirected --quick --device=0

Testing Sbatch:
$ sbatch submit.sh
sbatch: Submitted batch job 99999999

Slurm Tutorials:
https://www.cs.virginia.edu/wiki/doku.php?id=compute_slurm

Resolving Gunrock error on Slurm:
/net/bigtemp/oj2zf/gunrock_wole/build/bin/pr: unrecognized option '--normalized'
[/net/bigtemp/oj2zf/gunrock_wole/gunrock/util/test_utils.cu, 82 @ gpu 32766] cudaSetDevice failed. (CUDA error 35: CUDA driver version is insufficient for CUDA runtime version)				
[/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/test_base.cuh, 715 @ gpu 0] error encountered (CUDA error 35: CUDA driver version is insufficient for CUDA runtime version)
srun: error: nibbler1: task 0: Exited with exit code 35

Important Gunrock paths:
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/pr/pr_proclem.cuh* (specifies maximum number of iterations)
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/enactor_loop.cuh/Iteration_Loop* (the main loop)
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/pr/pr_enactor.cuh/Core* (defines advance and filter ops, launches advance and filter ops)
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/bfs/bfs_enactor.cuh/Core* (defines advance and filter ops, launches advance and filter ops)
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/pr/pr_enactor.cuh/Run*
/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/AE_advance/kernel.cuh/Launch* (the advance function caller)
/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/AE_advance/kernel.cuh/Advance_Edges* (the advance function for PR. this function calls ProcessNeighbor)
/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/LB_advance/kernel.cuh/RelaxLightEdges* (the advance function for BFS etc. this function calls ProcessNeighbor)
/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/advance/advance_base/ProcessNeighbor* (the process edge calling function - called by oprtr/AE_advance/kernel.cuh/Advance_Edges)
/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/BP_filter/kernel.cuh/Launch* (the filter function)
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/pr/pr_enactor.cuh/Core
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/enactor_loop.cuh/Iteration_Loop
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/pr/pr_enactor/Core
/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/oprtr.cuh/Filter
/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/oprtr.cuh (shows the assigned advance, filter or compute types based on parameters spaecified)
/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/BP_filter/kernel.cuh/Launch
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/enactor_loop.cuh/Iteration_Loop
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/pr/pr_enactor.cuh/Core
/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/oprtr.cuh/Advance
/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/AE_advance/kernel.cuh/Launch
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/pr/pr_enactor.cuh
/net/bigtemp/oj2zf/gunrock_wole/gunrock/app/pr/pr_app.cu/RunTests

Check which GPU is Installed:
nvidia-smi
nvidia-smi -q (gives more information)
Thu Apr  1 12:51:12 2021       
+-----------------------------------------------------------------------------+
| NVIDIA-SMI 450.36.06    Driver Version: 450.36.06    CUDA Version: 11.0     |
|-------------------------------+----------------------+----------------------+
| GPU  Name        Persistence-M| Bus-Id        Disp.A | Volatile Uncorr. ECC |
| Fan  Temp  Perf  Pwr:Usage/Cap|         Memory-Usage | GPU-Util  Compute M. |
|                               |                      |               MIG M. |
|===============================+======================+======================|
|   0  TITAN X (Pascal)    Off  | 00000000:06:00.0 Off |                  N/A |
| 21%   35C    P0    51W / 250W |      0MiB / 12196MiB |      3%      Default |
|                               |                      |                  N/A |
+-------------------------------+----------------------+----------------------+
                                                                               
+-----------------------------------------------------------------------------+
| Processes:                                                                  |
|  GPU   GI   CI        PID   Type   Process name                  GPU Memory |
|        ID   ID                                                   Usage      |
|=============================================================================|
|  No running processes found                                                 |
+-----------------------------------------------------------------------------+
https://www.techpowerup.com/gpu-specs/titan-x-pascal.c2863
https://www.techpowerup.com/gpu-specs/tesla-p100-pcie-16-gb.c2888




