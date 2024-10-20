#include <stdio.h>
#include <stdbool.h>

// Description: Identify count of numbers with repeating digits between
// a defined floor and ceiling
// Input: stdin int floor, stdin int ceiling
// Output: Count of numbers between (inclusive) floor and ceiling which
// have at least one digit repeated

// Check if a number has repeating digits using a bitmask
int hasRepeatingDigits(int num)
{
    // Initialize bitmask
    // Visualization:
    // 0000000000 : 9876543210
    // 0000000100 : 2 is present
    int digitMask = 0;

    while (num > 0)
    {
        // Get current focus digit
        int digit = num % 10;
        // Create bitmask for digit via binary shift
        int digitBit = 1 << digit;

        // Check if digit has already been seen (bit is set in digitMask)
        if (digitMask & digitBit)
        {
            return 1;
        }

        // Mark digit as seen, update the mask
        digitMask |= digitBit;

        // Increment to next digit
        num /= 10;
    }

    return 0;
}

int getCount(int floor, int ceiling)
{
    int count = 0;

    for (int i = floor; i <= ceiling; i++)
    {
        count += hasRepeatingDigits(i);
    }

    return count;
}

int main()
{
    int floor = 0;
    int ceiling = 100;

    // Get floor from stdin
    printf("Check repeating digits floor: ");
    if (scanf("%d", &floor) != 1)
    {
        printf("Invalid input for floor\n");
        return 1;
    }

    // Get ceiling from stdin
    printf("Check repeating digits ceiling: ");
    if (scanf("%d", &ceiling) != 1)
    {
        printf("Invalid input for ceiling\n");
        return 1;
    }

    // Call getCount and print the result
    printf("Between %i and %i (inclusive) there are %i numbers with repeating digits!\n", floor, ceiling, getCount(floor, ceiling));

    return 0;
}