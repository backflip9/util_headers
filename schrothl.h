//#include <pair>
#ifdef DEBUG
#define debug if (true) cout
#else
#define debug if (false) cout
#endif

#include <array>
#include <utility>
#include <algorithm>
#include <cstdint>
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

  template<typename SumPred>
  size_t sum(int begin, int end, SumPred pred)
  {
    size_t accumulator = 0;
    for(int i = begin; i < end; ++i)
    {
      pred(accumulator, i);
    }
    return accumulate;
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
