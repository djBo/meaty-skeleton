unsigned short inportw (unsigned short port)
{
    unsigned short ret;
    __asm__ __volatile__ ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}
