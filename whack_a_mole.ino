#include <SevSeg.h>
#include "Button.h"

# define SEQUENCEDURATION 59000
# define FINALSCOREDURATION 5000

SevSeg sevseg;

/*
**  Définition des broches pour les LEDs
*/

const char LED_1 = A0;
const char LED_2 = A1;
const char LED_3 = A2;
const char LED_4 = A3;
const char LED_5 = A4;  
const char LED_6 = A11;  
const char LED_7 = A7;  
const char LED_8 = A8;  
const char LED_9 = A12;  
const char LED_START = A10;

/*
** Définition des broches pour les buttons
*/

const char start_button = 12;
const char button_1 = 14;
const char button_2 = 3;
const char button_3 = 13;
const char button_4 = 5;
const char button_5 = 11;  
const char button_6 = 15;  
const char button_7 = 7;  
const char button_8 = 9;  
const char button_9 = 10; 

/*
** Définition des signaux pour les sons
*/

const char signal1 = 40;
const char signal2 = 41;
const char signal3 = 42;
const char signal4 = 43;
const char signal5 = 44;
const char signal6 = 45;
const char signal7 = 46;
const char signal8 = 47;
const char signal9 = 48;
const char signal_start = 49;

/* 
**  Global Variable
*/

unsigned long   pts_display;
unsigned int    highscore = 0;

Button  arrButton[] = 
{
    Button(LED_1, button_1, signal1, 1),
    Button(LED_2, button_2, signal2, 2),
    Button(LED_3, button_3, signal3, 3),
    Button(LED_4, button_4, signal4, 4),
    Button(LED_5, button_5, signal5, 5),
    Button(LED_6, button_6, signal6, 6),
    Button(LED_7, button_7, signal7, 7),
    Button(LED_8, button_8, signal8, 8),
    Button(LED_9, button_9, signal9, 9)
};


void    setup()
{
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(LED_3, OUTPUT);
    pinMode(LED_4, OUTPUT);
    pinMode(LED_5, OUTPUT);
    pinMode(LED_6, OUTPUT);
    pinMode(LED_7, OUTPUT);
    pinMode(LED_8, OUTPUT);
    pinMode(LED_9, OUTPUT);
    pinMode(LED_START, OUTPUT);

    pinMode(signal1, OUTPUT);  
    pinMode(signal2, OUTPUT);
    pinMode(signal3, OUTPUT);
    pinMode(signal4, OUTPUT);
    pinMode(signal5, OUTPUT);  
    pinMode(signal6, OUTPUT);
    pinMode(signal7, OUTPUT);
    pinMode(signal8, OUTPUT);
    pinMode(signal9, OUTPUT);
    pinMode(signal_start, OUTPUT);

    digitalWrite(LED_1, HIGH);
    digitalWrite(LED_2, HIGH);
    digitalWrite(LED_3, HIGH);
    digitalWrite(LED_4, HIGH);
    digitalWrite(LED_5, HIGH);
    digitalWrite(LED_6, HIGH);
    digitalWrite(LED_7, HIGH);
    digitalWrite(LED_8, HIGH);
    digitalWrite(LED_9, HIGH);
    digitalWrite(LED_START, HIGH);

    digitalWrite(signal1, HIGH);
    digitalWrite(signal2, HIGH);
    digitalWrite(signal3, HIGH);
    digitalWrite(signal4, HIGH);
    digitalWrite(signal5, HIGH);
    digitalWrite(signal6, HIGH);
    digitalWrite(signal7, HIGH);
    digitalWrite(signal8, HIGH);
    digitalWrite(signal9, HIGH);
    digitalWrite(signal_start, HIGH);


    pinMode(button_START, INPUT_PULLUP);
    pinMode(button_1, INPUT_PULLUP);
    pinMode(button_2, INPUT_PULLUP);
    pinMode(button_3, INPUT_PULLUP);
    pinMode(button_4, INPUT_PULLUP);
    pinMode(button_5, INPUT_PULLUP);
    pinMode(button_6, INPUT_PULLUP);
    pinMode(button_7, INPUT_PULLUP);
    pinMode(button_8, INPUT_PULLUP);
    pinMode(button_9, INPUT_PULLUP);

    randomSeed(analogRead(0)); 
    byte    numDigits = 6;
    byte    digitPins[] = {30, 31, 32, 50, 51, 52};
    byte    segmentPins[] = {21, 22, 23, 24, 25, 26, 27};
    bool    resistorsOnSegments = false;
    byte    hardwareConfig = COMMON_CATHODE;
    bool    updateWithDelays = false;
    bool    leadingZeros = false;
    bool    disableDecPoint = true;

    sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint); // initialisation objet sevseg
    sevseg.setBrightness(100);
}

void    updateDisplay()
{
    sevseg.setNumber(pts_display);
    sevseg.refreshDiplay();
}

void    turnOffAllLight()
{
    for(int i = 0; i < 9; i++)
    {
        arrButton[i].turnOffLight();
    }
}

void    blinkDisplay()
{
    const unsigned long     blinkInterval = 500;
    unsigned long timer = millis();
    unsigned long lastBlink = 0;
    bool    displayOn = true;

    while (millis() - timer <= FINALSCOREDURATION)
    {
        if (timer - lastBlink >= blinkInterval)
        {
            lastBlink = millis();
            displayOn = !displayOn;
        }
        if (displayOn)
            sevseg.setNumber(pts_display);
        else
            sevseg.blank();
        sevseg.refreshDisplay();
    }
}

/*
**  line 203, 218 : should I add protected on ledOn > 8 ??
*/

unsigned long    game()
{
    unsigned long   startGame = millis();
    int             ledOn = -1;
    unsigned long   score = 0;
    int             lastLedOn = -1;
    int             secondLedOn = -1;

    pts_display = (score * 1000) + highscore;
    updateDisplay();
    while (millis() - startGame < SEQUENCEDURATION)
    {
        if (ledOn == -1 || (ledOn != -1 && arrButton[ledOn].timerLightPast()))
        {
            ledOn = random(0, 9);
            if (ledOn == lastLedOn)
            {
                if (ledOn < 8)
                    ledOn++;
                else
                    ledOn--;
            }
            lastLedOn = ledOn;
            arrButton[ledOn].turnOnLight();
        }
        if (score >= 30)
        {
            if (secondLedOn == -1 || (secondLedOn != -1 && arrButton[secondLedOn]. timerLightPast()))
            {
                secondLedOn = random(0, 9);
                if (secondLedOn == ledOn && ledOn < 8)
                    secondLedOn++;
                else if (secondLedOn == ledOn && ledOn == 8)
                    secondLedOn--;
                arrButton[secondLedOn].turnOnLight();
            }
        }
        for (int i = 0; i < 9; i++)
        {
            if (digitalRead(arrButton[i].getButton()) == LOW)
            {
                if (arrButton[i].isGood())
                {
                    score++;
                    arrButton[i].turnOffLight(); 
                    arrButton[i].sendSign();
                }
                else
                {
                    if (arrButton[i].timerPush())
                    {
                        if (score >= 5)
                            score -= 5;
                        else
                            score = 0;
                    }
                }
                pts_display = (score * 1000) + highscore;
                updateDisplay();
            }
        }
    }
    turnOffAllLight();
    return (score);
}

/*
**  If we want to let the last score on display before starting a new game
**  remove line 284, 285 and 286
*/

void    loop()
{
    unsigned long   score = 0;

    pts_display = (score * 1000) + highscore;
    updateDisplay();

    if (digitalRead(start_button) == LOW)
    {
        digitalWrite(LED_START, LOW);
        digitalWrite(signal_start, LOW);
        score = 0;
        digitalWrite(signal_start, HIGH); /* Why signal_start High just after we turn it to LOW ??? */
        score = game();
        if (score >= highscore)
        {
            highscore = score;
            pts_display = (highscore * 1000) + highscore;
            blinkDisplay();
        }
        else
        {
            pts_display = (score * 1000) + highscore;
            updateDisplay();
            delay(FINALSCOREDURATION);
        }
        score = 0;
        pts_display = (score * 1000) + highscore;
        updateDisplay();
    }
}