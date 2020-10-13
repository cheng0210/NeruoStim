#include "command_parse.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim1;
extern LPTIM_HandleTypeDef hlptim2;
extern ADC_HandleTypeDef hadc1;
void parse_command(char *command){
#if COMMAND_TYPE == 0
    char **result = split(command,':');
    if (strcmp(result[0], "stop") == 0)
    {
    	if(HAL_HSEM_IsSemTaken(STIM_HSEM_ID)){
    		while(PULSE_PROBE != 0);
    		STIM_STATUS = STIM_STATUS_STOP;
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
    	}
    }
    else if(strcmp(result[0],"start_recording")==0){
    	//setup sampling rate and enable timer1 to trigger ADC DMA
    	if(ENABLE_RECORD == 0 && HAL_TIM_Base_GetState(&htim1) == HAL_TIM_STATE_READY){
    		TIM1->ARR = 100000 / RECORD_FREQ;
			HAL_TIM_Base_Start(&htim1);
			ENABLE_RECORD = 1;
    	}
	}
    else if(strcmp(result[0],"stop_recording")==0){
    	if(ENABLE_RECORD && HAL_TIM_Base_GetState(&htim1) == HAL_TIM_STATE_BUSY){
			HAL_TIM_Base_Stop(&htim1);
			ENABLE_RECORD = 0;
		}
	}
    else if(strcmp(result[0],"show_dac")==0){
		if(result[1] != NULL){
			int dac_value = atoi(result[1]);

			while((SPI1->SR & 2) == 0);
			SPI1->DR = dac_value;

			for(int x=0;x<2;x++);//short delay

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, RESET);
			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);

		}
	}
    else if(strcmp(result[0],"return_idle")==0){
    	while((SPI1->SR & 2) == 0);
		SPI1->DR = DAC_GAP;

		for(int x=0;x<2;x++);//short delay

		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, SET);
	}
    else if(strcmp(result[0],"stim_amp")==0){
		if(result[1] != NULL){
			STIM_AMP = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "stim_type")==0)
	{
		if(result[1] != NULL){
			STIM_TYPE = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "anodic_cathodic")==0)
	{
		if(result[1] != NULL){
			ANODIC_CATHODIC = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "phase_one_time")==0)
	{
		if(result[1] != NULL){
			PHASE_ONE_TIME = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "inter_phase_gap")==0)
	{
		if(result[1] != NULL){
			INTER_PHASE_GAP = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "phase_two_time")==0)
	{
		if(result[1] != NULL){
			PHASE_TWO_TIME = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "inter_stim_delay")==0)
	{
		if(result[1] != NULL){
			INTER_STIM_DELAY = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "pulse_num")==0)
	{
		if(result[1] != NULL){
			PULSE_NUM = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "burst_num")==0)
	{
		if(result[1] != NULL){
			BURST_NUM = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "inter_burst_delay")==0)
	{
		if(result[1] != NULL){
			INTER_BURST_DELAY = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "ramp_up") == 0)
	{
		if(result[1] != NULL){
			RAMP_UP = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "short_electrode") == 0)
	{
		if(result[1] != NULL){
			SHORT_ELECTRODE = atoi(result[1]);
		}
	}
	else if (strcmp(result[0], "pulse_num_in_one_burst") == 0)
	{
		if(result[1] != NULL){
			PULSE_NUM_IN_ONE_BURST = atoi(result[1]);
		}
	}
    else if (strcmp(result[0], "record_freq") == 0)
	{
    	if(result[1] != NULL){
    		RECORD_FREQ = atoi(result[1]);
		}
	}
    else if (strcmp(result[0], "dac_phase_one") == 0)
    {
    	if(result[1] != NULL){
    		DAC_PHASE_ONE = atoi(result[1]);
		}
    }
    else if (strcmp(result[0], "dac_phase_two") == 0)
    {
    	if(result[1] != NULL){
    		DAC_PHASE_TWO = atoi(result[1]);
		}
    }
    else if (strcmp(result[0], "dac_gap") == 0)
    {
    	if(result[1] != NULL){
    		DAC_GAP = atoi(result[1]);
		}
    }
    /*else if (strcmp(result[0], "bypass_cap") == 0)
	{
		if(result[1] != NULL){
			BYPASS_CAP = atoi(result[1]);
		}
	}*/
	else if (strcmp(result[0], "start") == 0)
    {
    	//check whether stimulation has been started
    	if(HAL_HSEM_Take(STIM_HSEM_ID, STIM_HSEM_PROCESS_ID)==HAL_OK){

    		//enable stim cct
    		HAL_GPIO_WritePin(GPIOB, STIM_EN_Pin, SET);

    		//setup timer's value for each state
    		PHASE_ONE_TIMER = 4 * (PHASE_ONE_TIME-2);
			PHASE_TWO_TIMER = 4 * (PHASE_TWO_TIME-2);
			if(INTER_PHASE_GAP > 2){
				PHASE_GAP_TIMER = 4 * (INTER_PHASE_GAP-2);
			}else{
				PHASE_GAP_TIMER = 4 * INTER_PHASE_GAP;
			}

			if(INTER_STIM_DELAY > 2){
				STIM_DELAY_TIMER = 4 * (INTER_STIM_DELAY - 2);
			}else{
				STIM_DELAY_TIMER = 4 * INTER_STIM_DELAY;
			}
			BURST_DELAY_TIMER = 4 * (INTER_BURST_DELAY - 2);

			//init temp value for pulse num/burst num
			TEMP_PULSE_NUM = PULSE_NUM;
			TEMP_PULSE_NUM_IN_BURST = PULSE_NUM_IN_ONE_BURST;
			TEMP_BURST_NUM = BURST_NUM;

			//choose stim mode by given parameters
			if(STIM_TYPE == 0){
				if(PULSE_NUM == 0){
					STIM_MODE = STIM_MODE_UNI_CONT;
				}else{
					STIM_MODE = STIM_MODE_UNI_NUM;
				}
			}else{
				if(BURST_NUM == 0){
					STIM_MODE = STIM_MODE_BURST_CONT;
				}else{
					STIM_MODE = STIM_MODE_BURST_NUM;
				}
			}

			//init stim state to phase one
			STIM_STATUS = STIM_STATUS_PHASE_ONE;

			//is ramp up?
			if(RAMP_UP){
				if(DAC_PHASE_ONE < DAC_PHASE_TWO){
					ANODIC_CATHODIC = 1;
				}else{
					ANODIC_CATHODIC = 0;
				}

				if(ANODIC_CATHODIC){
					DAC_PHASE_ONE_COMP = DAC_PHASE_ONE + 110;
					DAC_PHASE_TWO_COMP = DAC_PHASE_TWO - 110;
				}else{
					DAC_PHASE_ONE_COMP = DAC_PHASE_ONE - 110;
					DAC_PHASE_TWO_COMP = DAC_PHASE_TWO + 110;
				}
				TEMP_DAC_PHASE_ONE = DAC_GAP;
				TEMP_DAC_PHASE_TWO = DAC_GAP;
				TEMP_DAC_GAP = DAC_GAP;
				HAL_LPTIM_Counter_Start_IT(&hlptim2, 50000);
			}else{
				TEMP_DAC_PHASE_ONE = DAC_PHASE_ONE;
				TEMP_DAC_PHASE_TWO = DAC_PHASE_TWO;
				TEMP_DAC_GAP = DAC_GAP;
			}


			//WRITE DATA TO DAC
			while((SPI1->SR & 2) == 0);
			SPI1->DR = TEMP_DAC_PHASE_ONE;

			//clear timer2 cnt and enable timer2 with interrupts
			TIM2->CNT = 0;
			HAL_TIM_Base_Start_IT(&htim2);

			//use PB0(LED2) to indicate the start ot stimulation
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, SET);
    	}
    }

    int x = 0;
    while(result[x] != NULL){
        free(result[x]);
        x++;
    }
    free(result);
#else

#endif
}



char **split(const char *command, char splitter)
{
    char **result;
    int j, n = 0;
    int count = 1;
    int len = strlen(command);
    char tmp[len + 1];
    tmp[0] = '\0';

    for (int i = 0; i < len; ++i)
    {
        if (command[i] == splitter && command[i + 1] == '\0')
            continue;
        else if (command[i] == splitter && command[i + 1] != splitter)
            count++;
    }
    result = (char **)malloc((count + 1) * sizeof(char *));

    count = 0;
    for (int i = 0; i < len; ++i)
    {
        if (i == len - 1 && command[i] != splitter)
        {
            tmp[n++] = command[i];
            tmp[n] = '\0';
            j = strlen(tmp) + 1;
            result[count] = (char *)malloc(j * sizeof(char));
            strcpy(result[count++], tmp);
        }
        else if (command[i] == splitter)
        {
            j = strlen(tmp);
            if (j != 0)
            {
                tmp[n] = '\0';
                result[count] = (char *)malloc((j + 1) * sizeof(char));
                strcpy(result[count++], tmp);
                n = 0;
                tmp[0] = '\0';
            }
        }
        else
            tmp[n++] = command[i];
    }
    result[count] = NULL;
    return result;
}
