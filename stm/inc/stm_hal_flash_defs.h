#ifndef APP_STM_INC_STM_HAL_FLASH_DEFS_H_
#define APP_STM_INC_STM_HAL_FLASH_DEFS_H_

#include <stdint.h>

#define FLASH_TYPEPROGRAM_HALFWORD   (0x01U)
#define FLASH_TYPEPROGRAM_WORD       (0x02U)
#define FLASH_TYPEPROGRAM_DOUBLEWORD (0x03U)

#define FLASH_TYPEERASE_PAGES     (0x00U)
#define FLASH_TYPEERASE_MASSERASE (0x01U)


typedef enum {
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03
} HAL_StatusTypeDef;

typedef struct {
  uint32_t TypeErase;
  uint32_t PageAddress;
  uint32_t NbPages;                                     
} FLASH_EraseInitTypeDef;

HAL_StatusTypeDef HAL_FLASH_Lock(void);
HAL_StatusTypeDef HAL_FLASH_Unlock(void);
HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data);
HAL_StatusTypeDef HAL_FLASHEx_Erase(FLASH_EraseInitTypeDef *pEraseInit, uint32_t *PageError);

#endif  // !APP_STM_INC_STM_HAL_FLASH_DEFS_H_
