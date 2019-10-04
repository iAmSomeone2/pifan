#ifndef FAN_HH
#define FAN_HH 1

class Fan {
    private:
        int m_pi;
        const int m_tempBuffer = 5;
        
        int initializePins();
        int toggle();
    
    protected:
        int m_speed;
        bool m_running;
        
    public:
        const unsigned m_enablePin  =   15;
        const unsigned m_tachPin    =   14;
        const unsigned m_pwmPin     =   18;

        Fan(int pi);
        int determineState(int currentTemp, int targetTemp);
        bool isRunning();
};

#endif // FAN_HH