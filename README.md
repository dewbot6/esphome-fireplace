# ESPHome Fireplace Controller

ESPHome configurations for controlling fireplaces using ESP32-C6 devices. Supports two types of fireplace control:

- **Maxitrol Valve Control** (`esphome-fireplace-maxitrol.yaml`) - Dimmer-like control using a DRV8830 motor driver
- **Millivolt Fireplace Control** (`esphome-fireplace-millivolt.yaml`) - Simple on/off control with auto-off timer

## Features

### Maxitrol Configuration
- ✅ Dimmer-like brightness control (0-100%)
- ✅ Apple HomeKit and Home Assistant integration
- ✅ Configurable motor power and travel time
- ✅ Non-blocking motor control (prevents watchdog timer issues)
- ✅ Position memory across reboots

### Millivolt Configuration
- ✅ Simple on/off switch control
- ✅ Configurable auto-off timer (0.5-12 hours)
- ✅ Safety-first: Always starts in OFF state
- ✅ Apple HomeKit and Home Assistant integration

## Hardware Requirements

### Common Requirements
- ESP32-C6 development board (tested with ESP32-C6-DevKitC-1)
- USB cable for programming
- Power supply (USB or external)

### Maxitrol Configuration Additional Requirements
- DRV8830 I2C motor driver board
- Maxitrol fireplace valve with motor
- I2C connections (SDA/SCL)
- Appropriate power supply for motor driver

### Millivolt Configuration Additional Requirements
- Relay module (compatible with ESP32 GPIO)
- Millivolt fireplace (standard on/off type)
- Appropriate wiring for relay control

## Installation

### 1. Install ESPHome

Follow the [ESPHome installation guide](https://esphome.io/guides/getting_started_hassio.html) for your platform.

### 2. Clone This Repository

```bash
git clone https://github.com/yourusername/esphome-fireplace.git
cd esphome-fireplace
```

### 3. Configure Secrets

Create a `secrets.yaml` file in the project root (see `secrets.yaml.example` for reference):

```yaml
# For OpenThread/Thread network
thread_tlv: "your_thread_tlv_data_here"

# OR for WiFi (if not using OpenThread)
wifi_ssid: "YourWiFiSSID"
wifi_password: "YourWiFiPassword"
```

**Note:** If using OpenThread, you'll need to generate Thread credentials. If using WiFi, uncomment the WiFi section in your chosen YAML file and comment out the OpenThread section.

### 4. Customize Configuration

Choose the appropriate configuration file for your fireplace type:

- **Maxitrol valve**: Edit `esphome-fireplace-maxitrol.yaml`
- **Millivolt fireplace**: Edit `esphome-fireplace-millivolt.yaml`

#### Key Customizations:

**Device Name:**
```yaml
esphome:
  name: your-fireplace-name  # Change this
```

**GPIO Pins (if different from defaults):**
- Maxitrol: I2C pins (SDA/SCL) - default GPIO2/GPIO1
- Millivolt: Relay GPIO pin - default GPIO2

**I2C Address (Maxitrol only):**
```yaml
drv8830:
  address: 0x60  # Change if using different address
```

### 5. Compile and Upload

Using ESPHome Dashboard:
1. Open ESPHome Dashboard
2. Click "New Device" → "Continue"
3. Select "Edit" on your device
4. Copy/paste the contents of your chosen YAML file
5. Click "Install" → "Plug into this computer"
6. Select your ESP32-C6 board and click "Install"

Using ESPHome CLI:
```bash
esphome compile esphome-fireplace-maxitrol.yaml
esphome upload esphome-fireplace-maxitrol.yaml
```

## Configuration Guide

### Maxitrol Configuration

The Maxitrol configuration creates a dimmer-like light entity that controls valve position:

- **Brightness 0%** = Valve fully closed
- **Brightness 100%** = Valve fully open

#### Adjustable Settings (in Home Assistant):

1. **Fireplace Motor Power** (10-63)
   - Controls motor speed/power
   - Higher = faster movement
   - Default: 60
   - Start with default and adjust based on valve response

2. **Fireplace Travel Time** (5-20 seconds)
   - Time for valve to travel from 0% to 100%
   - Used to calculate partial movements
   - Default: 10.5 seconds
   - Measure your valve's full travel time and set accordingly

#### How It Works:

1. When brightness changes, the system calculates the difference
2. Motor runs in the appropriate direction for the calculated time
3. Changes smaller than 1% are ignored to prevent jitter
4. Motor control uses non-blocking scripts to prevent watchdog timer issues

### Millivolt Configuration

The Millivolt configuration creates a simple on/off switch:

- **ON** = Fireplace turns on
- **OFF** = Fireplace turns off

#### Adjustable Settings (in Home Assistant):

1. **Fireplace Auto-Off Timer** (0.5-12 hours)
   - Automatically turns off fireplace after set time
   - Timer restarts if fireplace is turned on again
   - Default: 6 hours
   - Adjustable in 0.5 hour increments

#### Safety Features:

- Always starts in OFF state (even after power loss)
- Auto-off timer prevents accidental extended operation
- Timer can be adjusted or disabled by turning fireplace off manually

## Network Configuration

### OpenThread (Default)

The configurations default to OpenThread for Thread/Matter support. You'll need:

1. A Thread border router (e.g., Home Assistant with Thread support, Apple HomePod, etc.)
2. Thread credentials (TLV format) in `secrets.yaml`

### WiFi (Alternative)

To use WiFi instead:

1. Uncomment the WiFi section in your YAML file
2. Comment out the OpenThread section
3. Add WiFi credentials to `secrets.yaml`

```yaml
# In your YAML file, change:
# openthread:
#   ...

# To:
wifi:
  ssid: !secret wifi_ssid
  password: !secret wifi_password
```

## Integration with Home Assistant

1. **Automatic Discovery**: If on the same network, ESPHome devices are auto-discovered
2. **Manual Add**: Go to Settings → Devices & Services → Add Integration → ESPHome
3. **Enter Device IP**: Or use the device name if mDNS is working

## Integration with Apple HomeKit

1. **Via Home Assistant**: Use Home Assistant's HomeKit Bridge integration
2. **Direct**: ESPHome devices with OpenThread can work with Apple HomePod/Thread border routers

## Troubleshooting

### Device Not Found

- **Check network**: Ensure device and controller are on same network
- **Check logs**: Use `esphome logs <device-name>` to see connection status
- **Web interface**: Access `http://<device-ip>` to verify device is running

### Maxitrol: Motor Not Moving

- **Check I2C connection**: Verify SDA/SCL connections
- **Check I2C address**: Review logs for I2C scan results, verify address matches config
- **Check motor power setting**: Try increasing motor power in Home Assistant
- **Check wiring**: Verify motor driver connections

### Maxitrol: Watchdog Timer Errors

- **Already fixed**: The configuration uses non-blocking scripts
- **If still occurring**: Check that script mode is set to `restart` (should be in config)

### Millivolt: Relay Not Switching

- **Check GPIO pin**: Verify relay is connected to correct GPIO (default GPIO2)
- **Check relay type**: Verify if relay is active-high or active-low (adjust `inverted` setting)
- **Check wiring**: Verify relay connections and power supply

### Logs Show Errors

- **Enable DEBUG logging**: Change `level: INFO` to `level: DEBUG` in logger section
- **Check serial output**: Connect via USB and monitor serial output
- **Review ESPHome logs**: Use `esphome logs <device-name>`

### OpenThread Not Connecting

- **Verify Thread credentials**: Check `thread_tlv` in secrets.yaml
- **Check border router**: Ensure Thread border router is active
- **Try WiFi**: Switch to WiFi configuration as fallback

## Advanced Configuration

### Changing GPIO Pins

If you need to use different GPIO pins:

**Maxitrol (I2C):**
```yaml
i2c:
  sda: GPIO2  # Change to your SDA pin
  scl: GPIO1  # Change to your SCL pin
```

**Millivolt (Relay):**
```yaml
switch:
  - platform: gpio
    pin:
      number: 2  # Change to your relay GPIO pin
```

### Adjusting Motor Behavior (Maxitrol)

Fine-tune motor response by adjusting these values in the lambda:

- **Change threshold**: Modify `1.0f` in `if (fabs(delta) < 1.0f)` to change minimum movement threshold
- **Power limits**: Adjust `min_value` and `max_value` in motor_power number entity

### Disabling Auto-Off Timer (Millivolt)

To disable the auto-off timer, simply turn off the fireplace manually before the timer expires. The timer will not turn it back on.

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is open source and available under the MIT License.

## Acknowledgments

- [ESPHome](https://esphome.io/) - Amazing platform for ESP32 devices
- [dewbot6/esphome-drv8830](https://github.com/dewbot6/esphome-drv8830) - DRV8830 component for Maxitrol control

## Support

For issues and questions:
1. Check the [Troubleshooting](#troubleshooting) section
2. Review ESPHome [documentation](https://esphome.io/)
3. Open an issue on GitHub

---

**Safety Warning**: Always ensure proper wiring and follow electrical safety guidelines. Fireplace control involves high voltages and gas systems - use caution and consult professionals if unsure.
