#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int memfd = -1;
    uint32_t gpio_size = 0;   
    uint32_t *gpio = (uint32_t *)MAP_FAILED;

    if ((memfd = open("/dev/gpiomem", O_RDWR | O_SYNC) ) < 0) 
    {
	  fprintf(stderr, "bcm2835_init: Unable to open /dev/gpiomem: %s\n",
		  strerror(errno)) ;
	  goto exit;
    }
    
    gpio_size = lseek(memfd, 0, SEEK_END);

    printf("gpio size %d\n", gpio_size);

exit:
    if (memfd >= 0)
    {
        close(memfd);
    }

    return 0;

    gpio = mmap(NULL, gpio_size, (PROT_READ | PROT_WRITE), MAP_SHARED, memfd, 0);
    if (gpio == MAP_FAILED) goto exit;
 
    if (gpio != MAP_FAILED) munmap(gpio, gpio_size);

    return 0;
}
