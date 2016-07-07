#ifndef _ROGEN_ROAUDIO_H_
#define _ROGEN_ROAUDIO_H_

#pragma comment(lib, "libogg_static.lib")
#pragma comment(lib, "libvorbis_static.lib")
#pragma comment(lib, "libvorbisfile_static.lib")
#pragma comment(lib, "OpenAL32.lib")

namespace roaudio {

void init();
void release();

unsigned createAudioObj();
void releaseAudioObj(unsigned audioObj);

unsigned loadOgg(const char *path);
void unloadOgg(unsigned oggObj);

void setOggAudioObj(unsigned audioObj, unsigned oggObj);
void setVolumeAudioObj(unsigned audioObj, float volume);
void setLoopAudioObj(unsigned audioObj, bool isLoop);

void play(unsigned audioObj);

} // namespace roaudio

#endif
