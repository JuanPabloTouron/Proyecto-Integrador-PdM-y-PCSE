/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef uint32_t tick_t;
typedef bool bool_t;
typedef struct{
	tick_t startTime;
	tick_t duration;
	bool_t running;

} delay_t;

/* USER CODE END ET */

/* USER CODE BEGIN EFP */
void delayInit(delay_t* delay, tick_t duration);
bool_t delayRead(delay_t* delay);
void delayWrite(delay_t* delay, tick_t duration);
/* USER CODE END EFP */
