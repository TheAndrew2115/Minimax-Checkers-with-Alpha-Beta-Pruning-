# Minimax-Checkers-with-Alpha-Beta-Pruning-

Welcome to minimax checkers! For this project, we created a digital checkers board using a 32x32 LED matrix and an Arduino Mega. We first developed a player vs player gamemode by creating the basic checkers board environment on the LED matrix. Then, for the player vs computer gamemode, we developed a minimax algorithm with alpha-beta pruning in order to generate the CPU’s moves in C++. Communication between an auxiliary device, such as a laptop, and the Arduino Mega is mandatory since the Mega does not have enough memory to run the minimax algorithm.

Below is the led board. We can see the checkers board, represented by white and black tiles, as well as the checkers pieces in blue and red.
![Board](./IMG_0258.jpg)

To control the pieces, we built our own controller using pushbuttons and a breadboard and hooked it up to the mega. here is a detailed view of it.
![Board](./IMG_0259.jpg)
The arrows move the green cursor in the appropriate direction. The 'Y' button selects a tile and allows the user to perform an attack afterwards. The 'N' button allows the user to cancel a tile selection.
