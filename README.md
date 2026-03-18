# 🃏 Black-Jack Game

A terminal-based Black-Jack game written in **Embedded C** as a final project.  
Plays a full game of Black-Jack between a human player and a computer-controlled dealer.

---

## 📁 Project Structure

```
├── Black-Jack Game.h            # Header — structs, enums, function declarations
├── Black-Jack Game Implements.c # Implementation — all game logic
└── Black-Jack Game.c            # Main — entry point and game loop
```

---

## ⚙️ How to Compile

```bash
gcc -Wall -Wextra -o blackjack "Black-Jack Game.c" "Black-Jack Game Implements.c"
```

Run:
```bash
./blackjack
```

---

## 🎮 How to Play

- You start with **$1000** in cash
- Each round you place a bet (multiples of $10)
- You and the dealer are each dealt 2 cards — one of the dealer's cards is hidden
- Choose to **hit** (draw a card) or **stand** (end your turn)
- The dealer then draws until reaching 17 or beating your hand
- Closest to 21 without going over wins

### Controls
| Input | Action |
|-------|--------|
| `hit` / `h` / `1` | Draw a card |
| `stand` / `s` / `0` | End your turn |
| `yes` / `y` / `1` | Play another round |
| `no` / `n` / `0` | Exit game |

---

## 📜 Game Rules

| Situation | Outcome |
|-----------|---------|
| Player gets 21 on initial deal (Blackjack) | Player wins pot + 1.5× pot |
| Player goes over 21 | Bust — player loses bet |
| Dealer goes over 21 | Dealer bust — player wins 2× pot |
| Player hand > dealer hand | Player wins 2× pot |
| Dealer hand > player hand | Dealer wins — player loses bet |
| Equal hands | Tie — pot carries to next round |
| Dealer reaches 17+ | Dealer stops drawing |

### Card Values
| Card | Value |
|------|-------|
| 2 – 10 | Face value |
| Jack, Queen, King | 10 |
| Ace | 1 or 11 (whichever keeps hand ≤ 21) |

---

## 🧱 Internal Design

### Card Encoding
Each card is stored in a single `uint8_t`:
```
bits [7:4] → rank  (1=Ace, 2-10, 11=Jack, 12=Queen, 13=King)
bits [3:0] → suit  (0x1=Hearts, 0x2=Clubs, 0x4=Diamond, 0x8=Spades)
```

### Data Structures
- **`Card`** — linked list node holding encoded card data
- **`CardList`** — singly linked list with head pointer and size
- **`GameState`** — holds deck, both hands, cash and pot

### Key Design Decisions
- Cards are **never reallocated** when moved — nodes are relinked between lists
- Memory is allocated once per card at initialization, freed once at program exit
- No global variables — only `const` lookup tables for rank/suit names
- Random draws use `rand() % deck_size` on the live deck (no shuffle needed)

---

## 👤 Author
**Redan Ganim**  
Embedded C Final Project