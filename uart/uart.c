#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

//http://www.raspberry-projects.com/pi/programming-in-c/uart-serial-port/using-the-uart

   
struct termios OLD_TERMIO_OPTIONS;
int uart_open(void)
{
    //-------------------------
    //----- SETUP USART 0 -----
    //-------------------------
    //At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
    int uart0_filestream = -1;
 
    //OPEN THE UART
    //The flags (defined in fcntl.h):
    //    Access modes (use 1 of these):
    //        O_RDONLY - Open for reading only.
    //        O_RDWR - Open for reading and writing.
    //        O_WRONLY - Open for writing only.
    //
    //    O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
    //                                            if there is no input immediately available (instead of blocking). Likewise, write requests can also return
    //                                            immediately with a failure status if the output can't be written immediately.
    //
    //    O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
    uart0_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);        //Open in non blocking read/write mode
    //uart0_filestream = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);        //Open in non blocking read/write mode
    if (uart0_filestream == -1)
    {
        //ERROR - CAN'T OPEN SERIAL PORT
        printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
        return -1;
    }
    
    //CONFIGURE THE UART
    //The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
    //    Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
    //    CSIZE:- CS5, CS6, CS7, CS8
    //    CLOCAL - Ignore modem status lines
    //    CREAD - Enable receiver
    //    IGNPAR = Ignore characters with parity errors
    //    ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
    //    PARENB - Parity enable
    //    PARODD - Odd parity (else even)
    struct termios options;
    tcgetattr(uart0_filestream, &options);
    OLD_TERMIO_OPTIONS = options;
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;        //<Set baud rate
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);

    return uart0_filestream;
}

int uart_close(int fd)
{
    //----- CLOSE THE UART -----
    tcsetattr(fd, TCSANOW, &OLD_TERMIO_OPTIONS);
    if (fd >= 0) close(fd);
}

int uart_read(int fd, unsigned char rx_buf[], int buf_len)
{
    int count = 0;
    if (fd < 0)
    {
        return 0;
    }
    //----- CHECK FOR ANY RX BYTES -----
    // Read up to 255 characters from the port if they are there
    count = read(fd, (void*)rx_buf, buf_len);        //Filestream, buffer to store in, number of bytes to read (max)
    if (count < 0)
    {
        //An error occured (will occur if there are no bytes)
        printf("%s: fail due to %s(%d)\n", __FUNCTION__, strerror(errno), errno);
    }
    else if (count == 0)
    {
        //No data waiting
        printf("%s: no data waiting\n", __FUNCTION__);
    }
    else
    {
        //Bytes received
        printf("%i bytes read\n", count);
    }
    return count;
}

int uart_write(int fd, unsigned char tx_buf[], int buf_len)
{
    //----- TX BYTES -----
    if (fd < 0)
    {
        return 0;
    }
    int count = write(fd, tx_buf, buf_len);        //Filestream, bytes to write, number of bytes to write
    if (count < 0)
    {
        printf("UART TX error\n");
        return -1;
    }
    return count;
}

int main(int argc, char* argv[])
{
    int fd = uart_open();
    if (fd < 0) return -1;

    unsigned char rx_buf[9];
    uart_read(fd, rx_buf, sizeof(rx_buf));

    uart_close(fd);
}
