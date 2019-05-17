// --------------------------------------------------------------------------
// Header file for the Jinghang Industrial Camera SDK.
// Copyright 2011-2016 (C) Shenzhen Jinghang Technology Co., Ltd.
// This is a proprietary software.
// --------------------------------------------------------------------------

#ifndef JHCAP_H
#define JHCAP_H
#include<Windows.h>

#if defined(JHCAP_LIBRARY)
# define DLL_EXPORT __declspec( dllexport )
#else
# define DLL_EXPORT __declspec( dllimport )
#endif

typedef int API_STATUS;
typedef int ( CALLBACK * CAPTURE_FRAME_PROC)(unsigned char *pImageBuffer, int width, int height, int format);
typedef int ( CALLBACK * CAPTURE_FRAME_PROC_EX)(unsigned char *pImageBuffer, int width, int height, int format, void *param);

enum {API_OK=0, API_ERROR};

#define CAMERA_ERROR_INDEX_OUT_OF_RANGE      1
#define CAMERA_ERROR_PARAMETER_UNREASONABLE  2
#define CAMERA_ERROR_NOT_INITIALIZED         3
#define CAMERA_ERROR_USB_DISCONNECTED        4
#define CAMERA_ERROR_NO_CAMERA_FOUND         5
#define CAMERA_ERROR_INITIALIZED_ALREADY     6
#define CAMERA_ERROR_RECONNECT_FAILURE       7
#define CAMERA_ERROR_NOT_SUPPORTED           8
#define CAMERA_ERROR_CONTROL_PIPE            9
#define CAMERA_ERROR_READ_PIPE               10
#define CAMERA_ERROR_WRITE_PIPE              11
#define CAMERA_ERROR_IMAGE_SIZE              12
#define CAMERA_ERROR_DATA_PIPE               13
#define CAMERA_ERROR_BAD_FRAME               14
#define CAMERA_ERROR_USER_ABORT              15
#define CAMERA_ERROR_DATA_XFER               16
#define CAMERA_ERROR_DATA_TIMEOUT            17

#define CAMERA_IMAGE_RAW8  0x1
#define CAMERA_IMAGE_GRAY8  0x2
#define CAMERA_IMAGE_RGB24  0x4
#define CAMERA_IMAGE_BGR   0x100
#define CAMERA_IMAGE_QUAD  0x200
#define CAMERA_IMAGE_SYNC    0x10000
#define CAMERA_IMAGE_TRIG    0x20000
#define CAMERA_IMAGE_STRETCH 0x1000000

#define CAMERA_IMAGE_BMP   (CAMERA_IMAGE_RGB24 | CAMERA_IMAGE_BGR)

#define CAMERA_IO_IN  0x02
#define CAMERA_IO_OUT  0x80

#define RESOLUTION_CROPPING 0x00
#define RESOLUTION_SKIPPING    0x01
#define RESOLUTION_BINNING 0x02

#define CAMERA_SNAP_CONTINUATION 0x01
#define CAMERA_SNAP_TRIGGER 0x02

#define CAMERA_RESOLUTION_CROPPING 0x0
#define CAMERA_RESOLUTION_SKIPPING 0x1
#define CAMERA_RESOLUTION_BINNING 0x2

#define CAMERA_INTERPOLATION_BILINEAR 0x0
#define CAMERA_INTERPOLATION_ALGO1  0x1
#define CAMERA_INTERPOLATION_ALGO2  0x2

#define CAMERA_FILE_BMP24  0x0
#define CAMERA_FILE_BMP8 0x1
#define CAMERA_FILE_JPEG 0x2

/*****************************************************************************
 *****************************************************************************
 **                      Core API Functions                                 **
 *****************************************************************************
 *****************************************************************************/
/*==============================================================
Name: CameraGetVersion
Description: Get the version of JHCap2
Parameters: *major, contains the release year information 
            *minor, contains the release month information
Return: always returns API_OK.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetVersion(int *major, int *minor);

/*==============================================================
Name: CameraGetFirmVersion
Description: Get the version of camera firmware
Parameters: index, the index of camera to query
            *ver, contains version information, ex 4505 for version 45.5
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetFirmVersion(int index,int *ver);

/*==============================================================
Name: CameraGetCount
Description: Get the number of cameras that connected
Parameters: *count,  return the number of cameras
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetCount(int *count);


/*==============================================================
Name: CameraGetName
Description: Get the camera name description
Parameters: index, the index of camera to query
            *name, the camera name description
            *model, the model information of the camera
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
 --------------------------------------------------------------*/


extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetName(int index, char *name, char *model);


/*==============================================================
Name: CameraGetID
Description: Get the camera numeric id
Parameters: index, the index of camera to query
            *modeID, the model ID of the camera(internal id, ex. 0 for JHSM36)
            *productID, the product ID of the camera (ex. 0x0030 for JHSM36)
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetID(int index,int *modeID, int *productID);

/*==============================================================
Name: CameraGetLastError
Description: Get the last error
Parameters: *last_error, last error value, refer to the constant for detail information
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetLastError(int *last_error);

/*==============================================================
Name: CameraInit
Description: Initialize the camera. Camera have to be initialized before other action can be taken.
Parameters: index, the index of camera to initialize
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraInit(int device_id);


/*==============================================================
Name: CameraFree
Description: Close camera, free resources related to opened camera. 
Parameters: device_id, the camera index
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraFree(int device_id);

/*==============================================================
Name: CameraReset
Description: Reset the data transfer pipe. If the data transfer is interrupted.
             by noise
Parameters: device_id, the camera index
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraReset(int device_id);

/*==============================================================
Name: CameraSetOption
Description: Set the image format, options used by CameraPlay
Parameters:device_id, the camera index
		   format, options are CAMERA_IMAGE_RAW8 / CAMERA_IMAGE_GRAY8 /
                                CAMERA_IMAGE_RGB24 / CAMERA_IMAGE_BGR /
                                CAMERA_IMAGE_BMP  /  CAMERA_IMAGE_STRETCH
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:the default image format is CAMERA_IMAGE_BMP
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetOption(int device_id,int format);


/*==============================================================
Name: CameraGetOption
Description: Set the image format
Parameters: device_id, the camera index
			format, options are CAMERA_IMAGE_RAW8 / CAMERA_IMAGE_GRAY8 /
                                CAMERA_IMAGE_RGB24 / CAMERA_IMAGE_BGR /
                                CAMERA_IMAGE_BMP  /  CAMERA_IMAGE_STRETCH
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:the default image format is CAMERA_IMAGE_BMP (CAMERA_IMAGE_RGB24 | CAMERA_IMAGE_BGR)
 --------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetOption(int device_id,int *format);

/*==============================================================
Name: CameraSetProperty
Description: Set the property stored in the camera memory
Parameters: device_id, the camera index
			name, property name
			value, property value
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:  These properties can be set by usbVideo or programming：
       "image_format", 
	   "resolution_index"
	   "enable_roi"
	   "roi_offset_x"
	   "roi_offset_y"
	   "roi_width"
	   "roi_height"
	   
 --------------------------------------------------------------*/
API_STATUS __stdcall CameraSetProperty(int device_id, char *name, int value);

/*==============================================================
Name: CameraGetProperty
Description: Get the property stored in the camera memory
Parameters: device_id, the camera index
			name, property name
            value, return the propery value
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: Refer to note for CameraSetProperty
 --------------------------------------------------------------*/
API_STATUS __stdcall CameraGetProperty(int device_id, char *name, int *value);

/*==============================================================
Name: CameraGetResolutionCount
Description: Get the number of resolution that the camera supports.
Parameters: device_id, the camera index
            *count, return the number of resolution
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetResolutionCount(int device_id, int *count);


/*==============================================================
Name: CameraGetResolutionMax
Description: Get the max resolution that the camera supports.
Parameters: device_id, the camera index
            *width, the width of resolution
            *height, the height of resolution
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetResolutionMax(int device_id, int *width, int *height);

/*==============================================================
Name: CameraGetResolutionMode
Description: Get resolution mode for small resolutions. 
Parameters: device_id, the camera index
            *mode, return the resolution mode.
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:The return value is RESOLUTION_CROPPING,RESOLUTION_SKIPPING or RESOLUTION_BINNING

 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetResolutionMode(int device_id, int *mode);


/*==============================================================
Name: CameraSetResolutionMode
Description: Set resolution mode for small resolutions.
Parameters: device_id, the camera index
            mode, resolution mode
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:the mode can equal to RESOLUTION_CROPPING,RESOLUTION_SKIPPING or RESOLUTION_BINNING
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetResolutionMode(int device_id, int mode);


/*==============================================================
Name: CameraGetResolution
Description: Get the resolution information according to the resolution index
Parameters: device_id, the camera index
            index, the index of camera
            *width, the width of resolution
            *height, the height of resolution
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetResolution(int device_id,int index,int *width, int *height);


/*==============================================================
Name: CameraSetResolution
Description: Set the resolution index, get the resolution information.
Parameters: device_id, the camera index
            index, the index of camera
            *width, return the width of the resolution
            *height, return the height of the resolution
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetResolution(int device_id,int index,int *width, int *height);


/*==============================================================
Name: CameraSetROI
Description: Set region of interested (ROI)
Parameters: device_id, the camera index
            offset_width, row offset
            offset_height, column offset
            width, ROI width
            height, ROI height
Return: returns API_OK on success , otherwise returns API_ERROR.
Note:
 --------------------------------------------------------------*/


extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetROI(int device_id,int offset_width, int offset_height, int width, int height);


/*==============================================================
Name: CameraSetGain
Description: Set camera gain
Parameters: device_id, the camera index
            gain, the value of gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: gain ranges from 1-255
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetGain(int device_id,int gain);


/*==============================================================
Name: CameraGetAdvancedGain
Description: Get camera advanced gain, only part of camera support this feature.
Parameters: device_id, the camera index
            *advanced_gain, return the value of gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Range from 0-3
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetAdvancedGain(int device_id,int *advanced_gain);


/*==============================================================
Name: CameraSetAdvancedGain
Description: Set camera advanced gain, , only part of camera support this feature.
Parameters: device_id, the camera index
            advanced_gain, the value of gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: gain ranges from 0-3
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetAdvancedGain(int device_id,int advanced_gain);


/*==============================================================
Name: CameraGetGain
Description: Get camera gain
Parameters: device_id, the camera index
            *gain, return the value of gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Range from 1-255
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetGain(int device_id,int *gain);

/*==============================================================
Name: CameraSetAGC
Description: Set auto gain control flag
Parameters: device_id, the camera index
            agc, set auto gain control, true for auto gain and false for manual gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetAGC(int device_id,bool agc);

/*==============================================================
Name: CameraGetAGC
Description: Get auto gain control flag
Parameters: device_id, the camera index
            *agc, return auto gain control status, true for auto gain and false for manual gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetAGC(int device_id,bool *agc);

/*==============================================================
Name: CameraSetWBGain
Description: Set white balance(WB) digital gain of the camera
Parameters: device_id, the camera index
            rg, set the value of red gain
            gg, set the value of green gain
            bg,  set the value of blue gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/


extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetWBGain(int device_id,double rg, double gg, double bg);

/*==============================================================
Name: CameraGetWBGain
Description: Get white balance digital gain of the camera
Parameters: device_id, the camera index
            *rg, return the current red gain
            *gg, return the current green gain
            *bg, return the current blue gain
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetWBGain(int device_id,double *rg, double *gg, double *bg);


/*==============================================================
Name: CameraSetExposure
Description: Set camera exposure
Parameters: device_id, the camera index
            exposure, the value of the exposure
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetExposure(int device_id,int exposure);

/*==============================================================
Name: CameraGetExposure
Description: Get camera exposure
Parameters: device_id, the camera index
            *exposure, return the value of the exposure
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetExposure(int device_id,int *exposure);

/*==============================================================
Name: CameraGetAvg
Description: Get image average illumination, as a reference for aec target
Parameters: device_id, the camera index
            *avg, return the average illumination of the image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetAvg(int device_id,int *avg);

/*==============================================================
Name: CameraGetExposure
Description: Get exposure unit in ms
Parameters: device_id, the camera index
            *exposure_unit, return the value of the exposure unit
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetExposureUnit(int device_id,double *exposure_unit);

/*==============================================================
Name: CameraGetExposureTime
Description: Get exposure time in ms
Parameters: device_id, the camera index
            *exposure_time, return the value of the exposure time
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetExposureTime(int device_id,double *exposure_time);

/*==============================================================
Name: CameraSetTimeout
Description: Set camera timeout
Parameters: device_id, the camera index
            timeout, the value of the timeout
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: Default value is 500ms, 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetTimeout(int device_id,int timeout);

/*==============================================================
Name: CameraGetTimeout
Description: Get camera timeout
Parameters: device_id, the camera index
            *timeout, return the value of the timeout
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetTimeout(int device_id,int *timeout);

/*==============================================================
Name: CameraSetAEC
Description: Set auto exposure control flag
Parameters: device_id, the camera index
            aec, set the auto exposure status(true for auto exposure and false for manual exposure)
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetAEC(int device_id,bool aec);

/*==============================================================
Name: CameraGetAEC
Description: Get auto exposure control flag
Parameters: device_id, the camera index
            *aec, return the auto exposure status(true for auto exposure and false for manual exposure)
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetAEC(int device_id,bool *aec);

/*==============================================================
Name: CameraSetGamma
Description: Set camera gamma
Parameters: device_id, the camera index
            gamma, the value of the gamma
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetGamma(int device_id,double gamma);


/*==============================================================
Name: CameraGetGamma
Description: Get camera gamma
Parameters: device_id, the camera index
            *gamma, return the gamma
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetGamma(int device_id,double *gamma);

/*==============================================================
Name: CameraSetContrast
Description: Set camera contrast
Parameters: device_id, the camera index
            contrast, the contrast value

Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetContrast(int device_id,double contrast);


/*==============================================================
Name: CameraGetContrast
Description: Get camera contrast
Parameters: device_id, the camera index
            *contrast, return the contrast

Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetContrast(int device_id,double *contrast);


/*==============================================================
Name: CameraSetSaturation
Description: Set camera saturation
Parameters: device_id, the camera index
            saturation, the saturation value
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetSaturation(int device_id,double saturation);

/*==============================================================
Name: CameraGetSaturation
Description: Get camera saturation
Parameters: device_id, the camera index
            *saturation, return the saturation

Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetSaturation(int device_id,double *saturation);


/*==============================================================
Name: CameraSetBlackLevel
Description: Set camera black level
Parameters: device_id, the camera index
            black, the black level
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: Usually, black level is used to compensate the gray value which not equal to 0 in totally dark environment.
      If in dark environment the image's average value is 10, then black value should set to 10.
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetBlackLevel(int device_id,int black);


/*==============================================================
Name: CameraGetBlackLevel
Description: Read camera black level
Parameters: device_id, the camera index
            *black, return the black level
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetBlackLevel(int device_id,int *black);


/*==============================================================
Name: CameraSetAETarget
Description: AE target is a important parameter for auto exposure.
             Auto exposure will stop when the image average reaches close to the target.
Parameters: device_id, the camera index
            target, the auto exposure target value

Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetAETarget(int device_id,int target);


/*==============================================================
Name: CameraGetAETarget
Description: Read AE target
Parameters: device_id, the camera index
            *target, return the ae target

Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetAETarget(int device_id,int *target);


/*==============================================================
Name: CameraOnePushWB
Description: Manually white balance by one push.
Parameters: device_id, the camera index

Return: returns API_OK on success, otherwise returns API_ERROR.
Note: Camera should face to a gray(black/white) target to get a correct white balance.
      One push white balance use gray world algorithm to calculate the r/g/b gain.
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraOnePushWB(int device_id);



/*****************************************************************************

Get the image information and image frame

 *****************************************************************************/

/*==============================================================
Name: CameraGetImageSize
Description: Get the size of the image that the camera capturing.
Parameters: device_id, the camera index
            *width, the width of the image
            *height, the height of image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: Call CameraGetImageSize beore CameraQueryImage to determine the realtime camera resolution.
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetImageSize(int device_id,int *width, int *height);

/*==============================================================
Name: CameraGetImageBufferSize
Description: Get the size of buffer to hold the current image.
Parameters: device_id, the camera index
            *size, return the width of the image
            option, the format of the image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: Call CameraGetImageBufferSize beore CameraQueryImage to determine the realtime image buffer size.
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetImageBufferSize(int device_id,int *size, int option);

/*==============================================================
Name: CameraGetISPImageBufferSize
Description: Get the size of buffer to hold the image produced by isp process.
Parameters: device_id, the camera index
            *size, return the size of the image
			width, the width of the image
			height,the height of image
            option, the format of the image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetISPImageBufferSize(int device_id,int *size, int width, int height, int option);

/*==============================================================
Name: CameraClearHostBuffer
Description: Clear the buffer on the host, to avoid get old images.
Parameters: device_id, the camera index
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraClearHostBuffer(int device_id);

/*==============================================================
Name: CameraQueryImage
Description: Get a image from camera
Parameters: device_id, the camera index
            *imgbuf, return the image data
            *length, return the image data size
            option, the format of the image, or trigger/sync option
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraQueryImage(int device_id,unsigned char *imgbuf, int *length, int option);


/*==============================================================
Name: CameraQueryImage2
Description: Deprecated. Same as CameraQueryImage
Parameters: device_id, the camera index
            *imgbuf, return the image data
            *length, return the image data size
            *meta, meaningless
            option, the format of the image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraQueryImage2(int device_id,unsigned char *imgbuf, int *length, int *meta,int option);

/*==============================================================
Name: CameraISP
Description: Soft isp to covert raw data to color/gray image.
Parameters: device_id, the camera index
            *pdata,input the raw data.........
            *imgbuf, return the image data
            width, the width of the image
			height,the height of image
            option, the format of the image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraISP(int device_id, unsigned char *pdata, unsigned char *imgbuf, int width, int height, int option);

/*==============================================================
Name: CameraCaptureImage
Description: Deprecated. Capture a high resolution image during low resolution preview.
Parameters: device_id, the camera index
            index, the index of the resolution to capture 
            *imgbuf, return the image data
            *length, return the image data size
            option, the format of the image
Return: returns API_OK on success and the image information from imgbuf
Note:Capture resolution must be greater than the preview resolution

--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraCaptureImage(int device_id,int index,unsigned char *imgbuf, int *length, int option);

/*==============================================================
Name: CameraSetMirrorX
Description: Set the horizontal mirror state of the output images.
Parameters: device_id, the camera index
            mx, horizontal mirror state
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetMirrorX(int device_id,bool mx);


/*==============================================================
Name: CameraSetMirrorY
Description: Set the vertical mirror state of the output images.
Parameters: device_id, the camera index
            my, vertical mirror state
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetMirrorY(int device_id,bool my);


/*==============================================================
Name: CameraSetRotate
Description: Set image rotated state of the output images.
Parameters: device_id, the camera index
            rotate, the angle of rotation 
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:the angle must equal to 360,270,180,90
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetRotate(int device_id,int rotate);


/*==============================================================
Name: CameraGetMirrorX
Description: Query the horizontal mirror state of the output images.
Parameters: device_id, the camera index
            mx, the current horizontal mirror state
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetMirrorX(int device_id,bool *mx);

/*==============================================================
Name: CameraGetMirrorY
Description: Query the vertical mirror state of the output images.
Parameters: device_id, the camera index
            my, the current vertical mirror state
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetMirrorY(int device_id,bool *my);


/*==============================================================
Name: CameraReadSerialNumber
Description: Read the Camera's GUID
Parameters: device_id, the camera index
            id[], the array to the GUID, the length must bigger than 12 byte.
            length, GUID's size,the size must equal to 12
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraReadSerialNumber(int device_id,char id[],int length);


/*==============================================================
Name: CameraWriteUserData
Description: Write user data to camera storage.
Parameters: device_id, the camera index
            data[], the array to the data, the length must less than 64 byte.
            length, the size of the user data ,the value must equal to 64
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraWriteUserData(int device_id,char data[],int length);

/*==============================================================
Name: CameraReadUserData
Description: Read user data from camera storage.
Parameters: device_id, the camera index
            data[], the array to the data, the length must less than 64 byte.
            length, the size of the user data ,the value must equal to 64
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraReadUserData(int device_id,char data[],int length);


/*==============================================================
Name: CameraSaveParameter
Description: Save parameters to camera storage.
Parameters: device_id, the camera index
            group_no, the index of parameters group
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: group_no can equal to 0, 1, 2 and 3, the default group is group 0.
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveParameter(int device_id,int group_no);


/*==============================================================
Name: CameraLoadParameter
Description: Load parameters from camera storage.
Parameters: device_id, the camera index
            group_no, the index of parameters group
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraLoadParameter(int device_id,int group_no);


/*==============================================================
Name: CameraEnableStrobe
Description: Enable camera strobe.
Parameters: device_id, the camera index
            en, true for enable strobe output, false for disable strobe output
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraEnableStrobe(int device_id, bool en);

/*==============================================================
Name: CameraSetTriggerPolarity
Description: Set trigger polarity.
Parameters: device_id, the camera index
            high, the trigger polarity, true for positive pole and false for negative pole
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetTriggerPolarity(int device_id, bool high);

/*==============================================================
Name: CameraSetStrobePolarity
Description: Set the strobe output polarity
Parameters: device_id, the camera index
            high, the strobe polarity, true for positive pole and false for negative pole
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetStrobePolarity(int device_id, bool high);

/*==============================================================
Name: CameraGetSnapMode
Description: Get camera snap mode.
Parameters: device_id, the camera index
            *snap_mode, return the image capture mode
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: During snap mode CAMERA_SNAP_TRIGGER, the camera is freezed waiting for an external or internal 
	  trigger signal.
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetSnapMode(int device_id, int *snap_mode);

/*==============================================================
Name: CameraSetSnapMode
Description: set snap mode
Parameters: device_id, the camera index
            snap_mode, the snap mode includes CAMERA_SNAP_TRIGGER or CAMERA_SNAP_CONTINUATION
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetSnapMode(int device_id, int snap_mode);

/*==============================================================
Name: CameraTriggerShot
Description: Shot a image with software trigger
Parameters: device_id, the camera index
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraTriggerShot(int device_id);


/*==============================================================
Name: CameraGetGPIO
Description: Get GPIO status
Parameters: device_id, the camera index
            *val, return the value of the GPIO
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetGPIO(int device_id,int *val);

/*==============================================================
Name: CameraSetGPIO
Description: set GPIO
Parameters: device_id, the camera index
            mask, the mask only valid for CAMERA_IO_IN or CAMERA_IO_OUT
            val, GPIO value
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetGPIO(int device_id,int mask, int val);

/*==============================================================
Name: CameraSetDelay
Description: Set frame delay so as to decrease the frame speed.
Parameters: device_id, the camera index
            delay, the time of delay 
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetDelay(int device_id,int delay);


/*==============================================================
Name: CameraGetDelay
Description: Get frame delay.
Parameters: device_id, the camera index
            *delay, return the time of delay 
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetDelay(int device_id,int *delay);


/*==============================================================
Name: CameraSetAntiFlicker
Description: Set the anti-flicker frequency to avoid the flicker.
Parameters: device_id, the camera index
            flicker, set the type of anti-flicker 0 for off, 1 for 50Hz, 2 for 60Hz
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetAntiFlicker(int device_id,int flicker);


/*==============================================================
Name: CameraGetAntiFlicker
Description: Get the anti-flicker frequency value.
Parameters: device_id, the camera index
            *flicker, return the type of anti-flicker 0 for off, 1 for 50Hz, 2 for 60Hz
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetAntiFlicker(int device_id,int *flicker);

/*==============================================================
Name: CameraSetEnhancement
Description: Set the color enhancement
Parameters: device_id, the camera index
            enhance, the enhancement state, true for enable and false for disable
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: If enhancement enabled, internal CCM matrix will take effect.
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetEnhancement(int device_id,bool enhance);

/*==============================================================
Name: CameraGetEnhancement
Description: Get the color enhancement
Parameters: device_id, the camera index
            *enhance, return the state of enhancement, true for enabled and false for disabled. 
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetEnhancement(int device_id,bool *enhance);

/*==============================================================
Name: CameraSetAWB
Description: Set auto white balance(AWB) state
Parameters: device_id, the camera index
            awb, set AWB state, true for AWB and false for turn off AWB.
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetAWB(int device_id,bool awb);

/*==============================================================
Name: CameraGetAWB
Description: Get auto white balance(AWB) state
Parameters: device_id, the camera index
            *awb, return the state of AWB,true for AWB and false for AWB off.
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetAWB(int device_id,bool *awb);

/*==============================================================
Name: CameraSetInterpolation
Description: Deprecated. Set interpolation algorithm
Parameters: device_id, the camera index
            interpolation, interpolation algorithm to interpolate color images
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetInterpolation(int device_id,int interpolation);

/*==============================================================
Name: CameraGetInterpolation
Description: Deprecated. Get interpolation algorithm
Parameters: device_id, the camera index
            *interpolation, return the interpolation mode                
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetInterpolation(int device_id,int *interpolation);

/*==============================================================
Name: CameraSetHighSpeed
Description: Set data transfer speed
Parameters: device_id, the camera index
            high, true for high speed 48Mhz clock and false for low speed with 24Mhz or 32Mhz clock
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: USB2.0 cameras have high speed option. Normal speed only runs at half speed compared to high speed mode.
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSetHighspeed(int device_id, bool high);

/*==============================================================
Name: CameraGetHighSpeed
Description: Get data transfer speed
Parameters: device_id, the camera index
            *high, return the current data transfer speed, true for high and false for low                
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraGetHighspeed(int device_id, bool *high);

/*==============================================================
Name: CameraReconnect
Description: reconnect the camera as if the camera re-plug to the computer physically
Parameters: device_id, the camera index
Return: returns API_OK on success, otherwise returns API_ERROR.
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraReconnect(int device_id);

/*****************************************************************************
 *****************************************************************************
 **                     Utility Functions                                   **
 *****************************************************************************
 *****************************************************************************/

/*==============================================================
Name: CameraSaveHBITMAP
Description: Create a HBITMAP handle by capturing data from camera
Parameters: device_id, the camera index
            *hBitmap, the handle of the BITMAP 
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: using DeleteObject to release the HBITMAP
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveHBITMAP(int device_id, HBITMAP* hBitmap);

/*==============================================================
Name: CameraSaveImage
Description: Save the image of different fomat,such as 24bit BMP,8bit BMP and JPEG
Parameters: device_id, the camera index
            fileName, the path and name that the image will save as
            color,     TRUE = RGB  FALSE = Grayscale
			option,    CAMERA_FILE_BMP24 =>bmp(RGB24)  
			           CAMERA_FILE_BMP8=>bmp(Gray)
					   CAMERA_FILE_JPEG=>jpeg
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveImage(int device_id,char *fileName,bool color,int option);

/*==============================================================
Name: CameraSaveBMP
Description: Save 32bit BMP file from the buffer.
Parameters: fileName, the path and name that the image will save as
            *buf, the color information(RGB or GBR or YUV)
            width, the width of the image
            height, the height of image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveBMP(char *fileName,BYTE * buf,UINT width,UINT height);

/*==============================================================
Name: CameraSaveBMP8
Description: Save 8bit BMP file from the buffer
Parameters: fileName, the path and name that the image will save as
            *buf, the color information(RAW or GRAY)
            width, the width of the image
            height, the height of image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:this function only convenient for 8bit format
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveBMP8(char *fileName,BYTE * buf,UINT width,UINT height);

/*==============================================================
Name: CameraSaveBMPB
Description: Save BMP file by capturing data from the camera
Parameters: device_id, the camera index
            fileName, the path and name that the image will save as
            width, the width of the image
            height, the height of the image
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:this function convenient for 24bits and 8bits format
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveBMPB(int device_id,char *fileName);

/*==============================================================
Name: CameraSaveBufferImage
Description: Save a named fomat file from the buffer
Parameters: fileName, the path and name that the image will save as
            dataBuf, RGB Buffer
            widthPix, the width of the image
            height, the height of the image
            color, TRUE = RGB  FALSE = Grayscale
            quality, 0-100
			option,    CAMERA_FILE_BMP =>bmp(RGB24)  
			           CAMERA_FILE_BMP8=>bmp(Gray)
					   CAMERA_FILE_JPEG=>jpeg
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: (1)quality parameter invalided if the option does not equal to CAMERA_FILE_JPEG
      (2)the dataBuf format is RGB not BGR if the option equal to CAMERA_FILE_JPEG
--------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveBufferImage(char *fileName, //path
                             BYTE *dataBuf,  //RGB Buffer
                             UINT widthPix,UINT height, BOOL color,int quality,int option);


/*==============================================================
Name: CameraSaveJpeg
Description: Save jpeg file from the buffer
Parameters: fileName, the path and name that the image will save as
            dataBuf, RGB Buffer
            widthPix, the width of the image
            height, the height of the image
            color, TRUE = RGB  FALSE = Grayscale
            quality, 0-100
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: the dataBuf format is RGB not BGR
--------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveJpeg(char *fileName, //path
                             BYTE *dataBuf,  //RGB Buffer
                             UINT widthPix,UINT height, BOOL color,int quality);

/*==============================================================
Name: CameraSaveJpegB
Description: Save jpeg file by capturing data from camera
Parameters: device_id, the camera index
            fileName, the path and name that the image will save as
            color, TRUE = RGB  FALSE = Grayscale
Return: returns API_OK on success, otherwise returns API_ERROR.
Note: the dataBuf format is RGB not BGR,besides the file is 24bits
--------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraSaveJpegB(int device_id,char *fileName,BOOL color);  // TRUE = RGB  FALSE = Grayscale

/*==============================================================
Name: CameraShowImage
Description: Show image in a control with capturing data from camera, callback function will be invoked if valid.
Parameters: device_id, the camera index
            hWnd, the handle of the window
            x, left scene
            y, top scene
            cx, the width of scene
            cy, the height of scene
            proc, the callback function
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraShowImage(int device_id,HDC hdc,int x,int y,int cx,int cy, CAPTURE_FRAME_PROC proc);


/*==============================================================
Name: CameraShowBufferImage
Description: Show image in a control with image data and other informations.
Parameters:  hWnd, the handle of the window
             buf, the image data
             width, the width of the image
             height, the height of the image
             color, true/RGB false/Gray
             showStretchMode, true/stretch false/no stretch
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:Recommendation showStretchMode is set to true.
 
--------------------------------------------------------------*/
extern "C" DLL_EXPORT API_STATUS __stdcall CameraShowBufferImage(HWND hWnd,unsigned char *buf,int width,int height,bool color,bool showStretchMode);//color :true/RGB false/Gray


/*==============================================================
Name: CameraPlayWithoutCallback
Description: Show the picture
Parameters: device_id, the camera index
            hWnd, the display control's handle
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:This function apply to situation without using the callback function. 
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraPlayWithoutCallback(int device_id,HWND hWnd);

/*==============================================================
Name: CameraPlay
Description: Show image stream in the control, invoke the callback function with every captured image.
Parameters: device_id, the camera index
            hWnd, the display control's handle
            proc, the callback function
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraPlay(int device_id,HWND hWnd,CAPTURE_FRAME_PROC proc);

/*==============================================================
Name: CameraPlayEx
Description: Show image stream in the control, invoke the callback function with every captured image.
Parameters: device_id, the camera index
            hWnd, the display control's handle
            proc, the callback function
		    param, parameter passed to callback function
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraPlayEx(int device_id,HWND hWnd,CAPTURE_FRAME_PROC_EX proc, void *param);


/*==============================================================
Name: CameraStop
Description: Stop show the image stream started by CameraPlay
Parameters: device_id, the camera index
Return: returns API_OK on success, otherwise returns API_ERROR.
Note:
--------------------------------------------------------------*/

extern "C" DLL_EXPORT API_STATUS __stdcall CameraStop(int device_id);

#endif // JHCAP_H

