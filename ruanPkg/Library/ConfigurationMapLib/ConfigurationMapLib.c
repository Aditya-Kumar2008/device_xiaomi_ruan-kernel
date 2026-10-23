/*
 * ruan (Redmi Pad Pro 5G, SM7435 "parrot") -- configuration map.
 *
 * GENERATED from the device's own firmware: the [ConfigParameters] section of uefiplat.cfg,
 * extracted from uefi.img of the OTA OS3.0.305.0.WFSINXM.
 * This replaces garnet's ConfigurationMapLib.c. Note where ruan differs from garnet --
 * EnableDisplayThread is 1 here and 0 on garnet, and SecPagePoolCount is 0x1000 vs 0x800.
 */
#include <Library/ConfigurationMapLib.h>

STATIC
EFI_CONFIGURATION_ENTRY_DESCRIPTOR
gConfigurationDescriptor[] = {
  // Configuration Map
  {"ConfigParameterCount", 64},
  {"EnableShell", 0x1},
  {"SecPagePoolCount", 0x1000},
  {"SharedIMEMBaseAddr", 0x146AA000},
  {"DloadCookieAddr", 0x01FD3000},
  {"DloadCookieValue", 0x10},
  {"PilSubsysDbgCookieAddr", 0x146AA6DC},
  {"PilSubsysDbgCookieVal", 0x53444247},
  {"NumCpus", 8},
  {"NumActiveCores", 8},
  {"MaxLogFileSize", 0x400000},
  {"UefiMemUseThreshold", 0x1900},
  {"USBHS1_Config", 0x0},
  {"UsbFnIoRevNum", 0x00010001},
  {"PwrBtnShutdownFlag", 0x0},
  {"Sdc1GpioConfigOn", 0x1E92},
  {"Sdc2GpioConfigOn", 0x1E92},
  {"Sdc1GpioConfigOff", 0xA00},
  {"Sdc2GpioConfigOff", 0xA00},
  {"EnableSDHCSwitch", 0x1},
  {"EnableUfsIOC", 1},
  {"UfsSmmuConfigForOtherBootDev", 1},
  {"SecurityFlag", 0xC4},
  {"DetectRetailUserAttentionHotkey", 0x00},
  {"DetectRetailUserAttentionHotkeyCode", 0x17},
  {"EnableOEMSetupAppInRetail", 0x0},
  {"EnableLogFsSyncInRetail", 0x1},
  {"ShmBridgememSize", 0xA00000},
  {"EnableMultiThreading", 1},
  {"MaxCoreCnt", 8},
  {"EarlyInitCoreCnt", 2},
  {"EnableUefiSecAppDebugLogDump", 0x0},
  {"AllowNonPersistentVarsInRetail", 0x1},
  {"EnableDisplayThread", 0x1},
  {"EnableDisplayImageFv", 0x1},
  {"DDRInfoNotifyFlag", 0x0},
  {"EnableMultiCoreFvDecompression", 1},
  {"EnableVariablePolicyEngine", 0},
  {"EnableACPIFallback", 0x0},
};

VOID
GetConfigurationMap (
  OUT EFI_CONFIGURATION_ENTRY_DESCRIPTOR **ConfigurationDescriptor,
  OUT UINT8                               *ConfigurationDescriptorCount)
{
  *ConfigurationDescriptor      = gConfigurationDescriptor;
  *ConfigurationDescriptorCount = ARRAY_SIZE (gConfigurationDescriptor);
}
