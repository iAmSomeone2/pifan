#ifndef FAN_HH
#define FAN_HH 1

class Fan {
    private:
        int m_pi;
        int m_lowerBuffer = 5;
        int m_upperBuffer = 1;
        
        int initializePins();
    
    protected:
        int m_speed;
        bool m_running;
        
    public:
        const unsigned m_enablePin  =   15;
        const unsigned m_tachPin    =   14;
        const unsigned m_pwmPin     =   18;

        explicit Fan(int pi, int upperBuffer, int lowerBuffer);
        int determineState(int currentTemp, int targetTemp);
        int toggle();
        int getFanSpeed(int *pulseCount, time_t initTime);
        bool isRunning();
};

#endif // FAN_HH
