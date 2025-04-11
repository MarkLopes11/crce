import tkinter as tk
from tkinter import messagebox
import math

# Initialize the board
board = [[' ' for _ in range(3)] for _ in range(3)]

# Evaluate the board to assign scores
def evaluate(board):
    for row in board:
        if row.count('X') == 3:
            return 10
        if row.count('O') == 3:
            return -10

    for col in range(3):
        if board[0][col] == board[1][col] == board[2][col]:
            if board[0][col] == 'X':
                return 10
            if board[0][col] == 'O':
                return -10

    if board[0][0] == board[1][1] == board[2][2]:
        if board[0][0] == 'X':
            return 10
        if board[0][0] == 'O':
            return -10

    if board[0][2] == board[1][1] == board[2][0]:
        if board[0][2] == 'X':
            return 10
        if board[0][2] == 'O':
            return -10

    return 0

# Check if there are moves left
def is_moves_left(board):
    for row in board:
        if ' ' in row:
            return True
    return False

# Minimax with Alpha-Beta Pruning
def minimax(board, depth, is_maximizing, alpha, beta):
    score = evaluate(board)
    if score == 10 or score == -10:
        return score
    if not is_moves_left(board):
        return 0
    
    if is_maximizing:
        best = -math.inf
        for i in range(3):
            for j in range(3):
                if board[i][j] == ' ':
                    board[i][j] = 'X'
                    best = max(best, minimax(board, depth + 1, False, alpha, beta))
                    board[i][j] = ' '
                    alpha = max(alpha, best)
                    if beta <= alpha:
                        break
        return best
    else:
        best = math.inf
        for i in range(3):
            for j in range(3):
                if board[i][j] == ' ':
                    board[i][j] = 'O'
                    best = min(best, minimax(board, depth + 1, True, alpha, beta))
                    board[i][j] = ' '
                    beta = min(beta, best)
                    if beta <= alpha:
                        break
        return best

# Find the best move and display scores on the board
def find_best_move():
    best_val = -math.inf
    best_move = (-1, -1)
    
    # Clear previous move values
    for i in range(3):
        for j in range(3):
            score_board[i][j]['text'] = ''
    
    for i in range(3):
        for j in range(3):
            if board[i][j] == ' ':
                board[i][j] = 'X'
                move_val = minimax(board, 0, False, -math.inf, math.inf)
                board[i][j] = ' '
                
                # Display possible move and score on score board
                score_board[i][j]['text'] = str(move_val)

                if move_val > best_val:
                    best_val = move_val
                    best_move = (i, j)
    
    return best_move

# Handle the player click
def handle_click(row, col):
    if board[row][col] == ' ':
        board[row][col] = 'O'
        buttons[row][col]['text'] = 'O'
        buttons[row][col]['state'] = 'disabled'
        
        # Check if player won
        result = evaluate(board)
        if result == -10:
            messagebox.showinfo("Game Over", "You Win!")
            reset_game()
            return
        elif not is_moves_left(board):
            messagebox.showinfo("Game Over", "It's a Draw!")
            reset_game()
            return

        # AI makes a move
        ai_move()

# AI's turn
def ai_move():
    x, y = find_best_move()
    if x != -1 and y != -1:
        board[x][y] = 'X'
        buttons[x][y]['text'] = 'X'
        buttons[x][y]['state'] = 'disabled'
        buttons[x][y]['bg'] = '#ffcccb'  # Highlight AI's move
        
        # Check if AI won
        result = evaluate(board)
        if result == 10:
            messagebox.showinfo("Game Over", "AI Wins!")
            reset_game()
        elif not is_moves_left(board):
            messagebox.showinfo("Game Over", "It's a Draw!")
            reset_game()

# Reset the game
def reset_game():
    global board
    board = [[' ' for _ in range(3)] for _ in range(3)]
    for i in range(3):
        for j in range(3):
            buttons[i][j]['text'] = ''
            buttons[i][j]['state'] = 'normal'
            buttons[i][j]['bg'] = 'SystemButtonFace'
            score_board[i][j]['text'] = ''

# Create the UI
root = tk.Tk()
root.title("Tic Tac Toe - AI vs Human")

# Create Tic Tac Toe grid buttons
buttons = [[None for _ in range(3)] for _ in range(3)]
for i in range(3):
    for j in range(3):
        buttons[i][j] = tk.Button(root, text='', font=('Arial', 24), width=6, height=2,
                                  command=lambda row=i, col=j: handle_click(row, col))
        buttons[i][j].grid(row=i, column=j, padx=2, pady=2)

# Create score board for possible moves
score_board_label = tk.Label(root, text="AI Scoreboard", font=("Arial", 16))
score_board_label.grid(row=3, column=0, columnspan=3, pady=5)

score_board = [[None for _ in range(3)] for _ in range(3)]
for i in range(3):
    for j in range(3):
        score_board[i][j] = tk.Label(root, text='', font=('Arial', 20), width=6, height=2, relief="ridge", borderwidth=2, bg="#f0f0f0")
        score_board[i][j].grid(row=i + 4, column=j, padx=2, pady=2)

# Start new game button
reset_button = tk.Button(root, text="New Game", font=('Arial', 14), command=reset_game, bg="#4CAF50", fg="white")
reset_button.grid(row=7, column=0, columnspan=3, pady=10)

# Start the game loop
root.mainloop()
