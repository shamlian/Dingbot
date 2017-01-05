#ifndef __KEYS_H

#define NUMKEYS 30
#define STARTKEY 60 // middle C
#define lSTRIKE 32 //= 78 ~= 20 ms; if defined, lstrike[] is unused

//                                C   C#  D   D#  E   F   F#  G   G#  A   A#  B   C   C#  D   D#  E   F   F#  G   G#  A   A#  B   C   C#  D   D#  E   F    
const uint8_t lstrike[NUMKEYS] = {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30};

void keysinit(void);
void queuenote(uint8_t k);
void strike(uint8_t k);
void release(uint8_t k);

#define __KEYS_H
#endif