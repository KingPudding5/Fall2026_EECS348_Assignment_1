#!/usr/bin/env bash
#
# run-tests.sh -- comparison bench for EECS 348 Assignment 1.
#
# PROVENANCE: written by Claude (Anthropic) at my direction, during a session on
# 2026-09-08, and reviewed by me before use. It is a measuring instrument, not a
# grader: it never decides pass or fail, it only runs both programs through the
# same inputs and prints exactly what each one did. Every claim in my analysis
# comes from this output.
#
# Usage:  ./run-tests.sh <fileA.c> <fileB.c>
# Example: ./run-tests.sh claude.c codex.c

set -u

A_SRC="${1:-claude.c}"          # first program's source file
B_SRC="${2:-codex.c}"           # second program's source file
A=$(basename "$A_SRC" .c)       # binary names, derived from the source names
B=$(basename "$B_SRC" .c)

rule() { printf '\n=== %s ===\n' "$1"; }

# ---------------------------------------------------------------------------
# 0. Environment. Recorded first, because every number below is only meaningful
#    against the machine that produced it. This is what makes it possible to
#    compare a run on my laptop against a run on the cycle server.
# ---------------------------------------------------------------------------
rule "ENVIRONMENT"
uname -m                        # processor architecture, e.g. x86_64 or aarch64
gcc --version | head -1         # compiler version
ldd --version | head -1         # C library version

# ---------------------------------------------------------------------------
# 1. Compilation under four different sets of flags. The point is not just
#    "does it build" -- it is that the same file can be silent under one set of
#    flags and complain under another, so the flags you choose decide what you
#    are allowed to find out.
# ---------------------------------------------------------------------------
rule "COMPILE: course command (gcc file.c -o file)"
for s in "$A_SRC" "$B_SRC"; do
    echo "--- $s ---"
    gcc "$s" -o "$(basename "$s" .c)"      # exactly the C Basics lecture command
    echo "exit=$?"                          # 0 means it built
done

rule "COMPILE: -Wall -Wextra (warnings turned up)"
for s in "$A_SRC" "$B_SRC"; do
    echo "--- $s ---"
    gcc -Wall -Wextra "$s" -o /dev/null     # -o /dev/null: build it, throw it away,
done                                        # we only want the diagnostics

rule "COMPILE: -O2 (optimization on)"
for s in "$A_SRC" "$B_SRC"; do
    echo "--- $s ---"
    gcc -O2 "$s" -o /dev/null               # some warnings only appear when gcc
done                                        # analyses deeply enough to optimize

rule "COMPILE: -std=c89 -pedantic (the 1989 rulebook)"
for s in "$A_SRC" "$B_SRC"; do
    echo "--- $s ---"
    gcc -std=c89 -pedantic "$s" -o /dev/null   # catches C99-only syntax
done

# ---------------------------------------------------------------------------
# 2. Behavior. Each case is "label:input". The input uses \n for Enter, and is
#    piped in so no human has to type it -- identical every run, on any machine.
#    timeout 5 kills anything that hangs, so an infinite loop is reported as a
#    hang instead of freezing the whole bench.
# ---------------------------------------------------------------------------
rule "BEHAVIOR"
CASES=(
    "spec example (5,8,7):5\n8\n7\n"
    "win on first try:7\n9\n"
    "lose (1,2,3):1\n2\n3\n"
    "not a number (abc):abc\n7\n"
    "no input at all:"
    "input ends mid-game:5\n"
    "out of range (0,11,7):0\n11\n7\n"
    "negative (-5,7):-5\n7\n"
    "junk after number (5xy):5xy\n7\n"
    "integer overflow (2^32+7):4294967303\n"
)
for prog in "$A" "$B"; do
    printf '\n######## %s ########\n' "$prog"
    for case in "${CASES[@]}"; do
        label="${case%%:*}"                 # text before the first colon
        input="${case#*:}"                  # everything after it
        printf -- '--- %s ---\n' "$label"
        printf "$input" | timeout 5 "./$prog"
        echo "[exit=$?]"                    # 0=success, 124=timed out/hung
    done
done

# ---------------------------------------------------------------------------
# 3. Byte-exact check against the assignment's example transcript. Reading the
#    output and thinking it looks right is not the same as it being right, so
#    diff does the comparison instead of my eyes. Piped input means the guesses
#    are not echoed, so only the program's own output appears.
# ---------------------------------------------------------------------------
rule "SPEC EXAMPLE: byte-exact"
cat > expected.txt <<'EOT'
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Too high! Try again.
Attempt 3/3. Enter your guess: Correct! You win!
EOT
for prog in "$A" "$B"; do
    printf '5\n8\n7\n' | "./$prog" > "got_$prog.txt"
    if diff -q expected.txt "got_$prog.txt" > /dev/null; then
        echo "$prog: BYTE-IDENTICAL to the assignment example"
    else
        echo "$prog: DIFFERS --"
        diff expected.txt "got_$prog.txt"
    fi
done

# ---------------------------------------------------------------------------
# 4. Timing. One run is far too short to measure, so each program runs 100 times
#    and the shell reports the total. `time` gives three numbers: real is wall
#    clock, user is CPU time spent in my code, sys is CPU time the OS spent on
#    my program's behalf. On a shared server only `user` is a fair comparison.
# ---------------------------------------------------------------------------
rule "TIMING: 100 runs each"
for prog in "$A" "$B"; do
    echo "--- $prog ---"
    time ( for _ in $(seq 1 100); do printf '1\n2\n3\n' | "./$prog" > /dev/null; done )
done

# ---------------------------------------------------------------------------
# 5. Memory. Five runs each, because peak memory wobbles between runs and a
#    single measurement can point the wrong way. If the gap between the two
#    programs is smaller than the spread within either one, there is no
#    difference to report.
# ---------------------------------------------------------------------------
rule "MEMORY: peak resident set size, 5 runs each"
for prog in "$A" "$B"; do
    echo "--- $prog ---"
    for _ in 1 2 3 4 5; do
        printf '1\n2\n3\n' | /usr/bin/time -v "./$prog" 2>&1 >/dev/null \
            | grep "Maximum resident"
    done
done

# ---------------------------------------------------------------------------
# 6. Compiled size. `size` reports the segments of the executable: text is the
#    machine instructions themselves, so it is a fair static comparison that
#    does not depend on how busy the machine was.
# ---------------------------------------------------------------------------
rule "COMPILED SIZE"
size "$A" "$B"

# ---------------------------------------------------------------------------
# 7. Uninitialized-value probe. This one only applies to a program that reads a
#    variable it never assigned. It makes a copy of the source with one extra
#    line that prints the value of `guess` BEFORE the game starts, then runs it
#    with different amounts of padding in the environment.
#
#    Why padding? Environment variables are stored just above the program's
#    stack, so making them bigger shifts everything below them. If the printed
#    value changes when only the environment size changed, that value is not
#    coming from the program -- it is leftover memory. That is the whole proof.
# ---------------------------------------------------------------------------
rule "UNINITIALIZED-VALUE PROBE (only meaningful for a program with an unchecked read)"
for s in "$A_SRC" "$B_SRC"; do
    if grep -qE '^[[:space:]]*scanf\(' "$s"; then      # scanf called, result discarded
        echo "--- $s (unchecked scanf found) ---"
        sed 's|printf("Guess a number|fprintf(stderr,"[value being compared = %d]\\n", guess); printf("Guess a number|' \
            "$s" > probe_tmp.c
        if gcc probe_tmp.c -o probe_tmp 2>/dev/null; then
            for pad in 0 64 512 4096; do
                P=$(awk -v n="$pad" 'BEGIN{while(i++<n)printf "x"}')
                printf 'abc\n' | env PAD="$P" ./probe_tmp 2>&1 >/dev/null | head -1 \
                    | sed "s/^/    env pad=${pad}b  -> /"
            done
        else
            echo "    (probe did not build for this file -- skipped)"
        fi
        rm -f probe_tmp.c probe_tmp
    else
        echo "--- $s: scanf result is checked, nothing to probe ---"
    fi
done

# ---------------------------------------------------------------------------
# 8. Where the overflow actually happens. Typing a number too big for an int
#    makes both programs announce a win, and the obvious guess is that the
#    program mishandled it. This checks that guess by asking scanf directly what
#    it returned and what it stored.
# ---------------------------------------------------------------------------
rule "OVERFLOW PROBE: what does scanf hand back?"
cat > ovf_tmp.c <<'EOT'
#include <stdio.h>
int main(void){
    int g = 0;
    int rc = scanf("%d", &g);
    printf("scanf(\"%%d\") returned rc=%d and stored value=%d\n", rc, g);
    return 0;
}
EOT
if gcc ovf_tmp.c -o ovf_tmp 2>/dev/null; then
    printf '  input 4294967303 -> '; printf '4294967303\n' | ./ovf_tmp
    printf '  input 7          -> '; printf '7\n'          | ./ovf_tmp
fi
rm -f ovf_tmp.c ovf_tmp

# ---------------------------------------------------------------------------
# 9. Static size comparison. Wall-clock timing on a shared server is noisy, so
#    counting the actual machine instructions in main() is a fairer measure of
#    how much work each program is. -S stops the compiler at assembly, one line
#    per operation the processor performs.
# ---------------------------------------------------------------------------
rule "INSTRUCTION COUNT in main()"
for opt in 0 2; do
    for s in "$A_SRC" "$B_SRC"; do
        b=$(basename "$s" .c)
        gcc -O$opt -S -o "${b}_O${opt}.s" "$s" 2>/dev/null
        n=$(awk "/^main:/,/^\\.size.*main/" "${b}_O${opt}.s" | grep -cE '^[[:space:]]+[a-z]')
        echo "  $b at -O$opt: $n instructions"
    done
done

# ---------------------------------------------------------------------------
# 10. Line-by-line difference between the two generations. Everything the two
#     models decided differently, in one place.
# ---------------------------------------------------------------------------
rule "SOURCE DIFF"
diff -u "$A_SRC" "$B_SRC"

echo
echo "=== done ==="
