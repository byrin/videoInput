//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.

/**
 * @file video_input.h
 * @author Mihail Ivanchev
 * @brief A C API for the videoInput library by Theodore Watson et. al.
 *
 * As videoInput itself, it's not bound to a particular license and thus can be
 * used without any restriction.
 */

#ifndef __VIDEO_INPUT_H__
#define __VIDEO_INPUT_H__

/*
 * These are the standard videoInput constants defined in videoInput.h and
 * redefined here to eliminate the dependence on the C++ header.
 */

#ifndef _VIDEOINPUT

#define VI_VERSION      0.200
#define VI_MAX_CAMERAS  20
#define VI_NUM_TYPES    19 //DON'T TOUCH
#define VI_NUM_FORMATS  18 //DON'T TOUCH

#define VI_COMPOSITE    0
#define VI_S_VIDEO      1
#define VI_TUNER        2
#define VI_USB          3
#define VI_1394         4

#define VI_NTSC_M   0
#define VI_PAL_B    1
#define VI_PAL_D    2
#define VI_PAL_G    3
#define VI_PAL_H    4
#define VI_PAL_I    5
#define VI_PAL_M    6
#define VI_PAL_N    7
#define VI_PAL_NC   8
#define VI_SECAM_B  9
#define VI_SECAM_D  10
#define VI_SECAM_G  11
#define VI_SECAM_H  12
#define VI_SECAM_K  13
#define VI_SECAM_K1 14
#define VI_SECAM_L  15
#define VI_NTSC_M_J 16
#define VI_NTSC_433 17

#define VI_MEDIASUBTYPE_RGB24   0
#define VI_MEDIASUBTYPE_RGB32   1
#define VI_MEDIASUBTYPE_RGB555  2
#define VI_MEDIASUBTYPE_RGB565  3
#define VI_MEDIASUBTYPE_YUY2    4
#define VI_MEDIASUBTYPE_YVYU    5
#define VI_MEDIASUBTYPE_YUYV    6
#define VI_MEDIASUBTYPE_IYUV    7
#define VI_MEDIASUBTYPE_UYVY    8
#define VI_MEDIASUBTYPE_YV12    9
#define VI_MEDIASUBTYPE_YVU9    0
#define VI_MEDIASUBTYPE_Y411    11
#define VI_MEDIASUBTYPE_Y41P    12
#define VI_MEDIASUBTYPE_Y211    13
#define VI_MEDIASUBTYPE_AYUV    14
#define VI_MEDIASUBTYPE_Y800    15
#define VI_MEDIASUBTYPE_Y8      16
#define VI_MEDIASUBTYPE_GREY    17
#define VI_MEDIASUBTYPE_MJPG    18

#endif // _VIDEOINPUT

#define VI_RGB              0x01
#define VI_VERTICAL_FLIP    0x02

#define DS_RESOLUTION 0x01
#define DS_CONNECTION 0x20

#ifdef BUILD_DLL
#define DLL __declspec(dllexport)
#else
#define DLL __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
    int flags;
    int width;
    int height;
    int connection;
} DEVICE_SETTINGS;

/**
 * Controls whether videoInput outputs miscellaneous information to stdout.
 *
 * This function does not require the library to be initialized by VI_Init(). A
 * value of 0 indicates that no information should be printed and any other
 * value enables the verbose output.
 */
DLL void VI_SetVerbose(int verbose);

/**
 * Controls whether the videoInput's COM usage with take place in a
 * multi-threaded apartment.
 *
 * This function should be called before calling any other function using COM.
 * If any other parts of your application use COM, make sure that the settings
 * match.
 */
DLL void VI_SetMultiThreadedCom(int multiThreaded);

/**
 * Enumerates the available video devices and returns their number.
 *
 * This function does not require the library to be initialized by VI_Init(). If
 * verbose is unequal 0, the function will output information about the found
 * devices to stdout.
 */
DLL int VI_EnumDevices(int verbose);

/**
 * Returns the names of the discovered video devices and optionally their count.
 *
 * This function does not require the library to be initialized by VI_Init(). If
 * numDevices is not NULL, the value it points to will be set to the number of
 * available devices (including 0) or -1 to indicate an error. The function
 * returns NULL if it fails to allocate memory or no devices are available. It's
 * not necessary to call VI_EnumDevices() prior to this function. The caller is
 * responsible for freeing the memory; because of the allocation scheme, only
 * the 1st element and the whole list must be freed.
 *
 * Example:
 * @code{.c}
 * int numDevices, i;
 * char **names = VI_GetDeviceNames(&numDevices);
 * if (names != NULL)
 * {
 *     for (i = 0; i < numDevices; i++)
 *     {
 *         printf("Device #%d: %s\n", i, names[i]);
 *     }
 *     free(names[0]);
 *     free(names);
 * }
 * else if (numDevices == -1)
 * {
 *     fprintf(stderr, "ERROR: Failed to allocate memory.\n");
 * }
 * @endcode
 */
DLL char** VI_GetDeviceNames(int *numDevices);

/**
 * Returns the name of the video device with the specified identifier.
 *
 * This function does not require the library to be initialized by VI_Init().
 * VI_EnumDevices() or one of the other functions with enumeration semantics
 * must be called prior to this function. NULL is returned if the function
 * cannot allocate memory or the identifier is greater or equal to
 * VI_MAX_CAMERAS. The caller is responsible for freeing the memory.
 */
DLL char *VI_GetDeviceName(int id);

/**
 * Returns the identifier of the video device with the specified name or -1 if
 * no device with this name is available.
 *
 * This function does not require the library to be initialized by VI_Init().
 * It's not necessary to call VI_EnumDevices() prior to this function.
 */
DLL int VI_GetDeviceId(const char *name);

/**
 * Initializes the videoInput library.
 *
 * This function must be called before most of the library's functions can be
 * used. It returns 0 upon successful completion and a positive value in case of
 * an error. The function returns successfully if the library is already
 * initialized.
 */
DLL int VI_Init();

/**
 * Deinitializes the videoInput library.
 *
 * The function has no effect if the library is not initialized.
 */
DLL void VI_Deinit();

DLL void VI_SetFramerate(int id, int framerate);
DLL void VI_SetReconnectOnFreeze(int id, int reconnect, int numMissedFrames);
DLL int VI_InitDevice(int id, const DEVICE_SETTINGS *settings);
DLL void VI_DeinitDevice(int id);
DLL int VI_ReinitDevice(int id);
DLL int VI_IsDeviceInitialized(int id);
DLL int VI_SetFormat(int id, int format);
DLL void VI_SetMediaSubType(int subType);
DLL int VI_HasNewFrame(int id);
DLL void VI_ShowSettingsWindow(int id);
DLL int VI_GetFrameWidth(int id);
DLL int VI_GetFrameHeight(int id);
DLL int VI_GetBufferSize(int id);
DLL int VI_GetPixels(int id, unsigned char *buffer, int flags);

#ifdef __cplusplus
}
#endif

#endif // __VIDEO_INPUT_H__
