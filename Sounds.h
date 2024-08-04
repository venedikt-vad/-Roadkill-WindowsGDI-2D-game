#pragma once
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <tchar.h>

#define fourccRIFF 'FFIR'
#define fourccDATA 'atad'
#define fourccFMT ' tmf'
#define fourccWAVE 'EVAW'
#define fourccXWMA 'AMWX'
#define fourccDPDS 'sdpd'

int initSounds();

class sound {
public:
    WAVEFORMATEXTENSIBLE wfx = { 0 };
    XAUDIO2_BUFFER buffer = { 0 };

    IXAudio2SourceVoice* pSourceVoice;

    int Open(const wchar_t* strFileName);
    int Play();
    void Stop();

    float vol = 1.;

    void SetVol(float volume);

    void SetLooping(bool do_loop);
};