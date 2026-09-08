# AGENT.md

Standing rules for this folder. PROMPT.md says what the program must do. This file says how it must be written. Both apply. If they ever conflict, PROMPT.md wins.

## Who is reading this code

Someone with about one year of Python and no C. Write for that reader.

## Comments

- Every line of code gets a comment.
- The comment says why the line is there, not what it says. `int guess;` is not "declare guess" — it is "reserve a slot for one whole number; C makes you name the type up front, Python doesn't."
- The first time a piece of C notation shows up, explain it: `#include`, `int main(void)`, `&guess`, `%d`, `break`, `return 0`, and what a function's return value is being used for.
- Where there was a real choice, name the option you didn't take and why this one won.
- Put no name, initials, model tag, or date on individual comments.

## Header

Every .c file opens with one block comment holding: program name, one-line description, inputs, outputs, author, date, and a line naming any person, model, or tool that helped. Once, at the top, nowhere else.

## Correctness

- Give every variable a value on the line you declare it. Never read a variable you have not written to.
- Check what every input call hands back. `scanf` returns how many values it actually read — compare that to how many you asked for before you trust the variable.
- On bad input, do not quit early unless PROMPT.md says to. Clear the leftover characters out of the input buffer and let the user spend the attempts they still have.
- Exit status 0 means the program did its job. Return non-zero if it gave up.
- Declare variables at the top of a block, not inside `for (...)`, so the file compiles under old and new C alike.
- It must compile silently under `gcc -Wall -Wextra -O2 file.c -o file`.

## Scope

Write only the file PROMPT.md names. Do not create, rename, or edit anything else. Do not add features nobody asked for.