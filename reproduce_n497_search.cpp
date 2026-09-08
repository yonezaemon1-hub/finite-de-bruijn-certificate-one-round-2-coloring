#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <tuple>
#include <vector>

// Deterministic reproduction of the n=497 local-search experiment.
//
// Start from the midpoint discretization of the Recursive3Param rule.
// Restrict candidate flips to W^3 with W={218,...,279}.
// Repeatedly flip the vertex with the most negative exact change Delta M in
// the number of monochromatic directed edges; break ties lexicographically
// by (a,b,c). Stop when no negative Delta M remains.
//
// This program discovers the 4750 flips from the base coloring. It does not
// read the canonical n497_certificate.json. It writes a newly generated JSON
// certificate whose bytes are intended to match the canonical certificate.

static constexpr int N = 497;
static constexpr int LO = 218;
static constexpr int HI = 279;
static constexpr int S = HI - LO + 1;
static constexpr uint64_t EXPECTED_BASE_MONO = 14715071917ULL;
static constexpr uint64_t EXPECTED_FINAL_MONO = 14714994317ULL;
static constexpr size_t EXPECTED_FLIPS = 4750;

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
    // Midpoint discretization of the Recursive3Param rule:
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

static inline int local(int x) { return x - LO; }
static inline int idx3(int a, int b, int c) {
    return (local(a)*S + local(b))*S + local(c);
}
static inline int idx2(int a, int b) {
    return local(a)*S + local(b);
}

struct Node {
    int delta;
    int a,b,c;
    uint32_t generation;
};

struct NodeGreater {
    bool operator()(const Node& x, const Node& y) const {
        if (x.delta != y.delta) return x.delta > y.delta;
        if (x.a != y.a) return x.a > y.a;
        if (x.b != y.b) return x.b > y.b;
        if (x.c != y.c) return x.c > y.c;
        return x.generation > y.generation;
    }
};

static uint64_t exact_base_mono() {
    uint64_t mono = 0;
    for (int b=0; b<N; ++b) {
        for (int c=0; c<N; ++c) {
            uint64_t left1 = 0, right1 = 0;
            for (int a=0; a<N; ++a) left1 += base_color(a,b,c);
            for (int d=0; d<N; ++d) right1 += base_color(b,c,d);
            mono += left1*right1 + (N-left1)*(N-right1);
        }
    }
    return mono;
}

static void write_certificate(const std::string& path,
                              const std::vector<std::array<int,3>>& flips,
                              uint64_t final_mono) {
    std::ofstream out(path, std::ios::binary);
    if (!out) {
        std::cerr << "ERROR: cannot open output file: " << path << "\n";
        std::exit(2);
    }
    out << "{\n";
    out << "  \"problem\": \"one-round randomized 2-coloring of directed cycles via DB_normal(n)\",\n";
    out << "  \"n\": 497,\n";
    out << "  \"base_rule\": {\n";
    out << "    \"representatives\": \"x_i=(2i+1)/(2n)\",\n";
    out << "    \"t\": \"5/8\",\n";
    out << "    \"t1\": \"3/8\",\n";
    out << "    \"t2\": \"17/32\",\n";
    out << "    \"color\": \"1 iff z < z0(x,y)\"\n";
    out << "  },\n";
    out << "  \"window\": [\n";
    out << "    218,\n";
    out << "    279\n";
    out << "  ],\n";
    out << "  \"flips\": [\n";
    for (size_t i=0; i<flips.size(); ++i) {
        const auto &v = flips[i];
        out << "    [\n";
        out << "      " << v[0] << ",\n";
        out << "      " << v[1] << ",\n";
        out << "      " << v[2] << "\n";
        out << "    ]" << (i+1<flips.size() ? "," : "") << "\n";
    }
    out << "  ],\n";
    out << "  \"monochromatic_edges\": " << final_mono << ",\n";
    out << "  \"total_edges\": 61013446081,\n";
    out << "  \"fraction\": \"14714994317/61013446081\",\n";
    out << "  \"published_exact_upper\": \"94835/393216\"\n";
    out << "}";
}

int main(int argc, char** argv) {
    const std::string output = argc >= 2 ? argv[1] : "n497_reproduced_certificate.json";

    std::vector<uint8_t> color(S*S*S);
    std::vector<int> left(S*S), right(S*S);
    std::vector<uint32_t> generation(S*S*S, 0);

    // Counts needed for exact one-vertex Delta M values inside W^3.
    for (int a=LO; a<=HI; ++a) {
        for (int b=LO; b<=HI; ++b) {
            int l=0, r=0;
            for (int x=0; x<N; ++x) l += base_color(x,a,b);
            for (int x=0; x<N; ++x) r += base_color(a,b,x);
            left[idx2(a,b)] = l;
            right[idx2(a,b)] = r;
        }
    }

    for (int a=LO; a<=HI; ++a)
        for (int b=LO; b<=HI; ++b)
            for (int c=LO; c<=HI; ++c)
                color[idx3(a,b,c)] = base_color(a,b,c);

    auto delta = [&](int a, int b, int c) -> int {
        const uint8_t x = color[idx3(a,b,c)];
        const int l = left[idx2(a,b)];
        const int r = right[idx2(b,c)];
        const int same_in  = x ? l : N-l;
        const int same_out = x ? r : N-r;
        // Incoming and outgoing sums both include the self-loop when a=b=c.
        // A self-loop remains monochromatic after a vertex flip, so compensate
        // for the double fixed-neighbor treatment by +2.
        return 2*N - 2*(same_in + same_out) + (a==b && b==c ? 2 : 0);
    };

    std::priority_queue<Node, std::vector<Node>, NodeGreater> heap;
    for (int a=LO; a<=HI; ++a)
        for (int b=LO; b<=HI; ++b)
            for (int c=LO; c<=HI; ++c)
                heap.push(Node{delta(a,b,c),a,b,c,0});

    std::vector<std::array<int,3>> flips;
    int64_t cumulative_delta = 0;
    int next_delta = 0;

    while (true) {
        Node q;
        while (true) {
            if (heap.empty()) {
                std::cerr << "ERROR: priority queue exhausted\n";
                return 2;
            }
            q = heap.top(); heap.pop();
            const int id = idx3(q.a,q.b,q.c);
            if (q.generation == generation[id] && q.delta == delta(q.a,q.b,q.c)) break;
        }

        if (q.delta >= 0) {
            next_delta = q.delta;
            break;
        }

        const int a=q.a, b=q.b, c=q.c;
        const int id = idx3(a,b,c);
        const uint8_t old = color[id];
        color[id] = uint8_t(1-old);
        const int count_change = color[id] ? +1 : -1;
        left[idx2(b,c)] += count_change;
        right[idx2(a,b)] += count_change;
        cumulative_delta += q.delta;
        flips.push_back({a,b,c});

        std::vector<int> affected;
        affected.reserve(2*S+1);
        for (int k=LO; k<=HI; ++k) affected.push_back(idx3(b,c,k));
        for (int i=LO; i<=HI; ++i) affected.push_back(idx3(i,a,b));
        affected.push_back(id);
        std::sort(affected.begin(), affected.end());
        affected.erase(std::unique(affected.begin(), affected.end()), affected.end());

        for (int u : affected) {
            const int lc = u % S;
            const int t = u / S;
            const int lb = t % S;
            const int la = t / S;
            const int aa=LO+la, bb=LO+lb, cc=LO+lc;
            ++generation[u];
            heap.push(Node{delta(aa,bb,cc),aa,bb,cc,generation[u]});
        }
    }

    const uint64_t base_mono = exact_base_mono();
    const int64_t final_signed = (int64_t)base_mono + cumulative_delta;
    if (final_signed < 0) {
        std::cerr << "ERROR: negative final edge count\n";
        return 2;
    }
    const uint64_t final_mono = (uint64_t)final_signed;
    const uint64_t edges = (uint64_t)N*N*N*N;
    const __int128 lhs = (__int128)final_mono*393216;
    const __int128 rhs = (__int128)94835*edges;

    write_certificate(output, flips, final_mono);

    const bool pass =
        base_mono == EXPECTED_BASE_MONO &&
        flips.size() == EXPECTED_FLIPS &&
        cumulative_delta == -77600 &&
        next_delta == 0 &&
        final_mono == EXPECTED_FINAL_MONO &&
        lhs < rhs;

    std::cout << "n=497\n";
    std::cout << "window=[218,279]\n";
    std::cout << "candidate_vertices=" << (uint64_t)S*S*S << "\n";
    std::cout << "base_monochromatic=" << base_mono << "\n";
    std::cout << "flip_count=" << flips.size() << "\n";
    std::cout << "cumulative_delta=" << cumulative_delta << "\n";
    std::cout << "next_best_delta=" << next_delta << "\n";
    std::cout << "final_monochromatic=" << final_mono << "\n";
    std::cout << "exact_strict_improvement=" << (lhs<rhs ? "TRUE" : "FALSE") << "\n";
    std::cout << "generated_certificate=" << output << "\n";
    std::cout << "RESULT=" << (pass ? "PASS" : "FAIL") << "\n";
    return pass ? 0 : 1;
}
