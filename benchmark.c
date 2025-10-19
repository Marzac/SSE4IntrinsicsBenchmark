/**
    Performance test
    Intrinsincs SSE4 vs Plain C
    (c) Fred's Lab 2025
    info@fredslab.net
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <smmintrin.h>
#include <emmintrin.h>
#include <time.h>

#define WIDTH 1920
#define HEIGHT 1080
#define ITERATIONS 6000

static inline double now();

static inline void fadeToBits(uint32_t *frame, size_t size, uint32_t color, uint16_t factor);
static inline void fadeToArray(uint32_t *frame, size_t size, uint32_t color, uint16_t factor);
static inline void fadeToUnion(uint32_t *frame, size_t size, uint32_t color, uint16_t factor);
static inline void fadeToSSE4(uint32_t *frame, size_t size, uint32_t color, uint16_t factor);

/*****************************************************************************/
int main(int argc, char * argv[])
{
// Allocate working buffer
    size_t size = WIDTH * HEIGHT;
    uint32_t * frame = malloc(size * sizeof(uint32_t));
    memset(frame, 0, size * sizeof(uint32_t));

    uint32_t color = 0x80402010;
    uint16_t factor = 128;

// Plain C "bits" version
    double t0 = now();
    for (int i = 0; i < ITERATIONS; i++)
        fadeToBits(frame, size, color, factor);
    double t1 = now();

// Plain C "array" version
    double t2 = now();
    for (int i = 0; i < ITERATIONS; i++)
        fadeToArray(frame, size, color, factor);
    double t3 = now();
    
// Plain C "union" version
    double t4 = now();
    for (int i = 0; i < ITERATIONS; i++)
        fadeToUnion(frame, size, color, factor);
    double t5 = now();
    
// SSE4 intrinsics version
    double t6 = now();
    for (int i = 0; i < ITERATIONS; i++)
        fadeToSSE4(frame, size, color, factor);
    double t7 = now();

// Display results
    double bitsTime = t1 - t0;
    double arrayTime = t3 - t2;
    double unionTime = t5 - t4;
    double sse4Time = t7 - t6;

    printf("Resolution: %dx%d (%zu pixels)\n", WIDTH, HEIGHT, size);
    printf("Iterations: %d\n", ITERATIONS);

    printf("C bits :%.6f s total (%.6f ms/call)\n",
           bitsTime, (bitsTime / ITERATIONS) * 1000.0);
   
    printf("C array:%.6f s total (%.6f ms/call)\n",
           arrayTime, (arrayTime / ITERATIONS) * 1000.0);

    printf("C union:%.6f s total (%.6f ms/call)\n",
           unionTime, (unionTime / ITERATIONS) * 1000.0);           
           
    printf("SSE4   :%.6f s total (%.6f ms/call)\n",
           sse4Time, (sse4Time / ITERATIONS) * 1000.0);

    printf("Speed-up over bits: %.2fx\n", bitsTime / sse4Time);
    printf("Speed-up over array: %.2fx\n", arrayTime / sse4Time);
    printf("Speed-up over union: %.2fx\n", unionTime / sse4Time);
    
    fflush(stdout);
    system("PAUSE");

// Free up memory
    free(frame);
    return 0;
}

/*****************************************************************************/
static inline double now()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

/*****************************************************************************/
inline void fadeToSSE4(uint32_t * frame, size_t size, uint32_t color, uint16_t factor)
{
    __m128i c = _mm_set1_epi32(color);

    // Unpack color to 16-bit lanes (B,G,R,A)
    __m128i c_lo = _mm_unpacklo_epi8(c, _mm_setzero_si128());
    __m128i c_hi = _mm_unpackhi_epi8(c, _mm_setzero_si128());
    __m128i f = _mm_set1_epi16((short)(256 - factor));

    for (size_t i = 0; i + 4 <= size; i += 4) {
        __m128i pix = _mm_loadu_si128((__m128i *) &frame[i]);

    // Unpack pixels to 16-bit lanes
        __m128i lo = _mm_unpacklo_epi8(pix, _mm_setzero_si128());
        __m128i hi = _mm_unpackhi_epi8(pix, _mm_setzero_si128());

    // (src - color) * inv_factor
        lo = _mm_mullo_epi16(_mm_sub_epi16(lo, c_lo), f);
        hi = _mm_mullo_epi16(_mm_sub_epi16(hi, c_hi), f);

    // Divide by 256 (>> 8)
        lo = _mm_srli_epi16(lo, 8);
        hi = _mm_srli_epi16(hi, 8);

    // Add color back
        lo = _mm_add_epi16(lo, c_lo);
        hi = _mm_add_epi16(hi, c_hi);

    // Pack back to 8-bit
        __m128i blended = _mm_packus_epi16(lo, hi);
        _mm_storeu_si128((__m128i*) &frame[i], blended);
    }
}

/*****************************************************************************/
inline void fadeToBits(uint32_t *frame, size_t size, uint32_t color, uint16_t factor)
{
    uint16_t cb = color >> 0;
    uint16_t cg = color >> 8;
    uint16_t cr = color >> 16;
    uint16_t ca = color >> 24;

    uint16_t inv = 256 - factor;

    for (size_t i = 0; i < size; i++) {
        uint32_t p = frame[i];

        uint8_t b = p >> 0;
        uint8_t g = p >> 8;
        uint8_t r = p >> 16;
        uint8_t a = p >> 24;

        b = cb + (((int)b - cb) * inv >> 8);
        g = cg + (((int)g - cg) * inv >> 8);
        r = cr + (((int)r - cr) * inv >> 8);
        a = ca + (((int)a - ca) * inv >> 8);

        frame[i] = (a << 24) | (r << 16) | (g << 8) | b;
    }
}

/*****************************************************************************/
inline void fadeToArray(uint32_t *frame, size_t size, uint32_t color, uint16_t factor)
{
    uint8_t *c = (uint8_t *) &color;
    uint16_t inv = 256 - factor;

    for (size_t i = 0; i < size; i++) {
        uint8_t *p = (uint8_t *)&frame[i];

        p[0] = c[0] + (((int)p[0] - c[0]) * inv >> 8);
        p[1] = c[1] + (((int)p[1] - c[1]) * inv >> 8);
        p[2] = c[2] + (((int)p[2] - c[2]) * inv >> 8);
        p[3] = c[3] + (((int)p[3] - c[3]) * inv >> 8);
    }
}

/*****************************************************************************/
typedef union {
    uint32_t value;
    struct {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    };
} Color;

inline void fadeToUnion(uint32_t *frame, size_t size, uint32_t color, uint16_t factor)
{
    Color c;
    c.value = color;

    uint16_t inv = 256 - factor;

    for (size_t i = 0; i < size; i++) {
        Color p;
        p.value = frame[i];

        p.b = c.b + (((int)p.b - c.b) * inv >> 8);
        p.g = c.g + (((int)p.g - c.g) * inv >> 8);
        p.r = c.r + (((int)p.r - c.r) * inv >> 8);
        p.a = c.a + (((int)p.a - c.a) * inv >> 8);

        frame[i] = p.value;
    }
}
