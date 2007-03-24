#ifndef _FFT_H_
#define _FFT_H_
#include <configure.h>
#ifndef FFT_CFG
//---------------------FFT ≈‰÷√----------------------//
#define FFT_CFG
#define NUM_FFT 64 // Length of FFT to process
// Must be 2^N, where N is an integer >= 2
#define WINDOW_TYPE 4 // WINDOW_TYPE specifies the window to use on the data
// The available window functions are:
// 0 = No Window
// 1 = Triangle Window
// 2 = Hanning Window
// 3 = Hamming Window
// 4 = Blackman Window
//-------------------FFT ≈‰÷√Ω· ¯--------------------//
#endif//FFT_CFG
extern int16 Real[NUM_FFT];
extern int16 Imag[NUM_FFT];
void WindowCalc(int16 Win_Array[], uint8 SE_data);
void Int_FFT(int16 ReArray[], int16 ImArray[]);
void Bit_Reverse(int16 BR_Array[]);

#endif//_FFT_H_
