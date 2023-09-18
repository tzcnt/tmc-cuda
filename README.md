## tmc-cuda
This is where the planned TooManyCooks - CUDA integration will be. There's nothing here yet...

### TODO
- Provide an awaitable that wraps a CUDA Graph (by attaching a callback at the end of the graph via [cudaGraphAddHostNode](https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__GRAPH.html#group__CUDART__GRAPH_1g30e16d2715f09683f0aa8ac2b870cf71)
- Investigate if CUDA provides other methods of async submission / notification.
- See if priority can be used to control access to the GPU.
- 
