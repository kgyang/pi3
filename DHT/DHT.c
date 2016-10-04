#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <bcm2835.h>

#define DHT_GPIO_OUTPUT bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP) 
#define DHT_GPIO_INPUT bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT)
#define DHT_GPIO_PUDUP bcm2835_gpio_set_pud(pin, BCM2835_GPIO_PUD_UP)
#define DHT_GPIO_SET bcm2835_gpio_set(pin)
#define DHT_GPIO_CLR bcm2835_gpio_clr(pin)
#define DHT_GPIO_READ bcm2835_gpio_lev(pin)
#define DHT_wait_ms(ms) bcm2835_delay(ms)
#define DHT_wait_us(us) bcm2835_delayMicroseconds(us)
#define DHT_GPIO_HIGH HIGH
#define DHT_GPIO_LOW LOW

typedef enum {
    DHT11   = 11,
    DHT21   = 21,
    DHT22   = 22,
    AM2302  = 22,
} DHT_Type;

typedef enum {
    DHT_ERROR_NONE = 0,
    DHT_ERROR_BUS_BUSY = 1,
    DHT_ERROR_NOT_PRESENT =2,
    DHT_ERROR_TIMEOUT = 3,
    DHT_ERROR_CHECKSUM = 4,
    DHT_ERROR_INVALID_PARAM = 5,
} DHT_Error;

int DHT_debug = 0;

int DHT_read(DHT_Type type, uint8_t pin, float* humidity, float* temperature)
{
    int count;
    int wait_ack_count;
    int ack_count;
    int data_ready_count;
    int bit_start_count[40];
    int bit_level_count[40];
    int bit;
    unsigned char sum;
    unsigned char data[5] = {0,0,0,0,0};

    // Send the activate pulse
    // Step 1: MCU send out start signal to DHT22 and DHT22 send
    //         response signal to MCU.
    // If always signal high-voltage-level, it means DHT22 is not 
    // working properly, plesee check the electrical connection status.
    DHT_GPIO_OUTPUT;
    DHT_wait_us(10); //20 us
    DHT_GPIO_CLR; //MCU send out start signal to DHT22
    DHT_wait_ms(1); //1 ms
    DHT_GPIO_SET; //MCU pull up
    DHT_wait_us(10); //20us
    DHT_GPIO_INPUT;
    //DHT_GPIO_PUDUP;

    // Find the start of the ACK Pulse
    count = 0;
    while(DHT_GPIO_READ == DHT_GPIO_HIGH)     // Exit on DHT22 pull low within 80us 
    {        
        if (count++ > 10)
	{
            fprintf(stderr,"no ACK\n");
            return DHT_ERROR_NOT_PRESENT;
        }
        DHT_wait_us(2);
    }
    wait_ack_count = count;
    
    // Find the last of the ACK Pulse
    count = 0;
    while(DHT_GPIO_READ == DHT_GPIO_LOW)     // Exit on DHT22 pull High within 80us 
    {        
        if (count++ > 50)
	{
            fprintf(stderr,"ACK pulse too long\n");
            return DHT_ERROR_NOT_PRESENT;
        }
        DHT_wait_us(2);
    }
    ack_count = count;
    
    // wait DHT pull low to sent the first bit.
    count = 0;
    while(DHT_GPIO_READ == DHT_GPIO_HIGH)     // Exit on DHT22 pull low  within 80us 
    {        
        if (count++ > 50)
	{
            fprintf(stderr,"no data sent\n");
            return DHT_ERROR_NOT_PRESENT;
        }
        DHT_wait_us(2);
    }
    data_ready_count = count;
    
     
    // Reading the 40 bit data stream
    // Step 2: DHT22 send data to MCU
    //         Start bit -> low volage within 50us
    //         0         -> high volage within 26-28 us
    //         1         -> high volage within 70us
    for (bit = 0; bit < 40; bit++)
    {
        // Getting start bit signal 
        count = 0;
	while(DHT_GPIO_READ == DHT_GPIO_LOW)        // Exit on high volage within 50us
	{        
            if (count++ > 40)
	    {
                fprintf(stderr,"bit %d start timeout\n", bit);
                return DHT_ERROR_TIMEOUT;
            }
            DHT_wait_us(2);
        }
	bit_start_count[bit] = count;
        
        // Measure the width of the data pulse
        count = 0;
	while(DHT_GPIO_READ == DHT_GPIO_HIGH)        // Exit on high volage within 50us
	{
            if (count++ > 50)
	    {
                fprintf(stderr,"bit %d width timeout\n", bit);
                return DHT_ERROR_TIMEOUT;
            }
            DHT_wait_us(2);
        }
	bit_level_count[bit] = count;

        if(count > 20)
	{
	    // connt > 20 is a 1(20*2 = 40us)
            data[bit/8] |= (1 << (7 - bit%8)); // start from MSB to LSB
        }
    }
    
    sum = data[0]+data[1]+data[2]+data[3];
    if(data[4] != sum)
    {
        fprintf(stderr,"error checksum %d %d %d %d %d %d \n",
                data[0], data[1], data[2], data[3], data[4], sum);
        return DHT_ERROR_CHECKSUM;
    }
    else
    {
        if (DHT_debug)
        {
            fprintf(stderr, "wait ack   %2d\n", wait_ack_count);
            fprintf(stderr, "ack        %2d\n", ack_count);
            fprintf(stderr, "data ready %2d\n", data_ready_count);
	    for (bit = 0; bit < 40; bit++)
	    {
                fprintf(stderr, "bit %2d start %2d level %2d\n",
	                bit, bit_start_count[bit], bit_level_count[bit]);
            }
        }
    }

    switch(type)
    {
        case DHT11:
            *humidity = (float)data[0];
            *temperature = (float)data[2];
            break; 

        case DHT21:
        case DHT22:
            *humidity = (float)((data[0] << 8)+data[1])/10;
            if((data[2]&0x80) == 0)
            {
                *temperature = (float)((data[2] << 8)+data[3])/10;
            }
            else
            {
                *temperature = -((float)(((data[2]&0x7F) << 8)+data[3])/10);
            }
            break;
       
        default:
            break;
    }
   
    return DHT_ERROR_NONE;
}

DHT_Error DHT_boardPinToBcmPin(int board_pin, RPiGPIOPin* bcm_pin)
{
    int bcm_pins[40+1];

    memset(bcm_pins, -1, sizeof(bcm_pins));

    bcm_pins[3] = RPI_BPLUS_GPIO_J8_03;
    bcm_pins[5] = RPI_BPLUS_GPIO_J8_05;
    bcm_pins[7] = RPI_BPLUS_GPIO_J8_07;
    bcm_pins[8] = RPI_BPLUS_GPIO_J8_08;
    bcm_pins[10] = RPI_BPLUS_GPIO_J8_10;
    bcm_pins[11] = RPI_BPLUS_GPIO_J8_11;
    bcm_pins[12] = RPI_BPLUS_GPIO_J8_12;
    bcm_pins[13] = RPI_BPLUS_GPIO_J8_13;
    bcm_pins[15] = RPI_BPLUS_GPIO_J8_15;
    bcm_pins[16] = RPI_BPLUS_GPIO_J8_16;
    bcm_pins[18] = RPI_BPLUS_GPIO_J8_18;
    bcm_pins[19] = RPI_BPLUS_GPIO_J8_19;
    bcm_pins[21] = RPI_BPLUS_GPIO_J8_21;
    bcm_pins[22] = RPI_BPLUS_GPIO_J8_22;
    bcm_pins[23] = RPI_BPLUS_GPIO_J8_23;
    bcm_pins[24] = RPI_BPLUS_GPIO_J8_24;
    bcm_pins[26] = RPI_BPLUS_GPIO_J8_26;
    bcm_pins[29] = RPI_BPLUS_GPIO_J8_29;
    bcm_pins[31] = RPI_BPLUS_GPIO_J8_31;
    bcm_pins[32] = RPI_BPLUS_GPIO_J8_32;
    bcm_pins[33] = RPI_BPLUS_GPIO_J8_33;
    bcm_pins[35] = RPI_BPLUS_GPIO_J8_35;
    bcm_pins[36] = RPI_BPLUS_GPIO_J8_36;
    bcm_pins[37] = RPI_BPLUS_GPIO_J8_37;
    bcm_pins[38] = RPI_BPLUS_GPIO_J8_38;
    bcm_pins[40] = RPI_BPLUS_GPIO_J8_40;

    if (board_pin < 1 || board_pin > 40)
    {
        return DHT_ERROR_INVALID_PARAM;
    }

    if (bcm_pins[board_pin] == -1)
    {
        return DHT_ERROR_INVALID_PARAM;
    }

    *bcm_pin = bcm_pins[board_pin];

    return DHT_ERROR_NONE;
}

void usage(void)
{
    fprintf(stderr, "DHT <bcm|board> <pin>\n");
    fprintf(stderr, "bcm pin range: 2-26\n");
    fprintf(stderr, "board pin range: 1-40\n");
}

int main(int argc, char* argv[])
{
    DHT_Type type = DHT22;
    RPiGPIOPin pin = RPI_BPLUS_GPIO_J8_03;
    float humidity = 0, temperature = 0;

    if (argc != 3)
    {
        usage();
        return DHT_ERROR_INVALID_PARAM;
    }

    if (strcasecmp(argv[1], "bcm") == 0)
    {
        pin = (RPiGPIOPin)atoi(argv[2]);
        if (pin < 2 || pin > 26)
        {
            usage();
            return DHT_ERROR_INVALID_PARAM;
        }
    }

    if (strcasecmp(argv[1], "board") == 0)
    {
        if (DHT_boardPinToBcmPin(atoi(argv[2]), &pin) != DHT_ERROR_NONE)
        {
            usage();
            return DHT_ERROR_INVALID_PARAM;
        }
    }

    if (!bcm2835_init())
    {
        fprintf(stderr,"bcm init fail\n");
	return(1);
    }

    DHT_Error ret = DHT_read(type, pin, &humidity, &temperature);
    if (ret != DHT_ERROR_NONE)
    {
        bcm2835_close();
        return(1);
    }

    if (!bcm2835_close())
    {
        fprintf(stderr,"bcm close fail\n");
	return(1);
    }

    printf("temperature %.2f humidity %.2f\n", temperature, humidity);

    return(0);
}

