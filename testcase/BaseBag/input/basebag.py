import random
import os

# Danh sách các lệnh
commands = ["INSERT", "GET", "GET INPUT"]

# Danh sách các item
items = ["MAGIC_BOOK", "ENERGY_DRINK", "FIRST_AID", "EXCEMPTION_CARD", "PASSING_CARD"]

# Lấy đường dẫn của thư mục hiện tại
current_dir = os.path.dirname(os.path.abspath(__file__))

for file_index in range(500):
    hp_sherlock = random.randint(0, 500)
    hp_watson = random.randint(0, 500)
    exp_sherlock = random.randint(0, 900)
    exp_watson = random.randint(0, 900)

    # Tạo đường dẫn file đầy đủ
    file_path = os.path.join(current_dir, f"input{file_index}.txt")

    with open(file_path, "w") as f:
        # Viết 4 số ngẫu nhiên vào file
        f.write(f"{hp_sherlock}\n{hp_watson}\n{exp_sherlock}\n{exp_watson}\n")

        # Viết N lệnh ngẫu nhiên
        N = random.randint(10, 20)
        for _ in range(N):
            cmd = random.choice(commands)
            if cmd == "INSERT":
                item = random.choice(items)
                f.write(f"{cmd} {item}\n")
            elif cmd == "GET INPUT":
                item = random.choice(items)
                f.write(f"GET {item}\n")
            else:
                f.write(f"{cmd}\n")

print(f"Đã tạo {file_index + 1} file input thành công.")