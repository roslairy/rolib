#ifndef _ROGEN_ROAUDIO_H_
#define _ROGEN_ROAUDIO_H_

/**
* API for Audio
* Use these functions to load ogg file and play it.
*/
namespace roaudio {

/**
* Initialize Audio Component
*/
void init();

/**
* Uninitialize Audio Component
*/
void release();

/**
* Create a new audio-object
* @return handle of audio-object
*/
unsigned createAudioObj();

/**
* Release audio-object.
* @param audioObj handle of audio-object
*/
void releaseAudioObj(unsigned audioObj);

/**
* Load ogg-object from file.
* @param path path of ogg file
* @return handle of ogg-object
*/
unsigned loadOgg(const char *path);

/**
* Unload ogg-object.
* @param oggObj handle of ogg-object
*/
void unloadOgg(unsigned oggObj);

/**
* Bind audio-object with ogg-object.
* @param audioObj handle of audio-object
* @param oggObj handle of ogg-object
*/
void setOggAudioObj(unsigned audioObj, unsigned oggObj);

/**
* Set volume of audio-object.
* @param audioObj handle of audio-object
* @param volume volume in range[0.0, 1.0]
*/
void setVolumeAudioObj(unsigned audioObj, float volume);

/**
* Set loop of audio-object.
* @param audioObj handle of audio-object
* @param isLoop activate/deactivate loop
*/
void setLoopAudioObj(unsigned audioObj, bool isLoop);

/**
* Play audio-object.
* @param audioObj handle of audio-object
*/
void play(unsigned audioObj);

/**
* Pause audio-object.
* @param audioObj handle of audio-object
*/
void pause(unsigned audioObj);

/**
* Stop audio-object.
* @param audioObj handle of audio-object
*/
void stop(unsigned audioObj);

/**
* Stop all audio-objects.
*/
void stopAll();

} // namespace roaudio

#endif
