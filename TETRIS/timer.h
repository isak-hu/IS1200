extern volatile int tick;
extern volatile int frames;

void timerinit(void);

void handle_interrupt(unsigned cause);