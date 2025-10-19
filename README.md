# SSE4IntrinsicsBenchmark

A benchmark of the example function `fadeTo()` using different C coding approaches.

---

## Benchmark Results

### Fred Benchmark A (`-O2` Optimization)

**System**  
- CPU: Intel Core i5-9400 @ 2.90 GHz  
- OS: Windows 10  
- GCC: version 10.2.0  
- Compiler:  
  ```bash
  gcc -std=gnu11 -O2 -msse4.1 benchmark.c -o benchmark.exe
  ```

**Parameters**  
- Resolution: 1920×1080 (2,073,600 pixels)  
- Iterations: 6000  

| Version | Total Time (s) | Avg / Call (ms) | Speed-up |
|----------|----------------|-----------------|-----------|
| C bits   | 25.931526 | 4.321921 | 1.00× |
| C array  | 18.124303 | 3.020717 | **1.43×** |
| C union  | 28.951343 | 4.825224 | 0.90× |
| **SSE4** | **4.066549** | **0.677758** | **6.38× (vs bits)** / **4.46× (vs array)** / **7.12× (vs union)** |

---

### Fred Benchmark B (`-O0` Optimization)

**System**  
- CPU: Intel Core i5-9400 @ 2.90 GHz  
- OS: Windows 10  
- GCC: version 10.2.0  
- Compiler:  
  ```bash
  gcc -std=gnu11 -O0 -msse4.1 benchmark.c -o benchmark.exe
  ```

**Parameters**  
- Resolution: 1920×1080 (2,073,600 pixels)  
- Iterations: 6000  

| Version | Total Time (s) | Avg / Call (ms) | Speed-up |
|----------|----------------|-----------------|-----------|
| C bits   | 82.828644 | 13.804774 | 1.00× |
| C array  | 83.536385 | 13.922731 | 0.99× |
| C union  | 93.390213 | 15.565036 | 0.89× |
| **SSE4** | **50.360215** | **8.393369** | **1.64× (vs bits)** / **1.66× (vs array)** / **1.85× (vs union)** |

---

## 🧩 Notes

- “C bits” refers to the version using explicit bit-shifts and masks.  
- “C array” is the pointer-based channel addressing with unrolled per-channel operations.  
- “C union” uses a `Color` union with per-channel access via fields.  
- “SSE4” is the optimized intrinsic implementation.

These results clearly demonstrate how SSE4 vectorization significantly improves performance, especially when compiled with full optimizations (`-O2`).

---
