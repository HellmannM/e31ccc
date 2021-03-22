#include <cstdint> // uint8_t
#include <fcntl.h> // O_RDONLY, O_NOCTTY
#include <sys/types.h> // open
#include <sys/stat.h> // open
#include <unistd.h> // read, close

#include "event.h"

class input_stream
{
public:
    input_stream(std::string& fd)
    : device(open(fd.c_str(), O_RDONLY | O_NOCTTY)) {}

    ~input_stream()
    {
        close(device);
    }

    read_event read_next_sequence()
    {
        // 0xf0 : start of new message
        // 0xfc : start of new message
        // 0x0c1c 701c : reset
        // 0x1c0c 1c70 : power
        // 0x1c1c 1c0c : minus
        // 0x1c1c 1c1c : idle
        // 0x1c70 0c1c : plus

        constexpr uint32_t v_reset = 0x0c1c701c;
        constexpr uint32_t v_power = 0x1c0c1c70;
        constexpr uint32_t v_minus = 0x1c1c1c0c;
        constexpr uint32_t v_idle  = 0x1c1c1c1c;
        constexpr uint32_t v_plus  = 0x1c700c1c;

        uint8_t buff;
        int bytes = 0;
        bytes = read(device, &buff, 1);
        if (bytes > 0)
        {
            if (buff == 0xf0 ||
                buff == 0xfc)
            {
                // 0xf0 : start of new message
                // 0xfc : start of new message
                // read next 4 bytes and compare to messages
                uint32_t message;
                uint8_t m[4];
                int b = 4;
                while (b != 0)
                {
                    b -= read(device, m+(b-1), 1);
                }
                message = *reinterpret_cast<uint32_t*>(m);
                switch (message)
                {
                    case v_reset :
                        return read_event::reset;
                    case v_power :
                        return read_event::power;
                    case v_minus :
                        return read_event::minus;
                    case v_idle :
                        return read_event::noop;
                    case v_plus :
                        return read_event::plus;
                    default :
                        return read_event::unknown;
                }
            }
            else
            {
                //std::cout << "\nunexpected symbol: 0x"
                //    << std::hex << std::setfill('0') << std::setw(2)
                //    << (unsigned int) buff << std::endl;
                return read_event::unknown;
            }
        }
        return read_event::error;
    }

private:
    int device;
};
