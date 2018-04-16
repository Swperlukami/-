
#ifndef _UDAPCM_H
#define _UDAPCM_H

#define FRAME_SIZE  1024    
#define SAMPLEBYTE  2      
#define CHANNEL_NUM 1       
#define SAMPLE_RATE 8000   
#define ALLOC_BUFSIZE 2048

void StopPlayBack(void);
void RePlayBack(void);
void PlayBack_Initial();
void PlayBack_Exit(void);
void PlayOneFrame(char *buf,int len);
void AdjustSampleRate(int nSamplerate);   

extern char *CaptureBuf;

void Capture_Initial(void);
void Capture_Exit(void);
void ReadOneFrame(void);
void StopCapture(void);
void ReCapture(void);

#endif
