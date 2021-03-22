#pragma once

enum class read_event
{
    error = -1,
    noop,
    power,
    reset,
    plus,
    minus,
    unknown
};

enum class write_event
{
    noop = 0,
    poweron,
    poweroff,
    reset,
    plus,
    minus
};
