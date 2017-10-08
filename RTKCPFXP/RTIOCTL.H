//
//========================================================================
//
#ifndef		__RTIOCTL__H__
#define		__RTIOCTL__H__
//
//========================================================================
//
#define	FILE_DEVICE_RTKCPF			0x00006942

#define IOCTL_RTKCPF_COPYFILE		( ULONG )CTL_CODE( FILE_DEVICE_RTKCPF, 0x01 , METHOD_BUFFERED, FILE_ANY_ACCESS )

#define IOCTL_RTKCPF_SETNICPNP		( ULONG )CTL_CODE( FILE_DEVICE_RTKCPF, 0x02 , METHOD_BUFFERED, FILE_ANY_ACCESS )
//
//========================================================================
//
#endif //	__RTIOCTL__H__
//
//========================================================================
//
