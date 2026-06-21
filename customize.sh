#!/system/bin/sh
# Zygisk Performance Suite - Customize Script
# This script runs during module installation to set up permissions and initial config

BASE_PATH=$MODPATH
ZYGISK_PATH="$BASE_PATH/zygisk"
CONFIG_PATH="$BASE_PATH/config"

echo "[*] Installing Zygisk Performance Suite..."

# Ensure zygisk directory exists
mkdir -p "$ZYGISK_PATH"

# Ensure config directory exists
mkdir -p "$CONFIG_PATH/apps"

# Set proper permissions
chmod 0755 "$ZYGISK_PATH"
chmod 0755 "$CONFIG_PATH"
chmod 0755 "$CONFIG_PATH/apps"
chmod 0644 "$CONFIG_PATH/global.conf" 2>/dev/null || true
chmod 0644 "$CONFIG_PATH/apps"/*.conf 2>/dev/null || true

echo "[+] Module installed successfully!"
echo "[+] Config files location: $CONFIG_PATH"
echo "[+] Edit global.conf and app-specific configs in config/apps/"
