// ----------------------------------------------------------------
// Gunrock -- Fast and Efficient GPU Graph Library
// ----------------------------------------------------------------
// This source code is distributed under the terms of LICENSE.TXT
// in the root directory of this source distribution.
// ----------------------------------------------------------------

/**
 * @file
 * kernel.cuh
 *
 * @brief bypass filter kernel
 */

#pragma once

#include <gunrock/oprtr/BP_filter/kernel_policy.cuh>

// #define MYOWN_FILTER
// #define WOLESPROFILER

namespace gunrock {
namespace oprtr {
namespace BP {

/**
 * Not valid for this arch (default)
 * @tparam FLAG Operator flags
 * @tparam InKeyT Input keys type
 * @tparam OutKeyT Output keys type
 * @tparam VALID
 */
template <OprtrFlag FLAG, typename InKeyT, typename OutKeyT, typename SizeT,
          bool VALID =
#ifdef __CUDA_ARCH__
              true
#else
              false
#endif
          >
struct Dispatch {
};

/*
 * @brief Dispatch data structure.
 * @tparam FLAG Operator flags
 * @tparam GraphT Graph type
 * @tparam InKeyT Input keys type
 * @tparam OutKeyT Output keys type
 */
#ifdef MYOWN_FILTER
template <OprtrFlag FLAG, typename InKeyT, typename OutKeyT, typename SizeT>
struct Dispatch<FLAG, InKeyT, OutKeyT, SizeT, true> {
  typedef KernelPolicy<SizeT, 8, 8, 9> KernelPolicyT;

  template <typename FilterOpT>
  static __device__ __forceinline__ void Kernel(const InKeyT *&keys_in,
                                                SizeT &num_inputs,
                                                OutKeyT *&keys_out,
                                                SizeT *out_counter,
                                                FilterOpT filter_op) {
    typedef typename KernelPolicyT::BlockScanT BlockScanT;
    // Shared storage for the kernel
    __shared__ typename KernelPolicyT::SmemStorage smem_storage;

    SizeT pos = (SizeT)threadIdx.x + blockIdx.x * blockDim.x;
    const SizeT STRIDE = (SizeT)blockDim.x * gridDim.x;
    while (pos - threadIdx.x < num_inputs) {
		// keys_out[pos] = keys_in[pos];
		// pos += STRIDE;
		
		bool to_process = true;
		
		if (keys_out != NULL && pos < num_inputs){
			keys_out[pos] = keys_in[pos] + pos;
		}
		
		SizeT output_offset;
		BlockScanT::LogicScan(to_process, output_offset, smem_storage.scan_space);
		if (threadIdx.x + 1 == blockDim.x)
		atomicAdd(out_counter, output_offset + ((to_process) ? 1 : 0));
	
		pos += STRIDE;
    }
  }
};
#else 
template <OprtrFlag FLAG, typename InKeyT, typename OutKeyT, typename SizeT>
struct Dispatch<FLAG, InKeyT, OutKeyT, SizeT, true> {
  typedef KernelPolicy<SizeT, 8, 8, 9> KernelPolicyT;

  template <typename FilterOpT>
  static __device__ __forceinline__ void Kernel(const InKeyT *&keys_in,
                                                SizeT &num_inputs,
                                                OutKeyT *&keys_out,
                                                SizeT *out_counter,
                                                FilterOpT filter_op) {
    typedef typename KernelPolicyT::BlockScanT BlockScanT;
    // Shared storage for the kernel
    __shared__ typename KernelPolicyT::SmemStorage smem_storage;

    SizeT pos = (SizeT)threadIdx.x + blockIdx.x * blockDim.x;
    const SizeT STRIDE = (SizeT)blockDim.x * gridDim.x;
    while (pos - threadIdx.x < num_inputs) {
      bool to_process = true;
      InKeyT key = pos;
      if (pos < num_inputs) {
        if (keys_in != NULL) key = keys_in[pos];
        SizeT inv_size = util::PreDefinedValues<SizeT>::InvalidValue;
        InKeyT inv_key = util::PreDefinedValues<InKeyT>::InvalidValue;
        to_process = filter_op(inv_key, key, inv_size, key, inv_size, inv_size);
      } else
        to_process = false;

      if (keys_out != NULL && pos < num_inputs)
        keys_out[pos] =
            (to_process) ? key : util::PreDefinedValues<OutKeyT>::InvalidValue;

      SizeT output_offset;
      BlockScanT::LogicScan(to_process, output_offset, smem_storage.scan_space);
      if (threadIdx.x + 1 == blockDim.x)
        atomicAdd(out_counter, output_offset + ((to_process) ? 1 : 0));

      // if (pos - threadIdx.x >= input_queue_length - STRIDE) break;
      pos += STRIDE;
    }
  }
};
#endif 

template <OprtrFlag FLAG, typename InKeyT, typename OutKeyT, typename SizeT,
          typename FilterOpT>
__launch_bounds__(
    Dispatch<FLAG, InKeyT, OutKeyT, SizeT, true>::KernelPolicyT::THREADS,
    Dispatch<FLAG, InKeyT, OutKeyT, SizeT, true>::KernelPolicyT::CTA_OCCUPANCY)
    __global__
    void Kernel(const bool queue_reset, const SizeT queue_index,
                const InKeyT *keys_in, SizeT num_inputs, OutKeyT *keys_out,
                util::CtaWorkProgress<SizeT> work_progress,
                FilterOpT filter_op) {
  PrepareQueue(queue_reset, queue_index, num_inputs, (SizeT *)NULL,
               work_progress, true);

  Dispatch<FLAG, InKeyT, OutKeyT, SizeT>::Kernel(
      keys_in, num_inputs, keys_out,
      work_progress.GetQueueCounter(queue_index + 1), filter_op);
}

template <OprtrFlag FLAG, typename GraphT, typename FrontierInT,
          typename FrontierOutT, typename ParametersT, typename AdvanceOpT,
          typename FilterOpT>
cudaError_t Launch(const GraphT graph, const FrontierInT *frontier_in,
                   FrontierOutT *frontier_out, ParametersT &parameters,
                   AdvanceOpT advance_op, FilterOpT filter_op) {
	#ifdef WOLESPROFILER
	util::PrintMsg("BP_filter Launched. (/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/BP_filter/kernel.cuh/Launch)", true);
	util::PrintMsg("BP_filter Launched. frontier size: " + std::to_string(parameters.frontier->queue_length), true);
	util::PrintMsg("BP_filter Launched. number of edges: " + std::to_string(graph.edges), true);
	util::PrintMsg("BP_filter Launched. number of nodes: " + std::to_string(graph.nodes), true);
	util::PrintMsg("BP_filter Launched. graph direction: " + std::to_string(graph.directed), true);
	#endif 
	
  typedef typename FrontierInT ::ValueT InKeyT;
  typedef typename FrontierOutT::ValueT OutKeyT;
  typedef typename GraphT::SizeT SizeT;
  typedef typename Dispatch<FLAG, InKeyT, OutKeyT, SizeT, true>::KernelPolicyT
      KernelPolicyT;
  cudaError_t retval = cudaSuccess;

  SizeT num_blocks = KernelPolicyT::BLOCKS;
  if (parameters.frontier->queue_reset) {
    GUARD_CU(parameters.frontier->work_progress.Reset_(0, parameters.stream));
    num_blocks =
        (parameters.frontier->queue_length + KernelPolicyT::THREADS - 1) /
        KernelPolicyT::THREADS;
    if (num_blocks > KernelPolicyT::BLOCKS) num_blocks = KernelPolicyT::BLOCKS;
  }
  
	///////////////////////////////////////////////////////
	#ifdef WOLESPROFILER
	util::PrintMsg("", true); // start timer
	util::CpuTimer cpu_timer;
	cpu_timer.Start();
	util::GRError(cudaStreamSynchronize(parameters.stream),
							"FullQueue_Core failed.", __FILE__, __LINE__);
	#endif 

  Kernel<FLAG, InKeyT, OutKeyT, SizeT>
      <<<num_blocks, KernelPolicyT::THREADS, 0, parameters.stream>>>(
          parameters.frontier->queue_reset, parameters.frontier->queue_index,
          (frontier_in == NULL) ? ((InKeyT *)NULL)
                                : frontier_in->GetPointer(util::DEVICE),
          parameters.frontier->queue_length,
          (frontier_out == NULL) ? ((OutKeyT *)NULL)
                                 : frontier_out->GetPointer(util::DEVICE),
          parameters.frontier->work_progress, filter_op);
		  
		  
	#ifdef WOLESPROFILER
	util::GRError(cudaStreamSynchronize(parameters.stream),
							"FullQueue_Core failed.", __FILE__, __LINE__);
	cpu_timer.Stop(); // stop timer
	util::PrintMsg(">>> BP_filter Kernel time elapsed (/net/bigtemp/oj2zf/gunrock_wole/gunrock/oprtr/BP_filter/Launch): " + std::to_string(cpu_timer.ElapsedMillis()) + " milliseconds", true); 
	float totalsize = parameters.frontier->queue_length;
	float totalsize_gbytes = (totalsize * sizeof(InKeyT)) / 1000000000;
	float totaltime_secs = cpu_timer.ElapsedMillis() / 1000;
	util::PrintMsg(">>> [FILTER] Total size (vertices):: " + std::to_string(		totalsize		) + " vertices", true);
	util::PrintMsg(">>> [FILTER] Total size (giga bytes):: " + std::to_string(		totalsize_gbytes		) + " giga bytes", true);
	util::PrintMsg(">>> [FILTER] Total time (ms):: " + std::to_string(		(totaltime_secs * 1000)		) + " milliseconds", true);
	util::PrintMsg(">>> [FILTER] Total time (s):: " + std::to_string(		totaltime_secs		) + " seconds", true);
	util::PrintMsg(">>> [FILTER] Effective Bandwidth (GB/s):: " + std::to_string(		(totalsize_gbytes / totaltime_secs)		) + " Gigabytes per sec", true);
	util::PrintMsg("", true);
	#endif 
	///////////////////////////////////////////////////////

  return retval;
}

}  // namespace BP
}  // namespace oprtr
}  // namespace gunrock

// Leave this at the end of the file
// Local Variables:
// mode:c++
// c-file-style: "NVIDIA"
// End:
