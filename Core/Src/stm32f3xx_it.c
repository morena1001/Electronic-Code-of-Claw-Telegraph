/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f3xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f3xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "liquidcrystal_i2c.h"
#include "telegraph_configurations.h"
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
bool toggle = false;

uint8_t end_of_letter_counter = 0;
uint16_t buzzer_length_counter = 0;

char current_letter[2];

uint8_t row = 0;
uint8_t col = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
void Move_Cursor(uint8_t* row, uint8_t* col);
void Check_Trie_Root(char letter);
void Generate_Tone(int period, int counter);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim6;
/* USER CODE BEGIN EV */
extern TIM_HandleTypeDef htim1;
extern UART_HandleTypeDef huart2;
extern trie_node* temporary_travel;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
   while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */
  HAL_IncTick();
  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F3xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f3xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	if (end_of_letter_counter == 1) {
		Add_Letter(current_letter[0]);
		current_letter[0] = '\0';
		end_of_letter_counter = 0;
		temporary_travel = root;

		Move_Cursor(&row, &col);
	}

	if (!HAL_GPIO_ReadPin(TRB_GPIO_Port, TRB_Pin)) {
		if (!toggle) {
			toggle = true;

			if (current_letter[0] != '\0') {
				Generate_Tone(TRANSMIT_PERIOD, TRANSMIT_COUNTER);

				Add_Letter(current_letter[0]);
				current_letter[0] = '\0';
				end_of_letter_counter = 0;
				temporary_travel = root;

				Move_Cursor(&row, &col);
			}
		}
	} else if (!HAL_GPIO_ReadPin(SPB_GPIO_Port, SPB_Pin)) {
		if (!toggle) {
			toggle = true;

			if (current_letter[0] != '\0') {
				Generate_Tone(SPACE_PERIOD, SPACE_COUNTER);

				Add_Letter(current_letter[0]);
				Add_Letter(' ');

				current_letter[0] = '\0';
				end_of_letter_counter = 0;
				temporary_travel = root;

				Move_Cursor(&row, &col);
				Move_Cursor(&row, &col);
			}
		}
	} else if (!HAL_GPIO_ReadPin(SMB_GPIO_Port, SMB_Pin)) {
		if (!toggle) {
			toggle = true;

			Generate_Tone(SEND_PERIOD, SEND_COUNTER);

			if (current_letter[0] != '\0') {
				Add_Letter(current_letter[0]);
				current_letter[0] = '\0';
			}

			Add_Letter('\r');
			Add_Letter('\n');

			HAL_UART_Transmit(&huart2, (uint8_t*) input_string, input_string_length, 100);
			end_of_letter_counter = 0;
			temporary_travel = root;

			row = 0;
			col = 0;
			HD44780_Clear();
		}
	} else if (!HAL_GPIO_ReadPin(CB_GPIO_Port, CB_Pin)) {
		if (!toggle) {
			toggle = true;

			Generate_Tone(CLICK_PERIOD, CLICK_COUNTER);

			HD44780_SetCursor(row, col);
			current_letter[0] = Traverse_Tree(&temporary_travel, '\\');
			HD44780_PrintStr(current_letter);
			Check_Trie_Root('\\');

			end_of_letter_counter = 251;
		}
	} else if (!HAL_GPIO_ReadPin(SB_GPIO_Port, SB_Pin)) {
		if (!toggle) {
			toggle = true;

			Generate_Tone(SCRATCH_PERIOD, SCRATCH_COUNTER);

			HD44780_SetCursor(row, col);
			current_letter[0] = Traverse_Tree(&temporary_travel, '|');
			HD44780_PrintStr(current_letter);
			Check_Trie_Root('|');

			end_of_letter_counter = 251;
		}
	} else if (!HAL_GPIO_ReadPin(TB_GPIO_Port, TB_Pin)) {
		if (!toggle) {
			toggle = true;

			Generate_Tone(TAP_PERIOD, TAP_COUNTER);

			HD44780_SetCursor(row, col);
			current_letter[0] = Traverse_Tree(&temporary_travel, '/');
			HD44780_PrintStr(current_letter);
			Check_Trie_Root('/');

			end_of_letter_counter = 251;
		}
	} else {
		toggle = false;
		if (end_of_letter_counter > 0) {
			end_of_letter_counter--;
		}
	}
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM6 global interrupt, DAC interrupts.
  */
void TIM6_DAC_IRQHandler(void)
{
  /* USER CODE BEGIN TIM6_DAC_IRQn 0 */
	if (!buzzer_length_counter--)	Generate_Tone(0, 0);
  /* USER CODE END TIM6_DAC_IRQn 0 */
  HAL_TIM_IRQHandler(&htim6);
  /* USER CODE BEGIN TIM6_DAC_IRQn 1 */

  /* USER CODE END TIM6_DAC_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void Move_Cursor(uint8_t* row, uint8_t* col) {
	if (*row < 15) {
		(*row)++;
	} else {
		(*row) = 0;
		(*col) ^= 1;
	}
}

void Check_Trie_Root(char letter) {
	if (temporary_travel->data == '-') {
		Move_Cursor(&row, &col);
		current_letter[0] = Traverse_Tree(&temporary_travel, letter);
		HD44780_PrintStr(current_letter);
	}
}

void Generate_Tone(int period, int counter) {
	__HAL_TIM_SET_AUTORELOAD(&htim1, period * 2);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, period);
	buzzer_length_counter = counter;
}
/* USER CODE END 1 */
