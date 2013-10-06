/*
 ******************************* C HEADER FILE *******************************
 **                                                                         **
 ** Gpio.h - 
 **                                                                         **
 ** 06/20/2013 09:34:55 AM                                                  **
 **                                                                         **
 ** Copyright (c) 2013, Kechang Dong                                        **
 ** All rights reserved.                                                    **
 **                                                                         **
 *****************************************************************************
 */
#ifndef __GPIO_HEADER__
#define __GPIO_HEADER__


namespace Zebra {


class Gpio 
    /// The gpio port class, support the normal gpio operation under linux
    /// Interrupt feature is not support currently
{
public:
    enum Direction{
        GPIO_IN,
        GPIO_OUT
    };

	Gpio(const std::string& pin, const Direction& direction);
        /// Create the gpio pin with the direction

    ~Gpio();
        
	bool init();
        /// initialize the gpio pin
        
	void set();
        /// Set the pin to high
        
	void reset();
        /// Set the pin to low
        
	bool read() const;
        /// read the state of pin
        
private:
	void setDirection(const Direction& direction);
        /// set direction of the gpio pin
        
	Direction getDirection() const;
        /// get direction of the gpio pin
        
private:
    std::string _pin;
	Direction _direction;
};


} // namespace zebra


#endif
