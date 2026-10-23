# POCO logo → black screen: diagnosis and fix

Your result is a **good** one, not a failure. It narrows the problem to a single layer.

## What "POCO logo, then black" tells us

The POCO logo is drawn by **ABL** (Android Bootloader), before our code runs. Seeing it means ABL
parsed our `ANDROID!` image, accepted it, and **jumped to our payload** — so the whole chain up to
that point works:

```
fastboot boot ─► ABL ─► BootShim ─► UEFI (our FD) ─► [nothing visible]
      ✓            ✓         ✓              ✓                    ✗
```

The failure is **inside our UEFI**, and it happens before anything reaches the display. That rules
out a large space: the image format, the BootShim, the FD layout, and the DTB append are all fine.

## The bug I found

I misread the device's memory map. ruan's `uefiplat.cfg` says:

```
0xA7000000, 0x00400000, "UEFI FD"
```

I took that at face value and built the FD at **`0xA7000000`, size `0x400000`**. That was wrong.

Mu-Silicium **carves `0x100000` off the front of that window** for its own region and builds its FD
immediately after. garnet does this too — its `uefiplat.cfg` reads the *same* `0xA7000000/0x400000`,
yet its working `MemoryMapLib.c` splits it:

```
garnet's MemoryMapLib.c (working):
  {"FD Reserved",       0xA7000000, 0x00100000, ...},
  {"UEFI FD",           0xA7100000, 0x00300000, ...},
  {"UEFI FD Reserved",  0xA7400000, 0x00200000, ...},
```

And `garnet.toml` confirms `base = 0xA7100000, size = 0x300000`.

### Why that produces exactly your symptom

`PlatformPeiLib/XblProtocols.c` does this:

```c
LocateMemoryRegionByName ("FD Reserved", &ReservedFd);
for (Current = ReservedFd.Address; Current < ReservedFd.Address + ReservedFd.Length;
     Current += ARM64_INSTRUCTION_LENGTH) {
  if (*(UINT64 *)Current != 0xA9017BFDD10083FF) continue;   // find the HOB-registration function
  ...
  *SchedulerInterfaceAddr = ParseHobFunction (Current, &gEfiSchedulerInterfaceHobGuid);
  *DtbExtensionAddr       = ParseHobFunction (Current, &gEfiDtbExtensionHobGuid);
}
```

PEI **scans that region instruction by instruction** to recover the XBL scheduler and DTB-extension
HOBs. My FD sat at `0xA7000000` — **on top of the region being scanned**. Two consequences: the scan
reads our firmware instead of XBL's code and finds nothing, and PEI has no scheduler interface. The
platform dies in PEI, long before `DisplayDxe` or `SimpleFbDxe` ever run. Black screen.

I also found that my generator had **silently dropped two regions** garnet has, because they are not
in any device's `uefiplat.cfg` — Mu-Silicium adds them by hand:

| Region | Address | Who needs it |
|---|---|---|
| `FD Reserved` | `0xA7000000`, `0x00100000` | `XblProtocols.c` — the scan above |
| `DT BLOB` | `0x80600000`, `0x00040000` | `IortDtDxe` — finds the DT to patch the UFS IORT node |

My generator read only the config file, so both were missing. That was a methodological error: I
treated `uefiplat.cfg` as the complete memory map, when Mu-Silicium's `MemoryMapLib.c` is the
authority and the cfg is only its input.

## The fix

`ruan.toml`:
```toml
[uefi_fd]
base = 0xA7100000     # was 0xA7000000
size = 0x300000       # was 0x400000
```

`MemoryMapLib.c` — the window is now carved, matching garnet:
```c
{"FD Reserved", 0xA7000000, 0x00100000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
{"UEFI FD",     0xA7100000, 0x00300000, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK},
```

Also re-applied: ruan's config says `AddDynamicMem` for two regions, which this Mu-Silicium tree has
no enum for (`MemoryMapLib.h` offers only `NoHob/AddMem/AddDev/HobOnlyNoCacheSetting/AllocOnly/MaxMem`).
Mapped to `AddMem`. **ruan's firmware is newer than this Mu-Silicium tree** — that enum is a real
version gap, not a typo.

## Verified in the rebuilt image

```
Mu-ruan.img        1,361,920 B
  BootShim _StackBase (FD_BASE) = 0xa7100000   ✓
  BootShim _StackSize (FD_SIZE) = 0x300000     ✓
  UEFI FD                      = 0x300000 B    ✓  (was 0x400000)
  FV declared length           = 0x300000      ✓
  _FVH at offset 152 (= 112-byte shim + 40)    ✓
  appended DTB 388,034 B, "Parrot 4Gb SoC"     ✓
```

Build: `PROGRESS - Success`, 49 images verified.

## Try it again

```
https://github.com/Aditya-Kumar2008/device_xiaomi_ruan-kernel/raw/ruan-uefi/build-output/Mu-ruan.img
```

```sh
fastboot boot Mu-ruan.img
```

**What to watch for, and what each outcome means:**

| Outcome | Meaning |
|---|---|
| Splash/menu appears | The FD-base fix worked; PEI survived. Now check buttons and USB. |
| Still black | PEI is still dying. Next suspects, in order: the `DALTLMM` → `ButtonsDxe` chain, or PEI needing something else I dropped. |
| Screen appears but no buttons/USB | Exactly the `DALTLMM`/`PmicDxe` dependency problem `RUAN_UEFI_DRIVERS.md` predicts — and now testable. |
| Different logo, then black | Progress — something else is being drawn. |

## Being straight about this

The fix is well-evidenced: garnet's working config, the PEI source that scans the region, and the
fact that my FD was demonstrably sitting on top of it. But I got it wrong once already, and a
correct diagnosis is not the same as a working boot. If it's still black, the next thing I want is
**any** output — a UART log, a different splash, even a crash — because silence means PEI never
reached the point where it could speak.
