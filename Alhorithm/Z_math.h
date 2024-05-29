#ifndef _Z_MATH_H_
#define _Z_MATH_H_




float FZ_math_absolute(float x);
float FZ_math_limt(float x,float max,float min);
float FZ_math_Sqrt(float x);






typedef struct 
{
    unsigned short input;
    unsigned short temp[3];
    unsigned short out;

}MeanFilt_msg_t;
unsigned short SZ_math_MeanFilt(MeanFilt_msg_t* date);




#endif
