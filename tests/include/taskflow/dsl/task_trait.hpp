// 2020/08/28 - Created by netcan: https://github.com/netcan
#ifndef TASKFLOW_DSL_TASK_TRAIT_HPP
#define TASKFLOW_DSL_TASK_TRAIT_HPP
#include <taskflow/core/flow_builder.hpp>
#include <taskflow/core/task.hpp>
#include <taskflow/dsl/type_list.hpp>
#include <type_traits>

namespace tf {
namespace dsl {
struct TaskSignature {};

template <typename TASK, typename CONTEXT> struct TaskCb {
  using TaskType = TASK;
  void build(FlowBuilder &build, const CONTEXT &context) {
    task_ = build.emplace(TaskType{context}());
  }

  Task task_;
};

template <typename TASK> struct IsTask {
  template <typename TaskCb> struct apply {
    constexpr static bool value =
        std::is_same<typename TaskCb::TaskType, TASK>::value;
  };
};

template <typename TASK, typename = void> struct TaskTrait;

template <typename... TASK> struct SomeTask {
  using TaskList =
      Unique_t<Flatten_t<TypeList<typename TaskTrait<TASK>::TaskList...>>>;
};

// a task self
template <typename TASK>
struct TaskTrait<
    TASK, std::enable_if_t<std::is_base_of<TaskSignature, TASK>::value>> {
  using TaskList = TypeList<TASK>;
};

template <typename... TASK> struct TaskTrait<SomeTask<TASK...>> {
  using TaskList = typename SomeTask<TASK...>::TaskList;
};
} // namespace dsl
} // namespace tf
#endif // TASKFLOW_DSL_TASK_TRAIT_HPP
