#include <cstdint> // uint8_t
#include <unistd.h> // read, close

#include "event.h"

class input_stream
{
public:
    input_stream(int& d)
    : device(d) {}

    ~input_stream()
    {
        close(device);
    }

    read_event read_next_sequence()
    {
        uint8_t buff;
        int bytes = 0;
        bytes = read(device, &buff, 1);
        if (bytes > 0)
        {
            switch (buff)
            {
                case 0x7f :
                    {
                        // 0x7f : idle. do nothing
                        return read_event::noop;
                    }
                case 0x61 :
                    {
                        // 617f : power on/off
                        bytes = read(device, &buff, 1);
                        if (bytes <= 0)
                            return read_event::error;
                        if (buff != 0x7f)
                            return read_event::unknown;
                        return read_event::power;
                    }
                case 0xa3 :
                    {
                        // a3a3 a3a3 : minus
                        for (int i=0; i<3; ++i)
                        {
                            bytes = read(device, &buff, 1);
                            if (bytes <= 0)
                                return read_event::error;
                            if (buff != 0xa3)
                                return read_event::unknown;
                        }
                        return read_event::minus;;
                    }
                case 0x63 :
                    {
                        // 633f : reset
                        // 6161 6363 7f : plus
                        bytes = read(device, &buff, 1);
                        if (bytes <= 0)
                            return read_event::error;
                        if (buff == 0x3f)
                            return read_event::reset;
                        else if (buff == 0x61)
                            return read_event::plus;
                        return read_event::unknown;
                    }
                case 0x3f :
                    {
                        // 3f63 : reset
                        bytes = read(device, &buff, 1);
                        if (bytes <= 0)
                            return read_event::error;
                        if (buff != 0x63)
                            return read_event::unknown;
                        return read_event::reset;
                    }
                default :
                    {
                        std::cout << "\nunexpected symbol: 0x"
                            << std::hex << std::setfill('0') << std::setw(2)
                            << (unsigned int) buff << std::endl;
                        return read_event::unknown;
                    }
            }
        }
        return read_event::error; // should be unreachable
    }

private:
    int device;
};
