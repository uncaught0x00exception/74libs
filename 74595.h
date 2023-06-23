/*
    20 jun 2023
    
    This file do work with parallel-output 8-channel shift registr (SR) 74595 easer 
    and give the opportunity to create the class-object of SR.
*/

#pragma once

#include <Arduino.h>

typedef unsigned short digit;

class SR_74595
{
/*
    Object SR_74595 operates with the variable _address which stores the byte-form value of SR's channels (Q0 - Q7) which will be activated.
    The last digit of address related to the last channel (Q7), the first digit - to the first channel (Q0);
    If digit is 1 then channel - related to this digit - will be activated (will have HIGH voltage).
    If digit is 0 then channel will be inactivated (will have LOW voltage).
    Below, you can study the functions of SR_74595 class-object.
*/
public:
    explicit SR_74595(digit _data_pin, digit _clock_pin, digit _latch_pin) : _data_pin(_data_pin), _clock_pin(_clock_pin), _latch_pin(_latch_pin) 
    {
        pinMode(_data_pin, OUTPUT);
        pinMode(_clock_pin, OUTPUT);
        pinMode(_latch_pin, OUTPUT);
    }

    SR_74595* activateChannel(digit dec)
    {
        /*
            Activate channel with number dec.
        */

        bitSet(this->_address, dec);
        
        return this;
    }

    SR_74595* inactivateChannel(digit dec)
    {
        /*
            Inactivate channel with number dec.
        */

        bitClear(this->_address, dec);
        
        return this;
    }

    SR_74595* setAddress(byte new_address)
    {
        /*
            Set new_address as _address. 
        */

        this->_address = new_address;

        return this;
    }

    SR_74595* channelState(digit dec, bool val)
    {
        /*
            Set the state at channel with number dec.
            val == true - to activate channel, val == false - to inactivate channel.
        */

        return (val ? this->activateChannel(dec) : this->inactivateChannel(dec));
    }

    SR_74595* clearAddress()
    {
        /*
            Clear the _address of SR.
        */

        this->_address = 00000000;
        
        return this;
    }

    void send()
    {
        /*
            Send set _address to SR.
        */

        digitalWrite(this->_latch_pin, LOW);
        shiftOut(this->_data_pin, _clock_pin, LSBFIRST, this->_address);
        digitalWrite(this->_latch_pin, HIGH);
    }

    void drop()
    {
        /*
            Clear _address and send it.
        */

        this->clearAddress()->send();
    }

private:
    digit _data_pin;
    digit _clock_pin;
    digit _latch_pin;
    byte _address = 00000000;
};