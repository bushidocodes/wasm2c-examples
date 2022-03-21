
extern int add(int a, int b) __attribute__((export_name("add")))
{
    return a + b;
}