# SSE4IntrinsicsBenchmark

A benchmark of the example function 'fadeTo()' using different coding approaches.
Supported languages:
- C 
- Rust 

---

## Benchmark Results

### Fred Benchmark 31.10.2025 ('-O0', '-O2', '-O3' Optimizations)

**System**  
- CPU: Intel Core i3-8100 @ 3.60 GHz  
- OS: Windows 10  
- GCC: version 15.2.0 (Rev8, Built by MSYS2 project)  

**Parameters**  
- Resolution: 1920×1080 (2,073,600 pixels)  
- Iterations: 6000  

---

#### 🔹 '-O0' Optimization

| Version | Total Time (s) | Avg / Call (ms) | Speed-up |
|----------|----------------|-----------------|-----------|
| C bits   | 90.310446 | 15.051741 | 1.00× |
| C array  | 92.459545 | 15.409924 | 0.98× |
| C union  | 97.801562 | 16.300260 | 0.92× |
| **SSE4** | **56.468796** | **9.411466** | **1.60× (vs bits)** / **1.64× (vs array)** / **1.73× (vs union)** |

---

#### 🔹 '-O2' Optimization

| Version | Total Time (s) | Avg / Call (ms) | Speed-up |
|----------|----------------|-----------------|-----------|
| C bits   | 18.847903 | 3.141317 | 1.00× |
| C array  | 4.830662 | 0.805110 | **3.90×** |
| C union  | 41.346140 | 6.891023 | 0.46× |
| **SSE4** | **4.308414** | **0.718069** | **4.37× (vs bits)** / **1.12× (vs array)** / **9.60× (vs union)** |

---

#### 🔹 '-O3' Optimization

| Version | Total Time (s) | Avg / Call (ms) | Speed-up |
|----------|----------------|-----------------|-----------|
| C bits   | 19.675927 | 3.279321 | 1.00× |
| C array  | 5.313976 | 0.885663 | **3.70×** |
| C union  | 42.152320 | 7.025387 | 0.47× |
| **SSE4** | **10.588619** | **1.764770** | **1.86× (vs bits)** / **0.50× (vs array)** / **3.98× (vs union)** |

---

## 🧩 Notes

- "C bits" refers to the version using explicit bit-shifts and masks.  
- "C array" is the pointer-based channel addressing with unrolled per-channel operations.  
- "C union" uses a `Color` union with per-channel access via fields.  
- "SSE4" is the optimized intrinsic implementation.

The results confirm that **SSE4 vectorization provides significant speed-up**, especially at '-O0' and '-O2' optimization levels.  
Interestingly, at '-O3', the gcc compiler does not process the intrinsics well and produces less efficient code.

---
