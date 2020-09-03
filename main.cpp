#include <cstdint>
#include <csignal> // SIGINT
#include <fcntl.h> // O_RDONLY, O_NOCTTY
#include <iomanip>
#include <iostream>
#include <unistd.h> // read, close

bool running = true;
size_t err_num = 0;

void sigint_handler(int signum)
{
    err_num = signum;
    running = false;
}

int main()
{
    // set signal callback
    signal(SIGINT, sigint_handler);

    std::string port = "/dev/ttyS8";
    int device = open(port.c_str(), O_RDONLY | O_NOCTTY);

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
                    //6161 6363 7f : plus
                    //617f : on/off
                    bytes = read(device, &buff, 1);
                    if (bytes <= 0){std::cout<<"read error"<<std::endl; break;}
                    switch (buff)
                    {
                        case 0x7f :
                        { // power on/off
                            std::cout << "p";
                            break;
                        }
//TODO DEBUG
                        case 0x63 :
                        case 0x61 :
                        { // possibly plus
//                            bytes = read(device, &buff, 1);
//                            if (bytes <= 0){std::cout << "ERROR read 0 bytes\n"; break;}
//                            if (buff != 0x63) break;
//                            bytes = read(device, &buff, 1);
//                            if (bytes <= 0){std::cout << "ERROR read 0 bytes\n"; break;}
//                            if (buff != 0x63) break;
                            std::cout << "+";
                            break;
                        }
                        default:
                        {
                            std::cout<<" found 0x61 0x"<<std::hex<<(unsigned int)buff<<std::endl;
                            break;
                        }
                    }
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
                        if (i == 2) std::cout << "-";
                    }
                    break;
                }
                case 0x63 :
                {
                    // 633f : reset
                    bytes = read(device, &buff, 1);
                    if (bytes <= 0){std::cout<<"read error"<<std::endl; break;}
                    if (buff != 0x3f) break;
                    std::cout << "r";
                    break;
                }
                case 0x3f :
                {
                    // 3f63 : reset
                    bytes = read(device, &buff, 1);
                    if (bytes <= 0){std::cout<<"read error"<<std::endl; break;}
                    if (buff != 0x63) break;
                    std::cout << "r";
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

    return err_num;
}
