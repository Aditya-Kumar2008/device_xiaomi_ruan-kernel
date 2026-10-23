# ruan UEFI — BUILD SUCCEEDED

**The firmware built.** First attempt to compile `ruanPkg` in Mu-Silicium, and it produced a
complete, flashable boot image.

```
ruan-port/build-output/
  Mu-ruan.img                 1,361,920 B   the image for `fastboot boot`
  SILICIUM_UEFI.fd            4,194,304 B   the UEFI firmware volume (= FD_SIZE 0x400000 exactly)
  SILICIUM_UEFI.fd-bootshim   4,194,416 B   FD + the 176-byte BootShim
  SILICIUM_UEFI.fd-bootshim.gz  969,770 B   gzipped, as it goes in the boot image
  BootShim.bin                    112 B     compiled with ruan's FD_BASE/FD_SIZE
  SHA256SUMS.txt
```

Build log: `PROGRESS - Success`, **`0049 Images Verified`**, exit 0.

## Verified structure of `Mu-ruan.img`

Decompressed and checked by hand, not assumed:

```
ANDROID! header                2,048 B
kernel  (gzip)             1,357,804 B  ->  4,194,416 B decompressed
                                            = BootShim (176) + UEFI FD (4,194,240)
                                            _FVH found at offset 152  (176-24)  ✓
ramdisk                            5 B   (the Mu-Silicium stub, as intended)
DTB appended                 388,034 B   FDT magic d00dfeed, declares 388,034 ✓
                                          model string inside: "Parrot 4Gb SoC"  ✓
```

So the image carries **ruan's own device tree**, not garnet's — and the FD inside it is the one
this build produced, sized to ruan's `0x400000`.

## What it took — the seven real failures

Each was a genuine fact about ruan that a garnet copy would have gotten wrong.

| # | Failure | Cause | Fix |
|---|---|---|---|
| 1 | `python3` **SIGSEGV** on `import ctypes` | The sandbox's `/usr/local/bin/python3` is broken; edk2 tools segfault in `ansi_handler.py` | Use `/usr/bin/python3`, install the pip requirements there |
| 2 | `DxeCore/DxeCore.inf is not found` | My generated `DXE.inc`/`APRIORI.inc` used **bare names**; EDK2 needs **package-relative paths** | Regenerate from garnet's proven list, remapping vendor names |
| 3 | `ruan/RawFiles/uefiplat.cfg is not found` | `RAW.inc` paths need the `Binaries/` prefix | `SECTION RAW = Binaries/ruan/RawFiles/...` |
| 4 | `Library/ConfigurationMapLib.h not found` | My INFs declared the wrong `[Packages]` | `MemoryMapLib` → ArmPkg/MdePkg/SiliciumPkg; `ConfigurationMapLib` → MdePkg/QcomPkg |
| 5 | `use of undeclared identifier 'AddDynamicMem'` | **ruan's firmware uses a HobOption this Mu-Silicium doesn't have** | Map to `AddMem` |
| 6 | `the 11th and 12th file have the same GUID: cf6dfc66` (TzDxeLA) | I listed `TzDxeLA` **twice** — APRIORI *and* DXE.inc | Dedupe: APRIORI defines order, DXE.inc must not repeat it |
| 7 | `the 4th and 72nd file have the same GUID: d93ce3d8` | **ruan's `RscRtDxe` IS EDK2's `ReportStatusCodeRouterRuntimeDxe`** — same GUID, Qualcomm's build | Drop the Qualcomm copy; let EDK2's win |

Failure 7 is the most interesting: I assumed every one of the 92 extracted drivers was
ruan-specific. It is not. `RscRtDxe` (`d93ce3d8`) and `SCHandlerRtDxe` (`6c2004ef`) are
Qualcomm's builds of EDK2's status-code modules, and their GUIDs match EDK2's source exactly.
Including both is a collision, so the Qualcomm copies had to go.

Also worth noting: failure 5 shows ruan's firmware is **newer than the Mu-Silicium tree** — its
`uefiplat.cfg` uses a memory-attribute enum (`AddDynamicMem`) that this version of SiliciumPkg
doesn't define yet.

## What is and is not proven

**Proven:** the package is structurally valid; it compiles; every module links; the FV generates;
image validation passes on all 49 PE images; the output image is well-formed and carries ruan's
real device tree.

**NOT proven:** that it **boots**. Nothing has run on hardware, and no emulator run is possible
for a phone-targeted UEFI. The build being green says the *description* of ruan is self-consistent
— it says nothing about whether the memory map, the pin tables, or the `DALTLMM` → `ButtonsDxe`
dependency chain behave correctly at runtime.

That distinction matters: a successful build is a strong filter, not a verdict.

## To try it on the tablet

```sh
# bootloader must be unlocked; this writes nothing to the tablet's storage
fastboot boot Mu-ruan.img
```

`fastboot boot` loads into RAM only — a reboot returns to Android. What to look for, in order:

1. the **BootShim / UEFI screen** (the `logo_m_lock.bmp` splash, or a menu),
2. the **display resolution the UEFI reports** — this is the `fb_base` value I still need,
3. **volume/power buttons** responding in the UEFI menu,
4. **USB** enumerating on the host (`lsusb` / `fastboot devices`).

If 1–2 appear but 3–4 don't, the `DALTLMM`/`PmicDxe` chain is the place to look — which is exactly
what `RUAN_UEFI_DRIVERS.md` predicts, and it is now testable rather than theoretical.

## Reproducing the build

```
/workspace/build/Mu-Silicium/            Mu-Silicium + submodules (3.3 GB)
  Platforms/Xiaomi/ruanPkg/              the package (synced to ruan-port/ruanPkg)
  Binaries/ruan/QcomPkg/Drivers/         92 drivers from ruan's firmware
  Binaries/ruan/RawFiles/                28 raw files
  Resources/Configs/ruan.toml            FD base 0xA7000000, size 0x400000
  Resources/DTBs/ruan.dtb                ruan's "Parrot 4Gb SoC" tree

/usr/bin/python3 build_uefi.py -d ruan -r RELEASE
```

Two environment notes for anyone repeating this: use `/usr/bin/python3` (the other interpreter
segfaults), and apply the `mu_nasm` workaround from HackiOS's `uefi/README.md` — symlink
`/usr/bin/nasm` and `/usr/bin/ndisasm` into
`Mu_Basecore/BaseTools/Bin/mu_nasm_extdep/Linux-x86-64/` with a stub `extdep_state.yaml`, which
avoids needing Mono.
