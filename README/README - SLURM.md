LOGIN TO AN INTERRACTIVE NODE (E.G. PORTAL NODE)
ssh portal01.cs.virgini.edu/wiki/doku
Titan GPU in Automata 20 does not have HBM2 memories. Lynx05, lynx06, and lynx 07 have HBM2 memory.

reset GPU: (nvidia-smi --gpu-reset)?

CHECK AVAILABLE NODES IN LIST
[pgh5a@portal01]$ sinfo
PARTITION AVAIL TIMELIMIT NODES STATE NODELIST
main* up 4-00:00:00 15 resv cortado[01-05],falcon[1-10]
main* up 4-00:00:00 5 mix cortado06,hermes3,slurm[1-3]
main* up 4-00:00:00 23 idle cortado[07-10],hermes[1-2,4],hydro,lynx[08-12],nibbler[1-4],optane01,slurm[4-5],trillian[1-3]
gpu up 4-00:00:00 3 resv cheetah01,lynx[05-06]
gpu up 4-00:00:00 14 mix ai[01,03-06],cheetah[02-03],lynx[01-04,07],ristretto[01-02]
gpu up 4-00:00:00 7 idle affogato[11-15],ristretto[03-04]

CHECK RESERVATIONS
>> scontrol show reservations

RUNNING GUNROCK ON SLURM:
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

TESTING SBATCH:
$ sbatch submit.sh
sbatch: Submitted batch job 99999999

SLURM TUTORIALS:
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








