#!/usr/bin/env bash
set -euo pipefail

CXX="${CXX:-g++}"
CXXFLAGS="${CXXFLAGS:--O2 -std=c++17}"

$CXX $CXXFLAGS reproduce_n497_search.cpp -o reproduce_n497_search
$CXX $CXXFLAGS check_n497_embedded_certificate.cpp -o check_n497
$CXX $CXXFLAGS check_n281_base.cpp -o check_n281

./reproduce_n497_search n497_reproduced_certificate.json | tee n497_search_rerun_log.txt
cmp -s n497_reproduced_certificate.json n497_certificate.json
printf 'certificate_byte_match=PASS\n' | tee -a n497_search_rerun_log.txt

./check_n497 | tee n497_checker_rerun_log.txt
cmp -s n497_checker_rerun_log.txt n497_checker_log.txt
printf 'n497_checker_log_match=PASS\n'

./check_n281 | tee n281_checker_rerun_log.txt
cmp -s n281_checker_rerun_log.txt n281_checker_log.txt
printf 'n281_checker_log_match=PASS\n'

printf 'FULL_REPRODUCTION=PASS\n'
