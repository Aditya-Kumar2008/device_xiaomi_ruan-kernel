  # already in APRIORI.inc or duplicated: MdeModulePkg/Core/Dxe/DxeMain.inf
  INF MdeModulePkg/Universal/PCD/Dxe/Pcd.inf

  INF Binaries/ruan/QcomPkg/Drivers/EnvDxe/EnvDxe.inf

  INF MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  INF MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  # already in APRIORI.inc or duplicated: ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  INF MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  # already in APRIORI.inc or duplicated: MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf

  # already in APRIORI.inc or duplicated: Binaries/ruan/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/TzDxeLA/TzDxeLA.inf
  # already in APRIORI.inc or duplicated: Binaries/ruan/QcomPkg/Drivers/TzDxeLA/TzDxeLA.inf

  INF MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  # already in APRIORI.inc or duplicated: MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  # already in APRIORI.inc or duplicated: MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  INF EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  # already in APRIORI.inc or duplicated: MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  INF EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  INF EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  INF MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  INF ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  INF ArmPkg/Drivers/TimerDxe/TimerDxe.inf

  INF Binaries/ruan/QcomPkg/Drivers/ChipInfo/ChipInfo.inf
  INF Binaries/ruan/QcomPkg/Drivers/IPCCDxe/IPCCDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/GlinkDxe/GlinkDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/PlatformInfoDxeDriver/PlatformInfoDxeDriver.inf
  # ruan has no equivalent of garnet's ProjectInfoDxe -- original: INF Binaries/garnet/QcomPkg/Drivers/ProjectInfoDxe/ProjectInfoDxe.inf
  # already in APRIORI.inc or duplicated: Binaries/ruan/QcomPkg/Drivers/DALSys/DALSys.inf
  INF Binaries/ruan/QcomPkg/Drivers/HALIOMMU/HALIOMMU.inf
  INF Binaries/ruan/QcomPkg/Drivers/HWIODxeDriver/HWIODxeDriver.inf
  INF Binaries/ruan/QcomPkg/Drivers/I2C/I2C.inf
  INF Binaries/ruan/QcomPkg/Drivers/DALTLMM/DALTLMM.inf
  INF Binaries/ruan/QcomPkg/Drivers/PmicDxe/PmicDxe.inf

  INF MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  INF MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  INF MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  INF FatPkg/EnhancedFatDxe/Fat.inf
  INF MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf

  INF Binaries/ruan/QcomPkg/Drivers/UFSDxe/UFSDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/CmdDbDxe/CmdDbDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/PwrUtilsDxe/PwrUtilsDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/RpmhDxe/RpmhDxe.inf
  # already in APRIORI.inc or duplicated: Binaries/ruan/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  # already in APRIORI.inc or duplicated: Binaries/ruan/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/VcsDxe/VcsDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/ClockDxe/ClockDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/SPMI/SPMI.inf
  INF Binaries/ruan/QcomPkg/Drivers/ICBDxe/ICBDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/SmemDxe/SmemDxe.inf
  INF MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  # already in APRIORI.inc or duplicated: EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  INF MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  INF MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  INF MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  INF MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/ruan/QcomPkg/Drivers/CPRDxe/CPRDxe.inf
!endif
  INF Binaries/ruan/QcomPkg/Drivers/GpiDxe/GpiDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/SPI/SPI.inf
  INF Binaries/ruan/QcomPkg/Drivers/SdccDxe/SdccDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/FeatureEnablerDxe/FeatureEnablerDxe.inf

!if $(USE_CUSTOM_DISPLAY_DRIVER) == 1
  INF Binaries/ruan/QcomPkg/Drivers/DisplayDxe/DisplayDxe.inf
!else
  INF SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
!endif

  INF Binaries/ruan/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/ChargerExDxe/ChargerExDxe.inf
  # already in APRIORI.inc or duplicated: Binaries/ruan/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/QcomChargerDxeLA/QcomChargerDxeLA.inf
  INF Binaries/ruan/QcomPkg/Drivers/UsbPwrCtrlDxe/UsbPwrCtrlDxe.inf
  # already in APRIORI.inc or duplicated: Binaries/ruan/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/DDRInfoDxe/DDRInfoDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/UsbfnDwc3Dxe/UsbfnDwc3Dxe.inf
  # already in APRIORI.inc or duplicated: Binaries/ruan/QcomPkg/Drivers/XhciPciEmulation/XhciPciEmulation.inf
  INF Binaries/ruan/QcomPkg/Drivers/XhciDxe/XhciDxe.inf

  INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf

  # already in APRIORI.inc or duplicated: Binaries/ruan/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/UsbDeviceDxe/UsbDeviceDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/UsbConfigDxe/UsbConfigDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/UsbInitDxe/UsbInitDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/UCDxe/UCDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/RngDxe/RngDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/PILDxe/PILDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/PILProxyDxe/PILProxyDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/SPSSDxe/SPSSDxe.inf


  # --- ruan-only drivers (not present in garnet's list) ---
  INF Binaries/ruan/QcomPkg/Drivers/ASN1X509Dxe/ASN1X509Dxe.inf
  # already in APRIORI.inc or duplicated: Binaries/ruan/QcomPkg/Drivers/FvDxe/FvDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/MiTokenDxe/MiTokenDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/ParserDxe/ParserDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/QcomBds/QcomBds.inf
  INF Binaries/ruan/QcomPkg/Drivers/QcomChargerApp/QcomChargerApp.inf
  INF Binaries/ruan/QcomPkg/Drivers/QcomWDogDxe/QcomWDogDxe.inf
  # dropped: same GUID as EDK2's status-code module -> INF Binaries/ruan/QcomPkg/Drivers/RscRtDxe/RscRtDxe.inf
  # dropped: same GUID as EDK2's status-code module -> INF Binaries/ruan/QcomPkg/Drivers/SCHandlerRtDxe/SCHandlerRtDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/ScmDxeLA/ScmDxeLA.inf
  # already in APRIORI.inc or duplicated: Binaries/ruan/QcomPkg/Drivers/SecRSADxe/SecRSADxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/SerialPortDxe/SerialPortDxe.inf
  INF Binaries/ruan/QcomPkg/Drivers/VerifiedBootDxe/VerifiedBootDxe.inf