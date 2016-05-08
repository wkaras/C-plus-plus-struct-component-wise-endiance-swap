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

#ifndef END_SWAP2_H_20160913_
#define END_SWAP2_H_20160913_

// Byte order/endiance swap, source and destination may be the same.
// This second version is layered on top of
// https://github.com/wkaras/C-plus-plus-library-default-operators
// which provides the composite_op.h header.

#include <stdint.h>

#include "comp_op/composite_op.h"

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
inline void end_swap<1>(void *dest, const void *src)
  {
    * static_cast<char *>(dest) = * static_cast<const char *>(src);
  }

template <>
inline void end_swap<0>(void *, const void *) { }

template <typename T>
inline T end_swap(T v) { end_swap<sizeof(T)>(&v, &v); return(v); }

template <typename T>
struct End_swap_impl
  {
    void operator () (T &d, const T &s)
      { end_swap<sizeof(T)>(&d, &s); }
  };

// COMPOSITE_OP(X, End_swap_impl) must be invoked for composite class X
// before use of unary or binary end_swap<X> functions.

// See tst2.cpp for example of use.

#define END_SWAP_TYPES \
  typedef COMPOSITE_OP_MBR_T(uint8_t) u8; \
  typedef COMPOSITE_OP_MBR_T(uint16_t) u16; \
  typedef COMPOSITE_OP_MBR_T(uint32_t) u32; \
  typedef COMPOSITE_OP_MBR_T(uint64_t) u64; \
  typedef COMPOSITE_OP_MBR_T(int8_t) i8; \
  typedef COMPOSITE_OP_MBR_T(int16_t) i16; \
  typedef COMPOSITE_OP_MBR_T(int32_t) i32; \
  typedef COMPOSITE_OP_MBR_T(int64_t) i64;

// Do component-wise swap for composite op class of type C<>.
// This will not work for structures sent between processor if the sizes
// of the primitive types that are swapped are not the same on both
// processor, or the packing/alignment of the structures is not the
// same on both processors.
//
template <class C>
inline void end_swap(C &dest, const C &src)
  { COMPOSITE_OP_CHECK(C, End_swap_impl) End_swap_impl<C>()(dest, src); }

#endif // Include once.
