# BB84 QKD Simulator

A C implementation of the BB84 quantum key distribution protocol, simulating key generation, measurement and sifting between two parties, Alice and Bob.

## Overview

BB84 (Bennett and Brassard, 1984) allows two parties to establish a shared secret key using qubits sent over a quantum channel and a public classical channel.

1. Alice generates random bits and encodes each in a randomly chosen basis.
2. Bob measures each qubit in a randomly chosen basis.
3. Alice and Bob publicly compare bases (not bits) and discard positions where they differ. This step is called sifting.

When the bases match, Bob recovers Alice's bit. When they differ, his outcome is random. Roughly half of all positions survive sifting.

## State Model

Qubit states are represented by polarisation angles.

| Angle | Basis       | Bit |
|-------|-------------|-----|
| 0°    | Rectilinear | 0   |
| 90°   | Rectilinear | 1   |
| 45°   | Diagonal    | 0   |
| 135°  | Diagonal    | 1   |

Two angles belong to the same basis if both are rectilinear (0°, 90°) or both are diagonal (45°, 135°).

## Implementation

All code is in `bb84.c`.

| Function | Description |
|----------|-------------|
| `bit_gen(n)` | Allocates and returns `n` random bits. |
| `basis_gen(n)` | Allocates and returns `n` random angles from {0, 90, 45, 135}. |
| `basis_of(angle)` | Returns the basis of an angle: `0` for rectilinear, `1` for diagonal. |
| `measure(...)` | Simulates Bob's measurement. Matching basis returns Alice's bit; otherwise a random bit. |
| `sift(...)` | Retains positions with matching bases and returns a `sifted_key`. |
| `print_bits(arr, k)` | Prints `k` values of an array. |

The `sifted_key` struct holds Alice's sifted bits, Bob's sifted bits, and the sifted length. All arrays are heap-allocated and must be freed by the caller.

## Build and Run

Requires `gcc` or any C99-compatible compiler.

```bash
gcc -Wall -g -fsanitize=address bb84.c -o bb84
./bb84
```

## Validation

- **Sifting rate:** for large `n`, the sifted length is approximately `n / 2`.
- **Error-free baseline:** with no eavesdropper and no noise, Alice's and Bob's sifted keys are identical.

## Roadmap

- [x] Key generation, measurement and sifting
- [x] Eavesdropper model (intercept-resend)
- [x] Noisy channel and QBER abort threshold
- [ ] Error correction
- [ ] Privacy amplification
- [ ] Parameter sweeps and CSV export

## Disclaimer

This is an educational simulation. Qubits are modelled classically and the code is not intended for real cryptographic use.
