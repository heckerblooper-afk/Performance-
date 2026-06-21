#!/system/bin/sh
# Zygisk Performance Suite - Post-FS Data Script
# Runs after /data is mounted; initialize configs and logging if needed

MODPATH=${0%/*}
LOG_PATH="$MODPATH/logs"
CONFIG_PATH="$MODPATH/config"

# Create log directory
mkdir -p "$LOG_PATH"
chmod 0755 "$LOG_PATH"

# Initialize global config if not exists
if [ ! -f "$CONFIG_PATH/global.conf" ]; then
    cat > "$CONFIG_PATH/global.conf" << 'EOF'
# Zygisk Performance Suite - Global Configuration
# Default settings applied to all apps unless overridden

# CPU Frequency Scaling (kHz)
default_cpu_min_freq=300000
default_cpu_max_freq=2880000
default_cpu_governor=schedutil

# GPU Frequency Scaling (MHz) - Adjust per device
default_gpu_min_freq=100
default_gpu_max_freq=800

# Scheduler tuning
default_schedtune_boost=0
default_uclamp_min=0
default_uclamp_max=1024

# General settings
enable_logging=1
log_path=/data/adb/modules/zygisk_perf_suite/logs
EOF
    chmod 0644 "$CONFIG_PATH/global.conf"
fi

echo "[$(date)]" >> "$LOG_PATH/perf-suite.log"
echo "Post-FS initialization complete" >> "$LOG_PATH/perf-suite.log"
