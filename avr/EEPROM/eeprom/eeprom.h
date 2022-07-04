/*
 * eeprom.h
 *
 *  Created on: 2021年10月6日
 *      Author: Lichmore
 */

#ifndef EEPROM_EEPROM_H_
#define EEPROM_EEPROM_H_

uint8_t eeprom_read(uint16_t address);

void eeprom_write(uint8_t data,uint16_t address);


#endif /* EEPROM_EEPROM_H_ */
