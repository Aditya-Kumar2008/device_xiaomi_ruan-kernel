# Does XNU boot on `ruan`? — verification, 2026-10-23

## Answer: the report is credible, but it is **not a boot yet**

I searched every public channel I could reach. Here is exactly what the evidence does and does not
support.

## What I could verify

| Source | Finding |
|---|---|
| `oosexgang/*` repos | **Nothing new.** Only branch is `main`; last commits 2026-10-22 (the release + LICENSE). No `ruan`, no `parrot`, no SM7435 branch or commit. |
| `oosexgang` org events | Only `WatchEvent` / `ForkEvent` since the release. No `PushEvent` at all — i.e. no code pushed anywhere in the org since 10-22. |
| OOSEX GANG Telegram (`t.me/s/oosexgangdevs`) | Zero mentions of `ruan`, `parrot`, `dizi`, `montague`, `SM7435`, "Pad Pro". The only device-specific post is **"We successfully launched iOS on Tensor"** (2026-10-21, i.e. Google Tensor, not a Redmi Pad). |
| UniXNU Telegram (`t.me/s/unixnu`) | Zero mentions of `ruan`/`parrot`/SM7435. |
| `unixnu.com/devices` (the full supported + community-port table, ~50 boards) | **No `ruan`.** No SM7435 board at all. Closest family entries are SM6xxx/SM8xxx. `ruan` appears 0 times. |
| UniXNU org repos / issues / PRs | 0 results for `ruan`, `parrot`, `sm7435`, `montague`, `dizi`. |
| GitHub-wide repo search | No XNU/iOS/iPadOS repo mentioning `ruan` exists. |
| `hackiosx.dev` (a *different* project, `ios-sweet`) | Compatibility is by chip family; no ruan entry. |
| Mu-Silicium | No `ruan`/`dizi`/`parrot` platform package. Only `garnetPkg` (SM7435, different board) exists. |

**Conclusion:** no published repo, commit, build, or official post backs "XNU booted on ruan
yesterday". If someone did it, the work is not public yet.

## What the actual chat says

The exchange you pasted is the only real evidence, and read carefully it says the opposite of a boot:

> **VildanG:** Just ported xnu from sweet — but i will switch to unixnu — and i try to patch some
> fixes from HackiOS
> **VildanG:** Maybe yes but i still fixing mu silicium — **Buttons and usb is not working**

Two facts fall out of that:

1. **"Ported XNU from `sweet`"** = he took the *Redmi Note 10 Pro* (SM7150/Snapdragon 732G) port and
   started retargeting it. That is a **different SoC generation** from `ruan` (SM7435). Retargeting
   `sweet`'s platform code to `parrot` is not a small edit — GIC, EPSS, GPU (a6xx→a7xx), Wi-Fi and
   the whole peripheral map differ. So this is *the first commit of a port*, not a finished boot.
2. **"Maybe yes"** is his own uncertainty about whether UniXNU has booted on ruan — he does not
   assert it. And he is **blocked in the UEFI (Mu-Silicium) stage**, which is *before* XNU ever
   runs.

## The real blocker, and why it is exactly what my analysis predicted

His symptom — **"buttons and usb is not working"** in the Mu-Silicium stage — is the single most
common first failure when adapting a Mu-Silicium package to a *different board on the same SoC*.
I pulled `garnetPkg`'s actual driver list, and this is what the UEFI needs for exactly those two
things:

**Buttons** come from `Binaries/<board>/QcomPkg/Drivers/ButtonsDxe/ButtonsDxe.inf`.
**USB** comes from a *chain*, not one driver:
```
UsbConfigDxe, UsbInitDxe, UsbDeviceDxe, UsbMsdDxe, UsbPwrCtrlDxe,
UsbfnDwc3Dxe, UCDxe, XhciDxe, XhciPciEmulationDxe
```
plus the generic `UsbBusDxe` / `UsbKbDxe` / `UsbMassStorageDxe`, and the PMIC path
(`PmicDxe`, `PmicGlinkDxe`, `ChargerExDxe`, `QcomChargerDxe`, `GpioDxe`, `SPMIDxe`, `TLMMDxe`).

Note also how those vendor `.efi` drivers are delivered: `Binaries/` in Mu-Silicium is a **git
submodule** pointing at `Project-Silicium/Device-Binaries`. That repo has **`garnet`** (verified,
HTTP 200) — and **no `ruan`, `parrot` or `dizi`** (404). So garnet's vendor drivers are public and
usable as a reference, but a `ruan` port must supply its **own**, extracted with **UEFIReader from
`ruan`'s own `xbl.img`**. That is almost certainly why he has no buttons and no USB: either the
vendor drivers are missing, or the GPIO/PMIC/pinctrl tables in them belong to a different board.

The practical consequence is a clean control experiment: build with **`Device-Binaries/garnet`**
first. If buttons and USB work there on ruan, the problem is in the board tables; if they do not,
the problem is deeper (TLMM/PMIC path). Then swap in `ruan`'s own extracted drivers.

### What he should check, in order

1. **Are the vendor drivers actually present and from *this* device?**
   `Binaries/ruan/...` must be extracted with **UEFIReader** from **`ruan`'s own `xbl.img`**, not
   copied from garnet. A board's `ButtonsDxe`/`PmicDxe` encode its GPIO and PMIC numbering.
2. **Is `ButtonsDxe` in `DXE.inc` and resolving?** An `.inf` listed but with missing binaries is
   silently skipped — this is the "a skip that is not counted" trap from HackiOS's own
   `docs/checks.md`.
3. **Is the TLMM/GPIO pin map right?** Buttons and USB-present detection both route through TLMM
   (at `0x0F000000` on this SoC family) and the PMIC. A tablet has no volume rocker in the same
   place as a phone; `ruan`'s pin table differs from garnet's.
4. **USB role / power:** `UsbPwrCtrlDxe` + `QcomChargerDxe` decide host vs device mode. Mu-Silicium's
   own status table lists **USB Host Mode ❌ and USB Power Delivery ❌** even on garnet — so some USB
   limits are upstream, not his fault.
5. **Check the same two things on garnet hardware first.** If garnet has working buttons+USB and
   ruan does not, the delta is the vendor binaries and the pin/PMIC tables. That is the experiment
   that isolates the cause.

### The one line worth telling him

His stated plan — *"i try to patch some fixes from HackiOS"* — is sound, but he is patching the
**kernel** while his failure is in the **UEFI**. HackiOS's `docs/PORTING.md` says this outright:
*"The first useful milestone is the loader's own screen and log over USB (`./xl log`), the next one
is the kernel's verbose boot text (`-v`), then SpringBoard."* Buttons and USB come **before** the
loader in that chain, so they are UEFI work, not XNU work. Fixing XNU will not make the buttons
work.

## Where this leaves your port

It is **not** a race you have lost — the opposite. Nobody has published a working `ruan` port:

- Mu-Silicium: no ruan/parrot package (only the same-SoC `garnetPkg` to adapt — the enabler I
  identified).
- UniXNU: no SM7435 board, but a clean **device-repository model** (`UniXNU-<device>` +
  `uefi-<device>` + `board.toml`) that is far more portable than forking HackiOS, and a working
  **iPadOS 17 + Metal on Xiaomi Pad 6S Pro** reference to copy from.
- HackiOS: the deepest kernel-side work (a7xx-adjacent, APFS, iOS 16.5 userland), but hard-coded to
  `redwood`.

Practical read: **UniXNU is now the better base for a `ruan` port** (its whole design is "boards with
a similar SoC need their own port and their own entry"), while HackiOS remains the best reference
for the kernel-side details. VildanG's instinct to switch to UniXNU matches that.

## What would settle it

- `ruan`'s **`xbl.img`** (for the vendor UEFI drivers) and **`vendor_boot.img`** (for the DTB).
- The **merged device tree** from the tablet: `adb pull /sys/firmware/fdt fdt.dtb`.
- Whether **garnet's buttons/USB work** on garnet hardware — the control that isolates his bug.
- If VildanG has published anything, a repo/branch URL. I found none; if one exists it is private.
