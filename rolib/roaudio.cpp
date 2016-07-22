#include "roaudio.h"

#include <vector>
#include <map>
#include <algorithm>
#include <assert.h>

#include "vorbis\vorbisfile.h"
#include <al\al.h>
#include <al\alc.h>

#pragma comment(lib, "libogg_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libvorbisfile_static.lib")
#pragma comment(lib, "OpenAL32.lib")

namespace roaudio {

using namespace std;

typedef struct _AudioObj {
    ALuint source = -1;
} AudioObj;

typedef struct _OggObj {
    ALuint buffer = -1;
} OggObj;

map<ALuint, AudioObj*> audioObjs;
map<ALuint, OggObj*> oggObjs;

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
    stopAll();
    for each (auto pair in audioObjs){
        alDeleteSources(1, &pair.first);
    }
    for each (auto pair in oggObjs) {
        alDeleteBuffers(1, &pair.first);
    }
    ALCdevice *Device;
    ALCcontext *Context;
    Context = alcGetCurrentContext();
    Device = alcGetContextsDevice(Context);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(Context);
    alcCloseDevice(Device);
}

unsigned createAudioObj() {
    ALuint source;
    alGenSources(1, &source);
    AudioObj* obj = new AudioObj;
    obj->source = source;
    audioObjs[source] = obj;
    return source;
}

void releaseAudioObj(unsigned audioObj) {
    assert(alIsSource(audioObj));
    stop(audioObj);
    alDeleteSources(1, &audioObj);
    audioObjs.erase(audioObj);
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

    OggObj* oo = new OggObj;
    oo->buffer = bo;
    oggObjs[bo] = oo;

    delete buffer;
    return bo;
}

void unloadOgg(unsigned oggObj) {
    assert(alIsBuffer(oggObj));
    alDeleteBuffers(1, &oggObj);
    audioObjs.erase(oggObj);
}

void setOggAudioObj(unsigned audioObj, unsigned oggObj) {
    assert(alIsSource(audioObj) && alIsBuffer(oggObj));
    alSourcei(audioObj, AL_BUFFER, oggObj);
}

void setVolumeAudioObj(unsigned audioObj, float volume) {
    assert(alIsSource(audioObj) && (volume >= 0.0f && volume <= 1.0f));
    alSourcef(audioObj, AL_GAIN, volume);
}

void setLoopAudioObj(unsigned audioObj, bool isLoop) {
    assert(alIsSource(audioObj));
    alSourcei(audioObj, AL_LOOPING, isLoop ? AL_TRUE : AL_FALSE);
}

void play(unsigned audioObj) {
    assert(alIsSource(audioObj));
    alSourcePlay(audioObj);
}

void pause(unsigned audioObj) {
    assert(alIsSource(audioObj));
    alSourcePause(audioObj);
}

void stop(unsigned audioObj) {
    assert(alIsSource(audioObj));
    alSourceStop(audioObj);
}

void stopAll() {
    for each (auto pair in audioObjs) {
        stop(pair.first);
    }
}

} // namespace roaudio
