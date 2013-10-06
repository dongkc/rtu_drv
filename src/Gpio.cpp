//
// Gpio.cpp
//
//

#include <fstream>

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

    setDirection(_direction);

    return true;
}

void Gpio::set()
{
    string path("/sys/class/gpio/gpio" + _pin + "/value");
    ofstream of(path.c_str());

    of << "1";

    of.close();
}

void Gpio::reset()
{
    string path("/sys/class/gpio/gpio" + _pin + "/value");
    ofstream of(path.c_str());

    of << "0";

    of.close();
}

void Gpio::setDirection(const Direction& direction)
{
    string path("/sys/class/gpio/gpio" + _pin + "/direction");
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

