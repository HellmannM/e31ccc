#include <chrono>
#include <iostream>

#define DELAY_FAST 0.2
#define DELAY_SLOW 0.5

class button
{
public:
    button(double d)
    : timestamp(std::chrono::system_clock::now()),
      delay((std::chrono::duration<double>)d)
    {}

    bool check_delay()
    {
        return std::chrono::system_clock::now() - timestamp > delay;
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
