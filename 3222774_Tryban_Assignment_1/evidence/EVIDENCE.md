# Measured comparison — Claude vs Codex raw generations (`00-raw.c`)

Evidence for rubric item 2e. Every number was measured, not estimated.
Raw terminal output: `raw-transcripts.md` (local runs) and `cycle-output.txt`.

## Method and environments

Both raw generations were compiled and run on **two different machines**, using
the compile command from the C Basics lecture (`gcc <file>.c -o <file>`):

| | Machine A | Machine B (graded) |
|---|---|---|
| Arch | aarch64 | **x86_64** |
| OS | Ubuntu 22.04 (Linux VM on my MacBook) | **cycle.eecs.ku.edu** |
| gcc | 11.4.0 | 11.4.0 |
| glibc | — | 2.35 |

Running on both is what makes the correctness section below provable rather than
theoretical. Inputs were piped, so guesses do not appear echoed the way they do
in an interactive terminal.

---

## 1. Correctness

### Both reproduce the spec example exactly

Byte-identical to the transcript in the assignment, on both machines.

### Behavior on inputs the example never shows (cycle server)

| Input | Claude | Codex |
|---|---|---|
| `5, 8, 7` (spec example) | correct | correct |
| `1, 2, 3` | correct loss | correct loss |
| `abc` | burns all 3 attempts on garbage | ends game: "Invalid input. You lose!" |
| no input at all | burns all 3 attempts on garbage | ends game immediately |
| input ends mid-game | burns remaining attempts on garbage | ends game on attempt 2 |
| `0`, `11`, `-5` | accepted as guesses | accepted as guesses |
| `5xy` | reads 5, burns rest on `xy` | reads 5, ends game on `xy` |
| `4294967303` | **"Correct! You win!"** | **"Correct! You win!"** |

### Defect A — shared: integer overflow wins the game

`4294967303` is 2³² + 7, which does not fit in a 32-bit `int`. `scanf("%d")`
wraps it to 7 and **both programs declare a win on attempt 1**:

```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Correct! You win!
```

Reproduced identically on aarch64 and on the x86_64 cycle server, so this is not
a platform quirk. Neither model range-checks the value it read against the 1–10
bound the spec states.

### Defect B — Claude only: unchecked `scanf`, and it is provably non-deterministic

Claude's line 14 is `scanf("%d", &guess);` with the return value discarded. On
non-numeric input `scanf` writes nothing, and `guess` holds whatever was already
on the stack.

**The same source, given the same input `abc`, produces different output on
different machines:**

| Machine | Output for `abc` |
|---|---|
| aarch64 (my laptop VM) | `Too high! Try again.` ×3 |
| **x86_64 (cycle server)** | `Too low! Try again.` ×3 |

Instrumenting a copy to print the value being compared, on one machine, varying
only the size of the environment:

```
env pad=0b     ->  [uninitialized guess = -663556032]
env pad=64b    ->  [uninitialized guess = -676323264]
env pad=512b   ->  [uninitialized guess = 1979629632]
env pad=4096b  ->  [uninitialized guess = 1487097920]
```

The number this program compares against 7 is arbitrary. It changes with the
machine and with the environment. This is undefined behavior, and it is a literal
instance of the warning in the assignment instructions — that "the same code may
produce different outputs in different environments." If that garbage ever lands
on 7, typing `abc` wins the game.

Codex is deterministic on the same input, because it checks the return value.

### Defect C — Codex only: one typo ends the game, and it reports success

Codex checks `scanf`, which is the right instinct, but responds by printing
"Invalid input. You lose!" and `return 0`. A single mistyped character ends the
game with attempts remaining — `5xy` loses on attempt 2, because `xy` is still
sitting in the input buffer. The exit status is `0`, meaning "success", on a
failure path.

### The course's compile command hides Defect B — on both machines

| Compile command | Warns about the unchecked `scanf`? |
|---|---|
| `gcc claude.c -o claude` (the course's command) | **no** |
| `gcc -Wall -Wextra claude.c -o claude` | **no** |
| `gcc -O2 claude.c -o claude` | **yes** |

On the cycle server, at `-O2`:

```
claude.c:14:9: warning: ignoring return value of 'scanf' declared with
attribute 'warn_unused_result' [-Wunused-result]
```

gcc knows about this bug. It stays silent under the exact command we are told to
use, and stays silent even with warnings turned all the way up. It only speaks
once optimization is enabled.

---

## 2. Execution time

Both are O(n) with n capped at 3 guesses. No loop over data, so cost is constant
in practice.

100 runs of the losing path, piped input, on the cycle server:

| | real | user | sys |
|---|---|---|---|
| Claude | 0.146s | 0.132s | 0.071s |
| Codex | 0.138s | 0.132s | 0.063s |

**User time is identical to the millisecond.** The entire 8 ms difference is
system time, on a shared multi-user server, which is scheduling and I/O rather
than anything either program does. There is no meaningful execution-time
difference between these two programs, and claiming one would be reading signal
into noise.

Static comparison (aarch64, instructions in `main`): Claude 74 at -O0 / 67 at
-O2; Codex 81 / 72. Codex is 5–7 instructions larger — the cost of the `scanf`
check.

---

## 3. Space complexity

O(1) for both. Every variable is a fixed-size scalar; nothing grows with input.

`/usr/bin/time -v`, cycle server, **5 runs each**:

| run | Claude (KB) | Codex (KB) |
|---|---|---|
| 1 | 1692 | 1572 |
| 2 | 1580 | 1680 |
| 3 | 1576 | 1672 |
| 4 | 1664 | 1672 |
| 5 | 1700 | 1676 |
| **mean** | **1642** | **1654** |
| spread | 124 | 108 |

**There is no memory difference between these two programs.** The 12 KB gap
between the means is roughly a tenth of the run-to-run spread within either
program, and the ranges overlap almost entirely.

This is worth recording as a method note, not just a result. A single run
measured earlier gave Claude 1672 KB and Codex 1580 KB — a 92 KB gap pointing the
opposite direction from the means above. Had I written the analysis from that one
measurement I would have confidently reported a difference that does not exist.
Page-fault counts behave the same way (72–75, overlapping).

Compiled text segment, cycle server: Claude 2117 bytes, Codex 2161 (+44). Same
direction as aarch64 (2420 vs 2473, +53) — Codex is consistently ~2% larger.

---

## 4. Maintainability

### Portability: Codex will not build under strict C89

On the cycle server:

```
codex.c:11:5: error: 'for' loop initial declarations are only allowed in C99
or C11 mode
```

Claude compiles clean under C89, C99, and the default. Codex declares its loop
variable inside the `for` header, which is C99+. Cycle's gcc 11.4 defaults to a
C17-family standard so this does not bite for this assignment — but it is a real
constraint, and I only know it does not bite because I compiled there.

### `const int` vs plain `int` costs nothing here

Codex writes `const int secret_number = 7`; Claude writes `int secret = 7`. At
-O0 both emit the identical instruction (`mov w0, 7` on aarch64). `const` buys
readability and intent at zero runtime cost for a local scalar.

### Naming

Codex's `secret_number` reads better than Claude's `secret`. Minor, but free.

### Branch order

Claude tests `guess < secret` first; Codex tests `guess > secret_number` first.
Functionally identical — a coin flip the two models happened to call differently.
Worth noting because it shows how much divergence between two models is arbitrary
rather than reasoned.

### Neither documents anything

Neither raw file has a prolog header, an author, a date, or any statement of
where the code came from. Neither contains a single comment. Against this
course's rubric that is 35 points missing before the program is even evaluated.

---

## Verdict

**Codex's generation is the better starting point**, for one reason that
outweighs everything else: it checks the return value of `scanf`. That single
check is the difference between defined and undefined behavior — and undefined
behavior is not a style complaint, it means the program's output is not
determined by its source code. I have the two-machine output to prove it.

Codex's faults are all a few lines to fix: turn the instant loss into a
re-prompt, add the 1–10 range check that closes the overflow win, and return a
non-zero status on the failure path. Fixing Claude means first adding the check
Codex already has.

What neither model did, and what the improvement work has to add: validate the
range, survive a typo without ending the game, tell the OS the truth on exit,
and document who wrote the code.
