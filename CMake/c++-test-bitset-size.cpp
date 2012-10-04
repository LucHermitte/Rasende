// Non bugged bitset implementations give sizeof bitset<0xffffffff> == 512Mb.
// See GCC Bug libstdc++/45713:
// http://gcc.gnu.org/bugzilla/show_bug.cgi?id=45713
#include <bitset>

int main ()
{
    typedef std::bitset<0xffffffff> bitset_max;
    return sizeof(bitset_max) == 1;
}
