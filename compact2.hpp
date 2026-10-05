#include <cstddef>
#include <memory>
#include <ranges>
#include <vector>
#include <ranges>
#include <type_traits>

template <typename T> void relocate(T* from, uintptr_t to){
  auto overlap = to < reinterpret_cast<uintptr_t>(from) + sizeof(T) &&
    reinterpret_cast<uintptr_t>(from) < to + sizeof(T);

  if constexpr (std::is_trivially_copyable_v<T>){
    std::memmove(reinterpret_cast<void*>(to), from, sizeof(T));
  } else if constexpr(std::is_move_constructible_v<T> && (!std::is_copy_constructible_v<T> || std::is_nothrow_move_constructible_v<T>)){
    if(overlap){
      auto temp = std::move(*from);
      std::destroy_at(from);
      std::construct_at(reinterpret_cast<T*>(to), std::move(temp));
    }else{
      std::construct_at(reinterpret_cast<T*>(to), std::move(*from));
      std::destroy_at(from);
    }
  } else {
    if(overlap) {
      auto temp = *from;
      std::destroy_at(from);
      std::construct_at(reinterpret_cast<T*>(to), temp);
    }else{
      std::construct_at(reinterpret_cast<T*>(to), *from);
      std::destroy_at(from);
    }
  }
}


template <typename T> void compact(std::vector<T *> &objects, void *address) {
  std::ranges::sort(objects);

  auto splitAddress = reinterpret_cast<uintptr_t>(address);
  size_t splitIndex = 0;

  while(splitIndex < objects.size() && reinterpret_cast<uintptr_t>(objects[splitIndex]) < splitAddress){
    splitAddress += sizeof(T);
    ++splitIndex;
  }

  auto forwardPassAddress = splitAddress;
  for(auto idx = splitIndex; idx < objects.size(); ++idx, forwardPassAddress += sizeof(T)){
    relocate(objects[idx], forwardPassAddress);
    objects[idx] = reinterpret_cast<T*>(forwardPassAddress);
  }

  auto backPassAddress = splitAddress - sizeof(T);
  for(int idx = splitIndex - 1; idx >= 0; --idx, backPassAddress -= sizeof(T)){
    relocate(objects[idx], backPassAddress);
    objects[idx] = reinterpret_cast<T*>(backPassAddress);
  }
};