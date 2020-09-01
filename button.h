// was passiert wenn ein knopf gedrueckt gehalten wird? immer wieder selbes zeichen oder start+endsequenz?
// abhaengig davon entweder on_press() + on_release() oder einfach nur pressed() mit check wann das letzte mal aufgerufen wurde.

struct button
{
    virtual void on_press();
    virtual void on_release();
};

struct plusbutton : public button
{
//    std::chrono::time_point<std::chrono::system_clock> timestamp;
    void on_press(){}
    void on_release(){}
};

struct minusbutton : public button
{
    void on_press(){}
    void on_release(){}
};

struct powerbutton : public button
{
    bool power = false;

    void on_press()
    {
        // switch state
        power = !power;
        if (power)
        {
            // send signal to switch on
        }
        else
        {
            // send signal to switch off
        }
    }

    void on_release(){}
};

struct resetbutton : public button
{
    void on_press()
    {
        // send signal to reset speed
    }

    void on_release(){}
};
