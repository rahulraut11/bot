# ♟️ Chess Engine — Core Architecture & Search

A high-performance chess engine focused on **fast move generation**, **aggressive pruning**, and **automated tuning**, designed with modern engine techniques used in competitive engines.

---

## 🏗️ Core Architecture & Move Generation

### ⚡ Pseudolegal Move Generation
- Generates all *physically possible* moves (e.g., rook rays, knight jumps).
- King safety is **not** checked during generation.
- Legality is verified **only when a move is searched**, making this much faster than fully legal generation.

### 📚 Polyglot Opening Book Support
- Uses **64-bit Zobrist hashing** to uniquely identify positions.
- Fully compatible with **Polyglot `.bin` opening books**.
- Transposition Table keys follow industry-standard formats.

### 🔑 Incremental Zobrist Hashing
- Hash keys are updated using XOR operations.
- Old piece/square values are XORed out, new ones XORed in.
- Avoids full-board recomputation during search.

---

## 🧠 Advanced Search (`search.c`)

The engine applies a layered hierarchy of pruning, reductions, and extensions to efficiently traverse the game tree.

### 🔍 Quiescence Search
- **Delta Pruning** — skips captures that cannot improve the score by a pawn margin.
- **QSEE** — filters losing captures during quiescence.
- **QChkPrune** — prunes low-value checks unlikely to lead to tactics or mate.

### ✂️ Main Search Pruning
- **RFP (Reverse Futility Pruning)** — cuts nodes when static eval is far above beta.
- **Razoring** — aggressively prunes nodes far below alpha.
- **NMP (Null Move Pruning)** — skips a turn to detect positions that are “too good.”
- **Probcut** — shallow high-beta search for early cutoffs.

### 🎯 Search Control
- **IID (Internal Iterative Deepening)** — shallow search to find a good TT move.
- **Extensions** — increases depth for critical lines (checks, passed pawns).
- **LMP (Late Move Pruning)** — prunes late-ordered moves at deeper depths.
- **Futility Pruning** — removes leaf moves unlikely to raise alpha.
- **LMR (Late Move Reductions)** — searches weaker moves at reduced depth.
- **SEE (Static Exchange Evaluation)** — evaluates capture sequences on a square.

---

## 📈 Evaluation & Automated Tuning

### ♟️ Handcrafted Evaluation + Pawn Hashing
- Dedicated **Pawn Hash Table** caches pawn structure evaluations.
- Pawn structures change slowly → **>95% cache hit rate**.
- Saves significant CPU time during deep searches.

### 🧪 Texel Tuning
Automatic optimization of evaluation parameters using:
- **SGDM(R)** — Stochastic Gradient Descent with Momentum (Resilient).
- **ADAMW(R)** — Adaptive optimizer with weight decay.

Used to tune piece-square tables and positional bonuses.

### 🤖 SPSA Tuning (`spsa.c`)
- **Simultaneous Perturbation Stochastic Approximation**.
- Runs thousands of self-play games between engine variants.
- Tunes both **search parameters** and **evaluation weights** as a black box.

---

## 🕒 Move Ordering & History Heuristics

Strong move ordering is critical for search speed. The engine maintains multiple history tables:

- **Corrective History** — adjusts static eval using past search outcomes.
- **Continuation History** — multi-ply history tracking successful move sequences.
- **Capture History** — tracks effectiveness of capture moves.
- **Regular History (Butterfly Table)** — records moves causing beta cutoffs.
- **Stat Tracking** — debugging metrics for heuristic effectiveness.

---

## 🚀 Summary
- Fast pseudolegal move generation
- Industry-standard hashing & opening books
- Modern pruning and reduction stack
- Automated tuning via Texel & SPSA
- Designed for performance, scalability, and experimentation

