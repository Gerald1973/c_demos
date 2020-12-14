#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define MILLISECOND(_x_) usleep(_x_ * 1000)

void displayValue(int value)
{
    switch (value)
    {
    case 0:
        printf("Input.\n");
        break;
    case 1:
        printf("Output\n");
        break;
    case 2:
        printf("SPI0_CE1_N\n");
        break;
    case 3:
        printf("SWE_N/SRW_N\n");
        break;
    case 4:
        printf("DPI_03\n");
        break;
    case 5:
        printf("SPI4_SCLK\n");
        break;
    case 6:
        printf("RTS3\n");
        break;
    case 7:
        printf("SCL4\n");
        break;
    }
}

static volatile unsigned int *gpio;
/**
 * @brief This program shows the blinking of led.
 * When the documentation of the BCM2711 ARM Peripherals
 * speaks about the pin, it refers the pin number of the BCM GPIO. 
 * 
 * @return int 
 */
int main(void)
{
    int fdgpio = open("/dev/gpiomem", O_RDWR | O_SYNC);
    if (fdgpio < 0)
    {
        printf("Error opening /dev/gpiomem");
        return -1;
    }
    gpio = (unsigned int *)mmap(NULL, 4096,
                                PROT_READ + PROT_WRITE, MAP_SHARED,
                                fdgpio, 0);
    printf("Pointer stqrt %p\n", gpio);
    // Read pin 8, by accessing bit 8 of GPLEV0
    printf("Address of GPIO[13]  &%p\n", &gpio[13]);
    printf("Value of   GPIO[13]  &%p\n", gpio[13]);
    for (int i = 0; i < 31; i++)
    {
        printf("Pin %d %d\n", i, (gpio[13] & (1 << i)));
    }
    for (int i = 0; i < 25; i++)
    {
        printf("Pin %d %d\n", i + 31, (gpio[14] & (1 << i)));
    }
    //fsel
    printf("fsel before gpio[0] = %p\n", gpio[0]);
    unsigned int value = (gpio[0] >> 12) & 7;
    displayValue(value);
    if (value == 0)
    {
        unsigned int output = 1 << 12;
        gpio[0] = gpio[0] | output;
        value = (gpio[0] >> 12) & 7;
        displayValue(value);
    }
    printf("fsel after gpio[0] = %p\n", gpio[0]);
    //set
    printf("&gpio[7] = %p\n", &gpio[7]);
    printf("gpio[7]  = %p\n", gpio[7]);

    for (int i = 0; i < 10000; i++)
    {
        gpio[7] = 1 << 4;
        MILLISECOND(5);
        gpio[10] = 1 << 4;
        MILLISECOND(5);
    }
    gpio[10] = 1 << 4;
    gpio[0] = 0;
    close(fdgpio);
    return 0;
}