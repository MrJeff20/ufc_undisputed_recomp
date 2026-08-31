#include "ppc_context.h"

namespace
{
constexpr uint64_t kNotImplemented = 0xC0000002;
}

PPC_FUNC(__imp____C_specific_handler)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp___snprintf)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp___vsnprintf)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__DbgBreakPoint)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__DbgPrint)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ExCreateThread)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ExGetXConfigSetting)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ExRegisterTitleTerminateNotification)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ExTerminateThread)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__FscSetCacheElementCount)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__HalReturnToFirmware)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__IoCheckShareAccess)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__IoCompleteRequest)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__IoCreateDevice)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__IoDeleteDevice)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__IoDismountVolume)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__IoDismountVolumeByFileHandle)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__IoInvalidDeviceRequest)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__IoRemoveShareAccess)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__IoSetShareAccess)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeAcquireSpinLockAtRaisedIrql)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeBugCheck)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeBugCheckEx)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeDelayExecutionThread)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeEnterCriticalRegion)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeGetCurrentProcessType)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeInitializeDpc)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeInsertQueueDpc)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeLeaveCriticalRegion)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeLockL2)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeQueryBasePriorityThread)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeReleaseSpinLockFromRaisedIrql)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeResetEvent)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeSetAffinityThread)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeSetBasePriorityThread)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeSetCurrentProcessType)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeSetEvent)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeUnlockL2)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeWaitForMultipleObjects)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KeWaitForSingleObject)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KfAcquireSpinLock)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KfReleaseSpinLock)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__KiApcNormalRoutineNop)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__MmAllocatePhysicalMemoryEx)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__MmFreePhysicalMemory)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__MmGetPhysicalAddress)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__MmMapIoSpace)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__MmQueryAddressProtect)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__MmQueryAllocationSize)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll___WSAFDIsSet)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_accept)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_bind)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_closesocket)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_connect)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_inet_addr)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_ioctlsocket)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_listen)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_recv)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_recvfrom)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_select)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_send)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_sendto)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_setsockopt)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_shutdown)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_socket)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_WSACleanup)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_WSAGetLastError)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_WSAStartup)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetCleanup)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetConnect)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetCreateKey)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetGetConnectStatus)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetGetEthernetLinkStatus)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetGetTitleXnAddr)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetInAddrToString)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetInAddrToXnAddr)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetQosListen)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetQosLookup)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetQosRelease)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetQosServiceLookup)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetServerToInAddr)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetStartup)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetUnregisterInAddr)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NetDll_XNetXnAddrToInAddr)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtAllocateVirtualMemory)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtCancelIoFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtCancelTimer)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtClearEvent)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtClose)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtCreateEvent)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtCreateFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtCreateMutant)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtCreateSemaphore)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtCreateTimer)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtDeviceIoControlFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtDuplicateObject)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtFlushBuffersFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtFreeVirtualMemory)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtOpenFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtQueryDirectoryFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtQueryFullAttributesFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtQueryInformationFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtQueryVirtualMemory)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtQueryVolumeInformationFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtReadFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtReadFileScatter)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtReleaseMutant)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtReleaseSemaphore)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtResumeThread)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtSetEvent)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtSetInformationFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtSetTimerEx)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtSuspendThread)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtWaitForMultipleObjectsEx)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtWaitForSingleObjectEx)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtWriteFile)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__NtWriteFileGather)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ObCreateSymbolicLink)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ObDeleteSymbolicLink)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ObDereferenceObject)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ObIsTitleObject)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ObLookupThreadByThreadId)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ObOpenObjectByPointer)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ObReferenceObject)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__ObReferenceObjectByHandle)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlCompareMemoryUlong)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlCompareStringN)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlFillMemoryUlong)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlFreeAnsiString)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlImageXexHeaderField)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlInitAnsiString)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlInitUnicodeString)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlMultiByteToUnicodeN)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlNtStatusToDosError)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlRaiseException)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlTimeFieldsToTime)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlTimeToTimeFields)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlUnicodeStringToAnsiString)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlUnicodeToMultiByteN)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlUnwind)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__RtlUpcaseUnicodeChar)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__sprintf)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__StfsControlDevice)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__StfsCreateDevice)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdCallGraphicsNotificationRoutines)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdEnableDisableClockGating)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdEnableRingBufferRPtrWriteBack)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdGetCurrentDisplayGamma)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdGetCurrentDisplayInformation)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdGetSystemCommandBuffer)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdInitializeEngines)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdInitializeRingBuffer)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdInitializeScalerCommandBuffer)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdIsHSIOTrainingSucceeded)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdPersistDisplay)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdQueryVideoFlags)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdQueryVideoMode)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdRetrainEDRAM)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdRetrainEDRAMWorker)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdSetDisplayMode)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdSetDisplayModeOverride)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdSetGraphicsInterruptCallback)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdSetSystemCommandBufferGpuIdentifierAddress)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdShutdownEngines)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__VdSwap)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamAlloc)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamBackgroundDownloadItemGetHistoryStatus)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamBackgroundDownloadItemGetStatus)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamContentClose)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamContentCreateEnumerator)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamContentCreateEx)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamContentDelete)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamContentFlush)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamContentGetCreator)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamContentGetDeviceData)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamContentGetDeviceState)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamContentGetLicenseMask)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamCreateEnumeratorHandle)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamEnumerate)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamFree)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamGetExecutionId)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamGetPrivateEnumStructureFromHandle)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamInputGetCapabilities)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamInputGetKeystrokeEx)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamInputGetState)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamInputSetState)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamLoaderTerminateTitle)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamNotifyCreateListener)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamSessionCreateHandle)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamSessionRefObjByHandle)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamShowDeviceSelectorUI)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamShowFriendsUI)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamShowGamerCardUIForXUID)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamShowMarketplaceDownloadItemsUI)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamShowMarketplaceUI)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamShowMessageBoxUI)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamShowMessageBoxUIEx)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamShowSigninUI)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamTaskCloseHandle)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamTaskSchedule)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamTaskShouldExit)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserAreUsersFriends)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserCheckPrivilege)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserCreateAchievementEnumerator)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserCreateStatsEnumerator)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserGetDeviceContext)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserGetMembershipTierFromXUID)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserGetName)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserGetOnlineCountryFromXUID)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserGetSigninInfo)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserGetSigninState)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserGetXUID)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserReadProfileSettings)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamUserWriteProfileSettings)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamVoiceClose)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamVoiceCreate)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamVoiceHeadsetPresent)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamVoiceIsActiveProcess)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XamVoiceSubmitPacket)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XAudioGetDuckerLevel)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XAudioGetSpeakerConfig)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XAudioGetVoiceCategoryVolume)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XAudioRegisterRenderDriverClient)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XAudioSubmitRenderDriverFrame)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XAudioUnregisterRenderDriverClient)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptSha)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptSha256Final)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptSha256Init)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptSha256Update)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptSha384Final)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptSha384Init)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptSha384Update)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptSha512Final)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptSha512Init)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptSha512Update)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptShaFinal)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptShaInit)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeCryptShaUpdate)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeKeysConsolePrivateKeySign)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XeKeysConsoleSignatureVerification)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XexCheckExecutablePrivilege)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XexGetModuleHandle)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XexGetProcedureAddress)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XexLoadImage)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XexUnloadImage)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XGetAVPack)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XGetGameRegion)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XGetVideoMode)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XMACreateContext)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XMAReleaseContext)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XMsgCancelIORequest)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XMsgInProcessCall)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XMsgStartIORequest)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XMsgStartIORequestEx)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XNetLogonGetMachineID)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XNetLogonGetTitleID)
{
    ctx.r3.u64 = kNotImplemented;
}

PPC_FUNC(__imp__XNotifyGetNext)
{
    ctx.r3.u64 = kNotImplemented;
}
