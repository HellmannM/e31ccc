#include <cstdint>
#include <csignal> // SIGINT
#include <fcntl.h> // O_RDONLY, O_NOCTTY
#include <iomanip>
#include <iostream>
#include <unistd.h> // read, close

#include "wheel.h"

bool running = true;
int err_code = 0;

void sigint_handler(int signum)
{
    err_code = signum;
    running = false;
}

int main()
{
    // set signal callback
    signal(SIGINT, sigint_handler);

    std::string port = "/dev/ttyS8";
    int device = open(port.c_str(), O_RDONLY | O_NOCTTY);

    wheel w;
    uint8_t buff;
    int bytes = 0;
    while (running)
    {
        bytes = read(device, &buff, 1);
        if (bytes > 0)
        {
            switch (buff)
            {
                case 0x7f :
                {
                    // 0x7f : idle. do nothing
                    break;
                }
                case 0x61 :
                {
                    // 617f : power on/off
                    bytes = read(device, &buff, 1);
                    if (bytes <= 0) break;
                    if (buff == 0x7f)
                        w.power.event();
                    break;
                }
                case 0xa3 :
                {
                    // a3a3 a3a3 : minus
                    for (int i=0; i<3; ++i)
                    {
                        bytes = read(device, &buff, 1);
                        if (bytes <= 0){std::cout<<"read error"<<std::endl; break;}
                        if (buff != 0xa3) break;
                        if (i == 2)
                            w.minus.event();
                    }
                    break;
                }
                case 0x63 :
                {
                    // 633f : reset
                    // 6161 6363 7f : plus
                    bytes = read(device, &buff, 1);
                    if (bytes <= 0){std::cout<<"read error"<<std::endl; break;}
                    if (buff == 0x3f)
                        w.reset.event();
                    else if (buff == 0x61)
                        w.plus.event();
                    break;
                }
                case 0x3f :
                {
                    // 3f63 : reset
                    bytes = read(device, &buff, 1);
                    if (bytes <= 0){std::cout<<"read error"<<std::endl; break;}
                    if (buff == 0x63)
                        w.reset.event();
                    break;
                }
                default :
                {
                    std::cout << "\nunexpected symbol: 0x"
                        << std::hex << std::setfill('0') << std::setw(2)
                        << (unsigned int) buff << std::endl;
                    break;
                }
            }
        }
    }

    close(device);

    return err_code;
}
