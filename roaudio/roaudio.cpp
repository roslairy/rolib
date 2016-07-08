#include "roaudio.h"

#include <vector>
#include <map>
#include <algorithm>

#include "vorbis\vorbisfile.h"
#include "al.h"
#include "alc.h"

#pragma comment(lib, "libogg_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libvorbisfile_static.lib")
#pragma comment(lib, "OpenAL32.lib")

namespace roaudio {

using namespace std;

vector<unsigned> audioObjs;
vector<unsigned> oggObjs;

void init() {
    ALCdevice *Device;
    ALCcontext *Context;
    Device = alcOpenDevice(NULL); // select the "preferred device"  
    if (Device) {
        Context = alcCreateContext(Device, NULL);
        alcMakeContextCurrent(Context);
    }
}

void release() {
    alDeleteSources(audioObjs.size(), &audioObjs[0]);
    alDeleteBuffers(oggObjs.size(), &oggObjs[0]);
    ALCdevice *Device;
    ALCcontext *Context;
    Context = alcGetCurrentContext();
    Device = alcGetContextsDevice(Context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Context);
    alcCloseDevice(Device);
}

unsigned createAudioObj() {
    ALuint audioObj;
    alGenSources(1, &audioObj);
    audioObjs.push_back(audioObj);
    return audioObj;
}

void releaseAudioObj(unsigned audioObj) {
    if (alIsSource(audioObj)) {
        alDeleteSources(1, &audioObj);
        auto it = find(audioObjs.begin(), audioObjs.end(), audioObj);
        audioObjs.erase(it);
    }
    else {
        printf("WARNING: invalid args for %s", __FUNCTION__);
    }
}

unsigned loadOgg(const char *path) {
    OggVorbis_File file;
    char *tmpBuffer = new char[4096];
    char *buffer = NULL;
    int section;
    int bt;
    int pointer = 0;

    ov_fopen(path, &file);
    vorbis_info *vi = ov_info(&file, 0);
    int freq = vi->rate;
    int format = vi->channels == 1 ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

    ogg_int64_t len = ov_pcm_total(&file, 0);
    buffer = new char[len * 4];
    int size = len * 4;
    while (bt = ov_read(&file, tmpBuffer, 4096, 0, 2, 1, &section)) {
        memmove(buffer + pointer, tmpBuffer, bt);
        pointer += bt;
    }
    ov_clear(&file);

    ALuint bo;
    alGenBuffers(1, &bo);
    alBufferData(bo, format, buffer, pointer, freq);
    oggObjs.push_back(bo);

    delete buffer;
    return bo;
}

void unloadOgg(unsigned oggObj) {
    if (alIsBuffer(oggObj)) {
        alDeleteBuffers(1, &oggObj);
        auto it = find(oggObjs.begin(), oggObjs.end(), oggObj);
        audioObjs.erase(it);
    }
    else {
        printf("WARNING: invalid args for %s", __FUNCTION__);
    }
}

void setOggAudioObj(unsigned audioObj, unsigned oggObj) {
    if (alIsSource(audioObj) && alIsBuffer(oggObj)) {
        alSourcei(audioObj, AL_BUFFER, oggObj);
    }
    else {
        printf("WARNING: invalid args for %s", __FUNCTION__);
    }
}

void setVolumeAudioObj(unsigned audioObj, float volume) {
    if (alIsSource(audioObj) && (volume >= 0.0f && volume <= 1.0f)) {
        alSourcef(audioObj, AL_GAIN, volume);
    }
    else {
        printf("WARNING: invalid args for %s", __FUNCTION__);
    }
}

void setLoopAudioObj(unsigned audioObj, bool isLoop) {
    if (alIsSource(audioObj)) {
        alSourcei(audioObj, AL_LOOPING, isLoop ? AL_TRUE : AL_FALSE);
    }
    else {
        printf("WARNING: invalid args for %s", __FUNCTION__);
    }
}

void play(unsigned audioObj) {
    if (alIsSource(audioObj)) {
        alSourceRewind(audioObj);
        alSourcePlay(audioObj);
    }
    else {
        printf("WARNING: invalid args for %s", __FUNCTION__);
    }
}

} // namespace roaudio
