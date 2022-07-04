#include <eeprom.h>
#include <avr/io.h>
#include <avr/interrupt.h>
uint8_t eeprom_read(uint16_t address){

while(EECR & (1<<EEWE));//wait EEWE=0 確定不在讀

while(SPMCR & (1<<SPMEN));//wait SPMEN=0 確定flash沒有在寫

//地址填裝
EEARH = ((address & 0xFF00) >> 8);
EEARL = (address) & 0xFF;

EECR |= (1<<EERE);
//wait read finished
while(EECR & (1<< EERE));

return EEDR;
}

void eeprom_write(uint8_t data,uint16_t address){

while(EECR & (1<<EEWE));//wait EEWE=0 確定不在讀
while(SPMCR & (1<<SPMEN));//wait SPMEN=0 確定flash沒有在寫

//地址填裝
EEARH = ((address & 0xFF00) >> 8);
EEARL = (address) & 0xFF;

EEDR = data;//write data

EECR |= (1<<EEMWE);
EECR |= (1<<EEWE);


}
