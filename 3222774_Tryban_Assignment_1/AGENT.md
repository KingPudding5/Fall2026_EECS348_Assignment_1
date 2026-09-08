# AGENT.md

Read PROMPT.md for the task. These rules come from the EECS 348 grading rubric
and apply to every assignment in this course.

## Boundaries

- PROMPT.md is frozen. Never edit it, not even a typo.
- Write only the file you were told to write. No extra scripts, configs, or notes.
- Never edit or overwrite a previous iteration. Each pass is a new numbered file.

## When the spec is silent

- Choose a behavior, implement it, and state the choice in your reply.
- Never present your choice as though the spec required it.

## The program (30 pts)

- Reproduce the example output in PROMPT.md exactly, character for character.
- Handle inputs the example does not show. Edge cases outside the example are
  graded, not extra credit. At minimum, handle:
  - text that is not a number ("abc")
  - no input at all, and input that ends mid-game (EOF)
  - numbers outside the range the spec states (0, 11)
  - a number too large to fit in an int, which can silently wrap around and be
    accepted as a valid guess
  - extra characters typed after a valid number ("5xy")
- Use the exact C constructs the spec names. If it says scanf, use scanf.
- The program must never hang, loop forever, or keep running with a garbage
  value when the user types something other than what was asked for.
- Every input read is checked for failure before its result is used.

## Prolog header (7 pts)

Every file opens with one comment block, before any code, containing all of:

- Program name (e.g. "EECS 348 Assignment 1")
- Brief description of what the program does
- Inputs
- Output
- All collaborators
- Other sources for the code (ChatGPT, Claude, Codex, StackOverflow, etc.)
- Author's full name
- Creation date
- Revision date
- Revisions made

A file with no prolog block loses these points automatically. Write it first.

## Comments in the code (14 pts)

- Every line's purpose must be clear, either from a comment on that line or
  from a block comment above a group of lines.
- Prefer one block comment above a group of related lines over a comment on
  every single line.
- Never write a comment that restates the code in different words. If deleting
  the comment loses no information, it was noise. "increment i" above `i++` is
  noise. So is describing what every standard library call takes and returns.
- Every block states who wrote it: the author, a source named in the prolog, a
  collaborator, or a combination of those.

## Collaborators (14 pts)

- Every source that contributed to the code is named in the comments.
- Internet sources get a URL. People get a full name. AI tools get named.
- This includes AI that wrote a helper script, a table, or a snippet you kept.
- Never invent a collaborator, and never omit a real one.

## Evidence to report (feeds the 35-pt analysis)

When you finish an iteration, report, in your reply:

- The exact compile command and its full output.
- A run of the example transcript from PROMPT.md, pasted verbatim.
- A run of every edge case listed above, pasted verbatim, showing what happened.
- Timing: the raw output of `time ./program`, not a summary.
- Memory: the raw output of the system's resource-usage tool
  (`/usr/bin/time -l` on macOS, `/usr/bin/time -v` on Linux).
- Time and space complexity in big-O, with one sentence of reasoning each.
- Maintainability: name the specific choices you made and why — variable
  naming, macro vs const, loop condition, how you represent true/false.

Never report a number you did not actually measure. If you could not measure it,
say so.

## Environment

- The only grading environment is the EECS cycle servers
  (cycle1 through cycle4.eecs.ku.edu) or the Eaton Hall lab machines.
- Compile with: gcc <file>.c -o <file>
- Code that works elsewhere but not there does not count. Never claim it runs on
  the cycle servers unless it was actually run there.

## Before you say it's done

- Compile it and run it. State the exact command you ran and what happened.
- Never say something works if you did not run it. If you couldn't run it, say so.
- List anything you were unsure about.
