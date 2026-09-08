#include <cstdint>
#include <iostream>

static constexpr int N = 281;

static inline bool pt_lt_frac(int i, int p, int q) {
    return (int64_t)(2*i+1)*q < (int64_t)2*N*p;
}
static inline bool pt_le_frac(int i, int p, int q) {
    return (int64_t)(2*i+1)*q <= (int64_t)2*N*p;
}
static inline bool pt_ge_frac(int i, int p, int q) {
    return (int64_t)(2*i+1)*q >= (int64_t)2*N*p;
}

static inline uint8_t base_color(int a, int b, int c) {
    // Midpoint discretization of the Recursive3Param rule
    // t=5/8, t1=3/8, t2=17/32, color=1 iff z<z0(x,y).
    bool x_in = pt_ge_frac(a,3,8) && pt_le_frac(a,5,8);
    bool y_in = pt_ge_frac(b,3,8) && pt_le_frac(b,5,8);

    if (x_in && y_in) {
        if (pt_lt_frac(b,17,32)) {
            if (pt_ge_frac(a,17,32)) return pt_lt_frac(c,3,8);
            if (a <= b) return pt_lt_frac(c,17,32);
            return c < b;
        }
        if (pt_lt_frac(a,17,32)) return pt_lt_frac(c,5,8);
        return pt_lt_frac(c,17,32);
    }

    if (pt_ge_frac(b,5,8)) {
        if (pt_lt_frac(a,5,8)) return 1;
        return pt_lt_frac(c,5,8);
    }
    if (pt_ge_frac(a,5,8)) return 0;
    if (a <= b) return pt_lt_frac(c,5,8);
    return c < b;
}

int main() {
    const uint64_t V = (uint64_t)N*N*N;
    const uint64_t E = V*(uint64_t)N;
    uint64_t mono = 0, cut = 0;

    for (int b=0; b<N; ++b) {
        for (int c=0; c<N; ++c) {
            uint64_t left1=0, right1=0;
            for (int a=0; a<N; ++a) left1 += base_color(a,b,c);
            for (int d=0; d<N; ++d) right1 += base_color(b,c,d);
            mono += left1*right1 + (N-left1)*(N-right1);
            cut  += left1*(N-right1) + (N-left1)*right1;
        }
    }

    const uint64_t KN=94835, KD=393216;
    __int128 lhs=(__int128)mono*KD;
    __int128 rhs=(__int128)KN*E;
    bool better=lhs<rhs;

    std::cout << "n=" << N << "\n";
    std::cout << "vertices=" << V << "\n";
    std::cout << "monochromatic=" << mono << "\n";
    std::cout << "cut=" << cut << "\n";
    std::cout << "edges=" << E << "\n";
    std::cout << "partition_check=" << (mono+cut==E ? "PASS" : "FAIL") << "\n";
    std::cout << "expected_mono_target=1503704985\n";
    std::cout << "target_match=" << (mono==1503704985ULL ? "PASS" : "FAIL") << "\n";
    std::cout << "exact_strict_improvement=" << (better ? "TRUE" : "FALSE") << "\n";
    std::cout << "RESULT=" << ((mono+cut==E && mono==1503704985ULL && better) ? "PASS" : "FAIL") << "\n";
    return (mono+cut==E && mono==1503704985ULL && better) ? 0 : 1;
}
