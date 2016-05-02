/*
Copyright (c) 2016 Walter William Karas

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef END_SWAP_H_20160501_
#define END_SWAP_H_20160501_

#include <stdint.h>

// Byte order/endiance swap, source and destination may be the same.

template <unsigned Num_bytes>
inline void end_swap(void *dest, const void *src)
  {
    char *d = static_cast<char *>(dest);
    const char *s = static_cast<const char *>(src);

    char tmp = s[0];
    d[0] = s[Num_bytes - 1];
    d[Num_bytes - 1] = tmp;

    end_swap<Num_bytes - 2>(d + 1, s + 1);
  }

template <>
inline void end_swap<1>(void *, const void *) { }

template <>
inline void end_swap<0>(void *, const void *) { }

template <typename T>
inline T end_swap(T v) { end_swap<sizeof(T)>(&v, &v); return(v); }

namespace End_swap_impl
{

enum Usage { Normal, Swap };

template <typename T, Usage U>
struct Swappable;

template <typename T>
struct Swappable<T, Normal>
  {
    typedef T Type;
  };

template <typename T>
struct Swappable<T, Swap>
  {
    class Type
      {
      private:
        T v;

      public:

        void operator = (const Type &src) { this->v = end_swap(src.v); }
      };
  };

} // end namespace End_swap_impl

// See tst.cpp for example of use.

// Define swappble templeted structure named STRUCT_NAME, this macro generates
// everyting before : or {.
#define END_SWAP_STRUCT(STRUCT_NAME) \
  template <End_swap_impl::Usage End_swap_impl_usage = End_swap_impl::Normal> \
  struct STRUCT_NAME

// Expands to type name for swapabble base or member structure.
#define END_SWAP_SUB(STRUCT_NAME) STRUCT_NAME<End_swap_impl_usage>

// Expands to type name for primitive swappable member.
#define END_SWAP_MEM_T(TYPE_NAME) \
  typename End_swap_impl::Swappable<TYPE_NAME, End_swap_impl_usage>::Type

// Typedefs for swappable versions of stdint.h explicit precision integral
// types.  Invoke this macro inside the swappable struct definition for
// convenient use of these types.

#define END_SWAP_TYPES \
  typedef END_SWAP_MEM_T(uint8_t) u8; \
  typedef END_SWAP_MEM_T(uint16_t) u16; \
  typedef END_SWAP_MEM_T(uint32_t) u32; \
  typedef END_SWAP_MEM_T(uint64_t) u64; \
  typedef END_SWAP_MEM_T(int8_t) i8; \
  typedef END_SWAP_MEM_T(int16_t) i16; \
  typedef END_SWAP_MEM_T(int32_t) i32; \
  typedef END_SWAP_MEM_T(int64_t) i64;

// Do component-wise swap for swappable structure of type C<>.
// This will not work for structures sent between processor if the sizes
// of the primitive types that are swapped are not the same on both
// processor, or the packing/alignment of the structures is not the
// same on both processors.
//
template <template<End_swap_impl::Usage> class C>
inline void end_swap(
  C<End_swap_impl::Normal> &dest, const C<End_swap_impl::Normal> &src)
  {
    typedef C<End_swap_impl::Swap> CS;
    CS &d = reinterpret_cast<CS &>(dest);
    const CS &s = reinterpret_cast<const CS &>(src);
    d = s;
  }

// Hopefully, if efforts to add class introspection/reflection to the
// C++ standard succeed, there will be a cleaner way to do this.

#endif // Include once.
