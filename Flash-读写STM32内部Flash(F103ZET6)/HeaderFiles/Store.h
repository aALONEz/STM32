#ifndef __STORE_H
#define __STORE_H

extern uint16_t Store_Data[];

void Store_Init(void);
void Store_Save(void);
void Store_WriteByte(uint32_t Address, uint8_t Data);
void Store_WriteHalfWord(uint32_t Address, uint16_t Data);
void Store_WriteWord(uint32_t Address, uint32_t Data);
void Store_Clear(void);
uint8_t Store_ReadByte(uint32_t Address);
uint16_t Store_ReadHalfWord(uint32_t Address);
uint32_t Store_ReadWord(uint32_t Address);
void Store_ClearFlag(void);

#endif
