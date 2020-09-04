#include "event.h"

class output_stream
{
public:
    void write_sequence(write_event& e)
    {
        switch (e)
        {
            case write_event::noop :
            {
                break;
            }
            case write_event::poweron :
            {
                std::cout << "power on" << std::endl;
                break;
            }
            case write_event::poweroff :
            {
                std::cout << "power off" << std::endl;
                break;
            }
            case write_event::reset :
            {
                std::cout << "reset" << std::endl;
                break;
            }
            case write_event::plus :
            {
                std::cout << "plus" << std::endl;
                break;
            }
            case write_event::minus :
            {
                std::cout << "minus" << std::endl;
                break;
            }
        }
    }

};
