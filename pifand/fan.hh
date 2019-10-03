#ifndef FAN_HH
#define FAN_HH 1

class Fan {
    private:
        int m_pi;
        int m_enablePin;
        int m_tachPin;
        int m_pwmPin;
        
        int initializePins();
    
    protected:
        int m_speed;
        bool m_running;
        
    public:
        Fan(int pi, int enablePin, int tachPin, int pwmPin);
        void toggleFan();
        bool isRunning();
};

#endif // FAN_HH
