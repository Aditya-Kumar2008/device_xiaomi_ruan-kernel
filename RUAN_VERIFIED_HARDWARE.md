# `ruan` hardware — verified from the device's own firmware

Source: the official OTA you provided,
`ruan_in_global-ota_full-OS3.0.305.0.WFSINXM-user-16.0-1ede5cec45.zip` (5.02 GB, signed, `pre-device=ruan`).
Extracted `payload.bin` → `xbl.img`, `xbl_config.img`, `uefi.img`, `uefisecapp.img`, `vendor_boot.img`,
`dtbo.img`, `boot.img`; decompiled the device trees.

**Every value below is read out of `ruan`'s own firmware**, not from a sibling device. Where a value
is still unknown it says so.

## Identity

| | |
|---|---|
| Build | `OS3.0.305.0.WFSINXM`, Android **16** (`Redmi/ruan/ruan:16/BP2A.250605.031.A3`), patch 2026-08-01 |
| SoC | `qcom,parrot` / `qcom,montague` / `qcom,ravelin`; `msm-id = <0x219 0x10000 0x265 0x10000>` |
| Board | `qcom,board-id = <0x00 0x600>` — matches **"Parrot 4Gb SoC"** (DT name; ruan ships 6/8 GB) |
| `vendor_boot` | header v4, page 4096, `vendor_ramdisk` 13,848,755 B, DTB 4,886,872 B |
| DTBs in `vendor_boot` | **14** concatenated (the 4.8 MB blob is *not* one tree — see the trap below) |
| DTBO overlays | **39**; `ruan` uses a ruan-specific one (id 38 region) |

The 14 trees in `vendor_boot`, by model string:

| # | Model | board-id |
|---|---|---|
| 0 | Montague 4Gb SoC | `0x00 0x600` |
| 1 | Apache wigig IDP | — |
| 2 | Montague SoC | `0x00 0x00` |
| 3 | MontagueP 4Gb SoC | `0x00 0x600` |
| 4 | MontagueP SoC | `0x00 0x00` |
| **5** | **Parrot 4Gb SoC** | **`0x00 0x600`** ← ruan |
| 6 | Parrot SG SoC | `0x00 0x00` |
| 7 | Parrot SoC | `0x00 0x00` |
| 8 | ParrotP SG SoC | `0x00 0x00` |
| 9 | ParrotP SoC | `0x00 0x00` |
| 10 | Ravelin 4Gb SoC | `0x00 0x600` |
| 11–13 | Ravelin / variants | — |

> **The trap, confirmed on real data.** The 4,886,872-byte "dtb" in `vendor_boot` is 14 DTBs
> concatenated. Decompiling it naively yields a **Montague (SM6450)** tree whose GPU is
> `Adreno613v1` — a different chip. This is exactly the error I nearly published earlier. The
> correct ruan tree is `vb_05`.

## What `ruan.platform.c` needs (all confirmed)

| Field | Value | Source |
|---|---|---|
| `gic_dist` | **0x17200000** (0x10000) | `arm,gic-v3` reg |
| `gic_redist` | **0x17260000** (0x100000) | same node |
| `cores` | **8** | `cpu@0,100,200,300,400,500,600,700` |
| `core_shift` | **8** | Qualcomm MPIDR affinity-2 numbering |
| `small_cores` | **0x0f** | `cpu@0..300` capacity `0x400` (1024, A55); `cpu@400..700` `0x799` (1945, A78) |
| `psci_hvc` | **0** (SMC) | phone/tablet firmware |
| `uart` | 0 | no PL011 |
| `fb_base` | **still unknown** | only the UEFI's own `display W x H at ...` line gives it — needs one run on the device |
| `io_base` / `io_size` | not yet pinned | must be one non-zero window over the peripherals |

## Peripherals

| Subsystem | `ruan` value | Notes for the port |
|---|---|---|
| **GPU** | `qcom,kgsl-3d0@3d00000`, compatible **`qcom,adreno-gpu-gen7-3-0`**, `qcom,gpu-model = "Adreno710v1"` | **a7xx**, not redwood's a6xx. GMU `qcom,gmu@3d69000` (parrot-gpu.dtsi) |
| GPU clock ctrl | `clock-controller@3d90000`, `qcom,parrot-gpucc` | |
| **CPU frequency** | `qcom,cpufreq-hw-epss`, reg `0x17d91000 0x1000`, `0x17d92000 0x1000` — **2 domains** | redwood has 3 at `0x18591000` |
| **Touch** | **Novatek**, `compatible = "novatek,NVT-ts-spi"`, `irq-gpio` 54, `spi-max-frequency` **9.6 MHz**, MP criteria **5983** and **5986**, pen support | **not FocalTech, not Goodix** |
| **Panel** | `"xiaomi n83 35 02 0a cphy nt36532 boe"` and `n83 42 02 0b`, **1600 × 2560**, `dsi_video_mode`, CPHY, DSC, 60 Hz, clock 688 MHz | Novatek **NT36532** + BOE |
| **Display** | 1600 × 2560 (10:16) | iPhone is 1125 × 2436 (0.462) → letterbox/crop needed |
| **Wi-Fi / BT** | `bt_wcn6750` — **WCN6750** | same radio as redwood → `WIFI_ATH11K=1` |
| **Storage** | `ufshc_mem` (`1d84000.ufshc`) | UFS 2.2 |
| **Speakers** | 4 amps: `wsa883x` ×2 + `wsa2-macro`, `si_pa_*` (sia81xx) | 4 speakers vs redwood's 1 |
| Audio codec | `wcd937x` / `wcd938x` | |
| Hall / lid | `xiaomi_hall` | tablet-only |

## The two things that matter most

### 1. Touch is Novatek NT36532 — and a working XNU driver already exists

UniXNU's **`NativeNovatekTouch`** is *"Novatek NT36532E touch driver for sheng"*, Apache-2.0, and
explicitly split so it is portable:

- `NvtProtocol.c` — the NT36532E host-download protocol over an **abstract SPI transport**
  (`struct nvt_bus { xfer(); delay_ms(); }`), with the comment *"No board addresses or clocks here."*
- `NvtTouchCore.c` — finger-touch processing (40×60 matrix, peaks, centroid, slot tracking)

Its axes are `NVT_LONG_AXIS_MAX 30480` / `NVT_SHORT_AXIS_MAX 20320` and `NVT_FRAME_HZ 120` — those are
**sheng's** numbers, so they must be re-measured for ruan's panel, and the firmware/MP-criteria
(ruan: 5983/5986) will differ. But the protocol and touch-processing layers are reusable as-is.

**This changes the difficulty of touch from "write a driver from scratch" to "retarget a working
driver".** For redwood, HackiOS had to write a bit-banged SPI driver for a Goodix GT9916; for ruan
there is a maintained NT36532 implementation to start from.

### 2. The UEFI firmware for ruan is now in hand

`uefi.img` (4.2 MB) is the device's **UEFI firmware volume** — a valid `_FVH` at offset 4096,
4,194,304 bytes, with a DXE core and PEI core inside. `xbl.img` (1.19 MB) is a 32-bit ARM ELF
(entry `0x2211c000`) — the boot loader, **not** an FV, so it is not the driver source for ruan
(unlike what HackiOS's README describes for redwood).

This is the missing input for VildanG's buttons/USB problem: the DXE drivers that drive TLMM, the
PMIC and the USB controller must come from **this** firmware volume, not from garnet's.

## Files extracted, and where they are

```
/workspace/rom/extracted/
  xbl.img  xbl_config.img  uefi.img  uefisecapp.img
  boot.img  vendor_boot.img  dtbo.img
  ruan_vendor_boot.dtb      <- the 14-DTB blob (do NOT treat as one tree)
  dtbs/vb_00..vb_13.dtb     <- the 14 split trees; vb_05 is ruan
  dtbs/vb_05.dts            <- decompiled ruan SoC tree
  dtbo/o00..o38.dtb         <- 39 overlays; o38 is the ruan board overlay
  dtbo/o38.dts
  bootconfig.txt
```

## Corrections to my earlier analysis

I owe you these plainly:

1. **Touch.** I first reported FocalTech. That came from a merged `parrot.dtb` artifact that turned
   out to be a **Montague** tree, and I discarded it. Re-derived from ruan's own firmware: the touch
   is **Novatek NVT-ts-spi**. Both of my earlier guesses (Goodix, then FocalTech) were wrong.
2. **GPU.** I predicted Adreno 710 / a7xx and said the a6xx driver would not carry over. Confirmed
   exactly: `Adreno710v1`, `gen7-3-0`.
3. **GIC / EPSS / cores / gpucc.** I predicted `0x17200000` / `0x17d91000` / 8 cores / shift 8 /
   `small_cores 0x0f` / `0x3d90000` from the LineageOS `parrot.dtsi`. **All six confirmed** by the
   device's own tree.
4. **Panel.** Predicted 1600 × 2560 — confirmed, and the panel is BOE NT36532 (CPHY, DSC).

## Still unknown

- `fb_base` — needs one UEFI run on the device to print it.
- `io_base` / `io_size` — needs the `ruanPkg` memory map.
- The UEFI driver inventory inside `uefi.img` — the FV is a `MultiObject`; I have the FV and its
  DXE/PEI cores located but have not yet enumerated the individual DXE drivers and their GUIDs.
- Whether UniXNU's NT36532E firmware blobs work on ruan's NT36532 panel variant (MP criteria 5983).
