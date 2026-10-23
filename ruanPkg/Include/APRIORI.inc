##
# ruan APRIORI -- DXE dispatch order from the device's own firmware
# (APRIORI FFS file fc510ee7-ffdc-11d4-bd41-0080c73c8881 in uefi.img, OS3.0.305.0.WFSINXM).
# Entries the FV does not provide are kept as comments so the ORDER is preserved.
##

APRIORI DXE {
  INF MdeModulePkg/Core/Dxe/DxeMain.inf
  # 08ae422f-1196-88e7-d380-4845d8e33cd9
  # 46cc8e8c-9e6a-3ccc-ef04-206c0e4ee44b
  # 9158aab4-f8c4-b601-b743-844795b1f422
  INF ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  # 4d21dec4-f1ad-3a59-bcc1-588267026e4c
  # 95140081-1a91-9d98-1e04-ea495267ca42
  # b74625fe-d663-f541-484a-aa40aabfff15
  INF Binaries/ruan/QcomPkg/Drivers/DALSys/DALSys.inf
  # 4109033b-178e-a956-8d38-0c92df93e110
  # dd597cb1-1e83-fc20-1b21-05b1bdbbdd4a
  # 4c15524a-771f-9a00-d436-d54d8916c48e
  INF Binaries/ruan/QcomPkg/Drivers/ULogDxe/ULogDxe.inf
  # 429b1f0b-f193-b00e-cf89-ddbfeba74279
  # 1e8308b9-e54d-585b-f81f-a0abcb2c124e
  # 9329744a-b830-0401-f9bf-a244a5f69573
  INF Binaries/ruan/QcomPkg/Drivers/NpaDxe/NpaDxe.inf
  # 46927f37-16a4-e893-2e21-976675b4d38b
  # eeed1593-a161-e5ea-a6de-b54d02531a4d
  # 290d3b68-f9de-5824-d217-1f4ba5fe5eb7
  INF Binaries/ruan/QcomPkg/Drivers/ShmBridgeDxe/ShmBridgeDxe.inf
  # 4f36401b-a9a6-3d53-fccd-fc335acc8186
  # 15e9b8b4-38c5-67f0-9185-de04b3d27740
  # b64e0970-4bef-2a7b-cd80-e149b473374b
  INF MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  # 4b75082d-0e9a-1dfe-13f7-a5d9b4f7386b
  # a2ac9390-a2b5-c453-9ef3-a11ffffeae4a
  # 09b6a370-afb6-cd3b-fb50-e84f8e4eab74
  INF Binaries/ruan/QcomPkg/Drivers/FvSimpleFileSystem/FvSimpleFileSystem.inf
  # 533f42c1-a834-be69-2465-31108ecd350d
  # 0d0faf96-f789-6765-fe78-1596b7b6c344
  # 1f2bcd05-fc66-cf6d-ab14-134e84db9c02
  INF MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  # 411b0afb-c99e-f300-fc59-cc133a79b07d
  # fad06e90-d2ba-7e70-29f5-041c12061943
  # 0a81165c-022f-4138-c706-794f9c947e33
  INF Binaries/ruan/QcomPkg/Drivers/FvDxe/FvDxe.inf
  # 4dacd5e6-f9be-6a6e-4eec-7add3477311d
  # a4e3ea83-2a2f-ff6b-6c7b-785d5462f543
  # 7ef99c32-d67f-f099-ae71-364cb2a3dceb
  INF MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  # 4ed5e1cf-8bbd-df16-6bcb-1e1a2df636b3
  # 71494eae-f0bb-5d88-1ca6-e47973ed1243
  # a9623356-0fce-9b68-6bad-3a4fb60bf598
  INF MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  # 4882bfbd-ce9e-0bc8-b1c4-783bd5ff15ed
  # 36951891-0066-8e18-81aa-505aaec30846
  # 94af9be6-bd23-7a32-35f7-574faa1a447d
  INF Binaries/ruan/QcomPkg/Drivers/PmicGlinkDxe/PmicGlinkDxe.inf
  # 4d88b21f-488e-c2c4-8a1e-50df367fce4b
  # 087ee399-4044-ba4d-f342-302eb92b3f46
  # b04a7c5b-a6a7-94f8-34dc-014188c19917
  INF Binaries/ruan/QcomPkg/Drivers/XhciPciEmulation/XhciPciEmulation.inf
  # 412ca759-e4ad-d0dc-3e7f-7c28b7120624
  # 90003a9a-3f27-4dc1-cf62-2e2dcf9eb743
  # fe3d71fc-b4a7-9fb4-c042-cd4b85409bcc
  INF Binaries/ruan/QcomPkg/Drivers/UsbMsdDxe/UsbMsdDxe.inf
  # 434615f0-1883-6371-3673-6d3e4d3a82cd
  # 34415dae-41fa-b827-0e4f-130a5e07b340
  # 63468006-81db-5bd1-8704-1a4fae73820e
  INF Binaries/ruan/QcomPkg/Drivers/TsensDxe/TsensDxe.inf
  # 4e503fa3-58b9-8820-7353-f809dea09e56
  # 1001cf84-e53f-e516-cd37-095ff9735e44
  # decbd3d5-689e-b0d3-f811-c6438ece023a
  INF EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  # 4e554fdf-5ba4-23e1-f84d-456aecdc8e40
  # 84b4a8a5-3d4e-81e2-280c-cbcc244bd511
  # 4dc13f27-a4f5-c2f9-b4f7-e743ba995ea8
  INF Binaries/ruan/QcomPkg/Drivers/SecRSADxe/SecRSADxe.inf
  # d6a2cb7f-6a18-4e2f-b43b-9920a733700a  # DxeCore
  # 08ae422f-1196-88e7-d380-4845d8e33cd9
  # 46cc8e8c-9e6a-3ccc-ef04-206c0e4ee44b
  # 9158aab4-f8c4-b601-b743-844795b1f422
  # b8d9777e-d72a-451f-9bdb-bafb52a68415  # ArmCpuDxe
  # 4d21dec4-f1ad-3a59-bcc1-588267026e4c
  # 95140081-1a91-9d98-1e04-ea495267ca42
  # b74625fe-d663-f541-484a-aa40aabfff15
  # 8e9bd160-b184-11df-94e2-0800200c9a66  # DALSys
  # 4109033b-178e-a956-8d38-0c92df93e110
  # dd597cb1-1e83-fc20-1b21-05b1bdbbdd4a
  # 4c15524a-771f-9a00-d436-d54d8916c48e
  # e43128a8-8692-42b6-8afa-676158578d18  # ULogDxe
  # 429b1f0b-f193-b00e-cf89-ddbfeba74279
  # 1e8308b9-e54d-585b-f81f-a0abcb2c124e
  # 9329744a-b830-0401-f9bf-a244a5f69573
  # cb29f4d1-7f37-4692-a416-93e82e219711  # NpaDxe
  # 46927f37-16a4-e893-2e21-976675b4d38b
  # eeed1593-a161-e5ea-a6de-b54d02531a4d
  # 290d3b68-f9de-5824-d217-1f4ba5fe5eb7
  # 9c1eb71f-dd6c-4ed5-9f6a-5cc0ca789f16  # ShmBridgeDxe
  # 4f36401b-a9a6-3d53-fccd-fc335acc8186
  # 15e9b8b4-38c5-67f0-9185-de04b3d27740
  # b64e0970-4bef-2a7b-cd80-e149b473374b
  # 3ae17db7-3cc5-4b89-9304-9549211057ef  # ResetRuntimeDxe
  # 4b75082d-0e9a-1dfe-13f7-a5d9b4f7386b
  # a2ac9390-a2b5-c453-9ef3-a11ffffeae4a
  # 09b6a370-afb6-cd3b-fb50-e84f8e4eab74
  # 907125c0-a5f1-11e3-a3fe-a3198b49350c  # FvSimpleFileSystem
  # 533f42c1-a834-be69-2465-31108ecd350d
  # 0d0faf96-f789-6765-fe78-1596b7b6c344
  # 1f2bcd05-fc66-cf6d-ab14-134e84db9c02
  # 2b0ecdce-01ae-446e-9ffd-c4217de0340f  # VariableDxe
  # 411b0afb-c99e-f300-fc59-cc133a79b07d
  # fad06e90-d2ba-7e70-29f5-041c12061943
  # 0a81165c-022f-4138-c706-794f9c947e33
  # e2534a24-1d9a-4a54-aaec-be8836d0f45d  # FvDxe
  # 4dacd5e6-f9be-6a6e-4eec-7add3477311d
  # a4e3ea83-2a2f-ff6b-6c7b-785d5462f543
  # 7ef99c32-d67f-f099-ae71-364cb2a3dceb
  # f80697e9-7fd6-4665-8646-88e33ef71dfc  # SecurityStubDxe
  # 4ed5e1cf-8bbd-df16-6bcb-1e1a2df636b3
  # 71494eae-f0bb-5d88-1ca6-e47973ed1243
  # a9623356-0fce-9b68-6bad-3a4fb60bf598
  # 42857f0a-13f2-4b21-8a23-53d3f714b840  # CapsuleRuntimeDxe
  # 4882bfbd-ce9e-0bc8-b1c4-783bd5ff15ed
  # 36951891-0066-8e18-81aa-505aaec30846
  # 94af9be6-bd23-7a32-35f7-574faa1a447d
  # 6d4a9cf7-cc0c-42b0-aa89-f6a6a8d99273  # PmicGlinkDxe
  # 4d88b21f-488e-c2c4-8a1e-50df367fce4b
  # 087ee399-4044-ba4d-f342-302eb92b3f46
  # b04a7c5b-a6a7-94f8-34dc-014188c19917
  # beb12bee-f6e1-11e1-9fb8-6c626de4aeb1  # XhciPciEmulation
  # 412ca759-e4ad-d0dc-3e7f-7c28b7120624
  # 90003a9a-3f27-4dc1-cf62-2e2dcf9eb743
  # fe3d71fc-b4a7-9fb4-c042-cd4b85409bcc
  # 5af77f10-90df-4e7e-8325-a17ec09d5443  # UsbMsdDxe
  # 434615f0-1883-6371-3673-6d3e4d3a82cd
  # 34415dae-41fa-b827-0e4f-130a5e07b340
  # 63468006-81db-5bd1-8704-1a4fae73820e
  # f09ad92e-485c-9cdb-3e05-3385c38f94cc  # TsensDxe
  # 4e503fa3-58b9-8820-7353-f809dea09e56
  # 1001cf84-e53f-e516-cd37-095ff9735e44
  # decbd3d5-689e-b0d3-f811-c6438ece023a
  # 857a8741-0eec-43bd-9482-27d14ed47d72  # SimpleTextInOutSerial
  # 4e554fdf-5ba4-23e1-f84d-456aecdc8e40
  # 84b4a8a5-3d4e-81e2-280c-cbcc244bd511
  # 4dc13f27-a4f5-c2f9-b4f7-e743ba995ea8
  # 32c71e68-83a8-46ed-aed1-094e71b12057  # SecRSADxe
}
