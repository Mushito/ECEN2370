/*
 * Scheduler.c
 *
 *  Created on: Jan 30, 2025
 *      Author: willa
 */

#include "Scheduler.h"

static uint32_t scheduledEvents = 0;

void addSchedulerEvent(uint32_t event){
    scheduledEvents |= event;
}

void removeSchedulerEvent(uint32_t removeEvent){
    scheduledEvents &= ~removeEvent;
}

uint32_t getScheduledEvents(){
    return scheduledEvents; 
}
