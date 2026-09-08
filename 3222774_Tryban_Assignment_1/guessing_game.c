/*
 * Program name: Number Guessing Game
 * Description: Gives the player three chances to guess a fixed number from 1 to 10.
 * Inputs: Whole-number guesses entered at the keyboard.
 * Outputs: Prompts, comparison hints, input errors, and a final win or lose message.
 * Author: OpenAI Codex
 * Date: 2026-09-08
 * Help received from: GPT-5 and the GCC compiler.
 */

#include <stdio.h> /* Ask C's preprocessor to include the declarations for printf, scanf, and getchar before compilation. */

int main(void) /* Start the program in main; int is its exit-status type, and void says main needs no arguments. */
{ /* Group all of the work that belongs to the main function. */
    const int secret_number = 7; /* Keep the answer fixed as required; const prevents an accidental change during the game. */
    const int maximum_attempts = 3; /* Name the limit so the loop and messages cannot silently disagree about it. */
    int attempt = 1; /* Begin counting at one because that is how the player sees attempt numbers. */
    int guess = 0; /* Give the guess a safe initial value because C variables do not initialize themselves as Python values do. */
    int scan_result = 0; /* Save scanf's return value so input is trusted only when one requested value was read. */
    int input_character = 0; /* Hold each leftover character as an int because getchar also needs the special EOF value. */
    int has_extra_text = 0; /* Remember whether anything besides harmless spacing followed the integer on its line. */
    int won = 0; /* Use zero for "not yet" so the final message can distinguish a win from three wrong guesses. */

    printf("Guess a number between 1 and 10.\n"); /* Explain the valid range before asking the player for input. */

    while (attempt <= maximum_attempts) /* Repeat until three valid guesses are used; invalid entries are re-prompted instead. */
    { /* Keep one complete prompt, validation, and comparison cycle together. */
        printf("Attempt %d/%d. Enter your guess: ", attempt, maximum_attempts); /* Use %d as a placeholder where printf displays each whole number. */
        scan_result = scanf("%11d", &guess); /* Ask for one integer; &guess gives scanf the address where it may store that value. */

        if (scan_result == EOF) /* Detect an ended input stream, which cannot be repaired by clearing and re-prompting. */
        { /* Separate an unavailable input stream from ordinary invalid text. */
            printf("Input ended before the game could finish.\n"); /* Tell the player why no win-or-lose result can be produced. */
            return 1; /* Send a nonzero status to the operating system because the program had to give up. */
        } /* Finish handling the end-of-input case before inspecting any characters. */

        has_extra_text = 0; /* Reset line validation because each new entry deserves an independent check. */
        input_character = getchar(); /* Read the next character so unwanted input does not remain for the next attempt. */

        while (input_character != '\n' && input_character != EOF) /* Clear through the line ending while also checking all trailing text. */
        { /* Apply the same trailing-character rule to every character left on this input line. */
            if (input_character != ' ' && input_character != '\t' && input_character != '\r') /* Permit common spacing but reject another value or word. */
            { /* Record that the line was not just one integer with optional spacing. */
                has_extra_text = 1; /* Make entries such as 5abc invalid instead of accepting only their numeric prefix. */
            } /* Finish classifying this leftover character. */

            input_character = getchar(); /* Advance toward the line ending so no bad characters contaminate the next scanf call. */
        } /* Stop clearing once the whole line is consumed or the input stream ends. */

        if (scan_result != 1) /* Reject the guess when scanf did not return the one successful conversion requested. */
        { /* Handle nonnumeric input without spending one of the player's three valid attempts. */
            printf("Invalid input. Enter an integer between 1 and 10.\n"); /* State both the type and range needed on the re-prompt. */
            continue; /* Restart the while loop now; continue was chosen over nested code to keep invalid input separate. */
        } /* Finish recovery from a failed integer conversion. */
        else if (has_extra_text != 0) /* Reject a numeric prefix followed by other text because the complete entry is not one integer. */
        { /* Give mixed input the same re-prompt behavior as entirely nonnumeric input. */
            printf("Invalid input. Enter one integer between 1 and 10.\n"); /* Clarify that only one clean integer belongs on the line. */
            continue; /* Preserve the current attempt number because no valid guess was supplied. */
        } /* Finish handling unwanted text after an integer. */
        else if (guess < 1 || guess > 10) /* Enforce both ends of the range after scanf has safely written the integer. */
        { /* Keep out-of-range numbers from counting as guesses, as the required re-prompt implies. */
            printf("Invalid input. Enter an integer between 1 and 10.\n"); /* Remind the player of the accepted boundaries. */
            continue; /* Ask for the same attempt again rather than reducing the three valid chances. */
        } /* Finish validating the range before comparing with the secret. */

        if (guess == secret_number) /* Test equality first so a correct guess can end the loop immediately. */
        { /* Keep the winning actions together. */
            won = 1; /* Preserve the outcome for the final-message decision after the loop. */
            printf("Correct! You win!\n"); /* Supply the required final win message as soon as the answer matches. */
            break; /* Exit the loop immediately; break was chosen instead of waiting for its condition to be checked again. */
        } /* Finish the correct-guess branch. */
        else if (guess < secret_number) /* Compare smaller guesses separately so the player receives the useful direction. */
        { /* Handle the too-low result without changing the secret number. */
            if (attempt < maximum_attempts) /* Mention another try only when at least one valid attempt remains. */
            { /* Use the encouraging form before the final attempt. */
                printf("Too low! Try again.\n"); /* Tell the player to raise the next guess. */
            } /* Finish the message used when another guess is available. */
            else /* Choose a truthful final-attempt message when no retry remains. */
            { /* Avoid promising another turn after the third valid guess. */
                printf("Too low!\n"); /* Still provide the required comparison hint for the last guess. */
            } /* Finish the last-attempt version of the low hint. */
        } /* Finish all handling for a guess below the secret. */
        else /* The validated guess can only be too high after equality and too-low cases have failed. */
        { /* Handle the remaining comparison without repeating an unnecessary greater-than test. */
            if (attempt < maximum_attempts) /* Mention another try only when the loop will actually allow one. */
            { /* Use the retry message before all attempts have been spent. */
                printf("Too high! Try again.\n"); /* Tell the player to lower the next guess. */
            } /* Finish the high message used when another guess is available. */
            else /* Choose the final-attempt wording after the third valid guess. */
            { /* Keep the last hint accurate without suggesting a fourth attempt. */
                printf("Too high!\n"); /* Provide the required comparison result for the final guess. */
            } /* Finish the last-attempt version of the high hint. */
        } /* Finish all handling for a guess above the secret. */

        attempt = attempt + 1; /* Spend exactly one attempt now that a valid but incorrect guess was processed. */
    } /* End the guessing loop after a win or after three valid guesses. */

    if (won == 0) /* Print the losing result only when the correct branch never set the win flag. */
    { /* Keep the final loss output out of successful games. */
        printf("You lose! The secret number was %d.\n", secret_number); /* Give closure and use %d to insert the fixed answer. */
    } /* Finish the required final lose message. */

    return 0; /* Return 0 to tell the operating system that the game completed normally, whether won or lost. */
} /* End the main function and therefore the program. */
