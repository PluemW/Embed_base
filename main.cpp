// #include <SD.h>                      // need to include the SD library
// //#define SD_ChipSelectPin 53  //example uses hardware SS pin 53 on Mega2560
// #define SD_ChipSelectPin 53  //using digital pin 4 on arduino nano 328, can use other pins
// #include <TMRpcm.h>           //  also need to include this library...
// #include <SPI.h>
// #include <Tone.h>


// TMRpcm tmrpcm;   // create an object for use in this sketch
// File myFile;
// Tone notePlayer[2];


// void setup(){

//   // myFile = SD.open("2");
//   tmrpcm.speakerPin = 47; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
//     // notePlayer[0].begin(46);

//   Serial.begin(9600);
//   if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
//     Serial.println("SD fail");  
//     return;   // don't do anything more if not
//   }
// }



// void loop(){  
//   // notePlayer[0].play(myFile);
//   tmrpcm.setVolume(10);
//   tmrpcm.play("2.wav");
//   Serial.println("123");
// }
// Duelling Tones - Simultaneous tone generation.
// To mix the output of the signals to output to a small speaker (i.e. 8 Ohms or higher),
// simply use 1K Ohm resistors from each output pin and tie them together at the speaker.
// Don't forget to connect the other side of the speaker to ground!

// This example plays notes 'a' through 'g' sent over the Serial Monitor.
// 's' stops the current playing tone.  Use uppercase letters for the second.

#include <Tone.h>
#include <Arduino.h>
int notes[] = { NOTE_A3,
                NOTE_B3,
                NOTE_C4,
                NOTE_D4,
                NOTE_E4,
                NOTE_F4,
                NOTE_G4 };

// You can declare the tones as an array
Tone notePlayer[2];

void setup(void)
{
  // Serial.begin(9600);
  notePlayer[0].begin(47);
  notePlayer[1].begin(12);
}

void loop(void)
{
  char c='g';
        notePlayer[0].play(notes[c - 'a']);

}
