/**************************************************************************
	MIDI2AIFF.c
	Reads a file containing MIDI values
	Converts MIDI values to sine waves
	Writes sine waves to an audio file
**************************************************************************/

#include <stdlib.h> 
#include <stdio.h>
#include <math.h> 		// Included in order to use sin() function
#include <portsf.h>		// Included in order to create audio files

int open_output(char* name, PSF_PROPS *pProps);																// Function creates output file with defined formatting
float* allocate_buffer(long bytes);																			// Function allocates the buffer in memory
long make_sine(float *buffer, double amplitude, long num_frames , double sineFreq, double samplingPeriod);	// Function creates the Sine Wave
long copy(int outfile, float* buffer, long num_frames);														// Function writes the created sine waves to the output file
void clean_up(int outfile, float* buffer);																	// Function cleans up and ends portsf and the buffer
long attack(float *buffer, long attack_frames, float warpin);												// Function implements the attack
long decay(float *endBuf, long decay_frames, float warpout);												// Function implements the decay
float velAmp(float velo);																					//Function converts MIDI velocity to amplitude
float midiFreq(float midi);																					// Function converts MIDI value to frequency

enum{ARG_NAME, ARG_INFILE, ARG_OUTFILE, ARG_SAMP_RATE, ARG_SPEED, ARG_TRAN, ARGC};							//Mnemonics for user inputted arguments

int main(int argc, char *argv[])																			// Main function										
{
	if (argc < ARGC)												// Check appropriate number of user arguments
	{
		printf("\n* Error - Not enough arguments provided *\n\n");	// Error message displayed if there aren't enough arguments
		return 0;													// Programme is ended
	}
	
		// Variable initialisation
	
	float semi = atof(argv[ARG_TRAN]);								// Variable holds transposition number, in Semitones
	long tempo = atol(argv[ARG_SPEED]);								// Variable holds tempo value
	
	if (tempo == 0)													// Check tempo value isn't '0', and sets it to '1' if it is
	{
		tempo = 1;
	}
	
	char* inputFile = argv[ARG_INFILE];								// Variable holding user specified input file
	long sampleRate = atol(argv[ARG_SAMP_RATE]);					// Variable holding user specified Sampling rate
	float samplingPeriod = 1.0/sampleRate;							// Variable holding the sampling period calculated from sampling rate

		// Sine wave variables initialisation

	float midivalue;												// Variable to hold the MIDI note value from the input file
	float midifreq;													// Variable to hold the note frequency value, calculated from the MIDI note value
	float velocity;													// Variable to hold the velocity value from the input file
	float amplitude;												// Variable to hold the amplitude value, calculated from velocity value
	float duration;													// Variable to hold the note duration value
	
		// Attack and decay variables initialisation
		
	float warpin;													// Variable to hold the attack warp factor from the input file
	float warpout;													// Variable to hold the decay warp factor from the input file
	float warptimein;												// Variable to hold the attack time from the input file
	float warptimeout;												// Variable to hold the decay time from the input file
		
	// Start portsf 
    
    if (psf_init())													// Error warning if portsf is unable to opened
    {
        printf("\nError: unable to open portsf\n");
        return -2;
    }
	
	// Portsf variable initialisation
		
    int i;

    PSF_PROPS props;
    int outfile;
    float* buffer;
    long num_frames;
    long bytes;
    
	// Set the output file format
	
    props.srate = sampleRate; 										// Sampling rate from user
	props.chans = 1; 												// Mono
	props.samptype = PSF_SAMP_16; 									// 16 bit
	props.format = PSF_AIFF; 										// AIFF file
	props.chformat = MC_MONO; 										// Mono speaker configuration
		
	// Create output audio file
	
	outfile = open_output(argv[ARG_OUTFILE], &props);
	if(outfile < 0)
        return -10;
	
	FILE *ifp, *ofp; 												// declare file pointer 
	ifp = fopen(inputFile, "r");									// Open desired file
	
	if (ifp == NULL)												// Error message if file name is invalid
	
	{
		printf("\n* Error - Read Unsuccessful *\n\n");		
		return 0;
	}
	
	// While loop reads each line of data from the input file, then converts the data into a sine wave to be outputted
	
	while(fscanf(ifp, "%f %f %f %f %f %f %f", &midivalue, &velocity, &duration, &warptimein, &warpin, &warptimeout, &warpout)!= EOF)
	{
		midifreq = midiFreq(midivalue+semi);								// Function calculates frequency of the MIDI note
		amplitude = velAmp(velocity);										// Function calculates the amplitude from the MIDI velocity
		duration = (duration / tempo);										// Sets the length of each note, using the tempo factor

    	// Calculate number of bytes and allocate buffer
    
    	num_frames = (long) (duration * sampleRate);						// Calculates number of frames
    	bytes = num_frames * props.chans * sizeof(float);					// Convert to bytes
    	buffer = allocate_buffer(bytes);									// Allocate buffer
    	
    	if(buffer == NULL)
    	{
    		clean_up(outfile, buffer);
    		return -15;
    	}
	
		make_sine(buffer, amplitude, num_frames , midifreq, samplingPeriod);// Function creates the Sine Wave for the note using the variables
	
		long attack_frames = warptimein*sampleRate;							// Calculates number of frames needed for the note attack 
		long decay_frames = warptimeout*sampleRate;							// Calculates number of frames needed for the note decay
	
		attack(buffer, attack_frames, warpin);								// Function creates and applies the attack
	
		float *endBuf = buffer + (num_frames - decay_frames);				// Setting the buffer to the beginning of the decay
	
		decay(endBuf, decay_frames, warpout);								// Function creates and applies the decay
	
	    copy( outfile, buffer, num_frames);									// Copy the created sound to the output file
	    
	    free(buffer);														// Free the buffer
	    
	    buffer = NULL;	

    }
	
	  
	printf("\nCreating %s...\n", argv[ARG_OUTFILE]);				// User message to confirm the output file is being created
		
   	clean_up(outfile, buffer);										// Cleans up and ends the buffer
    
    psf_finish();													// Finishes portsf

    printf("\nCompleted file creation\n\n");						// User message to confirm the process has been completed successfully
    
    return 0;														// End program
    
}

int open_output(char* name, PSF_PROPS *pProps)																// Function creates output file with defined formatting
{
    int outfile;																		

    outfile = psf_sndCreate(name, pProps, 0, 0, PSF_CREATE_RDWR);						// Creating file
    
    if (outfile < 0)
    {
        printf("\nError: unable to create %s\n", name);									// Error message if unable to create file
    }
    
    return outfile;																		// Returns if successful
}
	
float* allocate_buffer(long bytes)																			// Function allocates the buffer in memory
{   
	float *buffer;
	
    buffer = (float*) malloc(bytes);													// Allocating buffer
	if(buffer == 0)
	{
		printf("\nError: unable to allocate buffer\n");									// Error message if unable to allocate buffer
	}
	
	return buffer;																		// Returns if successful
}

long copy(int outfile, float* buffer, long num_frames)														// Function writes the created sine waves to the output file
{
	long frames;
	
    frames = psf_sndWriteFloatFrames(outfile, buffer, num_frames);						// Writing to the output file
    if(frames != num_frames)
    {
        printf("\nError writing to output\n");											// Error message if unable to write to the output
        return -1;
    }

    return frames;																		// Returns if successful
}

long make_sine(float *buffer, double amplitude, long num_frames , double sineFreq, double samplingPeriod)	// Function creates the Sine Wave
{

	long i;
	double time;
	
	double twoPi = 2*M_PI; 																// M_PI is in math.h. Calculating 2xM_PI saves many calculations that can slow down the application
	
	for(i = 0, time = 0 ; i < num_frames ; i++ )										// Steps through the sine wave, sample at a time
	{
		buffer[i] = amplitude*sin(twoPi * sineFreq * time); 							// multiply sine value by amplitude: sine value = sin(2 * PI * frequency * time)
		time += samplingPeriod; 														// update time
	}

	return i; 																			// Return the number of frames created
}

void clean_up(int outfile, float* buffer)																	// Function cleans up and ends portsf and the buffer
{
    
    if (outfile >= 0)
    {
        if (psf_sndClose(outfile))
        {
            printf("\nWarning: error closing output\n");								// Error message if the output closes
        }
    }
	
	if(buffer)																			// Frees the buffer
	{
		free(buffer);																	
    }
}

 long attack(float *buffer, long attack_frames, float warpin)												// Function implements the attack
{
	long i;
	double factor = 0.0; 																// Attack factor begins at 0 
	double increment = 1.0 / attack_frames;
	for(i = 0 ; i < attack_frames ; i++ )												// Loops for number of frames required
	{
		buffer[i] = pow(factor,warpin) * buffer[i];										// Applies attack, incorporating the warp factor
		factor += increment; 															// Increase factor
	}
	return i; 																			// Return the number of frames created
}

long decay(float *endBuf, long decay_frames, float warpout)													// Function implements the decay
{
	long i;
	double factor = 1.0; 																// Decay factor begins at 1
	double decrement = 1.0 / decay_frames;
	for(i = 0 ; i < decay_frames ; i++ )												// Loops for number of frames required
	{
		endBuf[i] = pow(factor,warpout) * endBuf[i];									// Applies decay, incorporating the warp factor
		factor -= decrement; 															// Decrease factor
	}
	return i; 																			// Return the number of frames created 
}

float midiFreq(float midi)																					// Function converts MIDI value to frequency
{
	float powerNum; 																	// The variable will be used to calculate the power of 2
	float freq;																			// The variable will hold calculated frequency of the MIDI number
		
	powerNum = (midi - 69) / 12;														// 		Calculations for the equation:
	freq = pow(2.0, powerNum) * 440;													// 		𝑓 = 2^(𝑝−69)/12 × 440

	return freq;																		// Returns the calculated frequency
}

float velAmp(float velo)																					//Function converts MIDI velocity to amplitude
{
	velo = velo/127;																	// Velocity divided by 127 gives an amplitude figure
	return velo;
}