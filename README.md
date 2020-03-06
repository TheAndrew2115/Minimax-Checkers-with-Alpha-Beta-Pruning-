# Minimax-Checkers-with-Alpha-Beta-Pruning-

Welcome to minimax checkers! For this project, we created a digital checkers board using a 32x32 LED matrix and an Arduino Mega. We first developed a player vs player gamemode by creating the basic checkers board environment on the LED matrix. Then, for the player vs computer gamemode, we developed a minimax algorithm with alpha-beta pruning in order to generate the CPU’s moves in C++. Communication between an auxiliary device, such as a laptop, and the Arduino Mega is mandatory since the Mega does not have enough memory to run the minimax algorithm.

Below is the led board. We can see the checkers board represented by white and black tiles, the checkers pieces in blue and red, and a green cursor in the top left corner.
![Board](./IMG_0258.jpg)

To control the pieces, we built our own controller using pushbuttons and a breadboard and hooked it up to the mega. here is a detailed view of it.
![Board](./IMG_0259.jpg)
The arrows move the green cursor in the appropriate direction. The 'Y' button selects a tile and allows the user to perform an attack afterwards. The 'N' button allows the user to cancel a tile selection.

For our player vs CPU component, we used the minimax algorithm, with alpha-beta pruning, to generate the moves. To implement our minimax algorithm, we created a class called Node whose instances would represent every “node” of the decision tree. The most important attribute in the class was the vector of type node called “children,” which stores the child nodes.



The minimax algorithm is simple to understand. First, we designated one player, the computer, as the “maximizing player” and the other, the user, as the “minimizing player.” This means that after the evaluation function is called on a board state, the maximizing player will favour boards with a higher (positive) value whereas the minimizing player will favour those with a lower (negative) value. To project the next move for the computer, we took the current board state as a parameter. From this board state, we alternated between projecting all the possible board states after the computer or the player makes a move. This was accomplished by scanning through the board for the player or the computer’s pieces. If the pieces could move, we created a new board state and stored it in a vector. Once we reached a certain predetermined “depth” (how many moves we chose to look ahead) we called the evaluate function on the final board states. From here, we performed a series of maximizing and minimizing on the evaluations, alternating as we go up the decision tree, until we received a final value. 
