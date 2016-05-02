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

// Test code for end_swap() templated functions.

#include "end_swap.h"

#include <iostream>
#include <cstring>

END_SWAP_STRUCT(A)
  {
    END_SWAP_TYPES

    u8 f1;

    u16 f2[3];

    char str[21];

    u32 f3;

    A()
      {
        f1 = 0x12;
        f2[0] = 0x2345;
        f2[1] = 0x3456;
        f2[2] = 0x4567;
        strcpy(str, "Don't swap this.");
        f3 = 0x56789abc;
      }
  };

using std::cout;

void dump(const A<> &a)
  {
    cout << "f1=0x" << unsigned(a.f1) << '\n';
    cout << "f2[0]=0x" << a.f2[0] << '\n';
    cout << "f2[1]=0x" << a.f2[1] << '\n';
    cout << "f2[2]=0x" << a.f2[2] << '\n';
    cout << "str=" << a.str << '\n';
    cout << "f3=0x" << a.f3 << '\n';
  }

END_SWAP_STRUCT(B) : public END_SWAP_SUB(A)
  {
    END_SWAP_TYPES

    END_SWAP_SUB(A) mbr_a;

    u64 f4;

    B() { f4 = 0x6789abcdef012345UL; }
  };

void dump(const B<> &b)
  {
    dump(static_cast<const A<> &>(b));

    dump(b.mbr_a);

    cout << "f4=0x" << b.f4 << '\n';
  }

int main()
  {
    B<> b1, b2;

    cout << std::hex;

    dump(b1);

    std::memset(&b2, 0xff, sizeof(b2));

    cout << "\nSWAP\n\n";

    end_swap(b2, b1);

    dump(b2);

    b1 = B<>();
    b2 = B<>();
    end_swap(b2, b1);
    end_swap(b2, b2);

    if (std::memcmp(&b1, &b2, sizeof(B<>)) != 0)
      cout << "FAIL\n";

    return(0);
  }
