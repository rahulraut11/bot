// headers
#include <stdio.h>

// definitions
#define U64 unsigned long long
// constants
const U64 not_a_file = 18374403900871474942ULL ;
const U64 not_h_file = 9187201950435737471ULL ;
const U64 not_ab_file = 18229723555195321596ULL ;
const U64 not_hg_file = 4557430888798830399ULL ;

// macros
#define get_bit(bitboard, square) (bitboard & (1ULL << square))
#define set_bit(bitboard, square) (bitboard |=(1ULL << square)) 
#define pop_bit(bitboard, square) (get_bit(bitboard,square) ? bitboard ^= (1ULL << square) : 0)
#define count_bits(bitboard) __builtin_popcountll(bitboard)
#define get_lsb_index(bb) __builtin_ctzll(bb)

enum{
    a8, b8, c8, d8, e8, f8, g8, h8, 
    a7, b7, c7, d7, e7, f7, g7, h7, 
    a6, b6, c6, d6, e6, f6, g6, h6, 
    a5, b5, c5, d5, e5, f5, g5, h5, 
    a4, b4, c4, d4, e4, f4, g4, h4, 
    a3, b3, c3, d3, e3, f3, g3, h3, 
    a2, b2, c2, d2, e2, f2, g2, h2, 
    a1, b1, c1, d1, e1, f1, g1, h1 
};

enum{
    white, black
    };

const char *square_to_cords[] = { 
 "a8" , "b8", "c8", "d8", "e8", "f8", "g8", "h8", 
 "a7" , "b7", "c7", "d7", "e7", "f7", "g7", "h7", 
 "a6" , "b6", "c6", "d6", "e6", "f6", "g6", "h6", 
 "a5" , "b5", "c5", "d5", "e5", "f5", "g5", "h5", 
 "a4" , "b4", "c4", "d4", "e4", "f4", "g4", "h4", 
 "a3" , "b3", "c3", "d3", "e3", "f3", "g3", "h3", 
 "a2" , "b2", "c2", "d2", "e2", "f2", "g2", "h2", 
 "a1" , "b1", "c1", "d1", "e1", "f1", "g1", "h1" 
};

// Attacks 

// bishop relevant occupancy bit count for every square on board
const int bishop_relevant_bits[64] = {
    6, 5, 5, 5, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 9, 9, 7, 5, 5, 
    5, 5, 7, 7, 7, 7, 5, 5, 
    5, 5, 5, 5, 5, 5, 5, 5, 
    6, 5, 5, 5, 5, 5, 5, 6
};

// rook relevant occupancy bit count for every square on board
const int rook_relevant_bits[64] = {
    12, 11, 11, 11, 11, 11, 11, 12, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    11, 10, 10, 10, 10, 10, 10, 11, 
    12, 11, 11, 11, 11, 11, 11, 12
};

//  pawn attacks table [side][square]
U64 pawn_attacks[2][64] ;
// knight attacks table (no side req)
U64 knight_attacks[64] ;
// king attacks table 
U64 king_attacks[64] ;  

// generate pawn attacks 
U64 mask_pawn_attacks(int side, int square) 
{
    U64 attacks = 0ULL , bitboard = 0ULL ;
    set_bit(bitboard,square);

    //white 
    if(side==0){
        if((bitboard>>7) & not_a_file) attacks |= (bitboard >> 7) ;
        if((bitboard>>9) & not_h_file) attacks |= (bitboard >> 9) ;
    }
    //black
    else{
        if((bitboard<<7) & not_h_file) attacks |= (bitboard << 7) ;
        if((bitboard<<9) & not_a_file) attacks |= (bitboard << 9) ;
    }
    return attacks ;
}

// generate knight attacks
U64 mask_knight_attacks(int square){

    U64 attacks = 0ULL , bitboard = 0ULL ;
    set_bit(bitboard,square);

    if((bitboard >> 17) & not_h_file ) attacks |= (bitboard >> 17) ;
    if((bitboard >> 15) & not_a_file ) attacks |= (bitboard >> 15) ;
    if((bitboard >> 10) & not_hg_file) attacks |= (bitboard >> 10) ;
    if((bitboard >> 6 ) & not_ab_file) attacks |= (bitboard >> 6) ;
    
    if((bitboard << 17) & not_a_file ) attacks |= (bitboard << 17) ;
    if((bitboard << 15) & not_h_file ) attacks |= (bitboard << 15) ;
    if((bitboard << 10) & not_ab_file) attacks  |= (bitboard << 10) ;
    if((bitboard << 6 ) & not_hg_file) attacks |= (bitboard << 6) ;


    return attacks ;
}

// generate king attacks 
U64 mask_king_attacks(int square){

    U64 attacks = 0ULL , bitboard = 0ULL ;
    set_bit(bitboard,square);

    if((bitboard<<1) & not_a_file) attacks |= (bitboard<<1) ; 
    if((bitboard>>1) & not_h_file) attacks |= (bitboard>>1) ; 
    attacks |= (bitboard<<8) ; 
    attacks |= (bitboard>>8) ;
    if((bitboard<<9) & not_a_file) attacks |= (bitboard<<9) ; 
    if((bitboard>>9) & not_h_file) attacks |= (bitboard>>9) ; 
    if((bitboard<<7) & not_h_file) attacks |= (bitboard<<7) ; 
    if((bitboard>>7) & not_a_file) attacks |= (bitboard>>7) ; 

    return attacks;
}

// generate bishop mask attacks
U64 mask_bishop_attacks(int square){
    U64 attacks = 0ULL ;

    int rank = square/8 , file = square%8 ;

    for(int r = rank + 1 , f = file + 1 ; r<=6 && f<=6 ; r++ , f++ ) set_bit(attacks, r*8 + f);
    for(int r = rank - 1 , f = file + 1 ; r>=1 && f<=6 ; r-- , f++ ) set_bit(attacks, r*8 + f);
    for(int r = rank + 1 , f = file - 1 ; r<=6 && f>=1 ; r++ , f-- ) set_bit(attacks, r*8 + f);
    for(int r = rank - 1 , f = file - 1 ; r>=1 && f>=1 ; r-- , f-- ) set_bit(attacks, r*8 + f);

    return attacks ;
}
// generate rook mask attacks
U64 mask_rook_attacks(int square){
    U64 attacks = 0ULL ;

    int r, f;
    
    int tr = square / 8;
    int tf = square % 8;
    
    for (r = tr + 1; r <= 6; r++) attacks |= (1ULL << (r * 8 + tf));
    for (r = tr - 1; r >= 1; r--) attacks |= (1ULL << (r * 8 + tf));
    for (f = tf + 1; f <= 6; f++) attacks |= (1ULL << (tr * 8 + f));
    for (f = tf - 1; f >= 1; f--) attacks |= (1ULL << (tr * 8 + f));

    return attacks ;
}
// generate bishop attacks
U64 bishop_attacks(U64 bitboard, int square){
    U64 attacks = 0ULL ;

    int rank = square/8 , file = square%8 ;

    for(int r = rank + 1 , f = file + 1 ; r<=7 && f<=7 ; r++ , f++ ) 
    {
        set_bit(attacks, r*8 + f);
        if((1ULL << r*8 + f) & bitboard) break ;
    }
    for(int r = rank + 1 , f = file - 1 ; r<=7 && f>=0 ; r++ , f-- ) 
    {
        set_bit(attacks, r*8 + f);
        if((1ULL << r*8 + f) & bitboard) break ;
    }
    for(int r = rank - 1 , f = file + 1 ; r>=0 && f<=7 ; r-- , f++ ) 
    {
        set_bit(attacks, r*8 + f);
        if((1ULL << r*8 + f) & bitboard) break ;
    }
    for(int r = rank - 1 , f = file - 1 ; r>=0 && f>=0 ; r-- , f-- ) 
    {
        set_bit(attacks, r*8 + f);
        if((1ULL << r*8 + f) & bitboard) break ;
    }
    return attacks ;
}
// generate rook attacks 
U64 rook_attacks(U64 bitboard , int square){
    U64 attacks = 0ULL ;

    int r, f;
    
    int tr = square / 8;
    int tf = square % 8;
    
    for (r = tr + 1; r <= 7; r++) 
    {
        set_bit(attacks ,r * 8 + tf);
        if((1ULL << (r * 8 + tf)) & bitboard) break ;
    }
    for (r = tr - 1; r >= 0; r--)
    {
        set_bit(attacks ,r * 8 + tf);
        if((1ULL << (r * 8 + tf)) & bitboard) break ;
    }
    for (f = tf + 1; f <= 7; f++)
    {
        set_bit(attacks ,tr * 8 + f);
        if((1ULL << (tr * 8 + f)) & bitboard) break ;
    }
    for (f = tf - 1; f >= 0; f--)
    {
        set_bit(attacks ,tr * 8 + f);
        if((1ULL << (tr * 8 + f)) & bitboard) break ;
    }
    return attacks ;
}

//initialise attack arrays 
void init_leapers_attacks(){

    for(int square = 0 ; square < 64 ; square++){
        pawn_attacks[white][square] = mask_pawn_attacks(white,square) ; 
        pawn_attacks[black][square] = mask_pawn_attacks(black,square) ; 
    
        knight_attacks[square] = mask_knight_attacks(square);
        king_attacks[square] = mask_king_attacks(square) ;
    }
}

U64 set_occupancy(int index, int bits_in_mask, U64 attack_mask){

    U64 occupancy = 0ULL ;

    for(int i = 0 ; i < bits_in_mask ; i++){

        int square = get_lsb_index(attack_mask) ;

        pop_bit(attack_mask,square) ;

        if(index & (1 << i) ) 
            set_bit(occupancy,square) ;
    }

    return occupancy ;
}
// XorShift64* Pseudo-RNG
static U64 rng_state = 88172645463325252ULL;
U64 xorshift64star()
{
    U64 x = rng_state;
    x ^= x >> 12;
    x ^= x << 25;
    x ^= x >> 27;
    rng_state = x;
    return x * 2685821657736338717ULL;
}

// XorShift32 Pseudo-RNG
unsigned int state = 1804289383;
unsigned int get_random_number()
{
    unsigned int number = state;
    number ^= number << 13;
    number ^= number >> 17;
    number ^= number << 5;
    state = number;
    return number;
}

//print
void print_board(U64 bitboard)
{
    printf("\n");
    for (int rank = 0; rank < 8; rank++)
    {
        for (int file = 0; file < 8; file++)
        {
            if (file == 0)
                printf("%d  ", 8 - rank);

            int square = rank * 8 + file;
            printf("%d ", get_bit(bitboard, square) ? 1 : 0);
        }
        printf("\n");
    }
    printf("\n   a b c d e f g h \n\n");

    printf("Bitboard: %llu \n", bitboard);
}

// Main 
int main()
{
    
    return 0;
}
