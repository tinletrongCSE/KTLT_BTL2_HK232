import random
import os

def generate_random_pos(num_rows, num_cols, num_walls, num_fake_walls):
    positions = []
    while len(positions) < num_walls + num_fake_walls + 3:
        x = random.randint(0, num_rows - 1)
        y = random.randint(0, num_cols - 1)
        pos = (x, y)
        if pos not in positions:
            positions.append(pos)
    return positions

def generate_random_moves(num_steps):
    moves = []
    directions = ['L', 'R', 'U', 'D']
    for _ in range(random.randint(1, num_steps)):
        moves.append(random.choice(directions))
    return ''.join(moves)

# Lấy đường dẫn của thư mục hiện tại
current_dir = os.path.dirname(os.path.abspath(__file__))

for file_index in range(0, 500):
    while True:
        num_steps = random.randint(20, 40)
        num_rows = random.randint(5, 30)
        num_cols = random.randint(5, 30)
        num_walls = random.randint(1, 25)
        num_fake_walls = random.randint(1, 25)
        
        if num_walls + num_fake_walls < num_rows * num_cols:
            break

    moving_rules1 = generate_random_moves(num_steps)
    moving_rules2 = generate_random_moves(num_steps)
    positions = generate_random_pos(num_rows, num_cols, num_walls, num_fake_walls)

    testcase_content = f"{num_steps}\n{num_rows}\n{num_cols}\n{num_walls}\n{num_fake_walls}\n{moving_rules1}\n{moving_rules2}\n"
    for pos in positions:
        testcase_content += f"({pos[0]},{pos[1]})\n"

    # Tạo đường dẫn file đầy đủ
    file_path = os.path.join(current_dir, f"input{file_index}.txt")

    with open(file_path, "w") as file:
        file.write(testcase_content)