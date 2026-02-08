🏗️ Core Architecture & Move Generation

This engine is built around high-performance pseudolegal move generation combined with a deeply optimized search pipeline.

Pseudolegal Move Generation

Generates all physically possible moves (e.g., rook sliding moves) without immediately checking king safety.

Legality (king-in-check validation) is deferred until the move is actually searched.

This design significantly improves performance compared to fully legal move generation.

Polyglot Opening Book Support

Uses 64-bit Zobrist hashing to uniquely identify positions.

Fully compatible with Polyglot .bin opening books.

Ensures interoperability with standard chess tooling and engines.

Incremental Zobrist Hashing

Hash keys are updated incrementally using XOR operations.

Old piece/square values are XORed out and new ones XORed in.

Avoids expensive full-board rehashing during search.

🧠 Advanced Search (search.c)

The search algorithm uses a layered hierarchy of modern pruning, reduction, and extension techniques to aggressively cut the game tree while preserving tactical accuracy.

Quiescence Search Enhancements

Delta Pruning: Prunes captures that cannot improve the score beyond a pawn-sized margin.

QSEE (Quiescence Static Exchange Evaluation): Filters losing captures during quiescence.

QChkPrune: Prunes low-value checks unlikely to yield tactical gain.

Main Search Pruning Techniques

RFP (Reverse Futility Pruning): Prunes nodes when static eval is already well above beta.

Razoring: Aggressively cuts nodes far below alpha at shallow depths.

NMP (Null Move Pruning): Skips a move to detect positions that are “too good to fail.”

Probcut: Uses a shallow, high-beta search to trigger early cutoffs.

Search Control & Stability

IID (Internal Iterative Deepening): Performs a shallow search to find a good TT move when none is stored.

Extensions: Increases depth for critical lines such as checks or passed pawns.

LMP (Late Move Pruning): Prunes late-ordered moves at deeper depths.

Futility Pruning: Cuts leaf moves unlikely to raise alpha.

LMR (Late Move Reductions): Searches less promising moves at reduced depth.

SEE (Static Exchange Evaluation): Evaluates material outcomes of capture sequences on a square.

📈 Evaluation & Automated Tuning
Handcrafted Evaluation with Pawn Hashing

Uses a dedicated Pawn Hash Table to cache pawn structure evaluations.

Pawn structures change infrequently, resulting in >95% cache hit rates.

Saves significant CPU time during deep searches.

Texel Tuning

Automated tuning of evaluation parameters using gradient-based methods:

SGDM(R): Stochastic Gradient Descent with Momentum (Resilient).

ADAMW(R): Adaptive optimizer with weight decay and learning-rate control.

These methods optimize parameters such as piece-square values and positional bonuses.

SPSA Tuning (spsa.c)

Uses Simultaneous Perturbation Stochastic Approximation.

Runs thousands of self-play games between engine variants.

Optimizes both search parameters and evaluation weights in a black-box manner.

🕒 Move Ordering & History Heuristics

Strong move ordering is central to search speed and effectiveness. The engine maintains multiple history tables:

Corrective History
Adjusts static evaluation based on historical outcomes of piece-square patterns.

Continuation History
Multi-ply (1-ply, 2-ply) tables tracking move success following previous moves.

Capture History
Tracks effectiveness of capture moves independently.

Regular History (Butterfly Table)
Records moves that caused beta cutoffs in the past.

Stat Tracking
Debugging infrastructure to measure effectiveness of each heuristic
(e.g., Null Move Pruning cutoff rate).