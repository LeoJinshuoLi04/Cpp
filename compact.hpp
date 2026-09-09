
#include <cstddef>
#include <memory>
#include <ranges>
#include <vector>

template <typename T> void compact(std::vector<T *> &objects, T *address) {
  std::ranges::sort(objects);

  auto skip_idx = size_t{0};

  for (auto *const object : objects) {
    const auto new_object_start = reinterpret_cast<uintptr_t>(address);
    const auto new_object_end = new_object_start + sizeof(T);

    const auto old_object_start = reinterpret_cast<uintptr_t>(object);
    const auto old_object_end = old_object_start + sizeof(T);

    if (new_object_start > old_object_end) {
      ++skip_idx;
      continue;
    }

    if (new_object_end > old_object_start) {
      auto temp = std::move_if_noexcept(*object);
      std::destroy_at(object);
      std::construct_at(address, std::move_if_noexcept(temp));
    } else {
      std::construct_at(address, std::move_if_noexcept(*object));
      std::destroy_at(object);
    }

    ++address;
  }

  for (auto idx = size_t{0}; idx < skip_idx; ++idx) {
    std::construct_at(address, std::move_if_noexcept(*objects[idx]));
    std::destroy_at(objects[idx]);
    ++address;
  }
};