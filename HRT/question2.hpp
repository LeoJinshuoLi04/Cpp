#include <vector>
#include <random>
#include <unordered_map>

/*
Design a data structure that supports insert, delete, and getRandom in O(1) average time.

Extend the data structure so each value has a weight, and getRandom returns values with probability proportional to their weight.

How would you support duplicate values being inserted?

How would you handle updating a weight after insertion?

How would you test that your getRandom implementation actually matches the expected probability distribution?
*/

class Fenwick{
  size_t size_;
  std::vector<size_t> tree;

  void update(size_t l, size_t r, size_t curr, size_t target, size_t weight) {
    if (l == r) {
      tree[curr] = weight;
      return;
    }

    size_t mid = l + (r - l) / 2;
    if(target > mid){
      update(mid + 1, r, curr * 2 + 1, target, weight);
    }else{
      update(l, mid, curr * 2, target, weight);
    }

    tree[curr] = tree[curr * 2] + tree[curr * 2 + 1];
  }

  public:
  Fenwick(size_t size) : size_(size){
    tree.resize(size * 4 + 1);
  }

  void update(size_t index, size_t weight) {
    if (index == 0 || index > size_) return;
    update(1, size_, 1, index, weight);
  }

  size_t queryWeight(size_t weight) {
    if (size_ == 0 || weight == 0 || weight > tree[1])
      return size_ + 1;

    size_t index = 1;
    size_t curr = 0;
    size_t l = 1, r = size_;

    while (l < r) {
      size_t mid = l + (r - l) / 2;

      if (curr + tree[index * 2] < weight) {
        curr += tree[index * 2];
        index = index * 2 + 1;
        l = mid + 1;
      } else {
        index = index * 2;
        r = mid;
      }
    }

    return l;
  }
};


template <typename T>
class Randomiser{
  private:
  std::unordered_map<T, size_t> indices;
  std::vector<T> values;

  std::random_device rd;
  mutable std::mt19937 gen{rd()};

  public:
  Randomiser() = default;

  void insert(T value){
    indices[value] = values.size();
    values.push_back(value);
  }

  void eraseAt(size_t index){
    if(index >= values.size()) return;

    indices.erase(values[index]);
    
    if(index != values.size() - 1){
      indices[values.back()] = index;
      std::swap(values[index], values.back());
    }

    values.pop_back();
  }

  void erase(const T& value){
    if(auto it = indices.find(value); it != indices.end()){
      erase(it->second);
    }
  }

  T* getRandom() const noexcept {
    if(values.empty()) return nullptr;

    std::uniform_int_distribution<size_t> distrib(0, values.size() - 1);
    return &values[distrib(gen)];
  }

};