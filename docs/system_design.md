# System Design

## 1. Overview

This system is a lightweight RF payload for a pico balloon capable of:

- Periodic beacon transmission
- Packet reception and command link
- Optional Digipeater on either VHF or UHF radio
- Optional crossband relay between VHF and UHF radios
- Basic failsafe behavior under fault conditions

The system is event-driven and operates as a single state machine with configurable behavior modes.

---

## 2. High-Level Data Flow

Incoming RF data is decoded into packets, processed by decision logic based on system mode, and optionally retransmitted.

---

## 3. System States

The firmware operates as a finite state machine:

### INIT
- Initialize hardware (radio, timers, GPIO)
- Enter RX state after setup

### RX
- Listen for incoming RF packets
- Detect squelch / packet start
- Capture and decode packet
- Handles Crossband Repeat Mode Logic

### PROCESS_PACKET
- Decode received packet
- Apply mode-based routing rules
- Decide whether to drop or forward

### TX
- Transmit queued packet
- Key PTT
- Return to RX after completion 

### FAILSAFE
- Triggered on error/watchdog reset
- Disable TX + Shutdown both radios
- Return to INIT

---

## 4. System Modes (Revised Architecture)

This system uses a layered mode model instead of single overloaded modes.

Each configuration is defined by three independent layers:

- **Role** → what the system is doing
- **Policy** → constraints on power/behavior
- **Radio Behavior** → how RF links are assigned and routed

---

# 4.1 Role Layer (Core System Function)

## BEACON
- Transmits periodic telemetry beacon
- Reads GPS for position and scheduling
- Adjusts beacon interval based on GPS-derived conditions
- May use APRS frequency mapping based on location
- Does not perform packet forwarding

---

## DIGIPEATER
- Receives packets and retransmits according to routing rules
- Supports VHF↔VHF or UHF↔UHF operation depending on radio assignment
- Uses standard or custom via paths (e.g., WIDE1-1)
- Command link remains active unless overridden by policy

---

## CROSSBAND
- Real-time audio relay between VHF and UHF
- Uses squelch-triggered PTT control
- No packet decoding or store-and-forward behavior
- Beaconing disabled while active

---

## IDLE
- Minimal system activity
- RX event monitoring optional depending on policy
- Used for safe standby or transitions

---

# 4.2 Policy Layer (System Constraints)

## NORMAL
- Full functionality enabled
- All modes available
- Standard beacon and RX behavior

---

## LOW_POWER
- Reduces beacon frequency significantly
- Disables or limits DIGIPEATER and CROSSBAND operation
- Packet command acceptance may be restricted
- Monitors system voltage for recovery conditions

---

## EMERGENCY
- Enables restricted DIGIPEATER operation only
- Prioritizes packet forwarding over other functions
- Activates temporary operational window (e.g., 30 minutes)
- Uses regional APRS frequency selection based on GPS location
- Suppresses non-essential functions (beacons may be reduced or disabled depending on conditions)

---

# 4.3 Radio Behavior Layer (Hardware Mapping)

## VHF_PRIMARY
- VHF used for RX/TX packet operations
- UHF may be used for command or standby

---

## UHF_PRIMARY
- UHF used for RX/TX packet operations
- VHF may be used for command or standby

---

## DUAL_ACTIVE
- Both VHF and UHF RX are active simultaneously
- Events are routed in software based on role and policy
- No manual RX switching required

---

# 4.4 Mode Composition Examples

Instead of selecting a single “mode”, the system uses combinations:

### BEACON SYSTEM
- Role: BEACON
- Policy: NORMAL or LOW_POWER
- Radio: DUAL_ACTIVE or VHF_PRIMARY

---

### STANDARD DIGIPEATER (V/V)
- Role: DIGIPEATER
- Policy: NORMAL
- Radio: VHF_PRIMARY

---

### UHF DIGIPEATER (U/U)
- Role: DIGIPEATER
- Policy: NORMAL
- Radio: UHF_PRIMARY

---

### EMERGENCY DIGIPEATER
- Role: DIGIPEATER
- Policy: NORMAL
- Radio: DUAL_ACTIVE
- Timer-limited operation window active

---

### CROSSBAND REPEATER (V/U)
- Role: CROSSBAND
- Policy: NORMAL
- Radio: DUAL_ACTIVE
- Routing: VHF RX -> UHF TX

---

### CROSSBAND REPEATER (U/V)
- Role: CROSSBAND
- Policy: NORMAL
- Radio: DUAL_ACTIVE
- Routing: UHF RX -> VHF TX

---

# 4.5 Key Design Principle

- Roles define **what the system is doing**
- Policies define **what the system is allowed to do**
- Radio behavior defines **which hardware is active**
- No single mode controls all system behavior

This keeps firmware logic modular and allows future system expansion.

## 5. Decision Logic

Decision logic is split into two independent paths:

- **Packet Processing Path (decoded data)**
- **Crossband Audio Path (squelch-driven real-time relay)**

---

### 5.1 Packet Processing (PROCESS_PACKET)

This path only handles decoded packet data.

During PROCESS_PACKET:

- If BEACON_ONLY -> check for valid command packets only
- If DIGI modes -> validate packet, check repeat conditions
- If EMERG_DIGI -> validate packet and apply emergency routing rules
- If NORMAL -> ignore or process for telemetry/commands depending on configuration

---

### Packet Filtering Rules (Packet Path Only)

- Ignore duplicate packets
- Reject invalid or corrupted frames
- Drop packets not matching routing or mode criteria

---

### 5.2 Crossband Decision Logic (RX / Squelch Path)

Crossband operation is NOT handled in PROCESS_PACKET.

Instead, it is triggered directly from RX squelch events:

- If CROSSBAND_V_U -> when VHF squelch opens:
  - Route RX audio -> UHF TX audio via analog switch
  - Key UHF PTT while squelch remains active

- If CROSSBAND_U_V -> when UHF squelch opens:
  - Route RX audio -> VHF TX audio via analog switch
  - Key VHF PTT while squelch remains active

---

### Crossband Rules

- No packet decoding is performed in crossband mode
- No duplicate filtering is applied (audio is transparent)
- Operation is fully real-time (squelch-driven)
- TX is active only while RX squelch is active


## 6. Hardware Interfaces

### Radio Interface
- UART: configuration of DRA818 modules
- Audio path: analog or DAC output for modulation
- PTT: GPIO control
- Audio Interface:
  - RX audio output from radio (analog line-level)
  - TX audio input to radio (analog modulation input)
  - MCU-generated audio via DAC for beacon

### Timing
- Timer interrupt for beacon scheduling
- Watchdog timer for failsafe recovery

---

## 7. Packet Model (initial)

Basic packet structure:

- Callsign
- Payload (telemetry or forwarded data)
- Sequence ID

Encoding: AX.25

---

## System Event Model

The system is event-driven with a validated command-to-mode transition layer.

---

### Core Events

- Timer event → enqueue beacon → TX
- Packet received → RX → PROCESS_PACKET
- TX complete → return to RX
- Fault detected → FAILSAFE

---

### Command Processing

When a packet is received:

1. Validate frame integrity
   - Check checksum / CRC
   - Reject duplicates

2. Extract command (if present)

3. If command exists:
   - Validate command authenticity (format + allowed source)
   - Check if command is allowed in current mode
   - Apply mode transition rules

---

### Mode Transition Rules

Allowed transitions are explicitly defined:

#### From BEACON_ONLY
- Can transition to:
  - DIGI_VHF
  - DIGI_UHF
  - CROSSBAND_V_U
  - CROSSBAND_U_V
  - LOW_POWER

#### From DIGI modes
- Can transition to:
  - BEACON_ONLY
  - LOW_POWER
- Auto-expires via timer to BEACON_ONLY mode

#### From CROSSBAND modes
- Can transition to:
  - BEACON_ONLY
  - LOW_POWER

#### From LOW_POWER
- Can only transition to:
  - BEACON_ONLY via a voltage threshold
  - FAILSAFE recovery mode (automatic or voltage-based)

#### From EMERG_DIGI
- Can transition to:
  - BEACON_ONLY
  - LOW_POWER
- Auto-expires via timer to BEACON_ONLY mode

---

### Command-to-Mode Mapping

Commands map to modes explicitly:

- CMD_BEACON_ONLY -> BEACON_ONLY
- CMD_DIGI_VHF -> DIGI_VHF
- CMD_DIGI_UHF -> DIGI_UHF
- CMD_CROSSBAND_V_U -> CROSSBAND_V_U
- CMD_CROSSBAND_U_V -> CROSSBAND_U_V
- CMD_LOW_POWER -> LOW_POWER
- CMD_EMERGENCY_DIGI -> EMERG_DIGI

---

### Safety Constraints

- Mode changes are ignored if:
  - command is invalid
  - packet fails validation
  - mode transition is not allowed from current state
  - system is in FAILSAFE

- FAILSAFE overrides all commands

## 9. Design Constraints

- Low power operation (duty-cycled TX)
- Minimal CPU load
- Deterministic state transitions
- Modular radio abstraction layer

---

