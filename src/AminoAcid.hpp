#ifndef AMINO_ACID_HPP
#define AMINO_ACID_HPP

#include <string>

/**
 * @brief Enumeration for amino acids (in single-letter format) plus X for null values
 */
enum AminoAcid {
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    K,
    L,
    M,
    N,
    P,
    Q,
    R,
    S,
    T,
    V,
    W,
    Y,
    Z,
    X
};

// for ease of conversion
const char readings[] = {
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'K',
    'L',
    'M',
    'N',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'V',
    'W',
    'Y',
    'Z',
    '-'
};

/**
 * @param letter - the amino acid
 * @return its reading
 */
char amino_acid_to_char(AminoAcid letter) { return readings[letter]; }

/**
 * @param letter - the reading of the amino acid
 * @return its capitalised enum
 */
AminoAcid char_to_amino_acid(char letter) {
    for (size_t i = 0; i < sizeof(readings); i++) if (readings[i] == letter) return static_cast<AminoAcid>(i);
    return X;
}

#endif