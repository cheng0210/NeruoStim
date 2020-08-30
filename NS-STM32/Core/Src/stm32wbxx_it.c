/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32wbxx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32wbxx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_common.h"
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
uint16_t SECOND_COUNTER = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc1;
extern LPTIM_HandleTypeDef hlptim2;
extern TIM_HandleTypeDef htim2;
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */

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
  * @brief This function handles Prefetch fault, memory access fault.
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
/* STM32WBxx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32wbxx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel2 global interrupt.
  */
void DMA1_Channel2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel2_IRQn 0 */
	printf("send %ld\n",(200-__HAL_DMA_GET_COUNTER(&hdma_adc1)));
  /* USER CODE END DMA1_Channel2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc1);
  /* USER CODE BEGIN DMA1_Channel2_IRQn 1 */

  /* USER CODE END DMA1_Channel2_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	//pause timer2
	TIM2->CR1 &= 0;
	switch(STIM_MODE){
		// UNIFORM CONTINUOUS STIMULATION
		case STIM_MODE_UNI_CONT:
			switch(STIM_STATUS){
				// STOP STATUS
				case STIM_STATUS_STOP:
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;
					HAL_TIM_Base_Stop_IT(&htim2);
					if(RAMP_UP){
						HAL_LPTIM_Counter_Stop_IT(&hlptim2);
					}
					break;
				// PHASE ONE STATUS
				case STIM_STATUS_PHASE_ONE:
					PULSE_PROBE = 1;	// MARK AS STIMULATION BEGIN SO THE STIM_STATUS CAN NOT BE CHANGED TO STOP STATUS

					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_PHASE_ONE;
					//while((SPI1->SR & 1) == 0);

					TIM2->ARR = PHASE_ONE_TIMER;
					TIM2->CR1 |= 1;
					if(INTER_PHASE_GAP >= 1){
						STIM_STATUS = STIM_STATUS_INTER_PHASE_GAP;
					}else{
						STIM_STATUS = STIM_STATUS_PHASE_TWO;
					}
					break;
				// INTER PHASE GAP STATUS
				case STIM_STATUS_INTER_PHASE_GAP:

					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;
					//while((SPI1->SR & 1) == 0);

					TIM2->ARR = PHASE_GAP_TIMER;
					TIM2->CR1 |= 1;
					STIM_STATUS = STIM_STATUS_PHASE_TWO;
					break;
				// PHASE TWO STATUS
				case STIM_STATUS_PHASE_TWO:

					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_PHASE_TWO;
					//while((SPI1->SR & 1) == 0);

					TIM2->ARR = PHASE_TWO_TIMER;
					TIM2->CR1 |= 1;
					if(INTER_STIM_DELAY >= 1){
						STIM_STATUS = STIM_STATUS_INTER_STIM_DEALY;
					}else{
						STIM_STATUS = STIM_STATUS_PHASE_ONE;
						PULSE_PROBE = 0;	// MARK AS STIMULATION END SO THE STIM_STATUS CAN BE CHANGED TO STOP STATUS
					}
					break;
				// INTER STIM DELAY STATUS
				case STIM_STATUS_INTER_STIM_DEALY:

					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;
					//while((SPI1->SR & 1) == 0);

					TIM2->ARR = STIM_DELAY_TIMER;
					TIM2->CR1 |= 1;
					STIM_STATUS = STIM_STATUS_PHASE_ONE;
					PULSE_PROBE = 0;	// MARK AS STIMULATION END SO THE STIM_STATUS CAN BE CHANGED TO STOP STATUS
					break;
				default: break;
			}
			break;
		// UNIFORM STIMULATION WITH PULSE NUM
		case STIM_MODE_UNI_NUM:
			switch(STIM_STATUS){
				case STIM_STATUS_STOP:
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;
					HAL_TIM_Base_Stop_IT(&htim2);
					if(RAMP_UP){
						HAL_LPTIM_Counter_Stop_IT(&hlptim2);
					}
					break;
				case STIM_STATUS_PHASE_ONE:
					PULSE_PROBE = 1;
					if(TEMP_PULSE_NUM > 0){
						while((SPI1->SR & 2) == 0);
						SPI1->DR = TEMP_DAC_PHASE_ONE;
						TIM2->ARR = PHASE_ONE_TIMER;
						TIM2->CR1 |= 1;
						if(INTER_PHASE_GAP >= 1){
							STIM_STATUS = STIM_STATUS_INTER_PHASE_GAP;
						}else{
							STIM_STATUS = STIM_STATUS_PHASE_TWO;
						}
					}else{
						while((SPI1->SR & 2) == 0);
						SPI1->DR = TEMP_DAC_GAP;
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
						HAL_TIM_Base_Stop_IT(&htim2);
					}
					break;
				case STIM_STATUS_INTER_PHASE_GAP:
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;
					TIM2->ARR = PHASE_GAP_TIMER;
					TIM2->CR1 |= 1;
					STIM_STATUS = STIM_STATUS_PHASE_TWO;
					break;
				case STIM_STATUS_PHASE_TWO:
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_PHASE_TWO;
					TIM2->ARR = PHASE_TWO_TIMER;
					TIM2->CR1 |= 1;
					if(INTER_STIM_DELAY >= 1){
						STIM_STATUS = STIM_STATUS_INTER_STIM_DEALY;
					}else{
						STIM_STATUS = STIM_STATUS_PHASE_ONE;
						TEMP_PULSE_NUM--;
						PULSE_PROBE = 0;
					}
					break;
				case STIM_STATUS_INTER_STIM_DEALY:
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;
					TIM2->ARR = STIM_DELAY_TIMER;
					TIM2->CR1 |= 1;
					STIM_STATUS = STIM_STATUS_PHASE_ONE;
					TEMP_PULSE_NUM--;
					PULSE_PROBE = 0;
					break;
				default: break;
			}
			break;
		case STIM_MODE_BURST_CONT:
			switch(STIM_STATUS){
				// STOP STATUS
				case STIM_STATUS_STOP:
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;
					HAL_TIM_Base_Stop_IT(&htim2);
					if(RAMP_UP){
						HAL_LPTIM_Counter_Stop_IT(&hlptim2);
					}
					break;
				// PHASE ONE STATUS
				case STIM_STATUS_PHASE_ONE:
					PULSE_PROBE = 1;	// MARK AS STIMULATION BEGIN SO THE STIM_STATUS CAN NOT BE CHANGED TO STOP STATUS

					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_PHASE_ONE;

					TIM2->ARR = PHASE_ONE_TIMER;
					TIM2->CR1 |= 1;
					if(INTER_PHASE_GAP >= 1){
						STIM_STATUS = STIM_STATUS_INTER_PHASE_GAP;
					}else{
						STIM_STATUS = STIM_STATUS_PHASE_TWO;
					}
					break;
				// INTER PHASE GAP STATUS
				case STIM_STATUS_INTER_PHASE_GAP:

					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;

					TIM2->ARR = PHASE_GAP_TIMER;
					TIM2->CR1 |= 1;
					STIM_STATUS = STIM_STATUS_PHASE_TWO;
					break;
				// PHASE TWO STATUS
				case STIM_STATUS_PHASE_TWO:

					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_PHASE_TWO;

					TIM2->ARR = PHASE_TWO_TIMER;
					TIM2->CR1 |= 1;
					if(INTER_STIM_DELAY > 1){
						STIM_STATUS = STIM_STATUS_INTER_STIM_DEALY;
					}else{
						if(TEMP_PULSE_NUM_IN_BURST > 1){
							STIM_STATUS = STIM_STATUS_PHASE_ONE;
							TEMP_PULSE_NUM_IN_BURST--;
						}else{
							STIM_STATUS = STIM_STATUS_INTER_BURST_DELAY;
						}
						PULSE_PROBE = 0;	// MARK AS STIMULATION END SO THE STIM_STATUS CAN BE CHANGED TO STOP STATUS
					}
					break;
				// INTER STIM DELAY STATUS
				case STIM_STATUS_INTER_STIM_DEALY:

					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;

					TIM2->ARR = STIM_DELAY_TIMER;
					TIM2->CR1 |= 1;
					if(TEMP_PULSE_NUM_IN_BURST > 1){
						STIM_STATUS = STIM_STATUS_PHASE_ONE;
						TEMP_PULSE_NUM_IN_BURST--;
					}else{
						STIM_STATUS = STIM_STATUS_INTER_BURST_DELAY;
					}
					PULSE_PROBE = 0;	// MARK AS STIMULATION END SO THE STIM_STATUS CAN BE CHANGED TO STOP STATUS
					break;

				case STIM_STATUS_INTER_BURST_DELAY:
					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;

					TIM2->ARR = BURST_DELAY_TIMER;
					TIM2->CR1 |= 1;

					TEMP_PULSE_NUM_IN_BURST = PULSE_NUM_IN_ONE_BURST;
					STIM_STATUS = STIM_STATUS_PHASE_ONE;

					break;
				default: break;
			}
			break;
		case STIM_MODE_BURST_NUM:
			switch(STIM_STATUS){
				// STOP STATUS
				case STIM_STATUS_STOP:
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;
					HAL_TIM_Base_Stop_IT(&htim2);
					if(RAMP_UP){
						HAL_LPTIM_Counter_Stop_IT(&hlptim2);
					}
					break;
				// PHASE ONE STATUS
				case STIM_STATUS_PHASE_ONE:
					if(TEMP_BURST_NUM > 0){
						PULSE_PROBE = 1;	// MARK AS STIMULATION BEGIN SO THE STIM_STATUS CAN NOT BE CHANGED TO STOP STATUS

						//WRITE DATA TO DAC
						while((SPI1->SR & 2) == 0);
						SPI1->DR = TEMP_DAC_PHASE_ONE;

						TIM2->ARR = PHASE_ONE_TIMER;
						TIM2->CR1 |= 1;
						if(INTER_PHASE_GAP >= 1){
							STIM_STATUS = STIM_STATUS_INTER_PHASE_GAP;
						}else{
							STIM_STATUS = STIM_STATUS_PHASE_TWO;
						}
					}else{
						while((SPI1->SR & 2) == 0);
						SPI1->DR = TEMP_DAC_GAP;
						HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
						HAL_TIM_Base_Stop_IT(&htim2);
					}
					break;
				// INTER PHASE GAP STATUS
				case STIM_STATUS_INTER_PHASE_GAP:

					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;

					TIM2->ARR = PHASE_GAP_TIMER;
					TIM2->CR1 |= 1;
					STIM_STATUS = STIM_STATUS_PHASE_TWO;
					break;
				// PHASE TWO STATUS
				case STIM_STATUS_PHASE_TWO:

					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_PHASE_TWO;

					TIM2->ARR = PHASE_TWO_TIMER;
					TIM2->CR1 |= 1;
					if(INTER_STIM_DELAY > 1){
						STIM_STATUS = STIM_STATUS_INTER_STIM_DEALY;
					}else{
						if(TEMP_PULSE_NUM_IN_BURST > 1){
							STIM_STATUS = STIM_STATUS_PHASE_ONE;
							TEMP_PULSE_NUM_IN_BURST--;
						}else{
							STIM_STATUS = STIM_STATUS_INTER_BURST_DELAY;
						}
						PULSE_PROBE = 0;	// MARK AS STIMULATION END SO THE STIM_STATUS CAN BE CHANGED TO STOP STATUS
					}
					break;
				// INTER STIM DELAY STATUS
				case STIM_STATUS_INTER_STIM_DEALY:

					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;

					TIM2->ARR = STIM_DELAY_TIMER;
					TIM2->CR1 |= 1;
					if(TEMP_PULSE_NUM_IN_BURST > 1){
						STIM_STATUS = STIM_STATUS_PHASE_ONE;
						TEMP_PULSE_NUM_IN_BURST--;
					}else{
						STIM_STATUS = STIM_STATUS_INTER_BURST_DELAY;
					}
					PULSE_PROBE = 0;	// MARK AS STIMULATION END SO THE STIM_STATUS CAN BE CHANGED TO STOP STATUS
					break;

				case STIM_STATUS_INTER_BURST_DELAY:
					//WRITE DATA TO DAC
					while((SPI1->SR & 2) == 0);
					SPI1->DR = TEMP_DAC_GAP;

					TIM2->ARR = BURST_DELAY_TIMER;
					TIM2->CR1 |= 1;

					TEMP_PULSE_NUM_IN_BURST = PULSE_NUM_IN_ONE_BURST;
					TEMP_BURST_NUM--;
					STIM_STATUS = STIM_STATUS_PHASE_ONE;
					break;
				default: break;
			}
			break;
		default: break;
	}



  /* USER CODE END TIM2_IRQn 0 */
  /* USER CODE BEGIN TIM2_IRQn 1 */
	//clear UIF
	TIM2->SR &= 0;
  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles HSEM global interrupt.
  */
void HSEM_IRQHandler(void)
{
  /* USER CODE BEGIN HSEM_IRQn 0 */

  /* USER CODE END HSEM_IRQn 0 */
  HAL_HSEM_IRQHandler();
  /* USER CODE BEGIN HSEM_IRQn 1 */

  /* USER CODE END HSEM_IRQn 1 */
}

/**
  * @brief This function handles LPTIM2 global interrupt.
  */
void LPTIM2_IRQHandler(void)
{
  /* USER CODE BEGIN LPTIM2_IRQn 0 */
	if(SECOND_COUNTER == 10){
		SECOND_COUNTER = 0;

		//do something here
		if(ANODIC_CATHODIC){
			if(TEMP_DAC_PHASE_ONE > DAC_PHASE_ONE_COMP){
				if(TEMP_DAC_PHASE_TWO < DAC_PHASE_TWO_COMP){
					while(PULSE_PROBE != 0);
					__disable_irq();
					TEMP_DAC_PHASE_ONE -= 110;
					TEMP_DAC_PHASE_TWO += 110;
					__enable_irq();
				}else{
					while(PULSE_PROBE != 0);
					__disable_irq();
					TEMP_DAC_PHASE_ONE -= 110;
					TEMP_DAC_PHASE_TWO = DAC_PHASE_TWO;
					__enable_irq();
				}
			}else{
				if(TEMP_DAC_PHASE_TWO < DAC_PHASE_TWO_COMP){
					while(PULSE_PROBE != 0);
					__disable_irq();
					TEMP_DAC_PHASE_ONE = DAC_PHASE_ONE;
					TEMP_DAC_PHASE_TWO += 110;
					__enable_irq();
				}else{
					while(PULSE_PROBE != 0);
					__disable_irq();
					TEMP_DAC_PHASE_ONE = DAC_PHASE_ONE;
					TEMP_DAC_PHASE_TWO = DAC_PHASE_TWO;
					__enable_irq();
					HAL_LPTIM_Counter_Stop_IT(&hlptim2);
				}
			}
		}else{
			if(TEMP_DAC_PHASE_ONE < DAC_PHASE_ONE_COMP){
				if(TEMP_DAC_PHASE_TWO > DAC_PHASE_TWO_COMP){
					while(PULSE_PROBE != 0);
					__disable_irq();
					TEMP_DAC_PHASE_ONE += 110;
					TEMP_DAC_PHASE_TWO -= 110;
					__enable_irq();
				}else{
					while(PULSE_PROBE != 0);
					__disable_irq();
					TEMP_DAC_PHASE_ONE += 110;
					TEMP_DAC_PHASE_TWO = DAC_PHASE_TWO;
					__enable_irq();
				}
			}else{
				if(TEMP_DAC_PHASE_TWO > DAC_PHASE_TWO_COMP){
					while(PULSE_PROBE != 0);
					__disable_irq();
					TEMP_DAC_PHASE_ONE = DAC_PHASE_ONE;
					TEMP_DAC_PHASE_TWO -= 110;
					__enable_irq();
				}else{
					while(PULSE_PROBE != 0);
					__disable_irq();
					TEMP_DAC_PHASE_ONE = DAC_PHASE_ONE;
					TEMP_DAC_PHASE_TWO = DAC_PHASE_TWO;
					__enable_irq();
					HAL_LPTIM_Counter_Stop_IT(&hlptim2);
				}
			}
		}
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
	}else{
		SECOND_COUNTER++;
	}
  /* USER CODE END LPTIM2_IRQn 0 */
  /* USER CODE BEGIN LPTIM2_IRQn 1 */
	__HAL_LPTIM_CLEAR_FLAG(&hlptim2, LPTIM_FLAG_ARRM);
  /* USER CODE END LPTIM2_IRQn 1 */
}

/* USER CODE BEGIN 1 */
void RTC_WKUP_IRQHandler(void)
{
  HW_TS_RTC_Wakeup_Handler();
}

void IPCC_C1_TX_IRQHandler(void)
{
  HW_IPCC_Tx_Handler();

  return;
}

void IPCC_C1_RX_IRQHandler(void)
{
  HW_IPCC_Rx_Handler();
  return;
}
/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
