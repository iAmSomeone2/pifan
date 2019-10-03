#ifndef FAN_HH
#define FAN_HH 1

class Fan {
    private:
        int m_pi;
        const unsigned m_enablePin  =   15;
        const unsigned m_tachPin    =   14;
        const unsigned m_pwmPin     =   18;
        
        int initializePins();
    
    protected:
        int m_speed;
        bool m_running;
        
    public:
        Fan(int pi);
        int toggle();
        bool isRunning();
};

#endif // FAN_HH
