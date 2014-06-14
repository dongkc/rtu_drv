#include <fstream>
#include <boost/lexical_cast.hpp>
#include "Gpio.h"

using namespace std;

namespace Zebra {

Gpio::Gpio(const std::string& pin, const Direction& direction):
    _pin(pin), _direction(direction)
{
};

Gpio::~Gpio()
{
    ofstream of("/sys/class/gpio/unexport");
    of << _pin << endl;
    of.close();
}

bool Gpio::init()
{
    ofstream of("/sys/class/gpio/export");
    if (!of) {
        return false;
    }

    of << _pin << endl;
    of.close();

    int num = boost::lexical_cast<int>(_pin);
    if (num < 32) {
        _sys_pin = "pioA" + _pin;
    } else if (num < 64) {
        _sys_pin = "pioB" + boost::lexical_cast<string>(num % 32);
    } else if (num < 96) {
        _sys_pin = "pioC" + boost::lexical_cast<string>(num % 32);
    } else if (num < 128) {
        _sys_pin = "pioD" + boost::lexical_cast<string>(num % 32);
    }

    setDirection(_direction);

    return true;
}

void Gpio::set()
{
    string path("/sys/class/gpio/" + _sys_pin + "/value");
    ofstream of(path.c_str());

    of << "1";

    of.close();
}

void Gpio::reset()
{
    string path("/sys/class/gpio/" + _sys_pin + "/value");
    ofstream of(path.c_str());

    of << "0";

    of.close();
}

bool Gpio::read() const
{
    string path("/sys/class/gpio/" + _sys_pin + "/value");
    ifstream f(path.c_str());
    char data;

    f >> data;

    f.close();

    return data == '1' ? true : false;
}

void Gpio::setDirection(const Direction& direction)
{
    string path("/sys/class/gpio/" + _sys_pin + "/direction");
    ofstream of(path.c_str());

    _direction = direction;
    switch (_direction) {
        case GPIO_IN:
            of << "in";
            break;
        default:
            of << "out";
    }

    of.close();
}

Gpio::Direction Gpio::getDirection() const
{
    return _direction;
}


}  // namespace Zebra

