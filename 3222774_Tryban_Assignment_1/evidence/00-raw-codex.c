#include <stdio.h>

int main(void)
{
    const int secret_number = 7;
    int guess;
    int won = 0;

    printf("Guess a number between 1 and 10.\n");

    for (int attempt = 1; attempt <= 3; attempt++) {
        printf("Attempt %d/3. Enter your guess: ", attempt);

        if (scanf("%d", &guess) != 1) {
            printf("Invalid input. You lose!\n");
            return 0;
        }

        if (guess == secret_number) {
            printf("Correct! You win!\n");
            won = 1;
            break;
        } else if (guess > secret_number) {
            printf("Too high! Try again.\n");
        } else {
            printf("Too low! Try again.\n");
        }
    }

    if (!won) {
        printf("You lose! The secret number was %d.\n", secret_number);
    }

    return 0;
}
