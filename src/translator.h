#include <chrono>
#include <iostream>

#include "event.h"

// activation delay in ms
#define DELAY_FAST 200
#define DELAY_SLOW 500

class button
{
public:
    button(unsigned int d)
    : timestamp(std::chrono::system_clock::now()),
      delay(std::chrono::milliseconds(d))
    {}

    bool check_delay()
    {
        std::chrono::duration<double> diff = std::chrono::system_clock::now() - timestamp;
        return diff > delay;
    }

    void update_timestamp()
    {
        timestamp = std::chrono::system_clock::now();
    }

    virtual write_event process() = 0;
 
private:
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    std::chrono::duration<double> delay;
};

class button_plus : public button
{
public:
    button_plus() : button(DELAY_FAST) {}

    write_event process()
    {
        if (check_delay())
        {
            update_timestamp();
            return write_event::plus;
        }
        return write_event::noop;
    }
};

class button_minus : public button
{
public:
    button_minus() : button(DELAY_FAST) {}

    write_event process()
    {
        if (check_delay())
        {
            update_timestamp();
            return write_event::minus;
        }
        return write_event::noop;
    }
};

class button_power : public button
{
public:
    button_power()
    : button(DELAY_SLOW), powerstate(false) {}

    write_event process()
    {
        if (check_delay())
        {
            powerstate = !powerstate;
            if (powerstate)
            {
                update_timestamp();
                return write_event::poweron;
            }
            else
            {
                update_timestamp();
                return write_event::poweroff;
            }
        }
        return write_event::noop;
    }

private:
    bool powerstate;
};

class button_reset : public button
{
public:
    button_reset()
    : button(DELAY_SLOW) {}

    write_event process()
    {
        if (check_delay())
        {
            update_timestamp();
            return write_event::reset;
        }
        return write_event::noop;
    }
};

class event_handler
{
public:
    write_event process(read_event e)
    {
        switch (e)
        {
            case read_event::noop :
                return write_event::noop;
            case read_event::power :
                return power.process();
            case read_event::reset :
                return reset.process();
            case read_event::plus :
                return plus.process();
            case read_event::minus :
                return minus.process();
            case read_event::error :
            {
                std::cout << "ERROR: couldn't read input stream" << std::endl;
                return write_event::noop;
            }
            case read_event::unknown :
            {
//                std::cout << "Warning: read unknown symbol" << std::endl;
                return write_event::noop;
            }
        }
        return write_event::noop; // should be unreachable
    }

private:
    button_plus plus;
    button_minus minus;
    button_power power;
    button_reset reset;
};
