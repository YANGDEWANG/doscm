//-----------------------------------------------------------------------------
// IntFFT_BRIN.c
//-----------------------------------------------------------------------------
// Copyright 2003 Cygnal Integrated Products, Inc.
//
// AUTH: BD
// DATE: 30 JAN 03
//
// This program collects data using ADC0 at <SAMPLE_RATE> Hz and performs
// an FFT on the data. The Real and Imaginary parts of the results are then
// sent to the UART peripheral at <BAUDRATE> bps, where they can be displayed
// or captured using a terminal program.
//
// Note that the FFT performed in this software is optimized for storage space
// (RAM). The resulting Frequency-domain data is not suitable for analyzing
// Signal-to-noise or distortion performance.
//
// This program uses a 22.1184 MHz crystal oscillator multiplied by (9/4)
// for an effective SYSCLK of 49.7664 Mhz. This program also initializes and
// uses UART0 at <BAUDRATE> bits per second.
//
// Target: C8051F12x
// Tool chain: KEIL C51 6.03
#include <string.h>
typedef union IBALONG { // Integer or Byte-addressable LONG
	int32	l; // int32: Var.l
	uint16	i[2]; // u int: Var.i[0]:Var.i[1]
	uint8	b[4]; // u char: Var.b[0]:Var.b[1]:
	// Var.b[2]:Var.b[3]
} IBALONG;
typedef union BAINT { // Byte-addressable INT
	int16	i; // int: Var.i
	uint8	b[2]; // u char: Var.b[0]:Var.b[1]
} BAINT;
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// XRAM storage of FFT: requires NUM_FFT*4 Bytes after DATA_BEGIN address
//int16 Real[NUM_FFT];
int16 Imag[NUM_FFT];




//-----------------------------------------------------------------------------
// WindowCalc
//-----------------------------------------------------------------------------
//
// Uses the values in WindowFunc[] to window the stored data.
//
// The WindowFunc[] Array contains window coefficients between samples
// 0 and (NUM_FFT/2)-1, and samples from NUM_FFT/2 to NUM_FFT-1 are the mirror
// image of the other side.
// Window values are interpreted as a fraction of 1 (WindowFunc[x]/65536).
// The value at NUM_FFT/2 is assumed to be 1.0 (65536).
//
// If SE_data = 1, the input data is assumed to be single-ended, and is
// converted to a 2¡¯s complement, differential representation, to cancel the DC
// offset.
//
#ifdef HFFT
void WindowCalc(int16 Win_Array[], uint8 SE_data)
{
	// NUM_FFT is defined in the ¡°FFT_Code_Tables.h¡± header file
#if (NUM_FFT >= 256)
	uint16 index;
#endif
#if (NUM_FFT < 256)
	uint8 index;
#endif
	int16 Win_ArrayTmp;
	uint16 WindowFuncTmp;
	if (SE_data) // If data is single-ended,
	{ // convert it to differential
		for (index = 0; index < NUM_FFT; index++)
		{
			Win_Array[index] ^= 0x8000; // XOR MSB with ¡®1¡¯ to invert
		}
	}
#if (WINDOW_TYPE != 0) // Use this section if a window has been specified
	IBALONG NewVal;
	//if (SE_data) // If data is single-ended,
	//	Win_Array[0] ^= 0x8000; // convert it to differential
	NewVal.l = (int32)Win_Array[0] * pgm_read_word(WindowFunc+0);
	if ((NewVal.l < 0)&&(NewVal.i[0]))
		Win_Array[0] = NewVal.i[1] + 1;
	else Win_Array[0] = NewVal.i[1];
	//if (SE_data) // If data is single-ended,
	//	Win_Array[NUM_FFT/2] ^= 0x8000; // convert it to differential
	for (index = 1; index < NUM_FFT/2; index++)
	{
		WindowFuncTmp = pgm_read_word(WindowFunc+index);
		// Array positions 1 to (NUM_FFT/2 - 1)
		Win_ArrayTmp = Win_Array[index];
		//if (SE_data) // If data is single-ended,
		//	Win_ArrayTmp ^= 0x8000; // convert it to differential
		NewVal.l = (int32)Win_ArrayTmp * WindowFuncTmp;//(int32)Win_Array[index] * WindowFunc[index];
		if ((NewVal.l < 0)&&(NewVal.i[0]))
			Win_Array[index] = NewVal.i[1] + 1;
		else Win_Array[index] = NewVal.i[1];
		// Array positions (NUM_FFT/2 + 1) to (NUM_FFT - 1)
		Win_ArrayTmp = Win_Array[NUM_FFT-index];
		//if (SE_data) // If data is single-ended,
		//	Win_ArrayTmp ^= 0x8000; // convert it to differential
		NewVal.l = (int32)Win_ArrayTmp * WindowFuncTmp;//(int32)Win_Array[NUM_FFT-index] * WindowFunc[index];
		if ((NewVal.l < 0)&&(NewVal.i[0]))
			Win_Array[NUM_FFT-index] = NewVal.i[1] + 1;
		else Win_Array[NUM_FFT-index] = NewVal.i[1];
	}
#endif//WINDOW_TYPE
} // END WindowCalc
#else
void WindowCalc(int16 Win_Array[], uint8 SE_data)
{
	// NUM_FFT is defined in the ¡°FFT_Code_Tables.h¡± header file
#if (NUM_FFT >= 256)
	uint16 index;
#endif
#if (NUM_FFT < 256)
	uint8 index;
#endif

	uint8 *Win_ArrayAt8 = (uint8*)Win_Array;
	Win_ArrayAt8++;
	if (SE_data)
	{ 
		for (index = 0; index < NUM_FFT; index++)
		{
			*Win_ArrayAt8^= 0x80;
			Win_ArrayAt8+=2;
			//Win_Array[index] ^= 0x8000; // XOR MSB with ¡®1¡¯ to invert
		}
	}
#if (WINDOW_TYPE != 0) // Use this section if a window has been specified
	int16 Win_ArrayTmp;
	uint16 WindowFuncTmp;
	IBALONG NewVal;
	NewVal.l = (int32)Win_Array[0] * pgm_read_word(WindowFunc+0);
	Win_Array[0] = NewVal.i[1];
	for (index = 1; index < NUM_FFT/2; index++)
	{
		WindowFuncTmp = pgm_read_word(WindowFunc+index);
		Win_ArrayTmp = Win_Array[index];
		NewVal.l = (int32)Win_ArrayTmp * WindowFuncTmp;
		Win_Array[index] = NewVal.i[1];
		Win_ArrayTmp = Win_Array[NUM_FFT-index];
		NewVal.l = (int32)Win_ArrayTmp * WindowFuncTmp;
		Win_Array[NUM_FFT-index] = NewVal.i[1];
	}
#endif//WINDOW_TYPE
} 
#endif
//-----------------------------------------------------------------------------
// Bit_Reverse
//-----------------------------------------------------------------------------
//
// Sorts data in Bit Reversed Address order
//
// The BRTable[] array is used to find which values must be swapped. Only
// half of this array is stored, to save code space. The second half is
// assumed to be a mirror image of the first half.
//
#if 1//size 
void Bit_Reverse(int16 BR_Array[])
{
#if (NUM_FFT >= 512)
	uint16 swapA, swapB, sw_cnt; // Swap Indices
#endif
#if (NUM_FFT <= 256)
	uint8 swapA, swapB, sw_cnt; // Swap Indices
#endif

	int16 TempStore;
	bool first;
	i16 *aP,*bP;
	// Loop through locations to swap
	for (sw_cnt = 1; sw_cnt < NUM_FFT/2; sw_cnt++)
	{
		first = true;
		swapA = sw_cnt; // Store current location
		swapB = pgm_read_byte(BRTable+sw_cnt)*2;//BRTable[sw_cnt] * 2; // Retrieve bit-reversed index
re:
		if (swapB > swapA) // If the bit-reversed index is
		{ // larger than the current index,
aP=&BR_Array[swapA];
bP=&BR_Array[swapB];
TempStore=*aP;
*aP=*bP;
*bP=TempStore;
			//TempStore = BR_Array[swapA]; // the two data locations are
			//BR_Array[swapA] = BR_Array[swapB]; // swapped. Using this comparison
			//BR_Array[swapB] = TempStore; // ensures that locations are only
		} // swapped once, and never with
		// themselves
		swapA += NUM_FFT/2; // Now perform the same operations
		swapB++; // on the second half of the data
		if(first)
		{
			first = false;
			goto re;
		}
	}
} 
#else// END Bit Reverse Order Sort
void Bit_Reverse(int16 BR_Array[])
{
#if (NUM_FFT >= 512)
	uint16 swapA, swapB, sw_cnt; // Swap Indices
#endif
#if (NUM_FFT <= 256)
	uint8 swapA, swapB, sw_cnt; // Swap Indices
#endif

	int16 TempStore;
	// Loop through locations to swap
	for (sw_cnt = 1; sw_cnt < NUM_FFT/2; sw_cnt++)
	{
		swapA = sw_cnt; // Store current location
		swapB = pgm_read_byte(BRTable+sw_cnt)*2;//BRTable[sw_cnt] * 2; // Retrieve bit-reversed index
		if (swapB > swapA) // If the bit-reversed index is
		{ // larger than the current index,
			TempStore = BR_Array[swapA]; // the two data locations are
			BR_Array[swapA] = BR_Array[swapB]; // swapped. Using this comparison
			BR_Array[swapB] = TempStore; // ensures that locations are only
		} // swapped once, and never with
		// themselves
		swapA += NUM_FFT/2; // Now perform the same operations
		swapB++; // on the second half of the data
		if (swapB > swapA)
		{
			TempStore = BR_Array[swapA];
			BR_Array[swapA] = BR_Array[swapB];
			BR_Array[swapB] = TempStore;
		}
	}
} 
#endif
//-----------------------------------------------------------------------------
// Int_FFT
//-----------------------------------------------------------------------------
//
// Performs a Radix-2 Decimation-In-Time FFT on the input array ReArray[]
//
// During each stage of the FFT, the values are calculated using a set of
// ¡°Butterfly¡± equations, as listed below:
//
// Re1 = Re1 + (Cos(x)*Re2 + Sin(x)*Im2)
// Re2 = Re1 - (Cos(x)*Re2 + Sin(x)*Im2)
// Im1 = Im1 + (Cos(x)*Im2 - Sin(x)*Re2)
// Im2 = Im1 - (Cos(x)*Im2 - Sin(x)*Re2)
//
// The routine implements this calculation using the following values:
//
// Re1 = ReArray[indexA], Re2 = ReArray[indexB]
// Im1 = ImArray[indexA], Im2 = ImArray[indexB]
// x = the angle: 2*pi*(sin_index/NUM_FFT), in radians. The necessary values
// are stored in code space in the SinTable[] array.
//
//
// Key Points for using this FFT routine:
//
// 1) It expects REAL data (in ReArray[]), in 2¡¯s complement, 16-bit binary
// format and assumes a value of 0 for all imaginary locations
// (in ImArray[]).
//


// 2) It expects the REAL input data to be sorted in bit-reversed index order.
//
// 3) SIN and COS values are retrieved and calculated from a table consisting
// of 1/4 of a period of a SIN function.
//
// 4) It is optimized to use integer math only (no floating-point operations),
// and for storage space. The input, all intermediate stages, and the
// output of the FFT are stored as 16-bit INTEGER values. This limits the
// precision of the routine. When using input data of less than 16-bits,
// the best results are produced by left-justifying the data prior to
// windowing and performing the FFT.
//
// 5) The algorithm is a Radix-2 type, meaning that the number of samples must
// be 2^N, where N is an integer. The minimum number of samples to process
// is 4. The constant NUM_FFT contains the number of samples to process.
//
//
#ifdef HFFT
void Int_FFT(int16 ReArray[], int16 ImArray[])
{
#if (NUM_FFT >= 512)
	uint16 sin_index, g_cnt, s_cnt; // Keeps track of the proper index
	uint16 indexA, indexB; // locations for each calculation
#endif
#if (NUM_FFT <= 256)
	uint8 sin_index, g_cnt, s_cnt; // Keeps track of the proper index
	uint8 indexA, indexB; // locations for each calculation
#endif
	uint16 group = NUM_FFT/4, stage = 2;
	int32 CosVal, SinVal;
	int32 TempImA, TempImB, TempReA, TempReB, TempReA2, TempReB2;
	IBALONG ReTwid, ImTwid, TempL;
	// FIRST STAGE - optimized for REAL input data only. This will set all
	// Imaginary locations to zero.
	//
	// Shortcuts have been taken to remove unnecessary multiplications during this
	// stage. The angle ¡°x¡± is 0 radians for all calculations at this point, so
	// the SIN value is equal to 0.0 and the COS value is equal to 1.0.
	// Additionally, all Imaginary locations are assumed to be ¡®0¡¯ in this stage of
	// the algorithm, and are set to ¡®0¡¯.
	indexA = 0;
	indexB = 1;//my add
	for (g_cnt = 0; g_cnt < NUM_FFT/2; g_cnt++)
	{
		//indexB = indexA + 1;//my c
		TempReA = ReArray[indexA];
		TempReB = ReArray[indexB];
		// Calculate new value for ReArray[indexA]
		TempL.l = (int32)TempReA + TempReB;
		if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
			TempReA2 = (TempL.l >> 1) + 1;
		else TempReA2 = TempL.l >> 1;
		// Calculate new value for ReArray[indexB]
		TempL.l = (int32)TempReA - TempReB;
		if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
			ReArray[indexB] = (TempL.l >> 1) + 1;
		else ReArray[indexB] = TempL.l >> 1;
		ReArray[indexA] = TempReA2;
		//ImArray[indexA] = 0; // set Imaginary locations to ¡®0¡¯//my c
		//ImArray[indexB] = 0;//my c
		//indexA = indexB + 1;//my c
		indexA+=2;//my add
		indexB+=2;//my add
	}
	dwmemset(ImArray,0,NUM_FFT*sizeof(int16));
	//g_cnt = 0;
	//while(g_cnt < NUM_FFT)
	//{
	//	ImArray[g_cnt++]=0;
	//}
	// END OF FIRST STAGE
	while (stage <= NUM_FFT/2)
	{
		indexA = 0;
		sin_index = 0;
		for (g_cnt = 0; g_cnt < group; g_cnt++)
		{
			for (s_cnt = 0; s_cnt < stage; s_cnt++)
			{
				indexB = indexA + stage;
				TempReA = ReArray[indexA];
				TempReB = ReArray[indexB];
				TempImA = ImArray[indexA];
				TempImB = ImArray[indexB];
				// The following first checks for the special cases when the angle ¡°x¡± is
				// equal to either 0 or pi/2 radians. In these cases, unnecessary
				// multiplications have been removed to improve the processing speed.
#if 1
				if (sin_index == 0||sin_index == NUM_FFT/4) // corresponds to ¡°x¡± = 0 radians
				{
					if(sin_index == 0)//½»»»TempReB£¬TempImB£»
					{
						TempL.l = TempReB;
						TempReB = -TempImB;
						TempImB = TempL.l;
					}
					//	TempL.l = (int32)TempReA + TempReB;
					//	TempReA2 = TempL.l >> 1;
					//	if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
					//		TempReA2++;
					//	TempL.l = (int32)TempReA - TempReB;
					//	TempReB2 = TempL.l >> 1;
					//	if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
					//		TempReB2++;
					//	TempL.l = (int32)TempImA - TempImB;
					//	TempImB = TempL.l >> 1;
					//	if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
					//		TempImB++;
					//	TempL.l = (int32)TempImA + TempImB;
					//	TempImA = TempL.l >> 1;
					//	if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
					//		TempImA++;
					//}
					//else if (sin_index == NUM_FFT/4) // corresponds to ¡°x¡± = pi/2 radians
					//{
					TempL.l = (int32)TempReA - TempImB;
					TempReB2 = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempReB2++;
					TempL.l = (int32)TempReA + TempImB;
					TempReA2 = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempReA2++;
					TempL.l = (int32)TempImA + TempReB;
					TempImB = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempImB++;
					if(sin_index==0)
					{
						TempReB = TempImB;
					}
					TempL.l = (int32)TempImA - TempReB;
					TempImA = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempImA++;
				}
#else
				if (sin_index == 0) // corresponds to ¡°x¡± = 0 radians
				{
					TempL.l = (int32)TempReA + TempReB;
					TempReA2 = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempReA2++;
					TempL.l = (int32)TempReA - TempReB;
					TempReB2 = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempReB2++;
					TempL.l = (int32)TempImA - TempImB;
					TempImB = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempImB++;
					TempL.l = (int32)TempImA + TempImB;
					TempImA = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempImA++;
				}
				else if (sin_index == NUM_FFT/4) // corresponds to ¡°x¡± = pi/2 radians
				{
					TempL.l = (int32)TempReA - TempImB;
					TempReB2 = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempReB2++;
					TempL.l = (int32)TempReA + TempImB;
					TempReA2 = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempReA2++;
					TempL.l = (int32)TempImA + TempReB;
					TempImB = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempImB++;
					TempL.l = (int32)TempImA - TempReB;
					TempImA = TempL.l >> 1;
					if ((TempL.l < 0)&&(0x01 & TempL.b[0]))
						TempImA++;
				}
#endif
				else
				{
					// If no multiplication shortcuts can be taken, the SIN and COS
					// values for the Butterfly calculation are fetched from the
					// SinTable[] array.
					if (sin_index > NUM_FFT/4)
					{
						SinVal = (int16)pgm_read_word(SinTable+(NUM_FFT/2) - sin_index);//SinTable[(NUM_FFT/2) - sin_index];
						CosVal = -(int16)pgm_read_word(SinTable+sin_index - (NUM_FFT/4));//-SinTable[sin_index - (NUM_FFT/4)];
					}
					else
					{
						SinVal = (int16)pgm_read_word(SinTable+sin_index);//SinTable[sin_index];
						CosVal = (int16)pgm_read_word(SinTable+(NUM_FFT/4) - sin_index);//SinTable[(NUM_FFT/4) - sin_index];
					}
					// The SIN and COS values are used here to calculate part of the
					// Butterfly equation
					ReTwid.l = ((int32)TempReB * CosVal) +
						((int32)TempImB * SinVal);
					ImTwid.l = ((int32)TempImB * CosVal) -
						((int32)TempReB * SinVal);
					// Using the values calculated above, the new variables
					// are computed
					// Calculate new value for ReArray[indexA]
					TempL.i[0] = 0;
					TempL.i[1] = TempReA;
					//TempL.l = TempL.l >> 1;
					ReTwid.l += TempL.l>>1;
					TempReA2 = ReTwid.i[1];
					if ((ReTwid.l < 0)&&(ReTwid.i[0]))
						TempReA2++;
					//else TempReA2 = ReTwid.i[1];
					// Calculate new value for ReArray[indexB]
					//TempL.l = TempL.l << 1;
					TempL.l -= ReTwid.l;
					TempReB2 = TempL.i[1];
					if ((TempL.l < 0)&&(TempL.i[0]))
						TempReB2++;
					//TempReB2 = TempL.i[1] + 1;
					//else TempReB2 = TempL.i[1];
					// Calculate new value for ImArray[indexA]
					TempL.i[0] = 0;
					TempL.i[1] = TempImA;
					//TempL.l = TempL.l >> 1;
					ImTwid.l += TempL.l>>1;
					TempImA = ImTwid.i[1];
					if ((ImTwid.l < 0)&&(ImTwid.i[0]))
						TempImA++;
					//TempImA = ImTwid.i[1] + 1;
					//else TempImA = ImTwid.i[1];
					// Calculate new value for ImArray[indexB]
					//TempL.l = TempL.l << 1;
					TempL.l -= ImTwid.l;
					TempImB = TempL.i[1];
					if ((TempL.l < 0)&&(TempL.i[0]))
						TempImB++;
					//	TempImB = TempL.i[1] + 1;
					//else TempImB = TempL.i[1];
				}
				ReArray[indexA] = TempReA2;
				ReArray[indexB] = TempReB2;
				ImArray[indexA] = TempImA;
				ImArray[indexB] = TempImB;
				indexA++;
				sin_index += group;
			} // END of stage FOR loop (s_cnt)
			indexA = indexB + 1;
			sin_index = 0;
		} // END of group FOR loop (g_cnt)
		group /= 2;
		stage *= 2;
	} // END of While loop
} // END Int_FFT
#else 
void Int_FFT(int16 ReArray[], int16 ImArray[])
{
#if (NUM_FFT >= 512)
	uint16 sin_index, g_cnt, s_cnt; // Keeps track of the proper index
	uint16 indexA, indexB; // locations for each calculation
#endif
#if (NUM_FFT <= 256)
	uint8 sin_index, g_cnt, s_cnt; // Keeps track of the proper index
	uint8 indexA, indexB; // locations for each calculation
#endif
	uint16 group = NUM_FFT/4, stage = 2;
	int16 CosVal, SinVal;
	int16 TempReA, TempReB;
	int16 TempImA, TempImB,  TempReA2, TempReB2,TempImA2;
	IBALONG ReTwid, ImTwid;
	// FIRST STAGE - optimized for REAL input data only. This will set all
	// Imaginary locations to zero.
	//
	// Shortcuts have been taken to remove unnecessary multiplications during this
	// stage. The angle ¡°x¡± is 0 radians for all calculations at this point, so
	// the SIN value is equal to 0.0 and the COS value is equal to 1.0.
	// Additionally, all Imaginary locations are assumed to be ¡®0¡¯ in this stage of
	// the algorithm, and are set to ¡®0¡¯.
	i16 *ReArrayPA = ReArray-1;
	i16 *ReArrayPB = ReArray;
	for (g_cnt = 0; g_cnt < NUM_FFT/2; g_cnt++)
	{
		TempReA = (*++ReArrayPA)>>1;
		TempReB = (*++ReArrayPB)>>1;
		*ReArrayPB++ = TempReA - TempReB;
		*ReArrayPA++ = TempReA + TempReB;
	}
	dwmemset(ImArray,0,NUM_FFT*sizeof(int16));
	indexB=NUM_FFT;
	while (stage <= NUM_FFT/2)
	{
		indexA = 0;
		sin_index = 0;
		for (g_cnt = 0; g_cnt < group; g_cnt++)
		{
			for (s_cnt = 0; s_cnt < stage; s_cnt++)
			{
				indexB = indexA + stage;
				TempReA = ReArray[indexA];
				TempReB = ReArray[indexB];
				TempImA = ImArray[indexA];
				TempImB = ImArray[indexB];
				if (sin_index == 0||sin_index == NUM_FFT/4) // corresponds to ¡°x¡± = 0 radians
				{
					if(sin_index == 0)//½»»»TempReB£¬TempImB£»
					{
						TempImA2 = TempReB;
						TempReB = -TempImB;
						TempImB = TempImA2;
					}
					TempReA>>=1;
					TempImB>>=1;
					TempImA>>=1;
					TempReB>>=1;
					TempReB2=TempReA-TempImB;
					TempReA2=TempReA+TempImB;
					TempImB=TempImA+TempReB;
					if(sin_index==0)
					{
						TempReB = TempImB;
					}
					TempImA=TempImA - TempReB;
				}
				else
				{
					// If no multiplication shortcuts can be taken, the SIN and COS
					// values for the Butterfly calculation are fetched from the
					// SinTable[] array.
					if (sin_index > NUM_FFT/4)
					{
						SinVal = (int16)pgm_read_word(SinTable+(NUM_FFT/2) - sin_index);//SinTable[(NUM_FFT/2) - sin_index];
						CosVal = -(int16)pgm_read_word(SinTable+sin_index - (NUM_FFT/4));//-SinTable[sin_index - (NUM_FFT/4)];
					}
					else
					{
						SinVal = (int16)pgm_read_word(SinTable+sin_index);//SinTable[sin_index];
						CosVal = (int16)pgm_read_word(SinTable+(NUM_FFT/4) - sin_index);//SinTable[(NUM_FFT/4) - sin_index];
					}
					// The SIN and COS values are used here to calculate part of the
					// Butterfly equation
					ReTwid.l = ((int32)TempReB * CosVal) +
						((int32)TempImB * SinVal);
					ImTwid.l = ((int32)TempImB * CosVal) -
						((int32)TempReB * SinVal);
					TempReA2 = ReTwid.i[1]+(TempReA>>1);
					TempReB2=TempReA-TempReA2;
					TempImA2=ImTwid.i[1]+(TempImA>>1);
					TempImB = TempImA-TempImA2;
					TempImA=TempImA2;
				}
				ReArray[indexA] = TempReA2;
				ReArray[indexB] = TempReB2;
				ImArray[indexA] = TempImA;
				ImArray[indexB] = TempImB;
				indexA++;
				sin_index += group;
			} // END of stage FOR loop (s_cnt)
			indexA = indexB + 1;
			sin_index = 0;
		} // END of group FOR loop (g_cnt)
		group /= 2;
		stage *= 2;
	} // END of While loop
} // END Int_FFT
#endif

