# ruan UEFI firmware — driver inventory (from the device's own firmware)

Cracked `uefi.img` (4,206,496 B) out of the OTA. Structure:

```
0x0000  ELF64 wrapper (entry 0xa7000000)
0x1000  FV "SILICIUM_UEFI"-style volume: _FVH, 4,194,304 B, hdr 72 B
          ├─ DXE Core (8af09f13-44c5-96ec-1437-dd899cb5ee5d, 323,608 B)
          ├─ PEI Core (dde58710-41cd-4306-dbfb-3fa90bb1d2dd, 7,783 B)
          ├─ FFS 9e21fd93-… → COMPRESSION → nested FV 4,005,892 B  ← all the drivers
          └─ FFS d14c51ed-… (998,542 B)
```

The nested volume holds **132 FFS files, 95 of them with a real PE/TE image**. Full list with GUIDs:
`ruan_uefi_drivers.txt`.

## The answer to "buttons and USB don't work"

**ruan's firmware contains the complete, correct driver set for both.** Nothing is missing:

**Buttons** — `ButtonsDxe`, GUID `5bd181db-0487-4f1a-ae73-820e165611b3`, 41,110 B.
**USB** — the whole chain is present:
`UsbConfigDxe`, `UsbInitDxe`, `UsbDeviceDxe`, `UsbMsdDxe`, `UsbPwrCtrlDxe`, `UsbfnDwc3Dxe`,
`UCDxe`, `XhciDxe`, `XhciPciEmulation`, `UsbBusDxe`, `UsbKbDxe`, `UsbMassStorageDxe`.
**GPIO / pin control** — `DALTLMM` (`8681cc5a-0df6-441e-b4b8-e915c538f067`), `GpiDxe`, `I2C`, `SPI`, `SPMI`.
**PMIC / power** — `PmicDxe`, `PmicGlinkDxe`, `ChargerExDxe`, `QcomChargerDxeLA`, `QcomChargerApp`,
`QcomChargerCfg.cfg`, `PwrUtilsDxe`, `RpmhDxe`, `RpmhDxe`, `NpaDxe`, `VcsDxe`, `ClockDxe`.

So the failure is **not** "the drivers don't exist for this board". It is that the build is using
the wrong ones (garnet's), or the pin/PMIC tables inside them.

## garnet vs ruan: same GUIDs, different bytes

I downloaded garnet's drivers from `Project-Silicium/Device-Binaries` and compared:

| Driver | GUID | Size (both) | Differing bytes |
|---|---|---|---|
| `ButtonsDxe` | identical | 40,960 | **3** (0.0%) |
| `UsbConfigDxe` | identical | 86,016 | **18,791 (21.8%)** |

This is the decisive result, and it is a *good* one:

- **`ButtonsDxe` is effectively identical** (3 bytes differ — build stamp noise). So garnet's
  `ButtonsDxe` is safe to reuse. If buttons do not work, the problem is **not** this driver: it is
  the TLMM/pin configuration, the PMIC, or `ButtonsDxe` not being reached at all.
- **`UsbConfigDxe` is 21.8% different.** That is a genuinely different binary. USB role
  configuration is board-specific (which port, host vs device, power switching), so this one
  **must** come from ruan.

This turns VildanG's debugging from guesswork into a two-way split:
if he uses garnet's `UsbConfigDxe`, USB will very likely fail; if he uses ruan's, it should not.

## The one structural difference that matters most

| ruan | garnet |
|---|---|
| **`DALTLMM`** (49,224 B, `8681cc5a-…`) | **`TLMMDxe`** |

ruan uses `DALTLMM` where garnet uses `TLMMDxe`. TLMM is the pin-mux controller — the thing that
decides which physical pad is a button, which is a UART, which drives a GPIO. **This is the single
most likely cause of "buttons not working"** on a port that copied garnet's package.

## ruan-specific content worth naming

| File | Why it matters |
|---|---|
| `Panel_N83_35_02_0a_wqxga_video_cphy.xml` | ruan's own panel config (BOE NT36532, 1600×2560) |
| `Panel_N83_42_02_0b_wqxga_video_cphy.xml` | the second panel variant |
| `MiTokenDxe` | Xiaomi-specific; not in garnet |
| `VerifiedBootDxe` | present in both |
| `BDS_Menu.cfg`, `uefiplat.cfg`, `uefilpil.cfg`, `SecParti.cfg`, `QcomChargerCfg.cfg` | raw config blobs |
| `logo_m_lock.bmp` | 4,097,142 B — the boot logo |
| 17 other `Panel_*.xml` | NT36672E / R66451 / VTDR6130 — not ruan's panels, carried for other boards |

Also present (and matching HackiOS's redwood `RAW.inc` GUIDs exactly, which is a good cross-check):
`uefipil.cfg` = `21e9bdd9-6c3f-4f10-84a5-bbec322741f1`,
`QcomChargerCfg.cfg` = `45fe4b7c-150c-45da-a021-4beb2048ec6f`.

## What this means for the port

1. **A `ruanPkg` can be built from ruan's own firmware**, and the driver list is now known
   (`ruan-port/ruanPkg/Include/DXE.inc`).
2. **garnet's `ButtonsDxe` is interchangeable** (3 bytes). Everything else must be checked
   driver by driver the same way.
3. **`DALTLMM` vs `TLMMDxe` is the prime suspect** for the button failure.
4. **`UsbConfigDxe` must be ruan's** — 21.8% divergence is far too much to paper over.

## Files

```
/workspace/rom/extracted/
  uefi.img                     the firmware volume (4.2 MB)
  ruan_uefi_drivers.txt        132 files: UI name, size, PE?, depex?, GUID
  ruan_drivers/                key drivers extracted as standalone .efi
    ButtonsDxe.efi  UsbConfigDxe.efi  DisplayDxe.efi  DALTLMM.efi  PmicDxe.efi
```

Hashes of the extracted ruan drivers:

```
b8b7ca78bf4e7a2e149548424db3afdd  DALTLMM.efi      (49,152)
66ad2be6d067a9a515af2075a3dbea45  PmicDxe.efi      (204,800)
2d0def3afb4bb48a836394ad9ccf0604  DisplayDxe.efi   (323,584)
711504761f61f9caf64dc29dee97e7b4  ButtonsDxe.efi   (40,960)
90d01d796c24e65b3c55da98f4cdd8ea  UsbConfigDxe.efi (86,016)
```

## Caveat

I have the driver *inventory* and the GUIDs. I have **not** verified that each PE image runs on
ruan — that needs the UEFI to actually be built and booted. And a driver being present in the
firmware volume does not prove the shipping Android UEFI loads it; the load order is in
`DXE.inc`-equivalent metadata and in the APRIORI file, which I have not yet extracted.
