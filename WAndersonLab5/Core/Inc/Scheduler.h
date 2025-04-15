/*
 * Scheduler.h
 *
 *  Created on: Jan 30, 2025
 *      Author: willa
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include <stdint.h>

uint32_t getScheduledEvents();

void addSchedulerEvent(uint32_t event);

void removeSchedulerEvent(uint32_t removeEvent);

#define LED_TOGGLE_EVENT (1 << 0)
#define DELAY_EVENT (1 << 1)
#define POLLING_EVENT (1 << 2)


#endif /* SCHEDULER_H_ */
