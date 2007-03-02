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
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
//#include <c8051f120.h> // SFR declarations
#include <stdio.h>
#include <global.h>
#include "FFT_Code_Tables.c" // Code Tables for FFT routines
//-----------------------------------------------------------------------------
// 16-bit SFR Definitions for ¡®F12x
//-----------------------------------------------------------------------------
sfr16 DP = 0x82; // data pointer
sfr16 ADC0 = 0xbe; // ADC0 data
sfr16 ADC0GT = 0xc4; // ADC0 greater than window
sfr16 ADC0LT = 0xc6; // ADC0 less than window
sfr16 RCAP2 = 0xca; // Timer2 capture/reload
sfr16 RCAP3 = 0xca; // Timer3 capture/reload
sfr16 RCAP4 = 0xca; // Timer4 capture/reload
sfr16 TMR2 = 0xcc; // Timer2
sfr16 TMR3 = 0xcc; // Timer3
sfr16 TMR4 = 0xcc; // Timer4
sfr16 DAC0 = 0xd2; // DAC0 data
sfr16 DAC1 = 0xd2; // DAC1 data
sfr16 PCA0CP5 = 0xe1; // PCA0 Module 5 capture
sfr16 PCA0CP2 = 0xe9; // PCA0 Module 2 capture
sfr16 PCA0CP3 = 0xeb; // PCA0 Module 3 capture
sfr16 PCA0CP4 = 0xed; // PCA0 Module 4 capture
sfr16 PCA0 = 0xf9; // PCA0 counter
sfr16 PCA0CP0 = 0xfb; // PCA0 Module 0 capture
sfr16 PCA0CP1 = 0xfd; // PCA0 Module 1 capture
//-----------------------------------------------------------------------------
// Global CONSTANTS and Variable Type Definitions
//-----------------------------------------------------------------------------
#define NUM_BITS 16 // Number of Bits in Data
#define DATA_BEGIN 0x0000 // Beginning of XRAM Data
#define EXTCLK 22118400 // External oscillator frequency in Hz
#define SYSCLK 49760000 // Output of PLL derived from
// (EXTCLK*9/4)
#define BAUDRATE 115200 // Baud Rate for UART0
#define SAMPLE_RATE 10000 // Sample frequency in Hz
#define RUN_ONCE 1 // Setting to a non-zero value will
// cause the program to stop after one
// data set.
typedef union IBALONG { // Integer or Byte-addressable LONG
	long l; // long: Var.l
	unsigned int i[2]; // u int: Var.i[0]:Var.i[1]
	unsigned char b[4]; // u char: Var.b[0]:Var.b[1]:
	// Var.b[2]:Var.b[3]
} IBALONG;
typedef union BAINT { // Byte-addressable INT
	int i; // int: Var.i
	unsigned char b[2]; // u char: Var.b[0]:Var.b[1]
} BAINT;
//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------
void WindowCalc(int Win_Array[], unsigned char SE_data);
void Int_FFT(int ReArray[], int ImArray[]);
void Bit_Reverse(int BR_Array[]);
void SYSCLK_Init (void);
void PORT_Init (void);
void UART0_Init (void);
void ADC0_Init (void);
void TIMER3_Init (int counts);
void ADC0_ISR (void);
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
// XRAM storage of FFT: requires NUM_FFT*4 Bytes after DATA_BEGIN address
int xdata Real[NUM_FFT] _at_ DATA_BEGIN;
int xdata Imag[NUM_FFT] _at_ (DATA_BEGIN + (NUM_FFT * 2));


// NUM_FFT is defined in the ¡°FFT_Code_Tables.h¡± header file
#if (NUM_FFT >= 256)
unsigned int index, ADC_Index;
#endif
#if (NUM_FFT < 256)
unsigned char index, ADC_Index;
#endif
unsigned int BinNum;
bit Conversion_Set_Complete; // This indicates when the data has been
// stored, and is ready to be processed
// using the FFT routines
//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
void main()
{ 
	// disable watchdog timer
	WDTCN = 0xde;
	WDTCN = 0xad;
	while(1)
	{
		int i =0;
		double ff = 0.0;
		for(;i<sizeof(Real)/sizeof(int16);i++)
		{
		switch(i%4)
		{
		case 2:
		case 0:Real[i] = (0);break;
		case 1:Real[i] = (1000);break;
		case 3:Real[i] = (-1000);break;
		}
			
		//	ff+=0.1;
		}
		WindowCalc(Real, 0); // Window Real Data, and convert to
		// differential if it is single-ended
		Bit_Reverse(Real); // Sort Real (Input) Data in bit-reverse
		// order
		Int_FFT(Real, Imag); // Perform FFT on data

	}

	SYSCLK_Init(); // initialize external clock and PLL
	PORT_Init (); // set up Port I/O
	UART0_Init (); // initialize UART0
	TIMER3_Init (SYSCLK/SAMPLE_RATE); // initialize Timer3 to overflow at
	// <SAMPLE_RATE>
	ADC0_Init (); // init ADC0
	EA = 1; // globally enable interrupts
	while (1)
	{
		ADC_Index = 0;
		Conversion_Set_Complete = 0;
		EIE2 |= 0x02; // enable ADC interrupts
		SFRPAGE = LEGACY_PAGE;
		while(!Conversion_Set_Complete);
		SFRPAGE = UART0_PAGE;
		printf("\nCollected Data\nSample\tValue\n");
		for (BinNum = 0; BinNum < NUM_FFT; BinNum++)
		{
			// Print Data in the format: Sample <tab> Value <tab>
			printf("%d\t%u\n", BinNum, Real[BinNum]);
		}
		WindowCalc(Real, 1); // Window Real Data, and convert to
		// differential if it is single-ended
		Bit_Reverse(Real); // Sort Real (Input) Data in bit-reverse
		// order
		Int_FFT(Real, Imag); // Perform FFT on data
		SFRPAGE = UART0_PAGE;
		printf("\nBin\tReal\tImag\n");
		// Output the FFT data to the UART
		for (BinNum = 0; BinNum < NUM_FFT; BinNum++)
		{
			// Print Data in the format: Bin <tab> Real <tab> Imaginary
			printf("%d\t%d\t%d\n", BinNum, Real[BinNum], Imag[BinNum]);
		}
		if (RUN_ONCE)
			while(1);
	}
} // END MAIN
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
void WindowCalc(int Win_Array[], unsigned char SE_data)
{
#if (WINDOW_TYPE != 0) // Use this section if a window has been specified
	IBALONG NewVal;
	if (SE_data) // If data is single-ended,
		Win_Array[0] ^= 0x8000; // convert it to differential
	NewVal.l = (long)Win_Array[0] * WindowFunc[0];
	if ((NewVal.l < 0)&&(NewVal.i[1]))
		Win_Array[0] = NewVal.i[0] + 1;
	else Win_Array[0] = NewVal.i[0];
	if (SE_data) // If data is single-ended,
		Win_Array[NUM_FFT/2] ^= 0x8000; // convert it to differential
	for (index = 1; index < NUM_FFT/2; index++)
	{
		// Array positions 1 to (NUM_FFT/2 - 1)
		if (SE_data) // If data is single-ended,
			Win_Array[index] ^= 0x8000; // convert it to differential
		NewVal.l = (long)Win_Array[index] * WindowFunc[index];
		if ((NewVal.l < 0)&&(NewVal.i[1]))
			Win_Array[index] = NewVal.i[0] + 1;
		else Win_Array[index] = NewVal.i[0];
		// Array positions (NUM_FFT/2 + 1) to (NUM_FFT - 1)
		if (SE_data) // If data is single-ended,
			Win_Array[NUM_FFT-index] ^= 0x8000; // convert it to differential
		NewVal.l = (long)Win_Array[NUM_FFT-index] * WindowFunc[index];
		if ((NewVal.l < 0)&&(NewVal.i[1]))
			Win_Array[NUM_FFT-index] = NewVal.i[0] + 1;
		else Win_Array[NUM_FFT-index] = NewVal.i[0];
	}
#endif
#if (WINDOW_TYPE == 0) // Compile this if no window has been specified
	if (SE_data) // If data is single-ended,
	{ // convert it to differential
		for (index = 0; index < NUM_FFT; index++)
		{
			Win_Array[index] ^= 0x8000; // XOR MSB with ¡®1¡¯ to invert
		}
	}
#endif
} // END WindowCalc
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
void Bit_Reverse(int BR_Array[])
{
#if (NUM_FFT >= 512)
	unsigned int swapA, swapB, sw_cnt; // Swap Indices
#endif
#if (NUM_FFT <= 256)
	unsigned char swapA, swapB, sw_cnt; // Swap Indices
#endif

	int TempStore;
	// Loop through locations to swap
	for (sw_cnt = 1; sw_cnt < NUM_FFT/2; sw_cnt++)
	{
		swapA = sw_cnt; // Store current location
		swapB = BRTable[sw_cnt] * 2; // Retrieve bit-reversed index
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
} // END Bit Reverse Order Sort
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
void Int_FFT(int ReArray[], int ImArray[])
{
#if (NUM_FFT >= 512)
	unsigned int sin_index, g_cnt, s_cnt; // Keeps track of the proper index
	unsigned int indexA, indexB; // locations for each calculation
#endif
#if (NUM_FFT <= 256)
	unsigned char sin_index, g_cnt, s_cnt; // Keeps track of the proper index
	unsigned char indexA, indexB; // locations for each calculation
#endif
	unsigned int group = NUM_FFT/4, stage = 2;
	long CosVal, SinVal;
	long TempImA, TempImB, TempReA, TempReB, TempReA2, TempReB2;
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
	for (g_cnt = 0; g_cnt < NUM_FFT/2; g_cnt++)
	{
		indexB = indexA + 1;
		TempReA = ReArray[indexA];
		TempReB = ReArray[indexB];
		// Calculate new value for ReArray[indexA]
		TempL.l = (long)TempReA + TempReB;
		if ((TempL.l < 0)&&(0x01 & TempL.b[3]))
			TempReA2 = (TempL.l >> 1) + 1;
		else TempReA2 = TempL.l >> 1;
		// Calculate new value for ReArray[indexB]
		TempL.l = (long)TempReA - TempReB;
		if ((TempL.l < 0)&&(0x01 & TempL.b[3]))
			ReArray[indexB] = (TempL.l >> 1) + 1;
		else ReArray[indexB] = TempL.l >> 1;
		ReArray[indexA] = TempReA2;
		ImArray[indexA] = 0; // set Imaginary locations to ¡®0¡¯
		ImArray[indexB] = 0;
		indexA = indexB + 1;
	}
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
				if (sin_index == 0) // corresponds to ¡°x¡± = 0 radians
				{
					// Calculate new value for ReArray[indexA]
					TempL.l = (long)TempReA + TempReB;
					if ((TempL.l < 0)&&(0x01 & TempL.b[3]))
						TempReA2 = (TempL.l >> 1) + 1;
					else TempReA2 = TempL.l >> 1;
					// Calculate new value for ReArray[indexB]
					TempL.l = (long)TempReA - TempReB;
					if ((TempL.l < 0)&&(0x01 & TempL.b[3]))
						TempReB2 = (TempL.l >> 1) + 1;
					else TempReB2 = TempL.l >> 1;
					// Calculate new value for ImArray[indexB]
					TempL.l = (long)TempImA - TempImB;
					if ((TempL.l < 0)&&(0x01 & TempL.b[3]))
						TempImB = (TempL.l >> 1) + 1;
					else TempImB = TempL.l >> 1;
					// Calculate new value for ImArray[indexA]
					TempL.l = (long)TempImA + TempImB;
					if ((TempL.l < 0)&&(0x01 & TempL.b[3]))
						TempImA = (TempL.l >> 1) + 1;
					else TempImA = TempL.l >> 1;
				}
				else if (sin_index == NUM_FFT/4) // corresponds to ¡°x¡± = pi/2 radians
				{
					// Calculate new value for ReArray[indexB]
					TempL.l = (long)TempReA - TempImB;
					if ((TempL.l < 0)&&(0x01 & TempL.b[3]))
						TempReB2 = (TempL.l >> 1) + 1;
					else TempReB2 = TempL.l >> 1;
					// Calculate new value for ReArray[indexA]
					TempL.l = (long)TempReA + TempImB;
					if ((TempL.l < 0)&&(0x01 & TempL.b[3]))
						TempReA2 = (TempL.l >> 1) + 1;
					else TempReA2 = TempL.l >> 1;
					// Calculate new value for ImArray[indexB]
					TempL.l = (long)TempImA + TempReB;
					if ((TempL.l < 0)&&(0x01 & TempL.b[3]))
						TempImB = (TempL.l >> 1) + 1;
					else TempImB = TempL.l >> 1;
					// Calculate new value for ImArray[indexA]
					TempL.l = (long)TempImA - TempReB;
					if ((TempL.l < 0)&&(0x01 & TempL.b[3]))
						TempImA = (TempL.l >> 1) + 1;
					else TempImA = TempL.l >> 1;
				}
				else
				{
					// If no multiplication shortcuts can be taken, the SIN and COS
					// values for the Butterfly calculation are fetched from the
					// SinTable[] array.
					if (sin_index > NUM_FFT/4)
					{
						SinVal = SinTable[(NUM_FFT/2) - sin_index];
						CosVal = -SinTable[sin_index - (NUM_FFT/4)];
					}
					else
					{
						SinVal = SinTable[sin_index];
						CosVal = SinTable[(NUM_FFT/4) - sin_index];
					}
					// The SIN and COS values are used here to calculate part of the
					// Butterfly equation
					ReTwid.l = ((long)TempReB * CosVal) +
						((long)TempImB * SinVal);
					ImTwid.l = ((long)TempImB * CosVal) -
						((long)TempReB * SinVal);
					// Using the values calculated above, the new variables
					// are computed
					// Calculate new value for ReArray[indexA]
					TempL.i[1] = 0;
					TempL.i[0] = TempReA;
					TempL.l = TempL.l >> 1;
					ReTwid.l += TempL.l;
					if ((ReTwid.l < 0)&&(ReTwid.i[1]))
						TempReA2 = ReTwid.i[0] + 1;
					else TempReA2 = ReTwid.i[0];
					// Calculate new value for ReArray[indexB]
					TempL.l = TempL.l << 1;
					TempL.l -= ReTwid.l;
					if ((TempL.l < 0)&&(TempL.i[1]))
						TempReB2 = TempL.i[0] + 1;
					else TempReB2 = TempL.i[0];
					// Calculate new value for ImArray[indexA]
					TempL.i[1] = 0;
					TempL.i[0] = TempImA;
					TempL.l = TempL.l >> 1;
					ImTwid.l += TempL.l;
					if ((ImTwid.l < 0)&&(ImTwid.i[1]))
						TempImA = ImTwid.i[0] + 1;
					else TempImA = ImTwid.i[0];
					// Calculate new value for ImArray[indexB]
					TempL.l = TempL.l << 1;
					TempL.l -= ImTwid.l;
					if ((TempL.l < 0)&&(TempL.i[1]))
						TempImB = TempL.i[0] + 1;
					else TempImB = TempL.i[0];
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
//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// Configure the Crossbar and GPIO ports
//
void PORT_Init (void)
{
	char old_SFRPAGE = SFRPAGE; // Store current SFRPAGE
	SFRPAGE = CONFIG_PAGE; // Switch to configuration page
	XBR0 = 0x04; // Enable UART0 on crossbar
	XBR1 = 0x00;
	XBR2 = 0x40; // Enable crossbar and weak pull-ups
	P0MDOUT |= 0x01; // Set TX0 pin to push-pull
	SFRPAGE = old_SFRPAGE; // restore SFRPAGE
}
//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
// Configure the UART0 using Timer1, for <baudrate> and 8-N-1. In order to
// increase the clocking flexibility of Timer0, Timer1 is configured to count
// SYSCLKs.
//
// To use this routine SYSCLK/BAUDRATE/16 must be less than 256. For example,
// if SYSCLK = 50 MHz, the lowest standard baud rate supported by this
// routine is 19,200 bps.
void UART0_Init (void)
{
	char old_SFRPAGE = SFRPAGE; // Store current SFRPAGE
	SFRPAGE = UART0_PAGE; // Switch to UART0 Page
	SCON0 = 0x50; // SCON0: mode 0, 8-bit UART, enable RX
	SSTA0 = 0x10; // Timer 1 generates UART0 baud rate and
	// UART0 baud rate divide by two disabled
	SFRPAGE = TIMER01_PAGE;
	TMOD &= ~0xF0;
	TMOD |= 0x20; // TMOD: timer 1, mode 2, 8-bit reload
	TH1 = -(SYSCLK/BAUDRATE/16); // Set the Timer1 reload value
	// When using a low baud rate, this
	// equation should be checked to ensure
	// that the reload value will fit in
	// 8-bits.
	CKCON |= 0x10; // T1M = 1; SCA1:0 = xx
	TL1 = TH1; // initialize Timer1
	TR1 = 1; // start Timer1
	SFRPAGE = UART0_PAGE;
	TI0 = 1; // Indicate TX0 ready
	SFRPAGE = old_SFRPAGE; // restore SFRPAGE
}
//-----------------------------------------------------------------------------
// SYSCLK_Init
//-----------------------------------------------------------------------------
//
// This routine initializes the system clock to use an external 22.1184 MHz
// crystal oscillator multiplied by a factor of 9/4 using the PLL as its
// clock source. The resulting frequency is 22.1184 MHz * 9/4 = 49.7664 MHz
//
void SYSCLK_Init (void)
{
	int i; // delay counter
	char old_SFRPAGE = SFRPAGE; // Store current SFRPAGE
	SFRPAGE = CONFIG_PAGE; // set SFR page
	OSCXCN = 0x67; // start external oscillator with
	// 22.1184MHz crystal
	for (i=0; i < 256; i++) ; // Wait for osc. to start up
	while (!(OSCXCN & 0x80)) ; // Wait for crystal osc. to settle
	CLKSEL = 0x01; // Select the external osc. as
	// the SYSCLK source
	OSCICN = 0x00; // Disable the internal osc.
	//Turn on the PLL and increase the system clock by a factor of M/N = 9/4
	SFRPAGE = PLL0_PAGE;
	PLL0CN = 0x04; // Set PLL source as external osc.
	SFRPAGE = LEGACY_PAGE;
	FLSCL = 0x10; // Set FLASH read time for 50MHz clk
	// or less
	SFRPAGE = PLL0_PAGE;
	PLL0CN |= 0x01; // Enable Power to PLL
	PLL0DIV = 0x04; // Set Pre-divide value to N (N = 4)
	PLL0FLT = 0x01; // Set the PLL filter register for
	// a reference clock from 19 - 30 MHz
	// and an output clock from 45 - 80 MHz
	PLL0MUL = 0x09; // Multiply SYSCLK by M (M = 9)
	for (i=0; i < 256; i++) ; // Wait at least 5us
	PLL0CN |= 0x02; // Enable the PLL
	while(!(PLL0CN & 0x10)); // Wait until PLL frequency is locked
	CLKSEL = 0x02; // Select PLL as SYSCLK source
	SFRPAGE = old_SFRPAGE; // restore SFRPAGE
}
//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
// Configure ADC0 to use Timer3 overflows as conversion source, to
// generate an interrupt on conversion complete, and to use left-justified
// output mode. Enables ADC0 end of conversion interrupt. Enables ADC0.
//
void ADC0_Init (void)
{
	char old_SFRPAGE = SFRPAGE; // Store current SFRPAGE
	SFRPAGE = ADC0_PAGE; // Switch to ADC0 Setup Page
	ADC0CN = 0x05; // ADC disabled; normal tracking
	// mode; ADC conversions are initiated
	// on overflow of Timer3, left-justify
	REF0CN = 0x03; // enable on-chip VREF and output buffer
	AMX0CF = 0x00; // Single-ended AIN0.0 input
	AMX0SL = 0x00;
	ADC0CF = (SYSCLK/(2*2500000)) << 3; // ADC conversion clock <= 2.5MHz
	ADC0CF |= 0x00; // PGA gain = 1
	AD0EN = 1; // enable ADC0
	SFRPAGE = old_SFRPAGE; // restore SFRPAGE
}
//-----------------------------------------------------------------------------
// TIMER3_Init
//-----------------------------------------------------------------------------
//
// Configure Timer3 to auto-reload at interval specified by <counts> (no
// interrupt generated) using SYSCLK as its time base.
//
void TIMER3_Init (int counts)
{
	char old_SFRPAGE = SFRPAGE; // Save Current SFR page
	SFRPAGE = TMR3_PAGE; // Switch to Timer3 Setup Page
	TMR3CN = 0x00; // Stop Timer3; Clear TF3
	TMR3CF = 0x08; // use SYSCLK as timebase
	RCAP3 = -counts; // Init reload values
	TMR3 = 0xffff; // set to reload immediately
	EIE2 &= ~0x01; // disable Timer3 interrupts
	TR3 = 0x01; // start Timer3
	SFRPAGE = old_SFRPAGE; // restore SFRPAGE
}
//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// ADC0_ISR
//-----------------------------------------------------------------------------
//
// ADC end-of-conversion ISR
// The ADC sample is stored in memory, and an index variable is incremented.
// If enough samples have been taken to process the FFT, then a flag is set,
// and ADC interrupts are disabled until the next set is requested.
//
void ADC0_ISR (void) interrupt 15 using 3
{
	AD0INT = 0; // clear ADC conversion complete
	// flag
	Real[ADC_Index] = ADC0; // store ADC value
	ADC_Index++; // Increment the index into memory
	if (ADC_Index >= NUM_FFT) // If enough samples have been collected
	{
		Conversion_Set_Complete = 1; // Tell the Main Routine and...
		EIE2 &= ~0x02; // disable ADC interrupts
	}
}