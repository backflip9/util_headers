#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <utility>
#include <memory>
#include <stdio.h>

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
  template <typename T, typename ...Args>
	T& Tap(T& container, Args ...unused)
	{
		return container;
	}


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

  template<typename TContainer, typename TValue>
  TValue sum(const TContainer& container)
  {
    return std::accumulate(container.begin(), container.end(), std::result_of_t<TContainer::operator[]>(0));
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

#if defined(WIN32)
  template <typename T>
  class ScopedComPtr
  {
    public:
      template <typename ...Types>
      ScopedComPtr<T>(Types ...args)
        : data(new T(args))
      {
        data->AddRef();
      }

      /*
      ScopedComPtr<T[]>(size_t count)
        : data(new T[count])
      {
      }
	  */

      T& operator*() const
      {
        return dereference();
      }

      T& operator->() const
      {
        return dereference();
      }

      ~ScopedComPtr()
      {
        // if this sends the refcount back to 0
        if (data->Release() == 0)
        {
          delete data;
        }
      }
      
    private:
      T& dereference()
      {
        return *data;
      }
      T* data;
  };

  template <typename T, typename ...Types>
  std::enable_if_t<!std::is_array<T>::value, ScopedComPtr<T>> MakeScopedComPtr(Types ...args)
  {
      return ScopedComPtr<T>(args);
  }

  template <typename T>
  ScopedComPtr<T[]> MakeScopedComPtr(size_t count)
  {
    return ScopedComPtr<T[]>(count);
  }
#endif

  template<typename T>
  class LinkedList
  {
    private:
			struct ListNode;
      using NodePtr = std::shared_ptr<ListNode>;
      struct ListNode
      {
        ListNode(T input)
          : data(input)
          , next()
        {
        }

        T data;
        NodePtr next;
      };
    public:
      LinkedList<T>(T args...)
      {
        for (auto arg: {args...})
        {
          Insert(arg);
        }
      }

      const T& Insert(T value)
      {
        if (first.get() == nullptr)
        {
          first = value;
        }
        else
        {
          NodePtr tmp;
          while (tmp.get() != nullptr) tmp = tmp->next;
          tmp = value;
        }
      }

      void Delete()
      {
        if (first.get() != nullptr)
        {
          NodePtr tmp;
          while (tmp->next.get() != nullptr) tmp = tmp.next;
          tmp->next.reset();
        }
      }

    private:
      NodePtr first;
  };

  template <size_t size = 128>
  class FmtBuffer
  {
  public:
    FmtBuffer(const char* fmt, ...args)
    {
      sprintf(m_buf, fmt, args);
    }

    const char* c_str()
    {
      return &m_buf;
    }
  private:
    char m_buf[size];
  };

	template <typename T>
	void ZeroObject(const T* obj)
	{
		std::memset(obj, NULL, sizeof(T));
	}

#if 0
  template <typename T>
  class ScopedPtrBase
  {
  public:
    T* get() = 0;
  };

  template <>
  class ScopedPtr<IUnknown> : ScopedPtrBase<IUnknown>
  {
  public:
    IUnknown* get()
    {
      return data.Get();
    }
  private:
    Microsoft::WRL::ComPtr data;
  }

  template <typename T>
  class ScopedPtr<T> : ScopedPtrBase<T>
  {
  public:
    T* get()
    {
      return data.get();
    }
  private:
    std::unique_ptr<T> data;
  }
#endif
}
