#include <dandy/core/DCore.hpp>
#include <EASTL/array.h>
#include <EASTL/shared_ptr.h>
#include <EASTL/unique_ptr.h>
#include <EASTL/vector.h>
#include <EASTL/intrusive_list.h>

class Foo
{
public:
    Foo(int v = 0)
        : m_v(v) {}
    ~Foo() { x_printf("destory foo %d\n", m_v); }

    int m_v;
};


static int Putc(int c)
{
    // from mbed
    extern serial_t stdio_uart;
    ::serial_putc(&stdio_uart, c);
    return c;
}


int main(int argc, char *argv[])
{

    {
        Serial pc(USBTX, USBRX);
        pc.baud(115200);
        pc.printf("Hello pc.printf\n");
    }
    x_printf("Hello picox\n");

    x_putc_stdout = Putc;
    x_putc_stderr = Putc;
    {
        eastl::array<int, 10> da;
    }

    {
        eastl::shared_ptr<Foo> sp = eastl::make_shared<Foo>();
        eastl::unique_ptr<Foo> up(new Foo());
    }

    {
        eastl::vector<Foo> iv;

        for (int i = 0; i < 10; i++)
        {
            iv.push_back(Foo(i));
        }
        iv.shrink_to_fit();
    }

    return 0;
}
