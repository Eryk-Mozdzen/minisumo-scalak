#ifndef ROBOT_H
#define ROBOT_H

#include <stdio.h>

#include <util/delay.h>

#include "rc5.h"
#include "led.h"
#include "motors.h"
#include "periph.h"

#include <scheduler.h>
#include <finite_state_machine.h>

#define ROBOT_EEPROM_ADDRESS_START		0x00
#define ROBOT_EEPROM_ADDRESS_STOP		0x01
#define ROBOT_EEPROM_ADDRESS_STATE		0x02

#define ROBOT_READY_COLOR_COUNTER_MAX	((uint16_t)240)

void robot_init();

#endif
