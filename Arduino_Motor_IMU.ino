
#include <stdint.h>
#include <Wire.h>

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
int16_t IMU_I_Roll,IMU_I_Pitch,IMU_I_Yaw;
float IMU_R_Roll,IMU_R_Pitch,IMU_R_Yaw;

#define IMU_PARSE_STEP_START        (0)
#define IMU_PARSE_STEP_ROLL_INT     (1)
#define IMU_PARSE_STEP_ROLL_REAL    (2)
#define IMU_PARSE_STEP_PITCH_INT    (3)
#define IMU_PARSE_STEP_PITCH_REAL   (4)
#define IMU_PARSE_STEP_YAW_INT      (5)
#define IMU_PARSE_STEP_YAW_REAL     (6)
#define IMU_PARSE_STEP_FINISH       (7)

#define MOTOR_DIRECTION_PIN_1       (7)
#define MOTOR_DIRECTION_PIN_2       (8)
#define MOTOR_CONTROL_PIN           (9)

void IMU_Data_Parsing(void);
void Motor_Control(void);

void setup() 
{
    Serial.begin(115200);

    pinMode(MOTOR_DIRECTION_PIN_1,OUTPUT);
    pinMode(MOTOR_DIRECTION_PIN_2,OUTPUT);
    pinMode(MOTOR_CONTROL_PIN,OUTPUT);
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
                if(received_data == '.')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_ROLL_REAL;
                    break;
                }
                IMU_RawData_I_Roll[IMU_RawData_I_Roll_length] = received_data;
                IMU_RawData_I_Roll_length++;
            break;
            case IMU_PARSE_STEP_ROLL_REAL :
                if(received_data == ',')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_PITCH_INT;
                    if(IMU_RawData_I_Roll[0] == '-')
                    {
                        IMU_RawData_I_Roll_length = IMU_RawData_I_Roll_length - 1;
                        if(IMU_RawData_I_Roll_length == 3)
                        {
                            IMU_I_Roll = 100 * (IMU_RawData_I_Roll[1]-0x30) + 10 * (IMU_RawData_I_Roll[2]-0x30) + (IMU_RawData_I_Roll[3]-0x30);
                        }
                        else if(IMU_RawData_I_Roll_length == 2)
                        {
                            IMU_I_Roll = 10 * (IMU_RawData_I_Roll[1]-0x30) + (IMU_RawData_I_Roll[2]-0x30);
                        }
                        else if(IMU_RawData_I_Roll_length == 1)
                        {
                            IMU_I_Roll = (IMU_RawData_I_Roll[1]-0x30);
                        }
                        else
                        {
                            IMU_I_Roll = 0;
                        }

                        IMU_R_Roll = 0.1f * (IMU_RawData_R_Roll[0]-0x30) + 0.01f * (IMU_RawData_R_Roll[1]-0x30);
                        IMU_Roll = -1.0f * (IMU_I_Roll + IMU_R_Roll);
                    }
                    else
                    {
                        if(IMU_RawData_I_Roll_length == 3)
                        {
                            IMU_I_Roll = 100 * (IMU_RawData_I_Roll[0]-0x30) + 10 * (IMU_RawData_I_Roll[1]-0x30) + (IMU_RawData_I_Roll[2]-0x30);
                        }
                        else if(IMU_RawData_I_Roll_length == 2)
                        {
                            IMU_I_Roll = 10 * (IMU_RawData_I_Roll[0]-0x30) + (IMU_RawData_I_Roll[1]-0x30);
                        }
                        else if(IMU_RawData_I_Roll_length == 1)
                        {
                            IMU_I_Roll = (IMU_RawData_I_Roll[0]-0x30);
                        }
                        else
                        {
                            IMU_I_Roll = 0;
                        }

                        IMU_R_Roll = 0.1f * (IMU_RawData_R_Roll[0]-0x30) + 0.01f * (IMU_RawData_R_Roll[1]-0x30);
                        IMU_Roll = IMU_I_Roll + IMU_R_Roll;                    
                    }
                    break;
                }
                IMU_RawData_R_Roll[IMU_RawData_R_Roll_length] = received_data;
                IMU_RawData_R_Roll_length++;
            break;
            case IMU_PARSE_STEP_PITCH_INT :
                if(received_data == '.')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_PITCH_REAL;
                    break;
                }
                IMU_RawData_I_Pitch[IMU_RawData_I_Pitch_length] = received_data;
                IMU_RawData_I_Pitch_length++;
            break;
            case IMU_PARSE_STEP_PITCH_REAL :
                if(received_data == ',')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_YAW_INT;
                    if(IMU_RawData_I_Pitch[0] == '-')
                    {
                        IMU_RawData_I_Pitch_length = IMU_RawData_I_Pitch_length - 1;
                        if(IMU_RawData_I_Pitch_length == 3)
                        {
                            IMU_I_Pitch = 100 * (IMU_RawData_I_Pitch[1]-0x30) + 10 * (IMU_RawData_I_Pitch[2]-0x30) + (IMU_RawData_I_Pitch[3]-0x30);
                        }
                        else if(IMU_RawData_I_Pitch_length == 2)
                        {
                            IMU_I_Pitch = 10 * (IMU_RawData_I_Pitch[1]-0x30) + (IMU_RawData_I_Pitch[2]-0x30);
                        }
                        else if(IMU_RawData_I_Pitch_length == 1)
                        {
                            IMU_I_Pitch = (IMU_RawData_I_Pitch[1]-0x30);
                        }
                        else
                        {
                            IMU_I_Pitch = 0;
                        }

                        IMU_R_Pitch = 0.1f * (IMU_RawData_R_Pitch[0]-0x30) + 0.01f * (IMU_RawData_R_Pitch[1]-0x30);
                        IMU_Pitch = -1.0f * (IMU_I_Pitch + IMU_R_Pitch);
                    }
                    else
                    {
                        if(IMU_RawData_I_Pitch_length == 3)
                        {
                            IMU_I_Pitch = 100 * (IMU_RawData_I_Pitch[0]-0x30) + 10 * (IMU_RawData_I_Pitch[1]-0x30) + (IMU_RawData_I_Pitch[2]-0x30);
                        }
                        else if(IMU_RawData_I_Pitch_length == 2)
                        {
                            IMU_I_Pitch = 10 * (IMU_RawData_I_Pitch[0]-0x30) + (IMU_RawData_I_Pitch[1]-0x30);
                        }
                        else if(IMU_RawData_I_Pitch_length == 1)
                        {
                            IMU_I_Pitch = (IMU_RawData_I_Pitch[0]-0x30);
                        }
                        else
                        {
                            IMU_I_Pitch = 0;
                        }

                        IMU_R_Pitch = 0.1f * (IMU_RawData_R_Pitch[0]-0x30) + 0.01f * (IMU_RawData_R_Pitch[1]-0x30);
                        IMU_Pitch = IMU_I_Pitch + IMU_R_Pitch;                    
                    }
                    break;
                }
                IMU_RawData_R_Pitch[IMU_RawData_R_Pitch_length] = received_data;
                IMU_RawData_R_Pitch_length++;
            break;
            case IMU_PARSE_STEP_YAW_INT :
                if(received_data == '.')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_YAW_REAL;
                    break;
                }
                IMU_RawData_I_Yaw[IMU_RawData_I_Yaw_length] = received_data;
                IMU_RawData_I_Yaw_length++;
            break;
            case IMU_PARSE_STEP_YAW_REAL :
                if(received_data == '\n')
                {
                    IMU_RawData_Parse_Step = IMU_PARSE_STEP_FINISH;
                    if(IMU_RawData_I_Yaw[0] == '-')
                    {
                        IMU_RawData_I_Yaw_length = IMU_RawData_I_Yaw_length - 1;
                        if(IMU_RawData_I_Yaw_length == 3)
                        {
                            IMU_I_Yaw = 100 * (IMU_RawData_I_Yaw[1]-0x30) + 10 * (IMU_RawData_I_Yaw[2]-0x30) + (IMU_RawData_I_Yaw[3]-0x30);
                        }
                        else if(IMU_RawData_I_Yaw_length == 2)
                        {
                            IMU_I_Yaw = 10 * (IMU_RawData_I_Yaw[1]-0x30) + (IMU_RawData_I_Yaw[2]-0x30);
                        }
                        else if(IMU_RawData_I_Yaw_length == 1)
                        {
                            IMU_I_Yaw = (IMU_RawData_I_Yaw[1]-0x30);
                        }
                        else
                        {
                            IMU_I_Yaw = 0;
                        }

                        IMU_R_Yaw = 0.1f * (IMU_RawData_R_Yaw[0]-0x30) + 0.01f * (IMU_RawData_R_Yaw[1]-0x30);
                        IMU_Yaw = -1.0f * (IMU_I_Yaw + IMU_R_Yaw);
                    }
                    else
                    {
                        if(IMU_RawData_I_Yaw_length == 3)
                        {
                            IMU_I_Yaw = 100 * (IMU_RawData_I_Yaw[0]-0x30) + 10 * (IMU_RawData_I_Yaw[1]-0x30) + (IMU_RawData_I_Yaw[2]-0x30);
                        }
                        else if(IMU_RawData_I_Yaw_length == 2)
                        {
                            IMU_I_Yaw = 10 * (IMU_RawData_I_Yaw[0]-0x30) + (IMU_RawData_I_Yaw[1]-0x30);
                        }
                        else if(IMU_RawData_I_Yaw_length == 1)
                        {
                            IMU_I_Yaw = (IMU_RawData_I_Yaw[0]-0x30);
                        }
                        else
                        {
                            IMU_I_Yaw = 0;
                        }

                        IMU_R_Yaw = 0.1f * (IMU_RawData_R_Yaw[0]-0x30) + 0.01f * (IMU_RawData_R_Yaw[1]-0x30);
                        IMU_Yaw = IMU_I_Yaw + IMU_R_Yaw;                    
                    }
                    break;
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
    float pitch_loc;
    if(IMU_Pitch>0)
    {
        pitch_loc = IMU_Pitch * 255.0f / 180.0f;
        digitalWrite(MOTOR_DIRECTION_PIN_1,HIGH);
        digitalWrite(MOTOR_DIRECTION_PIN_2,LOW);
        analogWrite(MOTOR_CONTROL_PIN,(uint8_t)(pitch_loc));
    }
    else
    {
        pitch_loc = -1.0f * IMU_Pitch * 255.0f / 180.0f;
        digitalWrite(MOTOR_DIRECTION_PIN_1,LOW);
        digitalWrite(MOTOR_DIRECTION_PIN_2,HIGH);
        analogWrite(MOTOR_CONTROL_PIN,(uint8_t)(pitch_loc));
    }
}
