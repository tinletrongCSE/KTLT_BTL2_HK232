import random
import os
from collections import Counter

def generate_test_case(file_name):
    num_rows = random.randint(5, 50)
    num_cols = random.randint(5, 50)
    max_moving_objects = random.randint(3, 20)
    num_walls = random.randint(2, min(10, num_rows * num_cols // 2))
    num_fake_walls = random.randint(1, min(5, num_rows * num_cols // 2 - num_walls))
    sherlock_moving_rule = ''.join(random.choices(['R', 'U', 'L', 'D'], k=random.randint(1, 20)))
    watson_moving_rule = ''.join(random.choices(['R', 'U', 'L', 'D'], k=random.randint(1, 20)))
    num_steps = random.randint(5, 300)

    all_positions = set((r, c) for r in range(1, num_rows) for c in range(1, num_cols))
    walls = random.sample(list(all_positions), num_walls)
    fake_walls = random.sample(list(all_positions - set(walls)), num_fake_walls)

    sherlock_init_pos = random.choice(tuple(all_positions - set(walls) - set(fake_walls)))
    watson_init_pos = random.choice(tuple(all_positions - set(walls) - set(fake_walls) - {sherlock_init_pos}))
    criminal_init_pos = random.choice(tuple(all_positions - set(walls) - set(fake_walls) - {sherlock_init_pos, watson_init_pos}))

    sherlock_init_hp = random.randint(0, 500)
    sherlock_init_exp = random.randint(0, 900)
    watson_init_hp = random.randint(0, 500)
    watson_init_exp = random.randint(0, 900)

    config = [
        f"MAP_NUM_ROWS={num_rows}",
        f"MAP_NUM_COLS={num_cols}",
        f"MAX_NUM_MOVING_OBJECTS={max_moving_objects}",
        f"ARRAY_WALLS=[{';'.join(f'({r},{c})' for r, c in walls)}]",
        f"ARRAY_FAKE_WALLS=[{';'.join(f'({r},{c})' for r, c in fake_walls)}]",
        f"SHERLOCK_MOVING_RULE={sherlock_moving_rule}",
        f"SHERLOCK_INIT_POS={sherlock_init_pos}",
        f"SHERLOCK_INIT_HP={sherlock_init_hp}",
        f"SHERLOCK_INIT_EXP={sherlock_init_exp}",
        f"WATSON_MOVING_RULE={watson_moving_rule}",
        f"WATSON_INIT_POS={watson_init_pos}",
        f"WATSON_INIT_HP={watson_init_hp}",
        f"WATSON_INIT_EXP={watson_init_exp}",
        f"CRIMINAL_INIT_POS={criminal_init_pos}",
        f"NUM_STEPS={num_steps}",
    ]

    random.shuffle(config)

    with open(file_name, 'w') as f:
        f.write('\n'.join(config))

if __name__ == "__main__":
    for i in range(500):
         # Tạo đường dẫn file đầy đủ
        current_dir = os.path.dirname(os.path.abspath(__file__))
        file_path = os.path.join(current_dir, f"input{i}.txt")
        generate_test_case(file_path)