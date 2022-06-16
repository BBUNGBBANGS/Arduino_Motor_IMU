
#include <stdint.h>

char IMU_RawData_I_Roll[4];
char IMU_RawData_R_Roll[3];
uint8_t IMU_RawData_I_Roll_length;
uint8_t IMU_RawData_R_Roll_length;
char IMU_RawData_I_Pitch[4];
char IMU_RawData_R_Pitch[3];
uint8_t IMU_RawData_I_Pitch_length;
uint8_t IMU_RawData_R_Pitch_length;
char IMU_RawData_I_Yaw[4];
char IMU_RawData_R_Yaw[3];
uint8_t IMU_RawData_I_Yaw_length;
uint8_t IMU_RawData_R_Yaw_length;

uint8_t IMU_RawData_Parse_Step;

float IMU_Roll,IMU_Pitch,IMU_Yaw;
sint16_t IMU_I_Roll,IMU_I_Pitch,IMU_I_Yaw;
float IMU_R_Roll,IMU_R_Pitch,IMU_R_Yaw;

#define IMU_PARSE_STEP_START        (0)
#define IMU_PARSE_STEP_ROLL_INT     (1)
#define IMU_PARSE_STEP_ROLL_REAL    (2)
#define IMU_PARSE_STEP_PITCH_INT    (3)
#define IMU_PARSE_STEP_PITCH_REAL   (4)
#define IMU_PARSE_STEP_YAW_INT      (5)
#define IMU_PARSE_STEP_YAW_REAL     (6)
#define IMU_PARSE_STEP_FINISH       (7)

void IMU_Data_Parsing(void);
void Motor_Control(void);

void setup() 
{
    Serial.begin(115200);
}

void loop() 
{
    IMU_Data_Parsing();
    Motor_Control();
}

void IMU_Data_Parsing(void)
{
    char received_data;
    if(Serial.available()>0)
    {
        received_data = Serial.read();
        switch(IMU_RawData_Parse_Step)
        {
            case IMU_PARSE_STEP_START :
                if(received_data == '*')
                {
                    IMU_RawData_I_Roll_length = 0;
                    IMU_RawData_R_Roll_length = 0;
                    IMU_RawData_I_Pitch_length = 0;
                    IMU_RawData_R_Pitch_length = 0;
                    IMU_RawData_I_Yaw_length = 0;
                    IMU_RawData_R_Yaw_length = 0;
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_ROLL_INT;
                }
            break;
            case IMU_PARSE_STEP_ROLL_INT :
                if(received_data = '.')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_ROLL_REAL;
                }
                IMU_RawData_I_Roll[IMU_RawData_I_Roll_length] = received_data;
                IMU_RawData_I_Roll_length++;
            break;
            case IMU_PARSE_STEP_ROLL_REAL :
                if(received_data = ',')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_PITCH_INT;
                    if(IMU_RawData_I_Roll[0] == '-')
                    {
                        IMU_RawData_I_Roll_length = IMU_RawData_I_Roll_length - 1;
                        if(IMU_RawData_I_Roll_length == 3)
                        {
                            IMU_I_Roll = 100 * IMU_RawData_I_Roll[1] + 10 * IMU_RawData_I_Roll[2] + IMU_RawData_I_Roll[3];
                        }
                        else if(IMU_RawData_I_Roll_length == 2)
                        {
                            IMU_I_Roll = 10 * IMU_RawData_I_Roll[1] + IMU_RawData_I_Roll[2];
                        }
                        else if(IMU_RawData_I_Roll_length == 1)
                        {
                            IMU_I_Roll = IMU_RawData_I_Roll[1];
                        }
                        else
                        {
                            IMU_I_Roll = 0;
                        }

                        IMU_R_Roll = 10 * IMU_RawData_R_Roll[0] + IMU_RawData_R_Roll[1];
                        IMU_Roll = -1.0f * (IMU_I_Roll + IMU_R_Roll);
                    }
                    else
                    {
                        if(IMU_RawData_I_Roll_length == 3)
                        {
                            IMU_I_Roll = 100 * IMU_RawData_I_Roll[0] + 10 * IMU_RawData_I_Roll[1] + IMU_RawData_I_Roll[2];
                        }
                        else if(IMU_RawData_I_Roll_length == 2)
                        {
                            IMU_I_Roll = 10 * IMU_RawData_I_Roll[0] + IMU_RawData_I_Roll[1];
                        }
                        else if(IMU_RawData_I_Roll_length == 1)
                        {
                            IMU_I_Roll = IMU_RawData_I_Roll[0];
                        }
                        else
                        {
                            IMU_I_Roll = 0;
                        }

                        IMU_R_Roll = 10 * IMU_RawData_R_Roll[0] + IMU_RawData_R_Roll[1];
                        IMU_Roll = IMU_I_Roll + IMU_R_Roll;                    
                    }
                }
                IMU_RawData_R_Roll[IMU_RawData_R_Roll_length] = received_data;
                IMU_RawData_R_Roll_length++;
            break;
            case IMU_PARSE_STEP_PITCH_INT :
                if(received_data = '.')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_PITCH_REAL;
                }
                IMU_RawData_I_Pitch[IMU_RawData_I_Pitch_length] = received_data;
                IMU_RawData_I_Pitch_length++;
            break;
            case IMU_PARSE_STEP_PITCH_REAL :
                if(received_data = ',')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_YAW_INT;
                    if(IMU_RawData_I_Pitch[0] == '-')
                    {
                        IMU_RawData_I_Pitch_length = IMU_RawData_I_Pitch_length - 1;
                        if(IMU_RawData_I_Pitch_length == 3)
                        {
                            IMU_I_Pitch = 100 * IMU_RawData_I_Pitch[1] + 10 * IMU_RawData_I_Pitch[2] + IMU_RawData_I_Pitch[3];
                        }
                        else if(IMU_RawData_I_Pitch_length == 2)
                        {
                            IMU_I_Pitch = 10 * IMU_RawData_I_Pitch[1] + IMU_RawData_I_Pitch[2];
                        }
                        else if(IMU_RawData_I_Pitch_length == 1)
                        {
                            IMU_I_Pitch = IMU_RawData_I_Pitch[1];
                        }
                        else
                        {
                            IMU_I_Pitch = 0;
                        }

                        IMU_R_Pitch = 10 * IMU_RawData_R_Pitch[0] + IMU_RawData_R_Pitch[1];
                        IMU_Pitch = -1.0f * (IMU_I_Pitch + IMU_R_Pitch);
                    }
                    else
                    {
                        if(IMU_RawData_I_Pitch_length == 3)
                        {
                            IMU_I_Pitch = 100 * IMU_RawData_I_Pitch[0] + 10 * IMU_RawData_I_Pitch[1] + IMU_RawData_I_Pitch[2];
                        }
                        else if(IMU_RawData_I_Pitch_length == 2)
                        {
                            IMU_I_Pitch = 10 * IMU_RawData_I_Pitch[0] + IMU_RawData_I_Pitch[1];
                        }
                        else if(IMU_RawData_I_Pitch_length == 1)
                        {
                            IMU_I_Pitch = IMU_RawData_I_Pitch[0];
                        }
                        else
                        {
                            IMU_I_Pitch = 0;
                        }

                        IMU_R_Pitch = 10 * IMU_RawData_R_Pitch[0] + IMU_RawData_R_Pitch[1];
                        IMU_Pitch = IMU_I_Pitch + IMU_R_Pitch;                    
                    }
                }
                IMU_RawData_R_Pitch[IMU_RawData_R_Pitch_length] = received_data;
                IMU_RawData_R_Pitch_length++;
            break;
            case IMU_PARSE_STEP_YAW_INT :
                if(received_data = '.')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_YAW_REAL;
                }
                IMU_RawData_I_Yaw[IMU_RawData_I_Yaw_length] = received_data;
                IMU_RawData_I_Yaw_length++;
            break;
            case IMU_PARSE_STEP_YAW_REAL :
                if(received_data = '\n')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_FINISH;
                    if(IMU_RawData_I_Yaw[0] == '-')
                    {
                        IMU_RawData_I_Yaw_length = IMU_RawData_I_Yaw_length - 1;
                        if(IMU_RawData_I_Yaw_length == 3)
                        {
                            IMU_I_Yaw = 100 * IMU_RawData_I_Yaw[1] + 10 * IMU_RawData_I_Yaw[2] + IMU_RawData_I_Yaw[3];
                        }
                        else if(IMU_RawData_I_Yaw_length == 2)
                        {
                            IMU_I_Yaw = 10 * IMU_RawData_I_Yaw[1] + IMU_RawData_I_Yaw[2];
                        }
                        else if(IMU_RawData_I_Yaw_length == 1)
                        {
                            IMU_I_Yaw = IMU_RawData_I_Yaw[1];
                        }
                        else
                        {
                            IMU_I_Yaw = 0;
                        }

                        IMU_R_Yaw = 10 * IMU_RawData_R_Yaw[0] + IMU_RawData_R_Yaw[1];
                        IMU_Pitch = -1.0f * (IMU_I_Yaw + IMU_R_Yaw);
                    }
                    else
                    {
                        if(IMU_RawData_I_Yaw_length == 3)
                        {
                            IMU_I_Yaw = 100 * IMU_RawData_I_Yaw[0] + 10 * IMU_RawData_I_Yaw[1] + IMU_RawData_I_Yaw[2];
                        }
                        else if(IMU_RawData_I_Yaw_length == 2)
                        {
                            IMU_I_Yaw = 10 * IMU_RawData_I_Yaw[0] + IMU_RawData_I_Yaw[1];
                        }
                        else if(IMU_RawData_I_Yaw_length == 1)
                        {
                            IMU_I_Yaw = IMU_RawData_I_Yaw[0];
                        }
                        else
                        {
                            IMU_I_Yaw = 0;
                        }

                        IMU_R_Yaw = 10 * IMU_RawData_R_Yaw[0] + IMU_RawData_R_Yaw[1];
                        IMU_Pitch = IMU_I_Yaw + IMU_R_Yaw;                    
                    }
                }
                IMU_RawData_R_Yaw[IMU_RawData_R_Yaw_length] = received_data;
                IMU_RawData_R_Yaw_length++;
            break;
            case IMU_PARSE_STEP_FINISH :
                IMU_RawData_Parse_Step = IMU_PARSE_STEP_START;
            break;
            default :
            break;
        }
    }
}

void Motor_Control(void)
{

}