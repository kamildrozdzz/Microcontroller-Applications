#ifndef DELAY_H_
#define DELAY_H_

#include <stdint.h>

/**
 * Delays execution for a given number of milliseconds based on SysTick timer.
 * @param[in] ms requested delay in milliseconds
 */
void msDelay(uint64_t ms);

/**
 * Gets the number of milliseconds elapsed since the SysTick was started.
 */
uint64_t msGetTicks(void);

#endif
