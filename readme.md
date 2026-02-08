🏗️ Core Architecture & Move Generation

    Pseudolegal Move Generation: Generates all moves that are physically possible (e.g., a Rook moving in a line), deferring the check that the King is safe. This is significantly faster than fully legal generation because you only check for "legality" if a move is actually searched.

    Polyglot Support via Zobrist Hashing: Uses 64-bit Zobrist keys to uniquely identify positions. This allows the engine to use standard .bin opening books (Polyglot format) and ensures your Transposition Table (TT) is compatible with industry standards.

    Hash Key Updates: Implements incremental XOR updates for Zobrist keys. Instead of re-calculating the hash from scratch, you simply XOR out the old piece/square and XOR in the new one—maximizing performance during the search.

🧠 Advanced Search (search.c)

The search follows a specific hierarchy of pruning and reduction techniques to traverse the game tree efficiently:

    Delta Pruning: Used in Quiescence search; if a capture can't improve the score by at least a "pawn's worth" (delta), it's pruned.

    QSEE (Quiescence Static Exchange Evaluation): Filters out "bad" captures in Quiescence search that would result in a material loss.

    QChkPrune: Prunes checks in Quiescence search that are unlikely to lead to a mate or tactical gain.

    RFP (Reverse Futility Pruning): Also known as Static Null Move Pruning; prunes nodes if the static evaluation is way above beta.

    Razoring: A very aggressive pruning technique for nodes that are significantly below alpha.

    NMP (Null Move Pruning): Skips a turn to see if the position is still "too good"—if so, the branch is discarded.

    Probcut: A variant of Null Move Pruning that uses a shallower search with a high beta to find cutoffs earlier.

    IID (Internal Iterative Deepening): If a position has no "best move" stored in the TT, IID performs a shallow search to find a good move to start the main search with.

    Extensions: Extends the search depth for critical lines (e.g., checks or passed pawns).

    LMP (Late Move Pruning): Prunes moves that are sorted late in the list if the search is already deep.

    Futility Pruning: Prunes moves at the leaves of the tree that are unlikely to raise alpha.

    SEE (Static Exchange Evaluation): Determines the "material balance" of a series of exchanges on a single square.

    LMR (Late Move Reductions): Searches moves that appear "worse" (via move ordering) at a reduced depth.

📈 Evaluation & Automated Tuning

    Handcrafted Evaluation with Pawn Hashing: Uses a specialized Pawn Hash Table to cache pawn-structure evaluations (doubled pawns, passed pawns, etc.). Since pawn structures change slowly, this cache can hit >95% of the time, saving massive CPU cycles.

    Texel Tuner (SGDM/ADAMW): An automated system to find the optimal values for your evaluation parameters (like the value of a Knight on the 5th rank).

        SGDM(R): Stochastic Gradient Descent with Momentum (Resilient).

        ADAMW(R): A modern optimizer that handles weight decay and adaptive learning rates.

    SPSA Tuner (spsa.c): Simultaneous Perturbation Stochastic Approximation. A "black-box" optimizer that plays thousands of games between different versions of your engine to find the best settings for both search and evaluation.

🕒 Move Ordering & History Heuristics

Finding the best move first is what makes the search fast. Your engine uses multiple "History" tables to track move quality:

    Corrective History: Adjusts the static evaluation based on historical search results for specific piece-square patterns.

    Continuation History: A multi-ply history (1-ply, 2-ply) that tracks how moves performed when following specific previous moves.

    Capture History: Specifically tracks the success rate of various capture moves.

    Regular History: The standard "Butterfly" table that tracks which moves caused beta-cutoffs in the past.

    Stat Tracking: A debugging tool that logs the success rate of each heuristic (e.g., "How often did Null Move Pruning actually cause a cutoff?").