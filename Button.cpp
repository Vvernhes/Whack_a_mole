#include "Button.h"

Button::Button(const char LED, const char but, const char sign, int pos) : _LED(LED), _button(but), _signal(sign), _position(pos)
{
    _timerLightOn = 0;
    _lastTimePush = 0;
}

Button::~Button(void) {}


/*
**  isGood function just check if the button match the LED tunr on
*/

bool    Button::isGood(int posLight) const
{
    if (_position == posLight && _timerLightOn != 0)
        return (true);
    else
        return (false);
}

void    Button::turnOffLight() const
{
    digitalWrite(_LED, HIGH);
    _timerLightOn = 0;
}

void    Button::turnOnLight() const
{
    _timerLightOn = millis();
    digitalWrite(_LED, LOW);
}

/*
**  timerLightPast() function just check if LED is light on more 
**  than MAXLEDONTIMER
*/

bool    Button::timerLightPast() const
{
    if (_timerLightOn != 0 && millis() - _timerLightOn < MAXLEDONTIMER)
    {
        turnOffLight();
        return (true);
    }
    else 
        return (false);
}

void    Button::sendSign() const
{
    digitalWrite(_signal, LOW);
    digitalWrite(_siganl, HIGH);
}

bool    Button::timerPush()  const
{
    if (millis() - _lastTimePush > INCORRECTINTERVAL)
    {
        _lastTimePush = millis();
        return (true);
    }
    else
        return (false);
}

const char  Button::getButton() const
{
    return (_button);
}