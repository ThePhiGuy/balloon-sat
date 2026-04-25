# Mission

## 1. Mission Overview

This project is a pico balloon RF payload designed to operate in the upper atmosphere as a lightweight, low-power communication node.

The system supports:

- Periodic telemetry beaconing
- Optional packet digipeating
- Optional crossband RF relay
- Remote command and configuration via RF link
- Autonomous failsafe operation

The mission is focused on demonstrating reliable long-duration autonomous RF operation under extreme environmental constraints with experimental additional modes for users. 

---

## 2. Mission Objectives

### Primary Objectives
- Maintain continuous telemetry beaconing during flight
- Provide basic position reporting via GPS-based data
- Ensure autonomous operation without ground intervention

### Secondary Objectives
- Support packet-based command link for in-flight configuration
- Enable optional digipeater functionality (VHF or UHF)
- Demonstrate crossband relay capability between radios

### Experimental Objectives
- Evaluate event-driven RF firmware architecture
- Test multi-mode RF behavior under low-power conditions
- Assess long-duration stability of dual-radio system

---

## 3. Operational Concept

The system operates as an event-driven embedded RF node.

Key operational inputs:
- Timer events (beacon scheduling)
- RF packet reception events
- Squelch-triggered audio events (crossband)
- System fault events (watchdog / voltage)

Outputs:
- RF telemetry beacon transmissions
- Packet forwarding (digipeater mode)
- Crossband audio relay (if enabled)
- Mode transitions via validated RF commands

---


## 4. Success Criteria

- Successful sustained beacon operation during flight
- Reliable GPS-based telemetry reporting
- Stable RF command reception and execution
- Safe autonomous behavior under low power conditions
- Successful command link control operation

---

## 5. Constraints

- Strict power limitation (battery / solar dependent)
- Minimal computational overhead required
- Deterministic RF behavior
- No reliance on continuous ground control
- No harmful interferance over outside governing bodies

---

## 6. Assumptions

- RF propagation conditions vary with altitude
- GPS availability may be intermittent
- Power budget is limited and non-uniform
- Environmental conditions are non-deterministic
- Command link will be powerful enough to provide consistent command
- DRA818 modules will not stick transmitting

---

## 7. Future Extensions (Out of Scope for Initial Mission)

- SSTV downlink support
- Advanced adaptive frequency selection
- Multi-hop via multiple balloon digipeaters
- Enabling of additional sensors to add telemetry data

---

## 8. Ground Control Interaction

Ground stations may:
- Send mode transition commands
- Request telemetry updates
- Enable or disable digipeater or crossband modes
- Monitor system health and position

All commands are validated and subject to system safety constraints.