# RF Requirements

## 1. Purpose
The RF System for the balloon-sat is the payload in a pico-balloon equivalent size class
With the goal of providing enhanced functionality compared to commercially available modules.

The other goal of the RF system is to utilize off-the-shelf RF hardware to provide ease of
construction simplicity and ease of integration into a final design.# RF Requirements


## 2. Operating Scenario

### 2.1 Mission Profile
- Maximum altitude: 20km
- Nominal altitude: 15km
- Maximum slant range: 500km
    (500 km represents an upper-bound estimate including atmospheric refraction)
- Nominal operating range: 250km

### 2.2 Environment Assumptions
- Line-of-sight conditions: 
    Line-of-sight propogation conditions are assumed
    Free-space path model used for all RF link budget calculations
    Earths Curvature utilized with altitude to calculate max range
- Atmospheric conditions:
    Atmospheric absorbtion assumed negligable at both 2m and 70cm
    Weather effects are negated as they are assumed to cause likely loss of payload
- Ground station assumptions:
    Ground Station is assumed to be suburban with a ~-115dBm noise floor
    Balloon noise floor is assumed to be -120dBm without much local interferance
- Rotation
    Ballon is assumed to be freely rotating and in uncontrolled motion
    Polarization mismatch is assumed at 10db for a ground station
    Ground stations are assumed to be vertically polarized
- Ground Stations
    Base assumption is a handheld radio with a 1/2 wavelength vertical antenna
    Antenna gain ranges from 0 dBi to higher with a base station
    All ground stations are assumed to have no obstructions to the balloon

---

## 3. Frequency & Modulation

### 3.1 Frequency Bands
- Primary band: 2m Ham Radio Band (144MHz)
- Secondary band (if applicable): 70cm Ham Radio Band (440MHz)

### 3.2 Modulation Schemes
- APRS (1200 baud AFSK)
- FM voice

---

## 4. Link Geometry & Path Loss

### 4.1 Worst-Case Geometry
- Maximum distance: 500 km
- Corresponding altitude: 15km

### 4.2 Path Loss
- Path loss (2m band): 128.75 dB
- Path loss (70cm band): 138.29 dB

---

## 5. System Losses

| Loss Source              | Value (dB) | Notes |
|--------------------------|-----------|-------|
| Polarization mismatch    | 6–10      | Uncontrolled rotation|
| Cable/connector losses   | 0–0.5     | Direct soldered antenna|
| Miscellaneous losses     | 1–2       | Detuning, body effects, temp shift |
| **Total System Loss**    | 12        | worst-case budget |

---

## 6. Receiver Requirements

### 6.1 Sensitivity
- Minimum detectable signal:

### 6.2 Noise Floor Assumptions
- Balloon receiver: -120 dBm
- Ground receiver: -115 dBm

### 6.3 Required SNR
- APRS (1200 baud): 10 dB
- FM voice: 15 dB

### 6.4 Sensitivity
- Minimum detectable signal:
  - APRS (12 dB SINAD equivalent): ≤ -118 dBm
  - Absolute sensitivity floor: ≈ -120 dBm (ideal conditions)

---

## 7. Transmitter Requirements

### 7.1 Output Power
- Balloon transmitter: 0.5 Watts
- Ground transmitter: 5 Watts

### 7.2 Frequency Stability
- Transmitter shall maintain frequency stability sufficient for narrowband FM operation over expected temperature range at 15km atmosphere

### 7.3 Duty Cycle
- Expected duty cycle in Crossband Repeat: <50%
- Expected duty cycle in Digipeater Mode : <25%
- Expected Beaconing Duty Cycle: < 5%

---

## 8. Antenna Requirements

### 8.1 Balloon Antenna (VHF)
- Gain: 0.0 dBi
- Radiation pattern: omnidirecitonal
- Polarization: vertical

### 8.2 Balloon Antenna (UHF)
- Gain: 2.5 dBi
- Radiation pattern: dipole
- Polarization: horizontal

### 8.3 Ground Antenna
- Gain assumptions: 0.0 dBi
- Polarization: Vertical

---

## 9. Link Budget Summary

### 9.1 Uplink (Ground → Balloon)
- Transmit power: +37 dBm (5w)
- Gains: Omni (0.0dBi) 1/2 Wave whip
- Losses: Polarization Mismatch (10 dB) Path Loss
- Received power @250km: VHF (-97 dBm) UHF (-103 dBm)

### 9.2 Downlink (Balloon → Ground)
- Transmit power: +27 dBm (0.5w)
- Gains: UHF (2.5 dBi) VHF (0.0 dBi)
- Losses: Polarization Mismatch (10 dB) Path Loss
- Received power @250km: VHF (-107 dBm) UHF (-114 dBm) 

---

## 10. Link Margin Requirements

- Minimum required link margin:
    10 dB SNR for APRS
    15 dB SNR for FM Voice
- Link margin at nominal range:
    for Balloon:
        Uplink is within margin for APRS and Voice on both 70cm and 2m
    for Ground Station:
        Downlink on 70cm would not be within margin, but 2m would be marginal.
- Link margin at maximum range:
    for Balloon:
        Uplink is within margin for APRS for 70cm and for both APRS and Voice for 2m.
    for Ground Station:
        Neither APRS or Voice is possible at the edge of the range of the balloon with an omni-directional
        whip due to polarization loss. with a higher gain antenna or a circularly polarized one would make this possible.

---

## 11. System Constraints & Observations

- Link asymmetry:
    The DRA818's output power of 0.5 watts becomes the limiting part of the link budget.
    For consistent performance (especially on UHF) it is needed to have a better recieving antenna
    than what is needed for TX
- Band performance differences:
    UHF has a much tighter link margin, so it makes sense to have it as the uplink for the crossband
    repeater, as a ground station likely just "do more output power" than build a bigger antenna
- Sensitivity limitations:
    The assumption is a suburban noise floor, but this could be far worse in urban enviornments.
- Other key constraints:
    The polarization mismatch on any ground station antenna that isn't circularly polarized
    will come into play with at least one band, although this helps with rejection between the close
    antennas on the balloon which should improve rx/tx simultanious performance.

## 11.1 Key Design Drivers

- Downlink is power-limited → prioritize 2m for reliable telemetry
- UHF link margin is limited → better suited for uplink/control
- Polarization loss dominates system uncertainty (Circularly polarized ground antennas fix this!)
- Ground station variability significantly impacts real-world performance

---

## 12. Regulatory Considerations

- Frequency allocation:
    This balloon should utilize the APRS 144.390 frequency primarily for beaconing
    Additionally for other experimental mode use frequencies that are within the experimental segment
- Identification requirements:
    This balloon in all modes should ID with the callsign of the station programmed in, at minium every 10 minutes. This should ID whenever a command is given as the command timers are generally 10 minute activations
- Other constraints:
    This balloon needs to be able to be commanded at "any time" to cease transmissions, and this would mean likely a "HALT" command is needed to be kept under lock and key, and otherwise meaningful protections should be made to prevent other operators from using the control link.

---

## 13. Open Questions / To Be Determined

- Potential other antenna designs on the balloon
    It could be possible to do a Turnstile on UHF to mitigate some of the path loss by
    reducing polarization losses, depending on physical constraints

- Reasonable other modes that could be possible with the VHF/UHF combined setup, such as potentially 
an inverse crossband repeater.
---