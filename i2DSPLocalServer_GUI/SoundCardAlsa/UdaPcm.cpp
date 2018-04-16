
#include "UdaPcm.h"
#include <alsa/asoundlib.h>
#include<stdio.h>

char *PlayBuf=NULL;
char *CaptureBuf=NULL;
snd_pcm_t *playback_handle;       
snd_pcm_t *capture_handle;        
snd_pcm_hw_params_t *hw_params;
snd_pcm_hw_params_t *params;

static int channels = 1;
static int sample_rate = 16000;
static snd_pcm_format_t format = SND_PCM_FORMAT_S16_LE;
static snd_pcm_sframes_t peroid_size = 1024;
static snd_pcm_sframes_t buffer_size = 1024*5;
static unsigned int resample = 1;

void PlayBack_Initial()
{
    int ret;
    unsigned int val;
    int dir=0;
    int size;

    ret = snd_pcm_open(&playback_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);  //"default"plughw:0,0
    if (ret < 0)
    {
        printf("snd_pcm_open");
        exit(1);
    }
    
    ret = snd_pcm_hw_params_malloc(&hw_params);
    if (ret < 0)
    {
        printf("snd_pcm_hw_params_malloc");
        exit(1);
    }

    ret = snd_pcm_hw_params_any(playback_handle, hw_params);
    if(ret < 0)
    {
        printf("snd_pcm_hw_params_any");
        exit(1);
    }

    ret = snd_pcm_hw_params_set_rate_resample(playback_handle,hw_params,resample);
    if(ret < 0)
    {
        printf("snd_pcm_hw_params_set_rate_resample\n");
        exit(1);
    }

    ret = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    if (ret < 0)
    {
        printf("snd_pcm_hw_params_set_access");
        exit(1);
    }
    
    ret = snd_pcm_hw_params_set_format(playback_handle, hw_params, format);
    if (ret < 0)
    {
        printf("snd_pcm_hw_params_set_format");
        exit(1);
    }
    
    val = sample_rate;
    ret = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &val, &dir);
    if (ret < 0)
    {
        printf("snd_pcm_hw_params_set_rate_near");
        exit(1);
    }
    
    ret = snd_pcm_hw_params_set_channels(playback_handle, hw_params, channels);
    if (ret < 0)
    {
        printf("snd_pcm_hw_params_set_channels");
        exit(1);
    }
    
    ret = snd_pcm_hw_params_set_period_size_near(playback_handle, hw_params, &peroid_size, 0);
    if (ret < 0)
    {
        printf("Unable to set period size %li : %s\n", peroid_size,  snd_strerror(ret));
    }
    
    ret = snd_pcm_hw_params_set_buffer_size_near(playback_handle,hw_params,(snd_pcm_uframes_t *)&buffer_size);
    if(ret < 0)
    {
        printf("Unable to set buffer size %li : %s\n", val, snd_strerror(ret));
    }

    ret = snd_pcm_hw_params(playback_handle, hw_params);
    if (ret < 0)
    {
        perror("snd_pcm_hw_params");
        exit(1);
    }

    snd_pcm_drop(playback_handle);
    snd_pcm_start(playback_handle);
    snd_pcm_prepare(playback_handle);
    printf("playback initial successful!\n");
}

void AdjustSampleRate(int nSamplerate)
{
    PlayBack_Exit();
    PlayBack_Initial(nSamplerate);
    printf("Now Change SampleRate To %d\n",nSamplerate);
}

void StopPlayBack(void)
{	
    snd_pcm_drain(playback_handle);
}

void RePlayBack(void)
{
    snd_pcm_prepare(playback_handle);
}

void PlayBack_Exit(void)
{
    free(PlayBuf);
    snd_pcm_drain(playback_handle);
    snd_pcm_close(playback_handle);
}

void PlayOneFrame(char *buf,int len)
{
    int ret=0;

    if(len != peroid_size *2)
    {
        printf("snd_pcm_writei frames error!\n");
    }

    ret = snd_pcm_writei(playback_handle,buf,peroid_size);  //peroid_size
    if(ret <0)
    {
        if (ret == -EPIPE)
        {
            snd_pcm_prepare(playback_handle);
        }
        else if(ret <0)
        {
            printf("error from writei");
        }
    }
}

void Capture_Initial(void)
{
    int rc;
    snd_pcm_uframes_t frames;
    unsigned int val;
    int dir;

    rc = snd_pcm_open(&capture_handle, "default",SND_PCM_STREAM_CAPTURE, 0);
    if (rc <0)
    {
        printf("unable to open capture pcm device!\n");
        exit(1);
    }

    snd_pcm_hw_params_malloc(&params);
    rc = snd_pcm_hw_params_any(capture_handle, params);
    if(rc < 0)
    {
        printf("snd_pcm_hw_params_any error\n");
    }

    rc = snd_pcm_hw_params_set_access(capture_handle, params,SND_PCM_ACCESS_RW_INTERLEAVED);
    if(rc < 0)
    {
        printf("snd_pcm_hw_params_set_access error\n");
    }

    rc = snd_pcm_hw_params_set_format(capture_handle, params,format);
    if(rc < 0)
    {
        printf("snd_pcm_hw_params_set_format error\n");
    }

    rc = snd_pcm_hw_params_set_channels(capture_handle, params, channels);
    if(rc < 0)
    {
        printf("snd_pcm_hw_params_set_channels error\n");
    }

    val = sample_rate;
    rc = snd_pcm_hw_params_set_rate_near(capture_handle,params,&val,&dir);
    if(rc < 0)
    {
        printf("snd_pcm_hw_params_set_rate_near error\n");
    }

    frames = 1024;
    // set period size
    rc = snd_pcm_hw_params_set_period_size_near(capture_handle,params, &frames, &dir);
    if(rc < 0)
    {
        printf("snd_pcm_hw_params_set_peroid_size_near error\n");
    }
    
    val = 5*frames;
    rc = snd_pcm_hw_params_set_buffer_size_near(capture_handle,params,(snd_pcm_uframes_t *)&val);
    if(rc < 0)
    {
        printf("Unable to set buffer size %li : %s\n", val, snd_strerror(rc));
    }

    rc = snd_pcm_hw_params(capture_handle, params);
    if (rc <0)
    {
        printf("error: %s\n",snd_strerror(rc));
        exit(1);
    }
    
    CaptureBuf = (char *) malloc(ALLOC_BUFSIZE);
    if(CaptureBuf == NULL)
    {
        printf("Can't Alloc Memery!\n");
    }
    
    snd_pcm_drop(capture_handle);
    snd_pcm_start(capture_handle);
    snd_pcm_prepare(capture_handle);
    printf("Capture initial successful!\n");
}

void Capture_Exit(void)
{
    free(CaptureBuf);
    snd_pcm_drain(capture_handle);
    snd_pcm_close(capture_handle);
}

void ReadOneFrame(void)
{
    int ret=0;

    ret = snd_pcm_readi(capture_handle, CaptureBuf, peroid_size);
    if (ret == -EPIPE)
    {
        printf("overrun occurred\n");
        snd_pcm_prepare(capture_handle);
    }
    else if (ret <0)
    {
        printf("error from read\n");
    }
    else if (ret != peroid_size)
    {
        printf("short read, read %d frames\n",ret);
    }
}

void StopCapture(void)
{
    snd_pcm_drain(capture_handle);
}

void ReCapture(void)
{
    snd_pcm_prepare(capture_handle);
}
