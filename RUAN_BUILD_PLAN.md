# ruan — build plan, and the root cause of "buttons and USB don't work"

Everything here comes from the device's own firmware (`OS3.0.305.0.WFSINXM`). This is the state of
a real port, not an estimate.

## 1. The root cause, found

I decoded the **APRIORI** file (DXE load order) and then every driver's **depex** (dependency
expression). Two facts combine:

**(a) `ButtonsDxe`, `DALTLMM`, `PmicDxe`, `GpiDxe`, `UsbConfigDxe` are not in APRIORI at all.**
They are 72 of the 94 drivers that load purely by *dependency*. If a dependency is never published,
DXE silently never dispatches them — no error, no log line. That is precisely the failure mode that
looks like "the driver is missing."

**(b) The dependency chain is real and it can break.** Decoded:

```
DALTLMM  depex:  PUSH ae37b942-457f-4c91-a196-d9669fd347a3  END
ButtonsDxe depex:
    PUSH 387477c1-... (EFI_SIMPLE_TEXT_INPUT_PROTOCOL)
    PUSH 157a5c45-21b2-43c5-ba7c-822fee5fe599
    PUSH 4bc5c6d7-4ca4-8835-d1bc-d29cc2025d4a
    PUSH 9ba45b66-efa4-441c-a3e4-ed2224786be2
    PUSH ad9aec18-7bf0-4809-9e96-3012309f3df7
    AND AND AND AND
    END
```

Who publishes `ae37b942-…` (the one `DALTLMM` waits on)? Searching the PE images:
`DALSys`, `ChipInfo`, `PlatformInfoDxeDriver`, `HWIODxeDriver`, `PmicDxe`, `RpmhDxe`, `NpaDxe`,
`GpiDxe`, `SPI`, `ClockDxe`, `VcsDxe`, `ICBDxe`, `CPRDxe`, `TsensDxe`, `ULogDxe`, `UsbConfigDxe`.

So: **`DALTLMM` needs `DALSys` (and friends) to publish `ae37b942`; `ButtonsDxe` needs `DALTLMM`
(`ad9aec18`) and `PmicDxe` (`157a5c45`, `4bc5c6d7`, `9ba45b66`).** If the DAL framework drivers
don't come up — wrong pin/PMIC tables, wrong `DALTLMM` binary, or `DALSys` not dispatched — the
whole chain goes quiet and you get exactly "buttons and USB don't work", with no diagnostic.

**This is why copying garnet's package fails.** garnet uses `TLMMDxe`; ruan uses `DALTLMM`. Same
GUID family, different driver, and `ButtonsDxe` hangs off it.

## 2. ruan's real memory map (from `uefiplat.cfg`)

This was the last unknown, and it is now confirmed from the device:

| | ruan | garnet |
|---|---|---|
| **UEFI FD** | `0xA7000000`, size **`0x400000`** | `0xA7100000`, size `0x300000` |
| UEFI Stack | `0xA760D000` | `0xA760D000` |
| DDR base | `0x80000000`, 6 MB unusable at start | same |
| Kernel region | `0xA8000000`, `0x10000000` | same |
| DXE Heap | `0xF8000000`, `0x07C00000` | same |
| `MaxMemoryRegions` | 74 | — |
| `NumCpus` / `NumActiveCores` | **8 / 8** | — |
| `EarlyInitCoreCnt` | 2 | — |
| TLMM | `0x0F000000`, `0x01000000` | same |
| QUP (touch SPI/I2C) | `0x00800000`, `0x00300000` | same |
| USB | `0x0A600000` | same |
| `SecurityFlag` | `0xC4` (variable services + secapp + keymaster + qsee logs) | — |
| `EnableShell` | 1 | — |
| `DefaultBDSBootApp` | `LinuxLoader` | — |
| `EnableDisplayThread` | 1 | — |

**Free RAM for `PAYLOAD_BASE`:** the map has no region between `VM PIL` (`0xE0000000` + `0x05700000`
= `0xE5700000`) and `DXE Heap` (`0xF8000000`) — that is a **~306 MB hole**. `0xF1400000` sits in it,
clear of everything. (garnet uses `0xF0000000`; both are in the hole, but `0xF1400000` keeps a
margin from `VM PIL`.)

## 3. What is built and ready

```
ruan-port/
  ruan.conf                                    device profile — FD_BASE/FD_SIZE/PAYLOAD_BASE confirmed
  ruan.platform.c                              xnuloader platform struct — GIC/cores/etc. confirmed
  ruanPkg/
    ruan.dec                                   package definition
    ruan.dsc                                   platform DSC (SMBIOS "Redmi Pad Pro 5G", FB 1600x2560)
    ruan.fdf                                   flash definition — FV layout matches ruan's firmware
    Include/DXE.inc                            92 INF entries, all verified to exist
    Include/APRIORI.inc                        36 INF + the full order (comments keep the sequence)
    Include/RAW.inc                            29 FREEFORM files with ruan's GUIDs
    Library/MemoryMapLib/MemoryMapLib.c        40 memory/register regions, from uefiplat.cfg
    Library/MemoryMapLib/MemoryMapLib.inf
  Binaries/ruan/QcomPkg/Drivers/               92 driver dirs: .efi + .inf (+ .depex where present)
  Binaries/ruan/RawFiles/                      8 raw config/panel files from the firmware
```

**92 drivers extracted from ruan's own firmware**, each in Mu-Silicium's expected layout, with a
generated `.inf` carrying the firmware's own GUID and a `PE32|<name>.efi|*` binary section:

```
Binaries/ruan/QcomPkg/Drivers/ButtonsDxe/   ButtonsDxe.efi  .depex  .inf
Binaries/ruan/QcomPkg/Drivers/DALTLMM/      DALTLMM.efi     .depex  .inf
Binaries/ruan/QcomPkg/Drivers/UsbConfigDxe/ UsbConfigDxe.efi .depex .inf
... 92 in all
```

All 92 PE images validated (MZ header present), 0 size mismatches against the firmware's own
declared sizes, 70 depex files. Total 7.4 MB.

### Cross-check that the FDF layout is right

garnet's `garnet.fdf` declares:

```
FILE FV_IMAGE = 9E21FD93-9C72-4c15-8C4B-E77F1DB2D792 {
  SECTION GUIDED EE4E5898-3914-4259-9D6E-DC7BD79403CF PROCESSING_REQUIRED = TRUE {
    SECTION FV_IMAGE = FVMAIN
```

and ruan's `uefi.img` contains **exactly those two FFS files** — `9e21fd93-9c72-4c15-8c4b-e77f1db2d792`
(1,389,190 B) wrapping a 4,005,892-byte nested volume, and the guided LZMA section inside it. So the
image layout in `ruan.fdf` matches the shipping firmware, not just garnet.

Plus the raw firmware files, which are **ruan's, not garnet's** and must be used as-is:

```
/workspace/rom/extracted/ruan_configs/
  uefiplat.cfg                  the memory map above
  SecParti.cfg  uefipil.cfg  QcomChargerCfg.cfg  BDS_Menu.cfg  PanelSimConfig.csv
  Panel_N83_35_02_0a_wqxga_video_cphy.xml    <- ruan's panel: 1600x2560, CPHY, DSC 1.2, 36bpp
  Panel_N83_42_02_0b_wqxga_video_cphy.xml    <- the second panel variant
```

## 4. Build steps

```sh
# 1. Mu-Silicium with submodules (~3 GB)
git clone --recurse-submodules https://github.com/Project-Silicium/Mu-Silicium.git
cd Mu-Silicium

# 2. put ruan's package and its vendor binaries in place
cp -a /path/to/ruanPkg                  Platforms/Xiaomi/ruanPkg
mkdir -p Binaries/ruan/QcomPkg/Drivers
#    -> unpack ruan_drivers/*.efi + *.depex there, one dir per driver,
#       matching the names in ruanPkg/Include/DXE.inc
#    -> plus the raw files from ruan_configs/ under Binaries/ruan/RawFiles/

# 3. add ruan to the device list and build
python3 build_uefi.py -d ruan -r RELEASE
#   -> Build/ruanPkg/RELEASE_CLANGPDB/FV/SILICIUM_UEFI.fd
```

Then HackiOS's `mkxnuimg.py` packs that with `BootShim` and the XNU loader into the image for
`fastboot boot`, using `FD_BASE=0xA7000000`, `FD_SIZE=0x400000`, `PAYLOAD_BASE=0xF1400000`.

### Known gaps before this builds cleanly

1. **`ruanPkg/Library/ConfigurationMapLib`** — the `dsc` references it; `.inf`/`.c` not written. Copy
   from garnet and adjust (its content is the platform-config list, which I have in `uefiplat.cfg`).
2. **`QcomPkg/Extra.fdf.inc`** and `SiliciumPkg/Common.fdf.inc` — these come from Mu-Silicium itself,
   not from this package.
3. **`fb_base`** — still needs one UEFI run to read `display W x H at ...`.
4. **Toolchain** — Mu-Silicium needs its `mu_nasm`/Mono workaround (documented in HackiOS's
   `uefi/README.md`).
5. **`.depex` for the 22 drivers that have none** — those drivers declare no dependency, so their
   generated `.inf` has no `DXE_DEPEX` line. That matches the firmware.

## 5. Honest status

**Verified, from ruan's own firmware:** the memory map; the 92-driver inventory; every GUID; the
APRIORI dispatch order; every depex; the panel config; the device tree; the GPU/touch/CPU values;
and the FV layout (cross-checked against the two FFS GUIDs in the shipping image).

**Not verified:** that this **builds**, and that it **boots**. No build has been attempted and
nothing has touched hardware. The remaining work is mechanical (item 1 above plus dropping the
package into Mu-Silicium), but "mechanical" is not "done", and the first build will almost certainly
surface errors — a wrong `ENTRY_POINT`, a missing package dependency, a duplicated GUID.

**The single highest-value next step** is attempting the Mu-Silicium build, because that is where
assumptions fail loudly instead of silently. A build error is worth more than another document.

## 6. Why this is a better position than garnet-copying

The previous approach was: take `garnetPkg`, point it at ruan. That fails specifically because:

| | garnet | ruan |
|---|---|---|
| UEFI FD base/size | `0xA7100000` / `0x300000` | **`0xA7000000` / `0x400000`** |
| Pin controller driver | `TLMMDxe` | **`DALTLMM`** |
| USB config driver | garnet's | **21.8% different bytes** |
| Panel | 1220×2712 | **1600×2560**, CPHY, DSC 1.2 |
| Memory regions | 31 | **40** |

This package instead carries **ruan's own drivers and ruan's own memory map**, and its `APRIORI.inc`
is the device's real dispatch order — so the `DALTLMM` → `ButtonsDxe` chain has the dependencies it
actually expects.
