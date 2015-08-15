#include <stdio.h>
#include <stdlib.h>
#include "videoInput.h"
#include "video_input.h"

static videoInput *VI = NULL;

void VI_SetVerbose(int verbose)
{
    videoInput::setVerbose(verbose);
}

void VI_SetMultiThreadedCom(int multiThreaded)
{
    videoInput::setComMultiThreaded(multiThreaded);
}

int VI_EnumDevices(int verbose)
{
    return videoInput::listDevices(!verbose);
}

char** VI_GetDeviceNames(int *numDevices)
{
    std::vector<std::string> names;
    char **res, *name, *buf;
    int cnt, len, i;

    names = videoInput::getDeviceList();
    cnt = names.size();

    if (cnt == 0)
    {
        if (numDevices)
            *numDevices = 0;

        return NULL;
    }

    res = (char**) malloc(sizeof(char*) * cnt);
    if (!res)
    {
        if (numDevices)
            *numDevices = -1;

        return NULL;
    }

    len = 0;
    for (i = 0; i < cnt; i++)
    {
        res[i] = (char*) names[i].c_str();
        len += strlen(res[i]) + 1;
    }

    buf = (char*) malloc(len);
    if (!buf)
    {
        free(res);
        return NULL;
    }

    for (i = 0; i < cnt; i++)
    {
        name = res[i];
        strcpy(buf, name);
        res[i] = buf;
        buf += strlen(name) + 1;
    }

    if (numDevices)
        *numDevices = cnt;

    return res;
}

char *VI_GetDeviceName(int id)
{
    char *name, *res;
    int len;

    name = videoInput::getDeviceName(id);
    if (!name)
        return NULL;

    len = strlen(name) + 1;
    res = (char*) malloc(len);
    if (res == NULL)
        return NULL;

    strcpy(res, name);
    return res;
}

int VI_GetDeviceId(const char *name)
{
    return videoInput::getDeviceIDFromName((char*) name);
}

int VI_Init()
{
    if (VI_IsInit())
        return 0;

    VI = new (std::nothrow) videoInput();
    if (!VI)
    {
        errno = ENOMEM;
        return 1;
    }

    return 0;
}

void VI_Deinit()
{
    if (VI_IsInit())
    {
        delete VI;
        VI = NULL;
    }
}

int VI_IsInit()
{
    if (VI)
        return 1;

    return 0;
}

void VI_SetBlocking(int blocking)
{
    VI->setUseCallback(blocking);
}

void VI_SetFramerate(int id, int framerate)
{
    VI->setIdealFramerate(id, framerate);
}

void VI_SetReconnectOnFreeze(int id, int reconnect, int numMissedFrames)
{
    VI->setAutoReconnectOnFreeze(id, reconnect != 0, numMissedFrames);
}

int VI_InitDevice(int id, const DEVICE_SETTINGS *settings)
{
    bool size = false, conn = false, ret;

    if (settings)
    {
        size = settings->flags & DS_RESOLUTION;
        conn = settings->flags & DS_CONNECTION;
    }

    if (size && conn)
        ret = VI->setupDevice(id, settings->width, settings->height, settings->connection);
    else if (size)
        ret = VI->setupDevice(id, settings->width, settings->height);
    else if (conn)
        ret = VI->setupDevice(id, settings->connection);
    else
        ret = VI->setupDevice(id);

    return !ret;
}

void VI_DeinitDevice(int id)
{
    VI->stopDevice(id);
}

int VI_ReinitDevice(int id)
{
    return !VI->restartDevice(id);
}

int VI_IsDeviceInit(int id)
{
    return VI->isDeviceSetup(id);
}

int VI_SetFormat(int id, int format)
{
    return !VI->setFormat(id, format);
}

void VI_SetMediaSubType(int subType)
{
    VI->setRequestedMediaSubType(subType);
}

int VI_HasNewFrame(int id)
{
    return VI->isFrameNew(id);
}

void VI_ShowSettingsWindow(int id)
{
    VI->showSettingsWindow(id);
}

int VI_GetFrameWidth(int id)
{
    return VI->getWidth(id);
}

int VI_GetFrameHeight(int id)
{
    return VI->getHeight(id);
}

int VI_GetBufferSize(int id)
{
    return VI->getSize(id);
}

int VI_GetPixels(int id, unsigned char *buffer, int flags)
{
    bool flipRedAndBlue = !(flags & VI_BGR);
    bool flipImage = flags & VI_VERTICAL_FLIP;

    return VI->getPixels(id, buffer, flipRedAndBlue, flipImage);
}
