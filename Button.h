#ifndef BUTTON_H
# define BUTTON_H
#include <Arduino.h>

# define MAXLEDONTIMER 2000
# define INCORRECTINTERVAL 400

class  Button
{
    public:
        Button(const char LED, const char but, const char sign, int pos);
        ~Button(void);

        bool        isGood(int posLight) const;
        bool        timerLightPast() const;
        void        turnOffLight() const;
        void        turnOnLight() const;
        void        sendSign() const;
        bool        timerPush() const;
        const char  getButton() const;
    
    private:
        const char      _LED;
        const char      _button;
        const char      _signal;
        const int       _position;
        unsigned long   _timerLightOn;
        unsigned long   _lastTimePush;

        Button(void);
};

#endif