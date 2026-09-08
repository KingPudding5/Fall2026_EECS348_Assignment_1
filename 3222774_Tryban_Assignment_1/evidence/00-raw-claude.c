#include <stdio.h>

int main(void)
{
    int secret = 7;
    int guess;
    int attempt;
    int won = 0;

    printf("Guess a number between 1 and 10.\n");

    for (attempt = 1; attempt <= 3; attempt++) {
        printf("Attempt %d/3. Enter your guess: ", attempt);
        scanf("%d", &guess);

        if (guess == secret) {
            printf("Correct! You win!\n");
            won = 1;
            break;
        } else if (guess < secret) {
            printf("Too low! Try again.\n");
        } else {
            printf("Too high! Try again.\n");
        }
    }

    if (won == 0) {
        printf("Out of tries! The number was %d. You lose.\n", secret);
    }

    return 0;
}
