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

#define DELAY_EVENT (1 << 0)
#define DEVICE_ID_AND_TEMP_EVENT (1 << 1)


#endif /* SCHEDULER_H_ */
