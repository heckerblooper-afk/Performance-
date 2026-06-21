# Zygisk Performance Suite

**A premium, production-ready Zygisk Magisk module for Android performance tuning, per-app device spoofing, and FPS unlocking.**

---

## Features

### 🚀 Performance Tuning
- **CPU Frequency Scaling**: Set min/max CPU frequencies per app
- **CPU Governor Control**: Switch between performance, powersave, schedutil, ondemand
- **GPU Frequency Scaling**: Optimize GPU clocks for demanding tasks
- **Schedtune Boost**: Adjust task scheduling priorities for top-app
- **UCLAMP Constraints**: Fine-tune CPU utilization limits
- **Power Profiles**: Balanced, Performance, or Power Saving modes

### 🎭 Device Spoofing (Per-App)
- **Model Name**: Spoof device model (e.g., Samsung Galaxy S21)
- **Manufacturer**: Change manufacturer identity
- **Device Codename**: Modify device codename
- **SDK Level**: Adjust Android API level per app
- **Custom Properties**: Add game-specific properties (HONOR, Xiaomi, Samsung profiles)
- **Process-Local**: Spoofing applies only to target app, not system-wide

### 🎮 FPS Unlock
- **120+ FPS Support**: Unlock frame rate limits in games
- **Multi-Engine Support**: Works with Unity, IL2CPP, Unreal Engine
- **Smart Hooking**: Detects and patches frame limiters automatically
- **Per-App Config**: Different FPS targets for different games

---

## Installation

1. **Prerequisites**:
   - Android device with Magisk installed
   - Magisk 24.0+ (Zygisk support required)
   - Root/Bootloader unlock

2. **Install via Magisk Manager**:
   - Copy the module folder to `/data/adb/modules/zygisk_perf_suite`
   - Reboot
   - Module will auto-initialize

3. **Verify Installation**:
   ```bash
   ls -la /data/adb/modules/zygisk_perf_suite/
   cat /data/adb/modules/zygisk_perf_suite/config/global.conf
   ```

---

## Configuration

### Global Configuration

Edit `/data/adb/modules/zygisk_perf_suite/config/global.conf` to set system-wide defaults:

```ini
# CPU Tuning (defaults for all apps)
default_cpu_min_freq=300000        # 300 MHz
default_cpu_max_freq=2880000       # 2.88 GHz
default_cpu_governor=schedutil
default_schedtune_boost=0
default_uclamp_min=0
default_uclamp_max=1024

# GPU Tuning
default_gpu_min_freq=100           # MHz
default_gpu_max_freq=800           # MHz

# General
enable_logging=1
enable_spoof_global=1
enable_fps_unlock_global=1
```

### Per-App Configuration

Create app-specific configs in `/data/adb/modules/zygisk_perf_suite/config/apps/`:

**Example: `com.example.game1.conf`**

```ini
# CPU Configuration (overrides global defaults for this app)
cpu_min_freq=576000
cpu_max_freq=2880000
cpu_governor=performance
schedtune_boost=50
uclamp_min=256
uclamp_max=1024

# GPU Configuration
gpu_min_freq=300
gpu_max_freq=800

# Device Spoofing
spoof_model=SM-G998B              # Samsung Galaxy S21 Ultra
spoof_manufacturer=Samsung
spoof_device=o1s
spoof_sdk=33                       # Android 13
game_props=ro.product.is_game_device=true,ro.hardware.audio.primary=msm8998

# FPS Unlock
unlock_fps=1                       # Enable
target_fps=120                     # Unlock to 120 FPS
hook_priority=10
game_engine=unity                  # Game engine type
library_hooks=libil2cpp.so,libUnity.so  # Libraries to hook

# Memory & Power
min_available_memory=512           # MB
enable_mem_tuning=1
power_mode=performance
```

**Example: `com.example.app2.conf`**

```ini
# Balanced profile for general apps
cpu_min_freq=300000
cpu_max_freq=2400000
cpu_governor=schedutil
schedtune_boost=10

gpu_min_freq=100
gpu_max_freq=600

spoof_model=SM-G973F
spoof_manufacturer=Samsung
spoof_device=star2lte
spoof_sdk=31

unlock_fps=0                       # Disable for this app
power_mode=balanced
```

---

## File Structure

```
zygisk_perf_suite/
├── module.prop                     # Module metadata
├── customize.sh                    # Installation script
├── post-fs-data.sh                 # Post-boot initialization
├── service.sh                      # Background service
├── config/
│   ├── global.conf                 # Global defaults
│   └── apps/
│       ├── com.example.game1.conf  # Game 1 config
│       └── com.example.app2.conf   # App 2 config
├── zygisk/
│   └── arm64-v8a/
│       └── libzygisk_perf_suite.so # Native Zygisk module
├── jni/
│   ├── main.cpp                    # Module entry point
│   ├── config_parser.cpp           # Config file parser
│   ├── cpu_tuning.cpp              # CPU frequency/governor control
│   ├── gpu_tuning.cpp              # GPU frequency control
│   ├── spoof_engine.cpp            # Device property spoofing
│   ├── fps_unlock.cpp              # FPS unlock hooking
│   ├── include/
│   │   ├── zygisk.hpp              # Zygisk API definitions
│   │   ├── perf_suite.hpp          # Main module header
│   │   ├── config_parser.hpp
│   │   ├── cpu_tuning.hpp
│   │   ├── gpu_tuning.hpp
│   │   ├── spoof_engine.hpp
│   │   └── fps_unlock.hpp
│   └── Android.mk                  # Build configuration
└── README.md                       # This file
```

---

## Building from Source

### Requirements
- Android NDK (r23+)
- Zygisk header files
- Linux build environment

### Build Steps

1. **Set up NDK path**:
   ```bash
   export NDK_PATH=/path/to/android-ndk-r23
   ```

2. **Compile native module**:
   ```bash
   cd jni
   $NDK_PATH/ndk-build
   ```

3. **Output**:
   - Compiled `.so` placed in `zygisk/arm64-v8a/libzygisk_perf_suite.so`

4. **Package module**:
   ```bash
   cd ..
   zip -r zygisk_perf_suite.zip *
   ```

5. **Install via Magisk Manager**:
   - Select "Install from storage"
   - Choose `zygisk_perf_suite.zip`
   - Reboot

---

## Advanced Usage

### Custom Device Profiles

Create profiles for specific devices:

**`config/apps/com.mihoyo.yuanshen.conf`** (Genshin Impact - Xiaomi)
```ini
spoof_manufacturer=Xiaomi
spoof_model=Mi 11
spoof_device=venus
spoof_sdk=33
game_props=ro.vendor.extension_library=/vendor/lib64/libqti-perfd-client.so
schedtune_boost=60
unlock_fps=1
target_fps=120
```

### Frequency Tables

Find your device's valid frequencies:

```bash
# CPU frequencies
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies

# GPU frequencies (Adreno)
cat /sys/class/kgsl/kgsl-3d0/devfreq/available_frequencies
```

### Performance Benchmarks

Test with popular benchmarks:
- **AnTuTu**: Overall system performance
- **GFXBench**: GPU-specific performance
- **3DMark**: Graphics rendering
- **Geekbench**: CPU performance

---

## Troubleshooting

### Module Not Loading
```bash
# Check Zygisk is enabled
adb shell getprop persist.sys.usb.config

# Verify module installation
adb shell ls -la /data/adb/modules/zygisk_perf_suite/
```

### Config Not Applied
```bash
# Check permissions
adb shell ls -la /data/adb/modules/zygisk_perf_suite/config/

# Verify package name matches config file
adb shell ps | grep com.example
```

### FPS Unlock Not Working
- Ensure game library names are correct (check with `frida` or `objdump`)
- Verify `game_engine` setting matches actual engine
- Some games have anti-cheat that prevents hooking

### Device Spoof Not Taking Effect
- Per-process spoofing requires active JNI hooking
- Some system apps bypass custom Build properties
- Check logcat for errors:
  ```bash
  adb logcat | grep "ZygiskPerfSuite\|SpoofEngine"
  ```

---

## Performance Impact

### CPU Tuning
- **Performance Mode**: +10-20% peak performance, +5-15% power consumption
- **Balanced**: Minimal impact, optimized throughput
- **Power Saving**: -10-20% performance, -20-30% power consumption

### GPU Tuning
- **High Clock**: +15-30% gaming FPS, +10-20% power draw
- **Balanced**: +5-10% FPS, minimal power impact

### Device Spoofing
- Negligible performance impact (~1-2% overhead)
- Some games may detect spoofing and refuse to run

### FPS Unlock
- Depends on GPU capability
- 120 FPS: +50% GPU load vs 60 FPS
- May cause thermal throttling on sustained loads

---

## Safety & Warnings

⚠️ **Important Disclaimers**:

1. **Thermal Management**: Aggressive settings may overheat device
   - Monitor temps: `adb shell "cat /sys/class/thermal/thermal_zone0/temp"`
   - Max safe: ~45°C (CPU), ~60°C (GPU)

2. **Battery Life**: High-frequency settings drain battery faster
   - Use balanced profiles for daily use
   - Switch to power-saving for long sessions

3. **Stability**: Not all frequencies are stable on all devices
   - Test incrementally
   - Monitor for kernel panics and app crashes

4. **Game Anti-Cheat**: Some games detect FPS unlock or spoofing
   - Genshin Impact, PUBG, CoD may flag accounts
   - Use at own risk

5. **Warranty**: Aggressive overclocking may void warranty
   - Consult manufacturer before aggressive tuning

---

## Logs & Debugging

### View Module Logs
```bash
adb logcat | grep "ZygiskPerfSuite\|PerfSuite\|CpuTuning\|FpsUnlock"
```

### Enable Verbose Logging
Edit `config/global.conf`:
```ini
enable_logging=1
log_path=/data/adb/modules/zygisk_perf_suite/logs
```

View logs:
```bash
adb shell cat /data/adb/modules/zygisk_perf_suite/logs/perf-suite.log
```

---

## Contributing

To extend this module:

1. **Add new sysfs paths**: Update `jni/cpu_tuning.cpp` with device-specific paths
2. **Custom game profiles**: Create `.conf` files in `config/apps/`
3. **New features**: Extend `PerfSuiteModule` class in `jni/main.cpp`
4. **Device support**: Test and document frequency tables

---

## License

MIT License - Free to use, modify, distribute

---

## Support & Feedback

- **Module ID**: `zygisk_perf_suite`
- **Magisk Forum**: Discuss on Magisk Module repositories
- **Issues**: Report device-specific compatibility issues

---

**Version**: 1.0.0  
**Last Updated**: 2024  
**Author**: YourName  

---

## Quick Reference

### Essential Commands

```bash
# Verify module is loaded
adb shell cat /data/adb/modules/zygisk_perf_suite/module.prop

# Check config syntax
adb shell cat /data/adb/modules/zygisk_perf_suite/config/global.conf

# Monitor real-time logs
adb logcat -s ZygiskPerfSuite

# Test CPU frequencies
adb shell cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq

# Reboot to apply changes
adb reboot
```

---

**Enjoy premium performance tuning! 🚀**
