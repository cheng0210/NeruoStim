#include "command_parse.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim1;
extern LPTIM_HandleTypeDef hlptim2;
extern ADC_HandleTypeDef hadc1;
void parse_command(char *command){
	int64_t res = -1; // -1 means invalid commands
    char **result = split(command,':');
    if (strcmp(result[0], "stop") == 0)
    {
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
        while(PULSE_PROBE != 0);
        STIM_STATUS = STIM_STATUS_STOP;
        if(RAMP_UP){
			HAL_LPTIM_Counter_Stop_IT(&hlptim2);
		}
        res = -3; // -3 means stop
    }
    else if(strcmp(result[0],"stim_amp")==0){
		if(result[1] != NULL){
			STIM_AMP = atoi(result[1]);
		}
		res =  STIM_AMP;
	}
	else if (strcmp(result[0], "stim_type")==0)
	{
		if(result[1] != NULL){
			STIM_TYPE = atoi(result[1]);
		}
		res = STIM_TYPE;
	}
	else if (strcmp(result[0], "anodic_cathodic")==0)
	{
		if(result[1] != NULL){
			ANODIC_CATHODIC = atoi(result[1]);
		}
		res = ANODIC_CATHODIC;
	}
	else if (strcmp(result[0], "phase_one_time")==0)
	{
		if(result[1] != NULL){
			PHASE_ONE_TIME = atoi(result[1]);
		}
		res = PHASE_ONE_TIME;
	}
	else if (strcmp(result[0], "inter_phase_gap")==0)
	{
		if(result[1] != NULL){
			INTER_PHASE_GAP = atoi(result[1]);
		}

		res = INTER_PHASE_GAP;
	}
	else if (strcmp(result[0], "phase_two_time")==0)
	{
		if(result[1] != NULL){
			PHASE_TWO_TIME = atoi(result[1]);
		}
		PHASE_TWO_TIME = atoi(result[1]);
		res = PHASE_TWO_TIME;
	}
	else if (strcmp(result[0], "inter_stim_delay")==0)
	{
		if(result[1] != NULL){
			INTER_STIM_DELAY = atoi(result[1]);
		}
		res =  INTER_STIM_DELAY;
	}
	else if (strcmp(result[0], "pulse_num")==0)
	{
		if(result[1] != NULL){
			PULSE_NUM = atoi(result[1]);
		}
		res =  PULSE_NUM;
	}
	else if (strcmp(result[0], "burst_num")==0)
	{
		if(result[1] != NULL){
			BURST_NUM = atoi(result[1]);
		}
		res = BURST_NUM;
	}
	else if (strcmp(result[0], "inter_burst_delay")==0)
	{
		if(result[1] != NULL){
			INTER_BURST_DELAY = atoi(result[1]);
		}
		res = INTER_BURST_DELAY;
	}
	else if (strcmp(result[0], "ramp_up") == 0)
	{
		if(result[1] != NULL){
			RAMP_UP = atoi(result[1]);
		}
		res = RAMP_UP;
	}
	else if (strcmp(result[0], "short_electrode") == 0)
	{
		if(result[1] != NULL){
			SHORT_ELECTRODE = atoi(result[1]);
		}
		res = SHORT_ELECTRODE;
	}
	else if (strcmp(result[0], "pulse_num_in_one_burst") == 0)
	{
		if(result[1] != NULL){
			PULSE_NUM_IN_ONE_BURST = atoi(result[1]);
		}
		res = PULSE_NUM_IN_ONE_BURST;
	}
	else if (strcmp(result[0], "enable_record") == 0)
	{
		if(result[1] != NULL){
			ENABLE_RECORD = atoi(result[1]);;
		}
		res = ENABLE_RECORD;
	}
    else if (strcmp(result[0], "record_freq") == 0)
	{
    	if(result[1] != NULL){
    		RECORD_FREQ = atoi(result[1]);;
		}
		res = RECORD_FREQ;
	}
    else if (strcmp(result[0], "record_start_offset") == 0)
    {
    	if(result[1] != NULL){
    		RECORD_START_OFFSET = atoi(result[1]);;
		}
		res = RECORD_START_OFFSET;
    }
    else if (strcmp(result[0], "record_end_offset") == 0)
	{
    	if(result[1] != NULL){
    		RECORD_END_OFFSET = atoi(result[1]);;
		}
		res = RECORD_END_OFFSET;
	}
    else if (strcmp(result[0], "dac_phase_one") == 0)
    {
    	if(result[1] != NULL){
    		DAC_PHASE_ONE = atoi(result[1]);;
		}
		res = DAC_PHASE_ONE;
    }
    else if (strcmp(result[0], "dac_phase_two") == 0)
    {
    	if(result[1] != NULL){
    		DAC_PHASE_TWO = atoi(result[1]);;
		}
		res = DAC_PHASE_TWO;
    }
    else if (strcmp(result[0], "dac_gap") == 0)
    {
    	if(result[1] != NULL){
    		DAC_GAP = atoi(result[1]);;
		}
		res = DAC_GAP;
    }else if (strcmp(result[0], "start") == 0)
    {
        PHASE_ONE_TIMER = 4 * PHASE_ONE_TIME;
        PHASE_TWO_TIMER = 4 * PHASE_TWO_TIME;
        PHASE_GAP_TIMER = 4 * INTER_PHASE_GAP;
        STIM_DELAY_TIMER = 4 * INTER_STIM_DELAY;
        BURST_DELAY_TIMER = 4 * INTER_BURST_DELAY;

        TEMP_PULSE_NUM = PULSE_NUM;
        TEMP_PULSE_NUM_IN_BURST = PULSE_NUM_IN_ONE_BURST;
        TEMP_BURST_NUM = BURST_NUM;

        TIM1->ARR = 100000 / RECORD_FREQ;
        HAL_TIM_Base_Start(&htim1);

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

        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, SET);
		STIM_STATUS = STIM_STATUS_PHASE_ONE;
		TIM2->CNT = 0;
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
        //HAL_ADCEx_InjectedStart(&hadc1);
        //ADC1->CR |= 8;
    	HAL_TIM_Base_Start_IT(&htim2);
    }

    int x = 0;
    while(result[x] != NULL){
        free(result[x]);
        x++;
    }
    free(result);
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
