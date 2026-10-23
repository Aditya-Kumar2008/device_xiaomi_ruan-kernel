# ruan UEFI — Redmi Pad Pro 5G (SM7435 "parrot")

A Mu-Silicium UEFI port for the **Redmi Pad Pro 5G** (`ruan`, Snapdragon 7s Gen 2 / SM7435),
generated from the device's **own firmware** rather than adapted from a sibling board.

**Status: it builds. It has NOT been booted on hardware.** See *What is proven* below.

## What is here

| Path | What |
|---|---|
| `ruanPkg/` | The UEFI platform package: `ruan.dsc`, `ruan.fdf`, `DeviceBuild.py`, `Include/` (APRIORI/DXE/RAW), `Library/` (MemoryMapLib, ConfigurationMapLib) |
| `Binaries/ruan/QcomPkg/Drivers/` | **92 drivers extracted from ruan's own firmware**, each with its `.efi`, `.depex` and a generated `.inf` carrying the firmware's real GUID |
| `Binaries/ruan/RawFiles/` | 28 raw config/panel files from the firmware (`uefiplat.cfg`, the two `n83` panel XMLs, `SecParti.cfg`, …) |
| `build-output/` | The build result: `Mu-ruan.img`, `SILICIUM_UEFI.fd`, checksums |
| `ruan.toml` | Build config: `FD base 0xA7000000`, `size 0x400000` |
| `ruan.dtb` | ruan's device tree, `"Parrot 4Gb SoC"`, `board-id <0x00 0x600>` |
| `ruan.conf`, `ruan.platform.c` | The HackiOS-side device profile and loader platform struct |

Provenance: everything device-specific comes from the official OTA
`ruan_in_global-ota_full-OS3.0.305.0.WFSINXM-user-16.0` (Android 16). No vendor firmware is
redistributed here — only the UEFI driver binaries extracted from the device's own `uefi.img`,
which is the same thing Mu-Silicium's `Device-Binaries` does for every supported board.

## Building

```sh
git clone --recurse-submodules https://github.com/Project-Silicium/Mu-Silicium.git
cd Mu-Silicium
cp -a /path/to/this/ruanPkg            Platforms/Xiaomi/ruanPkg
cp -a /path/to/this/Binaries/ruan      Binaries/ruan
cp /path/to/this/ruan.toml             Resources/Configs/ruan.toml
cp /path/to/this/ruan.dtb              Resources/DTBs/ruan.dtb
python3 build_uefi.py -d ruan -r RELEASE
```

Two environment notes (both cost a build):

- **Use `/usr/bin/python3`.** A stock-python build may segfault on `import ctypes` inside
  `edk2toollib/log/ansi_handler.py`, which looks like a mysterious `returncode=-11`.
- **`mu_nasm` without Mono:** symlink `/usr/bin/nasm` and `/usr/bin/ndisasm` into
  `Mu_Basecore/BaseTools/Bin/mu_nasm_extdep/Linux-x86-64/` and write
  `version: 20016.1.1` into `extdep_state.yaml` next to it.

## Running it

```sh
fastboot boot Mu-ruan.img
```

Bootloader must be unlocked. `fastboot boot` loads into RAM only — a reboot returns to Android.

## What is proven, and what is not

**Proven:** the package compiles, every module links, the FV generates, image validation passes on
all 49 PE images, and the output image is well-formed — it carries ruan's real device tree and the
UEFI FD sized to ruan's own `0x400000`.

**NOT proven:** that it boots. Nothing has run on hardware. A green build shows the *description*
of ruan is self-consistent; it says nothing about whether the memory map, the pin tables or the
driver dependency chain behave at runtime.

## Why not just copy garnet

`garnet` (Redmi Note 13 Pro 5G) shares the SoC, so it is the obvious starting point — but the two
boards differ in ways that matter:

| | garnet | ruan |
|---|---|---|
| UEFI FD | `0xA7100000`, `0x300000` | **`0xA7000000`, `0x400000`** |
| Pin controller | `TLMMDxe` | **`DALTLMM`** |
| `UsbConfigDxe` | — | **21.8% of bytes differ** |
| `ButtonsDxe` | — | identical apart from 3 bytes |
| Panel | 1220×2712 | **1600×2560**, CPHY, DSC 1.2 |
| Memory regions | 31 | **40** |

## The buttons/USB problem this port addresses

In ruan's firmware, `ButtonsDxe`, `DALTLMM`, `PmicDxe` and the USB chain are **not in APRIORI** —
they load by *dependency*. Decoded depex:

```
DALTLMM   : PUSH ae37b942-457f-4c91-a196-d9669fd347a3  END
ButtonsDxe: PUSH (text input) (DALTLMM's protocol) (PmicDxe's ×2) ... AND×4 END
```

If the DAL framework never publishes `ae37b942`, `DALTLMM` never dispatches, `ButtonsDxe` never
loads, and there is **no error and no log line**. That is exactly what "buttons and USB don't work"
looks like. This package carries ruan's own `DALTLMM` and its real APRIORI order, so the chain has
the dependencies it expects.

Also note: ruan's `RscRtDxe` and `SCHandlerRtDxe` are Qualcomm's builds of EDK2's
`ReportStatusCodeRouterRuntimeDxe` / `StatusCodeHandlerRuntimeDxe` — same GUIDs — so they must not
be listed alongside the EDK2 ones.

## Licence

The package structure follows Mu-Silicium (BSD-2-Clause-Patent). The driver binaries in
`Binaries/ruan/` are Xiaomi/Qualcomm firmware extracted from the device, distributed the way
Project Silicium distributes vendor binaries for other boards. `ruan.dtb` likewise comes from the
device's `vendor_boot`. Nothing here is Apple's.
