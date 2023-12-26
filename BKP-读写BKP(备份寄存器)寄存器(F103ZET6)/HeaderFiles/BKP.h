#ifndef __BKP_H
#define __BKP_H

uint16_t BKP_ReadData(uint16_t BKP_DRx);
void BKP_WriteData(uint16_t BKP_DRx, uint16_t Data);
void BKP_Init(void);


#endif
