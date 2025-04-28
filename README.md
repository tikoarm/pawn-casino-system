# SA-MP Casino Table System ??

A modular, multiplayer casino system for SA-MP RPG servers. Players join virtual tables, participate in competitive rounds, and one winner is determined based on random values. Originally developed as a commissioned project, now shared publicly with the client's permission for educational and non-commercial use.

## ?? What Is This?

This system simulates a live casino environment with multiplayer tables where several players can compete in luck-based rounds. It's not a classic slot machine – instead, players sit at a virtual table, numbers are randomly generated, and the highest one wins the round.

This brings a competitive, social aspect to in-game gambling and adds great value to RPG-style servers.

## ?? Features

- Multiplayer casino tables (supporting multiple players simultaneously)
- Random number generation per player each round
- Automatic winner selection based on the highest number
- Dynamic UI using SA-MP `TextDraws`
- Table join/leave logic with state control
- Configurable game flow and timing
- Utility sounds, messages, and formatting helpers
- Hook-based integration for easy gamemode compatibility

## ?? File Structure

- `main.inc` – Root file, connects all modules
- `functions.inc` – Main game logic: round start, number generation, winner selection
- `static_functions.inc` – Helper functions (math, format, sounds, etc.)
- `textdraws.inc` – All visual elements using `TextDraws`
- `hooks.inc` – Hooks for events like player join/leave, command handling, etc.

## ?? Technologies Used

- **Pawn** – Main scripting language
- **SA-MP 0.3.7** – Multiplayer platform
- TextDraw system – For UI and player interaction
- Optional: Extendable for MySQL integration

## ?? How to Use

1. Copy all `.inc` files to your `includes/` folder.
2. In your gamemode or filterscript:
   ```pawn
   #include "main"
3. Players can join tables using a command or interaction point.

## ?? Development Notes

This system was designed with a clean modular structure and attention to language-specific optimization:

- Replaced unnecessary global variables with `static` declarations to reduce memory usage and side effects
- Organized code into logical modules for better maintainability (`main`, `functions`, `textdraws`, etc.)
- Used hook-based patterns to keep integration with gamemodes flexible and non-intrusive
- Optimized for SA-MP’s single-threaded runtime environment

## ????? Author

Developed by **Tigran Kocharov**  
?? tiko.nue@icloud.com

## ?? License & Attribution

This project was originally built as a commissioned system for a private SA-MP RPG server.  
The client has granted full permission to share the source code publicly under the condition it remains **non-commercial** and properly **credited**.
