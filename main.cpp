#include <fcntl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "button.h"

#define BUFF_SIZE 256

void print_buffer(uint8_t* buffer, size_t size)
{
    std::cout << std::hex << std::setfill('0');
    for (int i=0; (16*i)<size; ++i)
    {
        for (int k=0; k<16; ++k)
        {
            if ( k % 4 == 0 ) std::cout << ' ';
            std::cout << std::setw(2) << (unsigned int) buffer[16*i + k];
        }
        std::cout << std::endl;
    }
    std::cout << std::dec << std::endl;
}

int main()
{
    std::string port = "/dev/ttyS8";
    int device = open(port.c_str(), O_RDONLY | O_NOCTTY);

    uint8_t buff[BUFF_SIZE];
    int bytes = 0;
    while ( bytes < BUFF_SIZE )
    {
        bytes += read(device, buff + bytes, 1);
    }

    std::cout << "read " << bytes << " bytes." << std::endl;

    print_buffer(buff, bytes);

    close(device);

    return 0;
}
