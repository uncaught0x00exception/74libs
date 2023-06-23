/*
    20 jun 2023
    
    This file do work with 8-channel multiplexor (MX) 74152 easer 
    and give the opportunity to create the class-object of MX.
*/

#pragma once

#include <Arduino.h>

typedef unsigned short digit;

class MX_74152
{
/*
    Object MX_74152 operates with the array _address_pins which stores pins of MX's address bus.
    You can set either decimal or binary value as number of pin which will be read via pin _data_pin.
    Below, you can study the functions of MX_74152 class-object.
*/
public:
    explicit MX_74152(digit _data_pin, digit _address_pin_Start) : _data_pin(_data_pin)
    {
        pinMode(_data_pin, INPUT);
        
        for (digit i = 0; i < MX_74152::__ADDRESS_BUS_SIZE__; i++)
        {
            this->_address_pins[i] = _address_pin_Start + i;

            pinMode(_address_pin_Start + i, OUTPUT);
        }
    }

    explicit MX_74152(digit _data_pin, digit _address_pin_0, digit _address_pin_1, digit _address_pin_2) : _data_pin(_data_pin)
    {
        this->_address_pins[0] = _address_pin_0;
        this->_address_pins[1] = _address_pin_1;
        this->_address_pins[2] = _address_pin_2;

        pinMode(_data_pin, INPUT);
        pinMode(_address_pin_0, OUTPUT);
        pinMode(_address_pin_1, OUTPUT);
        pinMode(_address_pin_2, OUTPUT);
    }

    MX_74152* set(digit address2, digit address1, digit address0)
    {
        /*
            Send address2.address1.address0 as address.
        */

        digitalWrite(this->_address_pins[0], address0);
        digitalWrite(this->_address_pins[1], address1);
        digitalWrite(this->_address_pins[2], address2);

        return this;
    }

    MX_74152* set(digit decimal)
    {
        /*
            Send converted decimal value as address.
        */

        int* address = new int[3];
        MX_74152::_toBin(address, decimal);

        set(address[2], address[1], address[0]);
        delete address;

        return this;
    }

    int read()
    {
        /*
            Read set pin via pin _data_pin.
        */

        return (digitalRead(this->_data_pin) == HIGH ? LOW : HIGH);
    }

    void recordBus(int* target_arr)
    {
        /*
            Read whole address bus of MX and record it to target_arr.
        */

        int n = 0;
        for (digit i = 0; i < 2; i++)
            for (digit j = 0; j < 2; j++)
                for (digit k = 0; k < 2; k++)
                {
                    target_arr[n] = this->set(i, j, k)->read();
                    n++;
                }
    }

    void drop()
    {
        /*
            Set 000 as address.
        */

        for (digit i = 0; i < MX_74152::__ADDRESS_BUS_SIZE__; i++)
            digitalWrite(this->_address_pins[i], LOW);
    }

private:
    static void _toBin(int* arr, int dec)
    {
        /*
            Convert dec to binary and record it arr.
        */

        unsigned int i = 0;
        while (dec > 0)
        {
            arr[i] = dec % 2;
            dec /= 2;
            i++;
        }
        for (; i < __ADDRESS_BUS_SIZE__; i++)
            arr[i] = 0;
    }

    digit _data_pin;
    digit _address_pins[3];
    static const digit __ADDRESS_BUS_SIZE__ = 3;
};