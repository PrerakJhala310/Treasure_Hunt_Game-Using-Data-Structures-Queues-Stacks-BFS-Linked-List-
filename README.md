# Doodle Maps: Escape the Monsters

A terminal maze game in C, built as a Data Structures course team project at NMIMS MPSTME.

**Team:** Prerak Jhala, [teammate names]

## The game
Reach the treasure `T` on a 10×10 map across 3 levels. Monsters `M` chase you, and levels 2 and 3 have a locked door `D` that needs the key `K`. You get 3 lives and 3 hints per level. Fewest total moves tops the leaderboard.

```
##########
#P...#...#
#.##.#.#.#
#.#....#.#
#.#.##.#M#
#...#....#
###.#.##.#
#.....#..#
#.###...T#
##########
```

## Data structures and algorithms
| What | How | File |
|---|---|---|
| Map as a graph | 100 cells, adjacency matrix `a[100][100]` | `game.c` |
| Monster chase | Breadth-first search from the player; each monster steps to its BFS parent | `game.c`, `queue.c` |
| Safe-path hint | BFS that avoids monsters, path rebuilt in order with a stack | `game.c`, `stack.c` |
| Leaderboard | Binary search tree ordered by moves, printed in-order | `bst.c` |
| Map memory | `malloc` / `free` for each level | `game.c` |

## Run it
```
gcc main.c game.c stack.c queue.c bst.c -o doodle
./doodle
```
Move with `w` `a` `s` `d`, press `h` for a hint.
