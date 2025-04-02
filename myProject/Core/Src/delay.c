#include <stdint.h>

static volatile uint64_t ticks;
// Tick counter
/** SysTick Interrupt Handler */
void SysTick_Handler(void) {
    ticks++;
}

// Delays execution for a given number of milliseconds.
void msDelay(uint64_t ms) {
    uint64_t timeout = ticks + ms;
    while (ticks < timeout) {
        ;
    }
}

uint64_t msGetTicks(void){
	return ticks;
}

