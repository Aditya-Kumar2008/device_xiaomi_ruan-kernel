##
# ruan -- Redmi Pad Pro 5G (SM7435 "parrot"), 12.1" 1600x2560 BOE NT36532.
#
# Values derived from the device's own firmware (OS3.0.305.0.WFSINXM): uefiplat.cfg
# [MemoryMap]/[ConfigParameters] and the vendor_boot device tree.
# Adapted from Mu-Silicium's garnetPkg (same SoC, different board).
##
[Defines]
  PLATFORM_NAME                  = ruan
  PLATFORM_GUID                  = C58473C0-5653-4201-A8FF-FF74468533AE
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/ruanPkg
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = ruanPkg/ruan.fdf
  USE_CUSTOM_DISPLAY_DRIVER      = 0

  #
  # 0 = SM7435   (ruan)
  # 1 = SM6450
  #
  SOC_TYPE                       = 0

!include NetraniPkg/NetraniPkg.dsc.inc

[PcdsFixedAtBuild]
  #
  # DDR Memory -- uefiplat.cfg UnusableDDRMemoryStartAddr
  #
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x80000000

  #
  # UEFI Stack -- uefiplat.cfg "UEFI Stack" 0xA760D000
  #
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0xA760D000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000

  #
  # SMBIOS
  #
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Xiaomi"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Redmi Pad Pro 5G"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"ruan"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"24074RPD2G"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"n83"

  #
  # Simple Frame Buffer -- ruan panel n83_35_02_0a is 1600x2560
  #
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|1600
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|2560
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32

  #
  # Platform PEI -- uefiplat.cfg PlatConfigFileName / OsTypeString
  #
  gQcomPkgTokenSpaceGuid.PcdPlatformType|"LA"

[LibraryClasses]
  #
  # Memory Libraries
  #
  MemoryMapLib|ruanPkg/Library/MemoryMapLib/MemoryMapLib.inf

  #
  # QCOM Libraries
  #
  ConfigurationMapLib|ruanPkg/Library/ConfigurationMapLib/ConfigurationMapLib.inf

[Components]
  #
  # ACPI Tables
  #
  #ruan/AcpiTables.inf
