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
 * @brief A C API for the videoInput library by Theodore Watson et al.
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

#define VI_BGR              0x01
#define VI_VERTICAL_FLIP    0x02

#define DS_RESOLUTION 0x01
#define DS_CONNECTION 0x20

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
void VI_SetVerbose(int verbose);

/**
 * Controls whether the videoInput's COM usage with take place in a
 * multi-threaded apartment.
 *
 * This function should be called before calling any other function using COM.
 * If any other parts of your application use COM, make sure that the settings
 * match.
 */
void VI_SetMultiThreadedCom(int multiThreaded);

/**
 * Enumerates the available video devices and returns their number.
 *
 * This function does not require the library to be initialized by VI_Init(). If
 * verbose is unequal 0, the function will output information about the found
 * devices to stdout.
 */
int VI_EnumDevices(int verbose);

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
char** VI_GetDeviceNames(int *numDevices);

/**
 * Returns the name of the video device with the specified identifier.
 *
 * This function does not require the library to be initialized by VI_Init().
 * VI_EnumDevices() or one of the other functions with enumeration semantics
 * must be called prior to this function. NULL is returned if the function
 * cannot allocate memory or the identifier is greater or equal to
 * VI_MAX_CAMERAS. The caller is responsible for freeing the memory.
 */
char *VI_GetDeviceName(int id);

/**
 * Returns the identifier of the video device with the specified name or -1 if
 * no device with this name is available.
 *
 * This function does not require the library to be initialized by VI_Init().
 * It's not necessary to call VI_EnumDevices() prior to this function.
 */
int VI_GetDeviceId(const char *name);

/**
 * Initializes the videoInput library.
 *
 * This function must be called before most of the library's functions can be
 * used. It returns 0 upon successful completion and a positive value in case of
 * an error. The function returns successfully if the library is already
 * initialized.
 */
int VI_Init();

/**
 * Deinitializes the videoInput library.
 *
 * The function has no effect if the library is not initialized.
 */
void VI_Deinit();

/**
 * TODO
 */
int VI_IsInit();

/**
 * TODO
 */
void VI_SetBlocking(int blocking);

/**
 * Sets the desired framerate for the specified video device in frames per
 * second.
 *
 * The actual framerate will depend on the device's capabilities. This
 * function should be called prior to the device initialization, otherwise it
 * has no effect.
 */
void VI_SetFramerate(int id, int framerate);

/**
 * Controls whether an automatic reinitialization will take place once the
 * specified video device is believed to have "frozen".
 *
 * The device is considered inactive if at least numMissedFrames checks for
 * VI_HasNewFrame() have been unsuccessful. Once the condition is met,
 * VI_HasNewFrame() attempts to reinitialize the device and continue normal
 * operation.
 */
void VI_SetReconnectOnFreeze(int id, int reconnect, int numMissedFrames);

/**
 * Initializes the video device with the specified identifier and prepares it
 * for capture.
 *
 * If settings is NULL, the device will be initialized with a default
 * configuration. This function returns 0 if successful, otherwise a non-zero
 * value if an error occurs or the device is already initialized. If you want
 * to reinitialize a device without losing the current configuration, use
 * VI_ReinitDevice().
 */
int VI_InitDevice(int id, const DEVICE_SETTINGS *settings);

/**
 * Deinitializes the video device with the specified identifier.
 */
void VI_DeinitDevice(int id);

/**
 * Reinitializes the video device with the specified identifier and prepares it
 * for capture.
 *
 * The semantics are generally the same as for VI_InitDevice() with the
 * difference that this function does not reset the device configuration to the
 * default one.
 */
int VI_ReinitDevice(int id);

/**
 * Returns 0 if the video device with the specified identifier is currently
 * initialized for capture, otherwise 1.
 */
int VI_IsDeviceInit(int id);

/**
 * TODO
 */
int VI_SetFormat(int id, int format);

/**
 * TODO
 */
void VI_SetMediaSubType(int subType);

/**
 * TODO
 */
int VI_HasNewFrame(int id);

/**
 * Opens the device configuration dialog usually offered by many video device
 * drivers.
 *
 * This is a very device specific feature so it may have no effect.
 */
void VI_ShowSettingsWindow(int id);

/**
 * Returns the frame width for the video device with the specified identifier or
 * 0 if the device has not been initialized.
 */
int VI_GetFrameWidth(int id);

/**
 * Returns the frame height for the video device with the specified identifier or
 * 0 if the device has not been initialized.
 */
int VI_GetFrameHeight(int id);

/**
 * Returns the size (in bytes) of a captured frame's image data for the video
 * device with the specified identifier or 0 if the device has not been
 * initialized.
 */
int VI_GetBufferSize(int id);

/**
 * TODO
 */
int VI_GetPixels(int id, unsigned char *buffer, int flags);

#ifdef __cplusplus
}
#endif

#endif // __VIDEO_INPUT_H__
