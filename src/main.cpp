#include <cstdint>
#include <csignal> // SIGINT
#include <iomanip>
#include <iostream>

#include "event.h"
#include "read.h"
#include "translator.h"
#include "write.h"

bool running = true;
int err_code = 0;

void sigint_handler(int signum)
{
    err_code = signum;
    running = false;
}

int main()
{
    // set SIGINT signal callback
    signal(SIGINT, sigint_handler);

    std::string fd = "/dev/ttyUSB0";
    input_stream input(fd);
    event_handler h;
    output_stream output;

    while (running)
    {
        read_event re = input.read_next_sequence();
        write_event we = h.process(re);
        output.write_sequence(we);
    }

    return err_code;
}
