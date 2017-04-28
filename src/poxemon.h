//
//  poxemon.h
//  Poxemon
//
//  Created by Sean D. Sollé on 27/04/2017.
//  Copyright © 2017 Sean D. Sollé. All rights reserved.
//

#ifndef poxemon_h
#define poxemon_h

// We only need 22 characters, so we drop all the vowels after 'A'
#define ALPHABASE22 "ABCDFGHJKLMNPQRSTVWXYZ"

// If we use half the total values as our offset for vaccinated players,
// the encoding starts on a nice round number - 'NAA'
#define VACC_OFFSET 5324

void poxencode(bool bVaccinated, unsigned int infections, unsigned int deaths, char* const output);

#endif /* poxemon_h */
