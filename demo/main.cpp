#include <rograph.h>
#include <roaudio.h>
#include <roinput.h>
#pragma comment(lib, "rolib.lib")

#include <fstream>
#include <stdio.h>

int main() {
    roaudio::init();
    unsigned ao = roaudio::createAudioObj();
    unsigned ogg = roaudio::loadOgg(".\\tt.ogg");
    roaudio::setOggAudioObj(ao, ogg);
    roaudio::setLoopAudioObj(ao, true);
    roaudio::setVolumeAudioObj(ao, 1.0f);
    roaudio::play(ao);

    rograph::init("Test frame", 800, 600);
    unsigned po = rograph::createPaintObj();

    std::ifstream f(".\\test.jpg", std::ios::binary | std::ios::in);
    int length;
    f.seekg(0, std::ios::end);    // go to the end  
    length = f.tellg();           // report location (this is the length)  
    f.seekg(0, std::ios::beg);    // go back to the beginning  
    char *buffer = new char[length];    // allocate memory for a buffer of appropriate dimension  
    f.read(buffer, length);       // read the whole file into the buffer  
    f.close();       // close file handle  
    unsigned tex = rograph::loadTexture((unsigned char*)buffer, length);

    rograph::setTexture(po, tex);
    rograph::setSize(po, 300, 300);
    rograph::setAnchor(po, 0.5, 0.5);
    rograph::setPosition(po, 400, 300);
    unsigned po1 = rograph::createPaintObj();
    unsigned tex1 = rograph::loadTexture(".\\test1.jpg");
    rograph::setTexture(po1, tex1);
    rograph::setSize(po1, 800, 600);
    float degree = 0;
    int x, y;
    int cnt = 0;
    while (!rograph::isFrameClosed()) {
        if (roinput::getKeyPress(roinput::KEY_Q)) break;
        if (cnt == 120) roaudio::pause(ao);
        else if (cnt == 240) roaudio::play(ao);
        else if (cnt == 360) roaudio::stop(ao);
        else if (cnt == 480) roaudio::play(ao);
        if (cnt == 120) rograph::unloadTexture(tex);
        cnt++;
        roinput::getCursorPos(&x, &y);
        //printf("x:%d\ty:%d\n", x, y);
        rograph::setRotate(po, degree);
        degree += 0.3f;
        rograph::clear();
        rograph::paint(po1);
        rograph::paint(po);
        rograph::updateFrame();
    }
    rograph::release();
    roaudio::release();
}