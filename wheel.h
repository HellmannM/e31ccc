#include <chrono>
#include <iostream>

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

    virtual void event() = 0;
 
private:
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    std::chrono::duration<double> delay;
};

class button_plus : public button
{
public:
    button_plus() : button(DELAY_FAST) {}

    void event()
    {
        if (check_delay())
        {
            std::cout << '+' << std::endl;
            update_timestamp();
        }
    }
};

class button_minus : public button
{
public:
    button_minus() : button(DELAY_FAST) {}

    void event()
    {
        if (check_delay())
        {
            std::cout << '-' << std::endl;
            update_timestamp();
        }
    }
};

class button_power : public button
{
public:
    button_power()
    : button(DELAY_SLOW), powerstate(false) {}

    void event()
    {
        if (check_delay())
        {
            powerstate != powerstate;
            if (powerstate)
                std::cout << "p-on" << std::endl;
            else
                std::cout << "p-off" << std::endl;
            update_timestamp();
        }
    }

private:
    bool powerstate;
};

class button_reset : public button
{
public:
    button_reset()
    : button(DELAY_SLOW) {}

    void event()
    {
        if (check_delay())
        {
            std::cout << 'r' << std::endl;
            update_timestamp();
        }
    }
};

struct wheel
{
    button_plus plus;
    button_minus minus;
    button_power power;
    button_reset reset;
};
