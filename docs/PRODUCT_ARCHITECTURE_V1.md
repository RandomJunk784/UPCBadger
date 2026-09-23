# UPCBadger — Product Architecture V1

Status: **LOCKED DIRECTION — prototype stage**
Date: 2026-09-22

## Purpose

Build a small family of UPCBadger prototypes first. Keep the system simple and inexpensive while proving the product concept. Do not over-engineer the backend or device configuration system until there is real demand.

## User setup experience

The badge must not require a dedicated phone app.

### Normal operation
- Wi-Fi connected
- Configuration radio is not continuously exposed
- Badge runs its normal boot/profile/application behaviour

### Entering setup
A physical rear **CONFIG** button is reserved on the PCB/wiring.
- Hold the button for approximately 3 seconds.
- Badge enters provisioning/configuration mode.
- Configuration radio is enabled only for this setup period.
- After setup, configuration mode is exited.

The physical button is preferred over permanently discoverable BLE because radio range is not a security boundary.

## Configuration UI

The first configurator is intentionally tiny. The temporary setup Wi-Fi is an open local network and is only enabled during deliberate setup mode:

### Wi-Fi
- Wi-Fi network
- Wi-Fi password

### Xbox
- Xbox Gamertag

### Display
Only essential display configuration should be exposed initially, if required by the firmware. Do not create an unnecessary settings/Extras screen.

The temporary setup network is **ConsoleBadger** and the local setup address is **192.168.4.1**. During setup, the phone should keep Wi-Fi enabled, turn **mobile data OFF**, connect to **ConsoleBadger**, and then open the local address.

The browser asks for:

- Wi-Fi network
- Wi-Fi password
- Xbox Gamer ID

The intended user flow is:

Hold button -> connect to ConsoleBadger -> open 192.168.4.1 -> enter Wi-Fi + Gamer ID -> Save & Connect -> badge restarts -> joins home Wi-Fi -> done

No dedicated mobile app is planned.

## Provisioning transport

BLE remains a possible provisioning transport, but it is not required to be the browser transport.

Preferred architecture:
1. Physical button activates configuration mode.
2. ESP32 provides a local provisioning path.
3. Phone uses a normal browser for the configuration UI.
4. Wi-Fi credentials and Gamertag are stored locally in ESP32 NVS.
5. Configuration radio is disabled after setup.

SoftAP + local web UI is currently the simplest browser-compatible implementation. BLE may be used for provisioning/activation if it provides a clear benefit.

Do not permanently enable BLE merely for convenience.

## Data/privacy policy for prototype

The badge does not need to create a user account with UPCBadger.

Do not store:
- user names
- email addresses
- Microsoft/Xbox passwords
- location
- friends
- messages
- telemetry
- analytics
- persistent MAC-address customer records

The badge only stores the configuration it needs locally.

## Xbox profile service — V1

Initial profile data is deliberately limited to:
- Gamertag
- Gamerscore
- Gamerpic
- XUID

Potential later additions such as presence/current game are explicitly deferred until the basic profile path works.

The ESP32 should receive a small UPCBadger-specific response rather than depending on the complete OpenXBL response structure.

OpenXBL API credentials must remain server-side and must never be embedded in firmware.

## Backend — prototype

Use a small cloud serverless endpoint rather than a laptop/home server.

Current intended prototype:

ESP32 -> HTTPS -> Cloudflare Worker -> OpenXBL -> Worker -> ESP32

No database is required for V1.

The Worker should return only the profile fields required by the badge.

## Development philosophy

- Prototype for family use first.
- Spend money only when it buys a real benefit.
- Add functionality only when there is a demonstrated reason.
- Keep provider-specific code behind the backend API boundary.
- If demand becomes real, revisit paid API/service arrangements and custom hardware.

## Future UI

The 360x360 display can eventually present:
- boot animation
- gamerpic
- Gamertag
- Gamerscore
- presence/current game
- custom user artwork

The visual design should be developed after the basic data pipeline is proven.

## Hardware note

Reserve one spare ESP32 GPIO for the rear CONFIG button now, even if the button is not physically installed yet.

Button wiring target:
- GPIO with internal pull-up
- momentary button to GND
- long press enters configuration mode

**Power off/unplug the ESP32 before soldering or changing wiring.**