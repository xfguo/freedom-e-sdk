typedef unsigned uint32_t;

#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))

#define _REG32(p, i) (*(volatile uint32_t *) ((p) + (i)))
#define UART0_REG(offset) _REG32(UART0_BASE_ADDR, offset)

#define UART_REG_TXFIFO         0x00
#define UART_REG_TXCTRL         0x08
#define UART_REG_DIV            0x18
#define UART_TXEN               0x1
#define UART0_BASE_ADDR         0x10013000UL

int lock;
int uart_inited;

void get_lock(int *lock);
void put_lock(int *lock);

int get_hartid(void);

void write(const char *message)
{
    while (*message != '\0') {
        while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
        UART0_REG(UART_REG_TXFIFO) = *message;

        if (*message == '\n') {
            while (UART0_REG(UART_REG_TXFIFO) & 0x80000000) ;
            UART0_REG(UART_REG_TXFIFO) = '\r';
        }

        message++;
    }
}

static long get_cpu_freq(void)
{
    return 65000000;
}

static void uart_init(long baud_rate)
{
    if (get_hartid() == 0) {
        UART0_REG(UART_REG_DIV) = get_cpu_freq() / baud_rate - 1;
        UART0_REG(UART_REG_TXCTRL) |= UART_TXEN;
        __asm__ volatile ("fence");
        uart_inited = 1;
        __asm__ volatile ("fence");
    } else {
        while (1) {
          __asm__ volatile ("fence");
          if (ACCESS_ONCE(uart_inited))
              return;
        }
    }
}

void print_hello(void)
{
    get_lock(&lock);
    write("Hello, World from hartid=");
    char id[] = {'0' + get_hartid(), '\n', '\0'};
    write(id);
    put_lock(&lock);
}

int main()
{
    uart_init(115200);

    while (1) print_hello();
}
