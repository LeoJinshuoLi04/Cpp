#include <tuple>
#include <utility>
#include <vector>
#include <algorithm>

/*

You are given two redundant market data feeds, A and B. Each feed contains packets
represented by (arrival_time, sequence_number), ordered by arrival time.

Both feeds transmit copies of the same sequence of messages, starting at first_seq.

Your task is to combine the feeds into one ordered stream, selecting the earliest 
available copy of each message. A sequence number must only be published once, and 
messages must be published in increasing sequence order.

Messages arriving ahead of a missing sequence number must be buffered until the 
missing message arrives. Duplicates must be ignored.

Return the arbitrated stream along with four statistics describing the arbitration.

feed_a = {
    {10, 1},
    {20, 3},
    {40, 4}
};

feed_b = {
    {12, 1},
    {15, 2},
    {22, 3},
    {30, 4}
};

first_seq = 1;

{
    {1, 10},  // From A
    {2, 15},  // From B
    {3, 20},  // From A
    {4, 30}   // From B
}

A selected — Number of messages taken from Feed A.
B selected — Number of messages taken from Feed B.
Duplicates — Number of arrivals discarded because that sequence number was already received.
Missing sequences — Number of sequence numbers missing from both feeds.
*/

class Getter {
  size_t index = 0;

  public:

  bool is_empty(std::vector<std::pair<long long,long long>>& feed){
    return (index >= feed.size());
  }

  std::pair<long long, long long>* front(std::vector<std::pair<long long,long long>>& feed){
    if(is_empty(feed)) return nullptr;
    return &feed[index];
  }

  void advance(){
    ++index;
  }
};

auto cmp = [](const std::pair<long long, long long>& a, const std::pair<long long, long long>& b){
  if(a.first == -1){
    return false;
  }
  if(b.first == -1){
    return true;
  }
  if(a.second == b.second){
    return a.first < b.first;
  }
  return a.second < b.second;
};

std::tuple<std::vector<std::pair<long long, long long>>, long long, long long, long long, long long>
arbitrate(
  const std::vector<std::pair<long long, long long>>& feed_a,
  const std::vector<std::pair<long long, long long>>& feed_b,
  long long first_seq
) {

  auto feedA = feed_a;
  auto feedB = feed_b;

  sort(feedA.begin(), feedA.end(), cmp);
  sort(feedB.begin(), feedB.end(), cmp);

  std::vector<std::pair<long long, long long>> res;
  long long fromA{0};
  long long fromB{0};
  long long missing{0};
  long long duplicates{0};

  auto aGetter = Getter{};
  auto bGetter = Getter{};

  std::pair<long long, long long>* last = nullptr;

  while(!aGetter.is_empty(feedA) || !bGetter.is_empty(feedB)){
    auto prev = last ? last->second : (res.empty() ? first_seq : res.back().second);
    std::pair<long long, long long>* next = nullptr;
    bool isFromA = false;
    if(aGetter.is_empty(feedA)){
      next = bGetter.front(feedB);
      bGetter.advance();
    }else if(bGetter.is_empty(feedB)){
      next = aGetter.front(feedA);
      aGetter.advance();
      isFromA = true;
    }else{
      if(cmp(*aGetter.front(feedA), *bGetter.front(feedB))){
        next = aGetter.front(feedA);
        aGetter.advance();
        isFromA = true;
      }else{
        next = bGetter.front(feedB);
        bGetter.advance();
      }
    }

    if(next->second <= prev){
      ++duplicates;
      continue;
    }else if(next->second == prev + 1){
      if(!last) {
        res.emplace_back(*next);
        if(isFromA){
          ++fromA;
        }else{
          ++fromB;
        }
      }else{
        last = next;
      }
    }else{
      missing += next->second - prev - 1;
      last = next;
    }
  }

  return {res, fromA, fromB, duplicates, missing};
}
