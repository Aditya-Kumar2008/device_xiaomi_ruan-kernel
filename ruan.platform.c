/*
 * ruan: Redmi Pad Pro 5G (SM7435 / "parrot"), Adreno 710, Novatek NT36532 touch, 1600x2560 BOE panel.
 *
 * Paste this into xnuloader/loader.c next to platform_redwood and add &platform_ruan to platforms[].
 *
 * PROVENANCE: every value below is read from ruan's OWN firmware -- the OTA
 * ruan_in_global-ota_full-OS3.0.305.0.WFSINXM (Android 16), vendor_boot DTB "Parrot 4Gb SoC",
 * board-id <0x00 0x600>. Not from a sibling device. See RUAN_VERIFIED_HARDWARE.md.
 *
 * gic_dist / gic_redist -- arm,gic-v3: reg = <0x17200000 0x10000>, <0x17260000 0x100000>
 * cores / core_shift    -- cpu@0,100,200,300,400,500,600,700; Qualcomm MPIDR affinity-2 -> shift 8
 * small_cores           -- cpu@0..300 capacity-dmips-mhz = 0x400 (A55); cpu@400..700 = 0x799 (A78)
 * psci_hvc              -- 0: SMC
 */
static const struct platform platform_ruan = {
        .name = "ruan",
        .io_base = 0x00400000, .io_size = 0x3FC00000,   /* TODO: from the ruanPkg memory map; must be non-zero */
        .gic_dist = 0x17200000, .gic_redist = 0x17260000,
        .uart = 0,                                      /* no PL011 */
        .log_other_cpu = 1,
        .cores = 8, .core_shift = 8,
        .small_cores = 0x0f,                            /* cpu@0..300, the A55 cluster */
        .fb_base = 0x0,                                 /* TODO: the UEFI's "display ... at" address (one run) */
        .psci_hvc = 0,                                  /* SMC */
};

/*
 * CONFIRMED device values, for the drivers that follow. (RUAN_VERIFIED_HARDWARE.md has sources.)
 *
 *   GPU        bsd/dev/redwood_adreno.c
 *              qcom,kgsl-3d0@3d00000, compatible "qcom,adreno-gpu-gen7-3-0", qcom,gpu-model
 *              "Adreno710v1"  -> a7xx, NOT redwood's a6xx (Adreno 642L). GMU at qcom,gmu@3d69000.
 *              GPU clock controller at 0x3d90000 (qcom,parrot-gpucc).
 *              The 22-step bring-up, the a660_* firmware names and every a6xx register offset
 *              must be redone against the a7xx GMU/HFI. Biggest single job.
 *
 *   CPU freq   bsd/dev/redwood_cpufreq.c
 *              qcom,cpufreq-hw-epss: 0x17d91000 (domain0) and 0x17d92000 (domain1) -- TWO domains
 *              (redwood: three, at 0x18591000).
 *
 *   Touch      bsd/dev/redwood_goodix.c  -> REPLACE ENTIRELY
 *              ruan is Novatek: compatible "novatek,NVT-ts-spi", irq-gpio 54,
 *              spi-max-frequency 9.6 MHz, MP criteria 5983 and 5986, pen support.
 *              NOT Goodix GT9916 and NOT FocalTech. Feed /dev/redwood-touch
 *              (bsd/dev/redwood_input.c) unchanged -- that contract still holds.
 *              START FROM UniXNU/NativeNovatekTouch ("NT36532E", Apache-2.0): NvtProtocol.c is an
 *              abstract-SPI NT36532 host-download implementation with no board addresses in it.
 *              Re-measure its axis maxima (currently sheng's 30480 x 20320) and NVT_FRAME_HZ.
 *
 *   Panel      iokit/Kernel/MiniIOMobileFramebuffer.cpp
 *              1600 x 2560, dsi_video_mode, CPHY, DSC, 60 Hz, clock 688 MHz.
 *              "xiaomi n83 35 02 0a cphy nt36532 boe" / n83_42_02_0b (BOE NT36532).
 *              iPhone is 1125 x 2436 (0.462) vs this panel's 0.625 -> a letterbox or crop branch
 *              is needed; redwood's 1080x2400 was nearly 1:1 so it never had to.
 *
 *   Wi-Fi      bsd/dev/redwood_wifi.c
 *              bt_wcn6750 -> WCN6750, the SAME radio as redwood. Mostly region/address changes.
 *
 *   Storage    bsd/dev/redwood_ufs*.c
 *              ufshc_mem at 1d84000; UFS 2.2.
 *
 *   Audio      bsd/dev/redwood_speaker.c
 *              FOUR speakers: wsa883x x2 + wsa2-macro, plus si_pa_* (sia81xx) amps.
 *              Codec wcd937x / wcd938x. redwood had one speaker.
 */
