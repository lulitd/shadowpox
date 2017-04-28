//
//  poxemon.cpp
//
//  Created by Sean D. Sollé on 11/04/2017.
//  Copyright © 2017 Sean D. Sollé. All rights reserved.
//


#include <string.h>
#include "poxemon.h"

/******************************************************************************

    void poxencode( bool bVaccinated,
                    unsigned int infections,
                    unsigned int deaths,
                    char* output)

    Author:	Sean D. Sollé
    Created:	2017/04/11

    Purpose:	Fill a three character string with a code representing vaccinations, infections and deaths.
    Inputs:     Output character string, player vaccination status, number of deaths & infections.
    Outputs:    Poxemon code, a three character string.

    Encoding vaccination status, infections and deaths into a single code is a two-step process.

    STEP 1:

    Calculate the 'outcome' number.

    With N infections, the number of possible deaths is N+1, since the death count can range
    from zero (no-one dies) to N (everyone dies).

    For example, 5 infections can produce 6 possible death counts: 0, 1, 2, 3, 4, or 5.
 
    If we arrange the possible death counts in a triangle, with one row per infection ...
 
    0
    0 1
    0 1 2
    0 1 2 3
    0 1 2 3 4
    0 1 2 3 4 5
    ...
 
    0 1 2 3  ..   97
    0 1 2 3  ...  97 98
    0 1 2 3  .... 97 98 99
 

    ... then the number of items preceding the row at which 'N' infections start
    is the triangle number of 'N'.

    https://en.wikipedia.org/wiki/Triangular_number

    Since the triangle number of N has the value N(N+1)/2, it can be seen that for
    up to N infections, there are N(N+1)/2 possible outcomes.
 
    So with a range of 0 to 99 possible infections, there are 5050 possible outcomes:

    Infections    Deaths    Outcome
        0           0           0
        1           0           1
        1           1           2
        2           0           3
        2           1           4
        2           2           5
        3           0           6
        3           1           7
            ...         ...
        99          96          5046
        99          97          5047
        99          98          5048
        99          99          5049

    (See poxemon.csv for the complete list).

    The outcome number can be readily calculated by adding the triangle number of infections to
    the number of death reports.

    If the player is vaccinated, we use exactly the same system, but start numbering
    outcomes with a value greater than 5049.

    STEP 2:

    By encoding the outcome number in a higher base, each of the 10100 possibilites
    can be represented with just three characters.

    Since 22^3 = 10648, we can represent all our outcome values using base 22.
    
    By dropping the vowels after 'A', we reduce our alphabet from base 26 to base 22,
    allowing codes from 'AAA' to 'ZZZ'

    If we use half the total values - 5324 - as our offset for vaccinated players,
    their encoding starts on a nice round number - 'NAA' - which also means that
    unvaccinated codes start with 'A-M', and vaccinated with 'N-Z':
 
 
    Infections    Deaths     Outcome    Infected  Protected
         0           0           0         AAA       NAA
         1           0           1         AAB       NAB
         1           1           2         AAC       NAC
         2           0           3         AAD       NAD
         2           1           4         AAF       NAF
         2           2           5         AAG       NAG
         3           0           6         AAH       NAH
         3           1           7         AAJ       NAJ
             ...         ...
        99          96          5046       MLK       ZLK
        99          97          5047       MLL       ZLL
        99          98          5048       MLM       ZLM
        99          99          5049       MLN       ZLN

    By algorithmically generating these codes we avoid the need for huge lookup
    tables, and by using triangle numbers we have a simple way to convert the
    encoded outcome back into its discrete infection and death counts.
 
 ******************************************************************************/


void poxencode(bool bVaccinated, unsigned int infections, unsigned int deaths, char* const output)
{

    // Calculate the triangle number of infections.

    unsigned int triangle = (infections * (infections + 1)) / 2;

    // The triangle number is the row we're starting at, so we add the deaths to find the code number.

    unsigned int code = triangle + deaths;

    // If player is vaccinated, we start numbering with a value after 5050 (the last possible non-vaccinated number).

    if (bVaccinated)
    {
        // Use half the total values - 5324 - as our offset for vaccinated players,
        code += VACC_OFFSET;
    }


    // We only need 22 characters, so we drop all the vowels after 'A'.
    char alphabet[] = ALPHABASE22;

    // With 22 characters, we're encoding in base 22.
    unsigned int base = int(strlen(alphabet));


    // We want to write from the rightmost character (i.e. the least significant digit), and move left.
    // So we start with our index pointing at the terminating null ...

    int index = int(strlen(output));

    // If we've been passed an empty string, we never enter this loop.
    while (index > 0)
    {
        // There's at least one character in the output, so we move one place to the left to overwrite it.
        index--;

        // Store the leastmost significant digit.
        output[index] = alphabet[code % base];

        // And shift the value we're converting one base N place to the right.
        code = code/base;
    }
}

