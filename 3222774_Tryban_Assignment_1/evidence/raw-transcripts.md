### claude — spec example (5,8,7)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Too high! Try again.
Attempt 3/3. Enter your guess: Correct! You win!
```

### claude — non-numeric: abc
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Too low! Try again.
Attempt 3/3. Enter your guess: Too low! Try again.
Out of tries! The number was 7. You lose.
```

### claude — no input at all (immediate EOF)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Too low! Try again.
Attempt 3/3. Enter your guess: Too low! Try again.
Out of tries! The number was 7. You lose.
```

### claude — EOF mid-game (5 then stream ends)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Too low! Try again.
Attempt 3/3. Enter your guess: Too low! Try again.
Out of tries! The number was 7. You lose.
```

### claude — out of range (0, 11, 7)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Too high! Try again.
Attempt 3/3. Enter your guess: Correct! You win!
```

### claude — negative (-5, 7)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Correct! You win!
```

### claude — junk after number (5xy, 7)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Too low! Try again.
Attempt 3/3. Enter your guess: Too low! Try again.
Out of tries! The number was 7. You lose.
```

### claude — integer overflow (4294967303)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Correct! You win!
```

### codex — spec example (5,8,7)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Too high! Try again.
Attempt 3/3. Enter your guess: Correct! You win!
```

### codex — non-numeric: abc
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Invalid input. You lose!
```

### codex — no input at all (immediate EOF)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Invalid input. You lose!
```

### codex — EOF mid-game (5 then stream ends)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Invalid input. You lose!
```

### codex — out of range (0, 11, 7)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Too high! Try again.
Attempt 3/3. Enter your guess: Correct! You win!
```

### codex — negative (-5, 7)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Correct! You win!
```

### codex — junk after number (5xy, 7)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Too low! Try again.
Attempt 2/3. Enter your guess: Invalid input. You lose!
```

### codex — integer overflow (4294967303)
```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: Correct! You win!
```

