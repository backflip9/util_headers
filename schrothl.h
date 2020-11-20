#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <utility>

#ifdef DEBUG
#define debug if (true) cout
#else
#define debug if (false) cout
#endif

#define _TOSTRING(arg) #arg

#define TOSTRING(arg) _TOSTRING(arg)

#define CONCAT(arg1, arg2) arg1 ## arg2

#define _UNIQUE_COUNTER(base, line) CONCAT(base, line)

#define RANGE(count) for (int _UNIQUE_COUNTER(i, __LINE__) = 0; _UNIQUE_COUNTER(i, __LINE__) < count; ++_UNIQUE_COUNTER(i, __LINE__))

namespace lls
{
  template<typename TContainer, typename T>
  const T* find(const TContainer& container, const T& value)
  {
    //return 0;
    return std::find(container.begin(), container.end(), value);
  }

  template<typename TContainer, typename T>
  bool doesNotExist(const TContainer& container, const T& value)
  {
    return find(container, value) == container.end();
  }

  template<typename TContainer>
  void printContainer(const TContainer& container, std::ostream& out = std::cout)
  {
    out << '{';
    for (int index = 0; index < container.size(); ++index)
    {
      out << container[index];
      if (index != container.size() - 1)
      {
        out << ", ";
      }
    }
    out << "}\n";
  }

  template<typename T>
  T& sort(T& container)
  {
    std::sort(container.begin(), container.end());
		return container;
  }

  template<typename T, typename BinaryPred>
  T& sort(T& container, BinaryPred pred)
  {
    std::sort(container.begin(), container.end(), pred);
		return container;
  }

  template<typename TContainer, typename TValue = typename TContainer::value_type>
  TValue sum(const TContainer& container)
  {
    return std::accumulate(container.begin(), container.end(), TContainer::value_type(0));
  }

  template<typename TValue, typename TContainer>
  TValue sum(const TContainer& container)
  {
    return std::accumulate(container.begin(), container.end(), TValue(0));
  }

  template<typename SumPred>
  size_t sum(int begin, int end, SumPred pred)
  {
    size_t accumulator = 0;
    for (int i = begin; i < end; ++i)
    {
      pred(accumulator, i);
    }
    return accumulator;
    //return std::accumulate(range.begin(), range.end(), 0L, [a, b, j](const int64_t& current, const size_t& index) -> int64_t
  }

  template<typename SumPred>
  size_t sum(int end, SumPred pred)
  {
    return sum(0, end, pred);
  }


  template<typename TContainer, typename T>
  bool exists(const TContainer& container, const T& value)
  {
    return doesNotExist(container, value) == false;
  }

  template<typename TKey, typename TValue, uint32_t Size>
  class Map
  {
    std::array<std::pair<TKey, TValue>, Size> data;
    constexpr TValue at(const TKey&) const;
  };

  template<typename TKey, typename TValue, uint32_t Size>
  constexpr TValue Map<TKey, TValue, Size>::at(const TKey& key) const
  {
    auto itr = std::find(data.begin(), data.end(), key);
    return itr;
  }

  int round(float x)
  {
    return static_cast<int>(x + 0.5f);
  }
}
