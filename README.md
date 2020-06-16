# User Manual

Thank you for using this program. In short, this program will take a user-specified text file which contains the MIDI figures for a series of notes, and convert the information and generate an audio file of the notes. This manual will walk you through how to use the program, explain all the features and answer any queries you may have.

\*\*The following instructions are for Mac only, information is online on how to run them on PC\*\*

## The Basics – Running the Program

The program is run from the command line; here&#39;s a step by step guide to use the program:

- First, you will need to open the terminal.
- Next, you will need the full file-path to the folder. Then type:

cd _file-path_

where _file-path_ is the file path to your folder. If there are any spaces in the path, then enclose it with a pair of single quote marks (e.g. &#39;_file-path&#39;_). Then press enter.

- Then run the program. For now we have provided an example input file and suggested values for you to get started quickly. Type:

./MIDI2AIFF BaaBaa.txt OutputFile.aiff 44100 1 0

 Then press enter, and you&#39;ll have a sound file called _OutputFile.aiff_.

 The line consists of the program name, followed by the text file you are using, then the name for the audio file being generated. The numbers will be explained later in the manual.

## The Features

The features allow you to be flexible with the output of your audio file. Some are controlled in the command, and some are controlled in the text file. The next few sections will explain each feature and how to use them.

### Sampling Rate

The first feature is the sampling rate of the output file. The figure you specify will be the number of samples per second; measured in hertz (Hz).

The value may be whatever you wish, however, just be aware of the issues than can arise if set too high or low. We recommend that setting it to around 44,100 Hz, as shown in the previous section. This is the music industry standard for CDs and other commercial formats.

To apply the sampling rate, simply change the first numerical value when typing the command to run the program to your chosen sample rate.

###

### Changing the Tempo

The next feature is the tempo of the audio file you create. This is controlled by a multiplication factor to apply an increase or decrease to the speed of the audio. The value 1 will keep the length of each note the same length as defined by in the input text file.
 A value greater than 1 will decrease the tempo, making each note longer than as defined in the input text file. E.g., the value 1.5 will mean each note is output 1.5 times longer than defined; so a two second note will become three seconds.
 A value less than 1 will increase the tempo, making each note shorter than as defined in the input text file. E.g., the value 0.5 will mean each note being output 0.5 times shorter than defined; so a two second note will become one second.

 You are unable to enter the value of 0, as this would result in no audio. The program will automatically correct a 0 to a value of 1.

To apply the tempo, simply change the second numerical value when typing the command to run the program to your chosen tempo factor.

### Transposing

The third feature is the transposing tool. You can change the pitch of the audio up or down from that defined by the input text file, by expressing how many semitones you wish to change it by.

A positive value will increase the pitch by the number of semitones entered. Conversely, a negative number will decrease the pitch by the number of semitones entered. A 0 means no transposition is applied, and the pitch will remain as defined.

To apply the tempo, simply change the third numerical value when typing the command to run the program to your chosen transposition factor.

### Attack and Decay

##

An attack and decay envelope can be applied to each note; this is controlled in the input text file.

The fourth and fifth columns in the input text file relate to the attack and the sixth and seventh columns to the decay. The left hand column in each case denotes the length, in seconds, of the attack/decay, and the right hand column denotes the warp factor.
 In the example below, the attack will last 0.1 seconds, and the decay will last 0.2 seconds; both with a warp factor of 0.5.

The warp factor denotes the shape of the attack or decay.

##

Figure 5

##

## Formatting the Text File

The text file needs to be formatted correctly, so the audio file is then generated as desired.

The file is made up of seven different columns. Column one is the MIDI note value, and column two is the MIDI velocity value. Column three is the note length in seconds. The final four columns are the attack and decay as explained previously.

Both the MIDI note and MIDI velocity values must be within the 0 to 127 range.

Each line details a single note; press enter to input another line.
 Each item of data for each note is separated pressing the tab button on your keyboard, not spacebar.
