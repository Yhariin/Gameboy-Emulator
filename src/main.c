#include "core.h"
#include "cpu.h"

#include "cpu.c"

int main(void)
{
    cpu_init();

    // registers.a = 0b0000'0001;
    // registers.f = 0b0000'0001;

    set_AF((u16)257);
    u16 af = get_AF();

    return 0;
}