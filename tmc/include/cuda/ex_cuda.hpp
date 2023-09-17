#pragma once
#include "tmc/detail/thread_locals.hpp"
#include "tmc/task.hpp"
#include <cassert>
#include <string>
#include <thread>
namespace tmc {
// A wrapper around a CUDA graphExec_t
struct aw_cuda_graph {
  // graphExec_t thing;
  // set in constructor to facilitate resume_on()
  detail::type_erased_executor *continuation_executor =
      detail::this_thread::executor;
  size_t prio = detail::this_thread::this_task.prio;
  std::coroutine_handle<> outer;
  inline bool await_ready() { return false; }

  inline void await_suspend(std::coroutine_handle<> h) {
    outer = h;
    // ex_cuda.execute([this]() {
    // continuation_executor->post_variant(std::move(outer), prio); });
  }

  inline void await_resume() {}

  inline aw_cuda_graph &resume_on(detail::type_erased_executor *executor) {
    continuation_executor = executor;
    return *this;
  }
  template <detail::TypeErasableExecutor Exec>
  aw_cuda_graph &resume_on(Exec &executor) {
    return resume_on(executor.type_erased());
  }
  template <detail::TypeErasableExecutor Exec>
  aw_cuda_graph &resume_on(Exec *executor) {
    return resume_on(executor->type_erased());
  }
};

struct ex_cuda {
  tmc::detail::type_erased_executor type_erased_this;
  bool is_initialized = false;
  inline void init_thread_locals(size_t slot) {
    detail::this_thread::executor = &type_erased_this;
    // detail::this_thread::this_task = {.prio = 0, .yield_priority =
    // &yield_priority[slot]}; detail::this_thread::thread_name =
    // std::string("cuda thread ") + std::to_string(slot);
  }

  inline void clear_thread_locals() {
    detail::this_thread::executor = nullptr;
    // detail::this_thread::this_task = {};
    // detail::this_thread::thread_name.clear();
  }
  inline void graceful_stop() {
    // shutdown CUDA
  }
  inline void init(int nstreams) {
    if (is_initialized) {
      return;
    }
    is_initialized = true;
  }

  inline void teardown() {
    if (!is_initialized) {
      return;
    }
    is_initialized = false;
  }
  inline ex_cuda() : type_erased_this(*this) {}
  inline ex_cuda(int nstreams) : type_erased_this(*this) { init(nstreams); }

  template <typename T> void post(T &&item, size_t priority) {
    assert("CUDA executor does not allow post(). You should only call co_await "
           "aw_cuda_graph.");
  }

  template <typename It>
  void post_bulk(It items, size_t priority, size_t count) {
    assert("CUDA executor does not allow post(). You should only call co_await "
           "aw_cuda_graph.");
  }
};
} // namespace tmc
