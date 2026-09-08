#include <stdio.h> // Provides printf, scanf, getchar, and EOF; takes no runtime input and returns declarations for standard input/output functions.

int main(void) { // Starts the program, takes no command-line arguments, and returns an integer exit status to the operating system.
    const int secret_number = 7; // Stores the fixed number the user must guess, takes the value 7, and provides that value without allowing changes.
    const int maximum_attempts = 3; // Stores the allowed number of valid guesses, takes the value 3, and provides the loop limit.
    int attempts_used = 0; // Tracks valid guesses already made, starts with 0, and provides the current attempt count.
    int user_guess = 0; // Stores the user's latest valid integer input, starts with 0, and provides the value to compare with the secret number.
    int scan_result = 0; // Stores scanf's result, starts with 0, and provides the number of successfully read values or EOF.
    int discarded_character = 0; // Stores one unwanted input character at a time, starts with 0, and provides the character used while clearing invalid input.
    int guessed_correctly = 0; // Tracks whether the secret was guessed, starts false as 0, and provides the final win-or-lose condition.

    printf("Guess a number between 1 and 10.\n"); // Prints the game instructions, takes the instruction string, and returns the number of characters printed.

    while (attempts_used < maximum_attempts && guessed_correctly == 0) { // Repeats while valid attempts remain and no win occurred, takes both conditions, and returns control to the body when both are true.
        printf("Attempt %d/%d. Enter your guess: ", attempts_used + 1, maximum_attempts); // Prompts for the next valid attempt, takes the attempt numbers, and returns the number of characters printed.
        scan_result = scanf("%d", &user_guess); // Reads one decimal integer into user_guess, takes the format and variable address, and returns 1 on success, 0 on invalid text, or EOF when input ends.

        if (scan_result == EOF) { // Checks whether input ended, takes scanf's result, and selects this block when no more input is available.
            printf("Input ended before all guesses were entered.\n"); // Reports the unavailable input, takes the message string, and returns the number of characters printed.
            break; // Stops the guessing loop immediately, takes no input, and returns control to the first statement after the loop.
        } // Ends the end-of-input branch, takes no input, and returns control to the next condition when the branch does not break.

        if (scan_result != 1) { // Checks for non-integer input, takes scanf's result, and selects this block when an integer was not read.
            printf("Invalid input. Enter an integer between 1 and 10.\n"); // Explains the valid input range, takes the error string, and returns the number of characters printed.
            do { // Begins clearing the invalid input line, takes no input, and returns control to its body at least once.
                discarded_character = getchar(); // Reads one character from standard input, takes no arguments, and returns that character or EOF.
            } while (discarded_character != '\n' && discarded_character != EOF); // Continues clearing until a newline or EOF, takes the last character conditions, and returns control to the next statement when clearing is complete.
            continue; // Restarts the loop without using an attempt, takes no input, and returns control to the while condition.
        } // Ends the non-integer branch, takes no input, and returns control to range validation after a successful scan.

        if (user_guess < 1 || user_guess > 10) { // Checks whether the integer is outside 1 through 10, takes user_guess, and selects this block for an out-of-range value.
            printf("Invalid input. Enter an integer between 1 and 10.\n"); // Repeats the valid range, takes the error string, and returns the number of characters printed.
            continue; // Restarts the loop without using an attempt, takes no input, and returns control to the while condition.
        } // Ends range validation, takes no input, and returns control to count and evaluate a valid guess.

        attempts_used++; // Counts the valid guess, takes the current count, and produces the count increased by one.

        if (user_guess == secret_number) { // Compares the guess with the secret, takes both integer values, and selects this block when they are equal.
            guessed_correctly = 1; // Records the win, takes the true value 1, and provides the final successful game state.
            printf("Correct! You win!\n"); // Prints the final win message, takes the message string, and returns the number of characters printed.
            break; // Stops guessing immediately after the correct answer, takes no input, and returns control after the loop.
        } else if (user_guess < secret_number) { // Checks whether an incorrect guess is low, takes the guess and secret, and selects this block when the guess is smaller.
            if (attempts_used < maximum_attempts) { // Checks whether another attempt remains, takes both attempt counts, and selects this block when the user can try again.
                printf("Too low! Try again.\n"); // Prints low-guess guidance, takes the message string, and returns the number of characters printed.
            } else { // Selects the last-attempt low-guess case, takes the failed prior condition, and returns control to this alternative block.
                printf("Too low!\n"); // Prints the final low-guess result, takes the message string, and returns the number of characters printed.
            } // Ends the remaining-attempt choice, takes no input, and returns control to the guessing loop.
        } else { // Selects the incorrect high-guess case, takes the failed equal and low conditions, and returns control to this alternative block.
            if (attempts_used < maximum_attempts) { // Checks whether another attempt remains, takes both attempt counts, and selects this block when the user can try again.
                printf("Too high! Try again.\n"); // Prints high-guess guidance, takes the message string, and returns the number of characters printed.
            } else { // Selects the last-attempt high-guess case, takes the failed prior condition, and returns control to this alternative block.
                printf("Too high!\n"); // Prints the final high-guess result, takes the message string, and returns the number of characters printed.
            } // Ends the remaining-attempt choice, takes no input, and returns control to the guessing loop.
        } // Ends guess comparison, takes no input, and returns control to the loop condition.
    } // Ends the guessing loop, takes no input, and returns control after three failed attempts, a win, or ended input.

    if (guessed_correctly == 0) { // Checks the final game state, takes the win flag, and selects this block when the user did not win.
        printf("You lose! The secret number was %d.\n", secret_number); // Prints the final lose message and answer, takes the secret number, and returns the number of characters printed.
    } // Ends the lose-message branch, takes no input, and returns control to program termination.

    return 0; // Reports successful program completion, takes the status value 0, and returns it to the operating system.
} // Ends main, takes no input, and returns the status supplied by its return statement.
//seeing if this makes diff
