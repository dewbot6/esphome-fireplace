# Installing the DRV8830 Component

## From GitHub

Add this to your ESPHome YAML configuration:

```yaml
external_components:
  - source: github://andrewsnodgrass/esphome-fireplace/components/drv8830
    version: main  # or a specific tag/commit
```

## Local Development

If you're developing locally, you can reference it directly:

```yaml
external_components:
  - source: ./components/drv8830
```

## Usage Example

```yaml
i2c:
  sda: GPIO2
  scl: GPIO1
  scan: true

drv8830:
  id: motor_driver
  address: 0x60  # Optional, defaults to 0x60

switch:
  - platform: template
    name: "Motor Forward"
    turn_on_action:
      - lambda: |-
          id(motor_driver)->set_motor(32, id(motor_driver)->FORWARD);
      - delay: 1s
      - lambda: |-
          id(motor_driver)->set_motor(0, id(motor_driver)->RELEASE);
```

See README.md for more details.
