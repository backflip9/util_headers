#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
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

#define CUSTOM_OPTIONAL_TEST 0

namespace lls
{
    template<typename T>
    static T Positive(const T& arg)
    {
        return +arg;
    }

    template<typename T>
    static T ClampAbs(const T& value, const T& clampLimit)
    {
      static_assert(std::is_floating_point_v<T>, "must be floating point");
      return std::clamp(value, -clampLimit, clampLimit);
    }

    template<typename T>
    static T Negative(const T& arg)
    {
        return -arg;
    }


  template<typename T>
  static T Plus(const T& arg1, const T& arg2)
  {
    return arg1 + arg2;
  }

  template<typename T>
  static T Minus(const T& arg1, const T& arg2)
  {
    return arg1 - arg2;
  }

  template<typename T>
  static T Times(const T& arg1, const T& arg2)
  {
    return arg1 * arg2;
  }

  template <typename T, typename ...Args>
	T& Tap(T& container, Args ...unused)
	{
		return container;
	}

  template<typename TContainer, typename T>
  const T* Find(const TContainer& container, const T& value)
  {
    //return 0;
    return std::find(std::begin(container), std::end(container), value);
  }

  template<typename TContainer>
  void PrintContainer(const TContainer& container, std::ostream& out = std::cout)
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
  T& Sort(T& container)
  {
    std::sort(std::begin(container), std::end(container));
		return container;
  }

  template<typename T, typename BinaryPred>
  T& Sort(T& container, BinaryPred pred)
  {
    std::sort(container.begin(), container.end(), pred);
		return container;
  }

  template<typename TContainer, typename TValue>
  TValue Sum(const TContainer& container)
  {
    return std::accumulate(container.begin(), container.end(), std::result_of_t<TContainer::operator[]>(0));
  }

  template<typename TValue, typename TContainer>
  TValue Sum(const TContainer& container)
  {
    return std::accumulate(container.begin(), container.end(), TValue(0));
  }

  template<typename SumPred>
  size_t Sum(int begin, int end, SumPred pred)
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
  size_t Sum(int end, SumPred pred)
  {
    return sum(0, end, pred);
  }

  template<typename TContainer, typename T>
  bool DoesNotExist(const TContainer& container, const T& value)
  {
    return lls::Find(container, value) == container.end();
  }


  template<typename TContainer, typename T>
  bool Exists(const TContainer& container, const T& value)
  {
    return DoesNotExist(container, value) == false;
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

  int Round(float x)
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


   //todo: concept

   struct OffsetInfo
   {
      uint32_t numBytes;
      uint8_t leftShiftBits;
   };


   template <typename Contained>
   class Optional
   {
   public:
      Optional()
      {
         for (size_t i = 0; i < kObjectSizeBytes; ++i)
         {
            m_storedValue[i] = 0;
         }
      }

      void Emplace()
      {
         ::new (m_storedValue) Contained();
         const OffsetInfo offsetInfo = getOffsetInfo<Contained>();
         uint8_t& targetByte = m_storedValue[offsetInfo.numBytes];
         targetByte |= (1 << offsetInfo.leftShiftBits);
      }

      void Destroy()
      {
         Contained* const castedContained = reinterpret_cast<Contained*>(m_storedValue);
         castedContained->~Contained();

         //could call getOffsetInfo<T>() here again, but it's probably faster to just zero out the whole struct?
         //if constexpr (kObjectSizeBytes < 100)
         if (false)
         {
            for (size_t i = 0; i < kObjectSizeBytes; ++i)
            {
               m_storedValue[i] = 0;
            }
         }
         else
         {
            const OffsetInfo offsetInfo = getOffsetInfo<Contained>();
            uint8_t& targetByte = m_storedValue[offsetInfo.numBytes];
            uint8_t andParam = ~(1 << offsetInfo.leftShiftBits);
            targetByte &= andParam;
         }
      }

      Contained& GetValue()
      {
         assert(HasValue() == true);
         Contained* const castedContained = reinterpret_cast<Contained*>(m_storedValue);
         return *castedContained;
      }

      bool HasValue() const
      {
         const OffsetInfo offsetInfo = getOffsetInfo<Contained>();

         assert(offsetInfo.numBytes < kObjectSizeBytes);
         const uint8_t relevantByte = m_storedValue[offsetInfo.numBytes];
         if ((relevantByte & (1 << offsetInfo.leftShiftBits)) == 0)
         {
            return false;
         }
         return true;
      }
   private:

      template <typename T>
      /*consteval*/ static OffsetInfo getOffsetInfo()
      {
         int32_t targetByte = -1;
         constexpr size_t kByteCount = sizeof(T);
         uint8_t zeroBuffer[kByteCount] = { 0 };
         T* castedInstance = reinterpret_cast<T*>(zeroBuffer);
         castedInstance->Optional_markReservedBit();
         for (uint32_t byteCounter = 0; byteCounter < kByteCount; ++byteCounter)
         {
            if (zeroBuffer[byteCounter] > 0)
            {
               targetByte = byteCounter;
               break;
            }
         }

         if (targetByte == -1)
         {
            assert(false);
         }

         int16_t leftShiftBits = -1;
         const uint8_t relevantByte = zeroBuffer[targetByte];
         for (uint8_t bitCounter = 0; bitCounter < 8; ++bitCounter)
         {
            if ((relevantByte & (1 << bitCounter)) > 0)
            {
               leftShiftBits = bitCounter;
               break;
            }
         }

         if (leftShiftBits == -1)
         {
            assert(false);
         }

         OffsetInfo result{};
         result.numBytes = uint32_t(targetByte);
         result.leftShiftBits = uint8_t(leftShiftBits);
         return result;
      }

      constexpr static size_t kObjectSizeBytes = sizeof(Contained);
      uint8_t m_storedValue[kObjectSizeBytes];
   };

   class ObjectOptions
   {
   public:
      using Self = ObjectOptions;

      ObjectOptions()
         : xCoord(0)
         , yCoord(0)
         , zCoord(0)
         , otherOptions()
         , id(0)
      {
      }

      friend Optional<Self>;
      void Optional_markReservedBit()
      {
         otherOptions.optionalReserved = 1;
      }

      const void* const Optional_GetByteOffset() const
      {
         //return offsetof(ObjectOptions, otherOptions);
         return &otherOptions;
      }
   public:

      float xCoord;
      float yCoord;
      float zCoord;
      struct otherOptionsStruct
      {
         otherOptionsStruct()
            : isTransparent(0)
            , needsReflection(0)
            , optionalReserved(0)
            , addToPickBuffer(0)
         {
         }

         uint32_t isTransparent : 1;
         uint32_t needsReflection : 1;
         uint32_t optionalReserved : 1;
         uint32_t addToPickBuffer : 1;
      } otherOptions;
      int id;
   };
}

#if CUSTOM_OPTIONAL_TEST
int main()
{
   lls::ObjectOptions options;
   options.xCoord = 93;
   options.yCoord = 94;
   options.zCoord = 95;
   options.otherOptions.isTransparent = 0;
   options.otherOptions.needsReflection = 1;
   options.otherOptions.optionalReserved = 0;
   options.otherOptions.addToPickBuffer = 1;
   options.id = 6432;
   const void* const memberPtr = options.Optional_GetByteOffset();
   lls::Optional<lls::ObjectOptions> optInstance = lls::Optional<lls::ObjectOptions>();
   const bool hasValue = optInstance.HasValue();
   optInstance.Emplace();
   const lls::ObjectOptions& containedValue = optInstance.GetValue();
   optInstance.Destroy();
   return 0;
}
#endif // CUSTOM_OPTIONAL_TEST
