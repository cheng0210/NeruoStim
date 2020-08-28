#include "command_parse.h"
extern TIM_HandleTypeDef htim2;
extern LPTIM_HandleTypeDef hlptim2;
void parse_command(char *command){
    char **result = split(command,':');
    if (strcmp(result[0], "start") == 0)
    {
        PHASE_ONE_TIMER = 64 * PHASE_ONE_TIME;
        PHASE_TWO_TIMER = 64 * PHASE_TWO_TIME;
        PHASE_GAP_TIMER = 64 * INTER_PHASE_GAP;
        STIM_DELAY_TIMER = 64 * INTER_STIM_DELAY;
        BURST_DELAY_TIMER = 64 * INTER_BURST_DELAY;

        TEMP_PULSE_NUM = PULSE_NUM;
        TEMP_PULSE_NUM_IN_BURST = PULSE_NUM_IN_ONE_BURST;
        TEMP_BURST_NUM = BURST_NUM;

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
        	TEMP_DAC_PHASE_ONE = DAC_GAP;
        	TEMP_DAC_PHASE_TWO = DAC_GAP;
        	TEMP_DAC_GAP = DAC_GAP;
        	HAL_LPTIM_Counter_Start_IT(&hlptim2, 50000);
        }else{
        	TEMP_DAC_PHASE_ONE = DAC_PHASE_ONE;
			TEMP_DAC_PHASE_TWO = DAC_PHASE_TWO;
			TEMP_DAC_GAP = DAC_GAP;
        }
    	HAL_TIM_Base_Start_IT(&htim2);
    }
    else if (strcmp(result[0], "stop") == 0)
    {
    	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, RESET);
        while(PULSE_PROBE != 0);
        STIM_STATUS = STIM_STATUS_STOP;
    }else if(strcmp(result[0],"stim_amp")==0){
        STIM_AMP = atoi(result[1]);
    }
    else if (strcmp(result[0], "stim_type")==0)
    {
        STIM_TYPE = atoi(result[1]);
    }
    else if (strcmp(result[0], "anodic_cathodic")==0)
    {
        ANODIC_CATHODIC = atoi(result[1]);
    }
    else if (strcmp(result[0], "phase_one_time")==0)
    {
        PHASE_ONE_TIME = atoi(result[1]);
    }
    else if (strcmp(result[0], "inter_phase_gap")==0)
    {
        INTER_PHASE_GAP = atoi(result[1]);
    }
    else if (strcmp(result[0], "phase_two_time")==0)
    {
        PHASE_TWO_TIME = atoi(result[1]);
    }
    else if (strcmp(result[0], "inter_stim_delay")==0)
    {
        INTER_STIM_DELAY = atoi(result[1]);
    }
    else if (strcmp(result[0], "pulse_num")==0)
    {
        PULSE_NUM = atoi(result[1]);
    }
    else if (strcmp(result[0], "burst_num")==0)
    {
        BURST_NUM = atoi(result[1]);
    }
    else if (strcmp(result[0], "inter_burst_delay")==0)
    {
        INTER_BURST_DELAY = atoi(result[1]);
    }
    else if (strcmp(result[0], "ramp_up") == 0)
    {
        RAMP_UP = atoi(result[1]);
    }
    else if (strcmp(result[0], "short_electrode") == 0)
    {
        SHORT_ELECTRODE = atoi(result[1]);
    }
    else if (strcmp(result[0], "pulse_num_in_one_burst") == 0)
    {
        PULSE_NUM_IN_ONE_BURST = atoi(result[1]);
    }
    else if (strcmp(result[0], "enable_record") == 0)
    {
        ENABLE_RECORD = atoi(result[1]);
    }
    else if (strcmp(result[0], "record_offset") == 0)
    {
        RECORD_OFFSET = atoi(result[1]);
    }
    else if (strcmp(result[0], "dac_phase_one") == 0)
    {
        DAC_PHASE_ONE = atoi(result[1]);
    }
    else if (strcmp(result[0], "dac_phase_two") == 0)
    {
        DAC_PHASE_TWO = atoi(result[1]);
    }
    else if (strcmp(result[0], "dac_gap") == 0)
    {
            DAC_GAP = atoi(result[1]);
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
