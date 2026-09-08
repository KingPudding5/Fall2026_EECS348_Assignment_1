/* Include the standard input/output library so this file can call printf()
   (writes formatted text to the screen) and scanf() (reads formatted input
   from the keyboard). It takes in nothing and returns nothing; it simply makes
   those function declarations, plus the EOF constant, visible below. */
#include <stdio.h>

/* SECRET_NUMBER: a compile-time constant naming the fixed number the player
   must find. It takes in nothing and returns nothing; every use of the name is
   replaced by the literal 7 before compilation. */
#define SECRET_NUMBER 7

/* MAX_ATTEMPTS: a compile-time constant naming how many guesses the player is
   allowed. It takes in nothing and returns nothing; every use of the name is
   replaced by the literal 3 before compilation. */
#define MAX_ATTEMPTS 3

/* LOWEST_ALLOWED: a compile-time constant naming the smallest legal guess.
   It takes in nothing and returns nothing; the name expands to the literal 1. */
#define LOWEST_ALLOWED 1

/* HIGHEST_ALLOWED: a compile-time constant naming the largest legal guess.
   It takes in nothing and returns nothing; the name expands to the literal 10. */
#define HIGHEST_ALLOWED 10

/* INPUT_ENDED: the sentinel value read_valid_guess() hands back when the input
   stream runs out, chosen because it can never be a legal guess. It takes in
   nothing and returns nothing; the name expands to the literal -1. */
#define INPUT_ENDED (-1)

/* discard_rest_of_line(): throws away every character still sitting in the
   input buffer up to and including the next newline, so that leftover junk
   from a bad entry cannot be re-read as the next guess.
   Takes in: nothing.
   Returns: nothing (void). */
void discard_rest_of_line(void)
{
    /* discarded_character holds one character at a time as it is thrown away;
       it is an int rather than a char so it can also hold the EOF marker. */
    int discarded_character;

    /* Read characters one by one: getchar() takes in nothing and returns the
       next character (or EOF), and the loop keeps going until a newline ends
       the line or the input stream is exhausted. */
    while ((discarded_character = getchar()) != '\n' && discarded_character != EOF) {
        /* Nothing to do with the character; reading it was the whole point, so
           the body stays empty and the loop simply advances the stream. */
    }
}

/* read_valid_guess(): prompts the player and keeps re-prompting until they
   type a whole number between 1 and 10, rejecting letters, symbols, and
   out-of-range numbers without spending one of their three attempts.
   Takes in: attempt_number, the 1-based try being made, used only to print the
   "Attempt N/3" prompt.
   Returns: the validated guess as an int in the range 1..10, or INPUT_ENDED
   (-1) if the input stream closed before a valid guess arrived. */
int read_valid_guess(int attempt_number)
{
    /* guessed_number holds the integer scanf() converts from what the player
       typed; it is only trustworthy once scanf() reports a successful read. */
    int guessed_number;

    /* items_converted holds scanf()'s return value: 1 when it filled
       guessed_number, 0 when the text was not a number, EOF when input ended. */
    int items_converted;

    /* Loop until an acceptable guess is obtained; the function exits by
       returning from inside the loop rather than by a loop condition. */
    while (1) {
        /* Ask for the guess: printf() takes in the format string and
           attempt_number and returns the character count, which is ignored. */
        printf("Attempt %d/%d. Enter your guess: ", attempt_number, MAX_ATTEMPTS);

        /* Try to read one integer: scanf() takes in the "%d" format and the
           address of guessed_number, and returns how many items it converted. */
        items_converted = scanf("%d", &guessed_number);

        /* Detect a closed input stream, which would otherwise make the
           re-prompting loop spin forever with nothing left to read. */
        if (items_converted == EOF) {
            /* Explain why the program is giving up on reading; printf() takes
               the message and returns the character count, which is ignored. */
            printf("\nInput ended unexpectedly.\n");

            /* Hand the sentinel back to the caller so it can stop the game. */
            return INPUT_ENDED;
        }

        /* Detect text that was not a number at all, such as "abc" or "?". */
        if (items_converted == 0) {
            /* Remove the offending characters, which scanf() left untouched in
               the buffer and would keep rejecting on every future read. */
            discard_rest_of_line();

            /* Tell the player what is expected, then loop around to re-prompt
               without charging them an attempt. */
            printf("That is not a whole number. Please enter a number between %d and %d.\n",
                   LOWEST_ALLOWED, HIGHEST_ALLOWED);

            /* Skip the rest of this pass and start the loop again. */
            continue;
        }

        /* A number was read successfully, so drop anything else typed on the
           same line (for example the "xy" in "5xy") before judging the value. */
        discard_rest_of_line();

        /* Check the number against the legal range of guesses. */
        if (guessed_number < LOWEST_ALLOWED || guessed_number > HIGHEST_ALLOWED) {
            /* Report the out-of-range entry, then loop around to re-prompt
               without charging the player an attempt. */
            printf("%d is out of range. Please enter a number between %d and %d.\n",
                   guessed_number, LOWEST_ALLOWED, HIGHEST_ALLOWED);

            /* Skip the rest of this pass and start the loop again. */
            continue;
        }

        /* The value is a whole number inside 1..10, so give it to the caller. */
        return guessed_number;
    }
}

/* main(): runs the guessing game, giving the player up to three tries to name
   the secret number and reporting whether they won or lost.
   Takes in: nothing (void).
   Returns: 0 to the operating system to signal that the program succeeded. */
int main(void)
{
    /* attempt_number counts which try the player is on, from 1 up to 3. */
    int attempt_number;

    /* current_guess holds the validated number the player entered this try. */
    int current_guess;

    /* player_has_won records the outcome: 0 means not yet correct, 1 means the
       secret number was found, and it decides the final message. */
    int player_has_won = 0;

    /* Introduce the game; printf() takes the message and returns the character
       count, which is ignored. */
    printf("Guess a number between %d and %d.\n", LOWEST_ALLOWED, HIGHEST_ALLOWED);

    /* Give the player up to MAX_ATTEMPTS tries, counting attempt_number from 1
       and stopping early on a correct guess or on lost input. */
    for (attempt_number = 1; attempt_number <= MAX_ATTEMPTS; attempt_number++) {
        /* Collect one guess: read_valid_guess() takes the attempt number and
           returns a number in 1..10, or INPUT_ENDED if the stream closed. */
        current_guess = read_valid_guess(attempt_number);

        /* Stop the game immediately if there is no more input to read. */
        if (current_guess == INPUT_ENDED) {
            /* Leave the loop; player_has_won stays 0 so the losing message
               below is printed. */
            break;
        }

        /* Compare the guess with the secret number to decide what to say. */
        if (current_guess == SECRET_NUMBER) {
            /* Announce the win; printf() takes the message and returns the
               character count, which is ignored. */
            printf("Correct! You win!\n");

            /* Record the win so the final message after the loop is the right
               one. */
            player_has_won = 1;

            /* Stop guessing immediately now that the number has been found. */
            break;
        } else if (current_guess < SECRET_NUMBER) {
            /* The guess was below the secret number, so nudge the player up. */
            printf("Too low! Try again.\n");
        } else {
            /* The only case left is a guess above the secret number, so nudge
               the player down. */
            printf("Too high! Try again.\n");
        }
    }

    /* Print the losing message only when the loop finished without a win. */
    if (player_has_won == 0) {
        /* Reveal the answer; printf() takes the message plus SECRET_NUMBER and
           returns the character count, which is ignored. */
        printf("Out of tries! The number was %d. You lose.\n", SECRET_NUMBER);
    }

    /* Return 0 to the operating system to report that the program ran
       successfully. */
    return 0;
}
