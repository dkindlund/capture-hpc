/*
 *	PROJECT: Capture
 *	FILE: MiniFilter.h
 *	AUTHORS: Ramon Steenson (rsteenson@gmail.com) & Christian Seifert (christian.seifert@gmail.com)
 *
 *	Developed by Victoria University of Wellington and the New Zealand Honeynet Alliance
 *
 *	This file is part of Capture.
 *
 *	Capture is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Capture is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Capture; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

/* Various methods and structures to interact with minifilter drivers */
typedef struct _INSTANCE_FULL_INFORMATION {
  ULONG  NextEntryOffset;
  USHORT  InstanceNameLength;
  USHORT  InstanceNameBufferOffset;
  USHORT  AltitudeLength;
  USHORT  AltitudeBufferOffset;
  USHORT  VolumeNameLength;
  USHORT  VolumeNameBufferOffset;
  USHORT  FilterNameLength;
  USHORT  FilterNameBufferOffset;
} INSTANCE_FULL_INFORMATION, *PINSTANCE_FULL_INFORMATION;

typedef struct _INSTANCE_BASIC_INFORMATION {
    ULONG  NextEntryOffset;
    USHORT  InstanceNameLength;
    USHORT  InstanceNameBufferOffset;
} INSTANCE_BASIC_INFORMATION, *PINSTANCE_BASIC_INFORMATION;

typedef struct _FILTER_VOLUME_BASIC_INFORMATION {
  USHORT  FilterVolumeNameLength;
  WCHAR  FilterVolumeName[1];
} FILTER_VOLUME_BASIC_INFORMATION, *PFILTER_VOLUME_BASIC_INFORMATION;

typedef enum _FILTER_VOLUME_INFORMATION_CLASS {

    FilterVolumeBasicInformation,
    FilterVolumeStandardInformation     //Longhorn and later

} FILTER_VOLUME_INFORMATION_CLASS, *PFILTER_VOLUME_INFORMATION_CLASS;

#define IRP_MJ_CREATE                   0x00
#define IRP_MJ_CREATE_NAMED_PIPE        0x01
#define IRP_MJ_CLOSE                    0x02
#define IRP_MJ_READ                     0x03
#define IRP_MJ_WRITE                    0x04
#define IRP_MJ_QUERY_INFORMATION        0x05
#define IRP_MJ_SET_INFORMATION          0x06
#define IRP_MJ_QUERY_EA                 0x07
#define IRP_MJ_SET_EA                   0x08
#define IRP_MJ_FLUSH_BUFFERS            0x09
#define IRP_MJ_QUERY_VOLUME_INFORMATION 0x0a
#define IRP_MJ_SET_VOLUME_INFORMATION   0x0b
#define IRP_MJ_DIRECTORY_CONTROL        0x0c
#define IRP_MJ_FILE_SYSTEM_CONTROL      0x0d
#define IRP_MJ_DEVICE_CONTROL           0x0e
#define IRP_MJ_INTERNAL_DEVICE_CONTROL  0x0f
#define IRP_MJ_SHUTDOWN                 0x10
#define IRP_MJ_LOCK_CONTROL             0x11
#define IRP_MJ_CLEANUP                  0x12
#define IRP_MJ_CREATE_MAILSLOT          0x13
#define IRP_MJ_QUERY_SECURITY           0x14
#define IRP_MJ_SET_SECURITY             0x15
#define IRP_MJ_POWER                    0x16
#define IRP_MJ_SYSTEM_CONTROL           0x17
#define IRP_MJ_DEVICE_CHANGE            0x18
#define IRP_MJ_QUERY_QUOTA              0x19
#define IRP_MJ_SET_QUOTA                0x1a
#define IRP_MJ_PNP                      0x1b
#define IRP_MJ_PNP_POWER                IRP_MJ_PNP      // Obsolete....
#define IRP_MJ_MAXIMUM_FUNCTION         0x1b

/* THIS IS A CUSTOM IRP. We use this in the user space process to make this nice */
#define IRP_MJ_DELETE         0x99

extern "C" {
HRESULT
WINAPI
  FilterConnectCommunicationPort(
    IN LPCWSTR  lpPortName,
    IN DWORD  dwOptions,
    IN LPVOID  lpContext OPTIONAL,
    IN DWORD  dwSizeOfContext,
    IN LPSECURITY_ATTRIBUTES  lpSecurityAttributes OPTIONAL,
    OUT HANDLE  *hPort
    ); 

HRESULT
WINAPI
  FilterSendMessage(
    IN HANDLE  hPort,
    IN LPVOID  lpInBuffer OPTIONAL,
    IN DWORD  dwInBufferSize,
    IN OUT LPVOID  lpOutBuffer OPTIONAL,
    IN DWORD  dwOutBufferSize,
    OUT LPDWORD  lpBytesReturned
    ); 

HRESULT
WINAPI
  FilterLoad(
    IN LPCWSTR  lpFilterName
    ); 
HRESULT
WINAPI
  FilterUnload(
    IN LPCWSTR  lpFilterName
    ); 

HRESULT
WINAPI
  FilterGetDosName(
    IN LPCWSTR  lpVolumeName,
    IN OUT LPWSTR  lpDosName,
    IN DWORD  dwDosNameBufferSize
    ); 

HRESULT
WINAPI
  FilterVolumeFindFirst(
    IN FILTER_VOLUME_INFORMATION_CLASS  dwInformationClass,
    IN LPVOID  lpBuffer,
    IN DWORD  dwBufferSize,
    OUT LPDWORD  lpBytesReturned,
    OUT PHANDLE  lpFilterFind
    ); 

HRESULT
WINAPI
  FilterVolumeFindNext(
    IN HANDLE  hFilterFind,
    IN FILTER_VOLUME_INFORMATION_CLASS  dwInformationClass,
    IN LPVOID  lpBuffer,
    IN DWORD  dwBufferSize,
    OUT LPDWORD  lpBytesReturned
    ); 
HRESULT
WINAPI
  FilterVolumeFindClose(
    IN HANDLE  hVolumeFind
    ); 
}