# PSM_Power_Supply_Module
PSM: Power_Supply_Module (Energy V4)

## Repository Initialization

To work with this repository ni your local machine, use the following commands:

```bash
cd your_local_directory
git init
git remote add origin https://github.com/LH-EML-Firmware/PSM-Power_Supply_Module.git
(you might be required to add your repository to "safe" or "trusted" repos and then re do previous command)
git pull origin main
```

## IDE Considerations

This firmware was developed for **PIC16F18446**. In order to compile/build this project you will need
- MPLAB v6.20
- XC8 v2.46
- You will need to go to MCC and hit the "generate" button in order to build this project.

## 📥 Download Latest Ready-to-Program Files

[🔗 Latest PSM HEX](https://github.com/LH-EML-Firmware/PSM-Power_Supply_Module/releases/latest/download/PSM_Power_Supply_Module.X.production.hex)  
[🔗 Latest PSM ELF](https://github.com/LH-EML-Firmware/PSM-Power_Supply_Module/releases/latest/download/PSM_Power_Supply_Module.X.production.elf)

## Features

This embedded system monitors and controls a battery-backed solar-powered system via Modbus RTU. It reads voltage and current from:
- Solar panel
- Battery
- Consumption/load

It also:
- Controls the battery charger (automatically or manually)
- Controls beacon indicators (blinking or fixed, automatic or manual mode)
- Handles UVP (Under Voltage Protection)
- Stores historical measurements
- Allows asynchronous triggering of measurements via Modbus coils
- Supports configurable measurement period (e.g., every 1s)
- Calculates and stores min, max, and median metrics for voltage and current
- Supports secure serial number setting using password-protected mechanism
- Provides EEPROM persistence for configuration and calibration data

All parameters are exposed via Modbus registers to be configured and monitored externally.

---

## 📝 Usage Summary

### Measurements

- Measurements are taken periodically (every 1 second by default).
- The measurement period can be changed by writing the holding register 2 (use the value in milliseconds).
- For each source of measurements (panel, battery, consumption) both Voltage and Current measurements are available.
- Eight history values, and the min, max and mean metrics (so 11 values total) are stored for each of this two magnitudes. 
- You can access this values by reading the corresponding input registers. For example, in order to read the panel voltage
  (history and metrics) you would use

|  Input Register   | Contents                    | Notes                      |
|-------------------|-----------------------------|----------------------------|
| Input Register 3  | `panel.voltage.hist[0]`     | Newest voltage sample      |
| Input Register 4  | `panel.voltage.hist[1]`     |                            |
| Input Register 5  | `panel.voltage.hist[2]`     |                            |
| Input Register 6  | `panel.voltage.hist[3]`     |                            |
| Input Register 7  | `panel.voltage.hist[4]`     |                            |
| Input Register 8  | `panel.voltage.hist[5]`     |                            |
| Input Register 9  | `panel.voltage.hist[6]`     |                            |
| Input Register 10 | `panel.voltage.hist[7]`     | Oldest voltage sample      |
| Input Register 11 | `panel.voltage.max`         | Highest value in history   |
| Input Register 12 | `panel.voltage.min`         | Lowest value in history    |
| Input Register 13 | `panel.voltage.med`         | Median of history values   |

**⏳ Notes on History Buffers**

- Each `hist` field stores **8 samples**.
- The newest sample is always in `hist[0]`, and the oldest in `hist[7]`.
- Every new sample pushes older values down the array.

📎 **Tip**: You can read all values with a single Modbus read of **11 input registers starting at Input Register 3** (starting at the HR corresponding hist[0] of the desired measurement). 	

---
- Alternatively you may trigger asynchronous measurements using modbus coils:
  - Coil 0            | Sense all measurements (panel, battery, consumption)
  - Coil 1            | Sense only from the solar panel (voltage & current)
  - Coil 2            | Sense only from the battery (voltage & current)
  - Coil 3            | Sense only from the consumption/load (voltage & current)
- These measurements will be stored in the newest value slot in the buffer (hist[0]) 
---

### 🔦 Beacon Control

1. **Choose Mode** via `beacon_mode` (Holding Register 6):
   - `0`: Auto mode (controlled by ambient light sensor)
   - `1`: Manual mode

2. If in **manual mode**, use:
   - `beacon` (Holding Register 5): Set to `1` to turn ON, `0` to turn OFF
   - `beacon_duty_mode` (Holding Register 7):
     - `0`: Blink (100ms ON every 1s)
     - `1`: Continuous ON/OFF based on `beacon` register

---

### 🔋 Charger Control

1. **Choose Mode** via `chrg_mode` (Holding Register 11):
   - `0`: Auto mode (based on voltage & tail current thresholds)
   - `1`: Manual mode

2. If in **manual mode**, use `chrg` (Holding Register 10):
   - Set to `1` to force charger ON
   - Set to `0` to turn charger OFF

3. In **automatic mode**, the charger is controlled based on:
   - `voltage_chrg_on` (Holding Register 3): Turns charger ON if voltage is too low
   - `curr_tail` (Holding Register 4): Turns charger OFF if charging current drops below this threshold while voltage is high

---

### Asigning Thresholds for Charger Control
You can (must) now write **real-world values** directly to configure thresholds like charger re-enable voltage and tail current — no need to calculate ADC units yourself!

| Register        | Name             | Input Format                  | Converted to...       |
|-----------------|------------------|-------------------------------|------------------------|
| HR3 (40003)     | `voltage_chrg_on`| Desired voltage in **V × 100** (e.g. 12.5V → 1250) | ADC value              |
| HR4 (40004)     | `curr_tail`      | Desired current in **mA** (e.g. 100 mA → 100)      | ADC value              |

### Notes:
- These values are automatically converted to ADC equivalents using the corresponding calibration factors:
  - `batt_volt_calib_factor` for voltage
  - `batt_curr_calib_factor` for current
- Rounding is applied: values are rounded to the nearest ADC integer
- The ADC value is stored into the same register you wrote to, replacing the user-friendly input
- These values are saved to EEPROM and used in charger control logic

---

### 🛡 UVP (Under Voltage Protection)

1. **Choose Mode** via `uvp_mode` (Holding Register 9):
   - `0`: Auto mode — UVP is always enabled and the Under Voltage Protection is performed via hardware
   - `1`: Manual mode — you can control UVP manually

2. If in **manual mode**, use `uvp` (Holding Register 8):
   - Set to `1` to enable UVP
   - Set to `0` to disable UVP (for testing)

---

### 🧪 Measurement Behavior

- Battery current direction is tracked:
  - During **charging** → ADC value from `IBMON-` (negated)
  - During **discharging** → ADC value from `IBMON+`

---

### 🧪 Protected Serial Number Write Operations

The serial number is stored in a **read-only Input Register (HR1)**, but you can assign it via a secure mechanism that uses Holding Registers:

#### Register Allocation

| Register        | Name               | Description                                                                  |
|-----------------|--------------------|------------------------------------------------------------------------------|
| HR18 (40018)    | `serial_number_in` | Serial number input (write only if password is accepted)                     |
| HR19 (40019)    | `sn_password`      | Password input to unlock serial number write (e.g. `0xBEEF`)                 |
| HR20 (40020)    | `sn_write_status`  | Status of last write: 0 = Idle, 1 = Success, 2 = Failed, 3 = Not Authorized  |

#### How It Works:
1. Write the correct password to `sn_password` (HR19) → unlocks 1 write for **15 seconds**
2. Within that time, write your desired serial number to `serial_number_in`
3. If successful:
   - Serial number is written into the read-only input register and stored to EEPROM
   - `sn_write_status` becomes `1` (success)
4. If the wrong password is submitted or a serial number write operation is attempted without submitting the password, `sn_write_status` becomes:
  - `2` **wrong password** or 
  - `3` **unauthorized** accordingly
5. All related registers are finally cleared (ready to use again).

---

## 📦 Modbus Holding Registers (`0x03/0x06`, Range: 40001+)

| Address  | Register | Name                        | Description                                                                 | Values / Units                 |
|----------|----------|-----------------------------|-----------------------------------------------------------------------------|--------------------------------|
| 40001    | HR 0     | `addr_slave`                | Modbus slave address                                                        | 1–255                          |
| 40002    | HR 1     | `baudrate`                  | Modbus RTU baudrate                                                         | e.g., 9600, 19200              |
| 40003    | HR 2     | `periode`                   | Measuring period (in ms) for periodic acquisition                           | 1–2000                         |
| 40004    | HR 3     | `voltage_chrg_on`           | Re-enable voltage threshold for charger (scaled: V × 250)                   | e.g., 12.5 V → 3125            |
| 40005    | HR 4     | `curr_tail`                 | Tail current threshold for charger cut-off (scaled: A × 10000)              | e.g., 100 mA → 1000            |
| 40006    | HR 5     | `beacon`                    | Beacon ON/OFF control                                                       | 0 = OFF, 1 = ON                |
| 40007    | HR 6     | `beacon_mode`               | Beacon mode selection                                                       | 0 = Auto (LDR), 1 = Manual     |
| 40008    | HR 7     | `beacon_duty_mode`          | Beacon duty mode in manual mode                                             | 0 = Blink, 1 = Always ON/OFF   |
| 40009    | HR 8     | `uvp`                       | UVP control                                                                 | 0 = OFF, 1 = ON (manual only)  |
| 40010    | HR 9     | `uvp_mode`                  | UVP mode selection                                                          | 0 = Auto (hardware), 1 = Manual|
| 40011    | HR 10    | `chrg`                      | Charger ON/OFF (manual mode only)                                           | 0 = OFF, 1 = ON                |
| 40012    | HR 11    | `chrg_mode`                 | Charger mode selection                                                      | 0 = Auto, 1 = Manual           |
| 40013    | HR 12    | `panel_volt_calib_factor`   | Panel voltage calibration factor                                            | in mV*100/count                    |
| 40014    | HR 13    | `batt_volt_calib_factor`    | Battery voltage calibration factor                                          | in mV*100/count                    |
| 40015    | HR 14    | `cons_volt_calib_factor`    | Consumption voltage calibration factor                                      | in mV*100/count                    |
| 40016    | HR 15    | `panel_curr_calib_factor`   | Panel current calibration factor                                            | in µA/count                    |
| 40017    | HR 16    | `batt_curr_calib_factor`    | Battery current calibration factor                                          | in µA/count                    |
| 40018    | HR 17    | `cons_curr_calib_factor`    | Consumption current calibration factor                                      | in µA/count                    |
| 40019    | HR 18    | `serial_number_in`          | Input value to write the serial number (if authorized)                      | uint16 (only if password accepted)      |
| 40020    | HR 19    | `sn_password`               | Serial number write password (triggers write permission)                    | 16-bit password                         |
| 40021    | HR 20    | `sn_write_status`           | Serial number write attempt status                                          | 0 = Idle, 1 = Success, 2 = Incorrect Password, 3 = Unauthorized |
---

## 📊 Modbus Input Registers (`0x04`, Range: 30001+)

| Address       | Register     | Name                               | Description                                                             | Format / Notes                          |
|---------------|--------------|------------------------------------|-------------------------------------------------------------------------|-----------------------------------------|
| 30001         | IR 0         | `sensor_type`                      | Sensor type code                                                        | e.g., 700 = Power Supply Module         |
| 30002         | IR 1         | `serial_number`                    | Unique serial number of the board                                       |                                         |
| 30003         | IR 2         | `chrg`                             | Charging status                                                         | 0 = OFF, 1 = ON                         |
| 30004–30011   | IR 3–10      | `panel.voltage.hist`               | Panel voltage history (8 samples, `hist[0]` = newest)                   | 8 × int16                               |
| 30012–30014   | IR 11–13     | `panel.voltage.{max,min,med}`      | Panel voltage stats                                                     | max, min, mean                          |
| 30015–30022   | IR 14–21     | `panel.current.hist`               | Panel current history (8 samples, `hist[0]` = newest)                   | 8 × int16                               |
| 30023–30025   | IR 22–24     | `panel.current.{max,min,med}`      | Panel current stats                                                     | max, min, mean                          |
| 30026–30033   | IR 25–32     | `battery.voltage.hist`             | Battery voltage history (8 samples, `hist[0]` = newest)                 | 8 × int16                               |
| 30034–30036   | IR 33–35     | `battery.voltage.{max,min,med}`    | Battery voltage stats                                                   | max, min, mean                          |
| 30037–30044   | IR 36–43     | `battery.current.hist`             | Battery current history (direction-aware, `hist[0]` = newest)           | 8 × int16                               |
| 30045–30047   | IR 44–46     | `battery.current.{max,min,med}`    | Battery current stats                                                   | max, min, mean                          |
| 30048–30055   | IR 47–54     | `consumption.voltage.hist`         | Consumption voltage history (8 samples, `hist[0]` = newest)             | 8 × int16                               |
| 30056–30058   | IR 55–57     | `consumption.voltage.{max,min,med}`| Consumption voltage stats                                               | max, min, mean                          |
| 30059–30066   | IR 58–65     | `consumption.current.hist`         | Consumption current history (8 samples, `hist[0]` = newest)             | 8 × int16                               |
| 30067–30069   | IR 66–68     | `consumption.current.{max,min,med}`| Consumption current stats                                               | max, min, mean                          |

---

## 🔘 Modbus Coils (`0x01/0x05`, Range: 00001+)

| Coil Address | Bit Index | Name               | Description                                     |
|--------------|-----------|--------------------|-------------------------------------------------|
| 00001        | Bit 0     | `sense_all`        | Trigger measurement of all sources              |
| 00002        | Bit 1     | `sense_panel`      | Trigger only solar panel measurement            |
| 00003        | Bit 2     | `sense_battery`    | Trigger only battery measurement                |
| 00004        | Bit 3     | `sense_consumption`| Trigger only load/consumption measurement       |

---

## LICENSE  
This project is licensed under the **MIT License**.  
You are free to **use, copy, modify, merge, publish, distribute, sublicense, and/or sell** copies of the software, under the terms of the MIT License.  
For more details, see the [LICENSE](./LICENSE) file.
