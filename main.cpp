#include "study_in_pink2.h"
#include <cstdlib>
#include <filesystem>

using namespace std;

// Sử dụng hàm compareFiles để so sánh 2 file
bool compareFiles(const string &p1, const string &p2)
{
    ifstream f1(p1, ifstream::binary | ifstream::ate);
    ifstream f2(p2, ifstream::binary | ifstream::ate);

    if (f1.fail() || f2.fail())
    {
        return false; // file problem
    }

    if (f1.tellg() != f2.tellg())
    {
        return false; // size mismatch
    }

    // seek back to beginning and use equal to compare contents
    f1.seekg(0, ifstream::beg);
    f2.seekg(0, ifstream::beg);
    return equal(istreambuf_iterator<char>(f1.rdbuf()),
                 istreambuf_iterator<char>(),
                 istreambuf_iterator<char>(f2.rdbuf()));
}

// Sử dụng hàm printFile để in nội dung file ra màn hình
void printFile(const string &p1)
{
    ifstream f1(p1);
    string line;
    while (getline(f1, line))
    {
        std::cout << line << endl;
    }
}

// Hàm clearScreen dùng để xóa màn hình console
void clearScreen()
{
#ifdef _WIN32
    string clear_command = "cls";
#else
    string clear_command = "printf '\\33c\\e[3J'";
#endif
    system(clear_command.c_str());
}

// Ham stringToItemType chuyen string thanh BaseItem
BaseItem *stringtoItem(string input)
{
    if (input == "MAGIC_BOOK")
        return new MagicBook();
    else if (input == "ENERGY_DRINK")
        return new EnergyDrink();
    else if (input == "FIRST_AID")
        return new FirstAid;
    else if (input == "EXCEMPTION_CARD")
        return new ExcemptionCard();
    else
        return new PassingCard("all");
}

// Ham stringToItemType chuyen string thanh ItemType
ItemType stringtoItemType(string input)
{
    if (input == "MAGIC_BOOK")
        return MAGIC_BOOK;
    else if (input == "ENERGY_DRINK")
        return ENERGY_DRINK;
    else if (input == "FIRST_AID")
        return FIRST_AID;
    else if (input == "EXCEMPTION_CARD")
        return EXCEMPTION_CARD;
    else
        return PASSING_CARD;
}

class TestStudyInPink
{
public:
    static void check_01(string input_file, string output_file)
    {
        ifstream infile(input_file); // Mở file

        int num_steps;
        int in_num_rows;
        int in_num_cols;
        int num_walls;
        int num_fake_walls;
        string moving_rules;
        Position init_pos;

        infile >> num_steps;
        infile >> in_num_rows;
        infile >> in_num_cols;
        infile >> num_walls;
        infile >> num_fake_walls;
        infile >> moving_rules;

        Position *arr_walls = new Position[num_walls];
        Position *arr_fake_walls = new Position[num_fake_walls];

        string temp_pos;
        int wall_idx = 0, fake_wall_idx = 0;
        while (infile >> temp_pos)
        {
            if (wall_idx < num_walls)
            {
                arr_walls[wall_idx] = Position(temp_pos);
                wall_idx++;
            }
            else if (fake_wall_idx < num_fake_walls)
            {
                arr_fake_walls[fake_wall_idx] = Position(temp_pos);
                fake_wall_idx++;
            }
            else
            {
                init_pos = Position(temp_pos);
                break;
            }
        }

        Map *map = new Map(in_num_rows, in_num_cols, num_walls, arr_walls, num_fake_walls, arr_fake_walls);
        Sherlock *sherlock = new Sherlock(1, moving_rules, init_pos, map, 100, 100);
        infile.close();

        ArrayMovingObject *arr_moving_obj = new ArrayMovingObject(10);
        arr_moving_obj->add(sherlock);

        ofstream outfile(output_file);
        outfile << sherlock->str() << endl;

        for (int i = 0; i < num_steps; i++)
        {
            outfile << "STEP" << i + 1 << ": * Sherlock makes a move" << endl;
            sherlock->move();
            outfile << arr_moving_obj->str() << endl;
        }
        outfile << "End." << endl;
        outfile.close();
        delete arr_moving_obj;
        delete sherlock;
        delete[] arr_walls;
        delete[] arr_fake_walls;
        delete map;
    }
    static void check_02(string input_file, string output_file)
    {
        ifstream infile(input_file); // Mở file

        int num_steps;
        int in_num_rows;
        int in_num_cols;
        int num_walls;
        int num_fake_walls;
        string moving_rules;
        Position init_pos;

        infile >> num_steps;
        infile >> in_num_rows;
        infile >> in_num_cols;
        infile >> num_walls;
        infile >> num_fake_walls;
        infile >> moving_rules;

        Position *arr_walls = new Position[num_walls];
        Position *arr_fake_walls = new Position[num_fake_walls];

        string temp_pos;
        int wall_idx = 0, fake_wall_idx = 0;
        while (infile >> temp_pos)
        {
            if (wall_idx < num_walls)
            {
                arr_walls[wall_idx] = Position(temp_pos);
                wall_idx++;
            }
            else if (fake_wall_idx < num_fake_walls)
            {
                arr_fake_walls[fake_wall_idx] = Position(temp_pos);
                fake_wall_idx++;
            }
            else
            {
                init_pos = Position(temp_pos);
                break;
            }
        }

        Map *map = new Map(in_num_rows, in_num_cols, num_walls, arr_walls, num_fake_walls, arr_fake_walls);
        Watson *watson = new Watson(0, moving_rules, init_pos, map, 100, 100);
        infile.close();

        ArrayMovingObject *arr_moving_obj = new ArrayMovingObject(10);
        arr_moving_obj->add(watson);

        ofstream outfile(output_file);
        outfile << watson->str() << endl;
        for (int i = 0; i < num_steps; i++)
        {
            outfile << "STEP" << i + 1 << ": * Watson makes a move" << endl;
            watson->move();
            outfile << arr_moving_obj->str() << endl;
        }
        outfile << "End." << endl;
        outfile.close();
        delete arr_moving_obj;
        delete watson;
        delete[] arr_walls;
        delete[] arr_fake_walls;
        delete map;
    }
    static void check_03(string input_file, string output_file)
    {
        ifstream infile(input_file); // Mở file

        int num_steps;
        int in_num_rows;
        int in_num_cols;
        int num_walls;
        int num_fake_walls;
        string moving_rules_sher;
        string moving_rules_wat;
        Position init_pos_sher;
        Position init_pos_watson;
        Position init_pos_criminal;

        infile >> num_steps;
        infile >> in_num_rows;
        infile >> in_num_cols;
        infile >> num_walls;
        infile >> num_fake_walls;
        infile >> moving_rules_sher;
        infile >> moving_rules_wat;

        Position *arr_walls = new Position[num_walls];
        Position *arr_fake_walls = new Position[num_fake_walls];

        string temp_pos;
        int wall_idx = 0, fake_wall_idx = 0;
        int index_object = 0;
        while (infile >> temp_pos)
        {
            if (wall_idx < num_walls)
            {
                arr_walls[wall_idx] = Position(temp_pos);
                wall_idx++;
            }
            else if (fake_wall_idx < num_fake_walls)
            {
                arr_fake_walls[fake_wall_idx] = Position(temp_pos);
                fake_wall_idx++;
            }
            else
            {
                if (index_object == 0)
                {
                    init_pos_sher = Position(temp_pos);
                }
                else if (index_object == 1)
                {
                    init_pos_watson = Position(temp_pos);
                }
                else
                {
                    init_pos_criminal = Position(temp_pos);
                    break;
                }
                index_object++;
            }
        }

        Map *map = new Map(in_num_rows, in_num_cols, num_walls, arr_walls, num_fake_walls, arr_fake_walls);
        Watson *watson = new Watson(2, moving_rules_sher, init_pos_watson, map, 100, 100);
        Sherlock *sherlock = new Sherlock(1, moving_rules_wat, init_pos_sher, map, 100, 100);
        Criminal *criminal = new Criminal(0, init_pos_criminal, map, sherlock, watson);
        infile.close();

        ArrayMovingObject *arr_moving_obj = new ArrayMovingObject(10);
        arr_moving_obj->add(sherlock);
        arr_moving_obj->add(watson);
        arr_moving_obj->add(criminal);

        ofstream outfile(output_file);
        outfile << sherlock->str() << endl;
        outfile << watson->str() << endl;
        outfile << criminal->str() << endl;
        for (int i = 0; i < num_steps; i++)
        {
            outfile << "STEP" << i + 1 << ": * Sherlock, Watson, Criminal makes a move" << endl;
            sherlock->move();
            watson->move();
            criminal->move();
            outfile << arr_moving_obj->str() << endl;
        }
        outfile << "End." << endl;
        outfile.close();

        delete sherlock;
        delete watson;
        delete criminal;
        delete arr_moving_obj;
        delete[] arr_walls;
        delete[] arr_fake_walls;
        delete map;
    }
    static void check_04(string input_file, string output_file)
    {
        Configuration *config = new Configuration(input_file);
        ofstream outfile(output_file);
        outfile << config->str() << endl;
        outfile << "End." << endl;
        outfile.close();
        delete config;
    }
    static void check_05(string input_file, string output_file)
    {
        ifstream infile(input_file); // Mở file

        int num_steps;
        int in_num_rows;
        int in_num_cols;
        int num_walls;
        int num_fake_walls;
        string moving_rules_sher;
        string moving_rules_wat;
        Position init_pos_sher;
        Position init_pos_watson;
        Position init_pos_criminal;

        infile >> num_steps;
        infile >> in_num_rows;
        infile >> in_num_cols;
        infile >> num_walls;
        infile >> num_fake_walls;
        infile >> moving_rules_sher;
        infile >> moving_rules_wat;

        Position *arr_walls = new Position[num_walls];
        Position *arr_fake_walls = new Position[num_fake_walls];

        string temp_pos;
        int wall_idx = 0, fake_wall_idx = 0;
        int index_object = 0;
        while (infile >> temp_pos)
        {
            if (wall_idx < num_walls)
            {
                arr_walls[wall_idx] = Position(temp_pos);
                wall_idx++;
            }
            else if (fake_wall_idx < num_fake_walls)
            {
                arr_fake_walls[fake_wall_idx] = Position(temp_pos);
                fake_wall_idx++;
            }
            else
            {
                if (index_object == 0)
                {
                    init_pos_sher = Position(temp_pos);
                }
                else if (index_object == 1)
                {
                    init_pos_watson = Position(temp_pos);
                }
                else
                {
                    init_pos_criminal = Position(temp_pos);
                    break;
                }
                index_object++;
            }
        }

        Map *map = new Map(in_num_rows, in_num_cols, num_walls, arr_walls, num_fake_walls, arr_fake_walls);
        Watson *watson = new Watson(2, moving_rules_sher, init_pos_watson, map, 100, 100);
        Sherlock *sherlock = new Sherlock(1, moving_rules_wat, init_pos_sher, map, 100, 100);
        Criminal *criminal = new Criminal(0, init_pos_criminal, map, sherlock, watson);
        infile.close();
        RobotC *robotC = nullptr;
        RobotS *robotS = nullptr;
        RobotW *robotW = nullptr;
        RobotSW *robotSW = nullptr;

        ArrayMovingObject *arr_moving_obj = new ArrayMovingObject(10);
        arr_moving_obj->add(sherlock);
        arr_moving_obj->add(watson);
        arr_moving_obj->add(criminal);

        ofstream outfile(output_file);
        outfile << sherlock->str() << endl;
        outfile << watson->str() << endl;
        outfile << criminal->str() << endl;
        for (int i = 0; i < num_steps; i++)
        {
            Position temp_pos = criminal->getCurrentPosition();
            outfile << "STEP" << i + 1 << ": * Moving objects make a move" << endl;

            criminal->move();
            sherlock->move();
            watson->move();
            if (robotC != nullptr)
                robotC->move();
            if (robotS != nullptr)
                robotS->move();
            if (robotW != nullptr)
                robotW->move();
            if (robotSW != nullptr)
                robotSW->move();

            outfile << arr_moving_obj->str() << endl;
            if (i == 3)
            {
                outfile << "* Create new RobotC" << endl;
                robotC = new RobotC(3, temp_pos, map, criminal);
                outfile << robotC->str() << endl;
                arr_moving_obj->add(robotC);
            }
            else if (i == 6)
            {
                outfile << "* Create new RobotS" << endl;
                robotS = new RobotS(4, temp_pos, map, criminal, sherlock);
                outfile << robotS->str() << endl;
                arr_moving_obj->add(robotS);
            }
            else if (i == 9)
            {
                outfile << "* Create new RobotW" << endl;
                robotW = new RobotW(5, temp_pos, map, criminal, watson);
                outfile << robotW->str() << endl;
                arr_moving_obj->add(robotW);
            }
            else if (i == 12)
            {
                outfile << "* Create new RobotCS" << endl;
                robotSW = new RobotSW(6, temp_pos, map, criminal, sherlock, watson);
                outfile << robotSW->str() << endl;
                arr_moving_obj->add(robotSW);
            }
        }
        outfile << "End." << endl;
        outfile.close();

        delete sherlock;
        delete watson;
        delete criminal;

        if (robotC != nullptr)
            delete robotC;
        if (robotS != nullptr)
            delete robotS;
        if (robotW != nullptr)
            delete robotW;
        if (robotSW != nullptr)
            delete robotSW;

        delete arr_moving_obj;
        delete[] arr_walls;
        delete[] arr_fake_walls;
        delete map;
    }
    static void check_06(string input_file, string output_file)
    {
        int num_walls = 3;
        Position arr_walls[] = {Position(1, 2), Position(2, 3), Position(3, 4)};
        int num_fake_walls = 1;
        Position arr_fake_walls[] = {Position(2, 0)};
        Map *map = new Map(10, 10, num_walls, arr_walls, num_fake_walls, arr_fake_walls);

        ifstream infile(input_file); // Mở file
        ofstream outfile(output_file);

        int hp_sher;
        int hp_wat;
        int exp_sher;
        int exp_wat;
        vector<BaseItem *> items;

        infile >> hp_sher;
        infile >> hp_wat;
        infile >> exp_sher;
        infile >> exp_wat;

        Sherlock *sherlock = new Sherlock(0, "RL", Position(1, 3), map, hp_sher, exp_sher);
        outfile << sherlock->str() << " HP: " << hp_sher << " EXP: " << exp_sher << endl;
        Watson *watson = new Watson(1, "UD", Position(2, 1), map, hp_wat, exp_wat);
        outfile << watson->str() << " HP: " << hp_wat << " EXP: " << exp_wat << endl;

        BaseBag *sher_bag = new SherlockBag(sherlock);
        outfile << sher_bag->str() << endl;
        BaseBag *wat_bag = new WatsonBag(watson);
        outfile << wat_bag->str() << endl;
        int numline = 3;
        string line;

        while (getline(infile, line))
        {
            numline++;
            istringstream iss(line);
            string command;
            iss >> command;

            if (command == "INSERT")
            {
                string input;
                iss >> input;
                outfile << "LINE " << numline << ": * Insert " << input << " into bag" << endl;
                BaseItem *tempitem = stringtoItem(input);
                items.push_back(tempitem);
                sher_bag->insert(tempitem);
                wat_bag->insert(tempitem);
                outfile << "SherlockBag: " << sher_bag->str() << endl;
                outfile << "WatsonBag " << wat_bag->str() << endl;
            }
            else if (command == "GET")
            {
                string input;
                iss >> input;
                outfile << "LINE " << numline << ": * Get " << input << " from bag" << endl;
                if (input.empty())
                {
                    sher_bag->get();
                    wat_bag->get();
                }
                else
                {
                    sher_bag->get(stringtoItemType(input));
                    wat_bag->get(stringtoItemType(input));
                }
                outfile << "SherlockBag: " << sher_bag->str() << endl;
                outfile << "WatsonBag " << wat_bag->str() << endl;
            }
        }
        infile.close();
        outfile << "End." << endl;
        outfile.close();

        delete sherlock;
        delete watson;
        delete sher_bag;
        delete wat_bag;
        delete map;
        for (int i = 0; i < items.size(); i++)
        {
            if (items[i] != nullptr)
                delete items[i];
        }
    }
    static void check_07(string input_file, string output_file)
    {
        StudyPinkProgram *program = new StudyPinkProgram(input_file);
        // Lưu lại buffer std::cout cũ
        streambuf *oldcoutStreamBuf = std::cout.rdbuf();

        // Mở file output và liên kết nó với std::cout
        ofstream ofs(output_file);
        std::cout.rdbuf(ofs.rdbuf());
        // Chạy hàm, output sẽ được ghi vào file
        program->run(true);
        std::cout << "End." << endl;
        // Khôi phục lại std::cout về buffer cũ
        std::cout.rdbuf(oldcoutStreamBuf);
    }
};

void printResult(string input_folder)
{

    std::cout << "Enter start input (0 - 499): ";
    int start_input = 0;
    // std::cin >> start_input;
    std::cout << "Enter end input (0 - 499): ";
    int end_input = 499;
    // std::cin >> end_input;

    std::string command = "mkdir testcase\\" + input_folder + "\\output";
    system(command.c_str());

    for (int i = start_input; i <= end_input; ++i)
    {
        clearScreen();
        std::cout << "Checking " << input_folder << " - Testcase: " << i << endl;
        string input_file = "testcase/" + input_folder + "/input/input" + to_string(i) + ".txt";
        string output_file = "testcase/" + input_folder + "/output/output" + to_string(i) + ".txt";
        string expect_file = "testcase/" + input_folder + "/expect/output" + to_string(i) + ".txt";

        if (input_folder == "Sherlock_move")
        {
            TestStudyInPink::check_01(input_file, output_file);
        }
        else if (input_folder == "Watson_move")
        {
            TestStudyInPink::check_02(input_file, output_file);
        }
        else if (input_folder == "Criminal_move")
        {
            TestStudyInPink::check_03(input_file, output_file);
        }
        else if (input_folder == "Configuration")
        {
            TestStudyInPink::check_04(input_file, output_file);
        }
        else if (input_folder == "Robot_move")
        {
            TestStudyInPink::check_05(input_file, output_file);
        }
        else if (input_folder == "BaseBag")
        {
            TestStudyInPink::check_06(input_file, output_file);
        }
        else if (input_folder == "StudyPinkProgram")
        {
            TestStudyInPink::check_07(input_file, output_file);
        }

        if (!compareFiles(output_file, expect_file))
        {
            clearScreen();
            std::cout << "Test case " << i << " failed" << endl;
            std::cout << "Input file: " << input_file << endl;
            std::cout << "-------------------------------" << endl;
            std::cout << "Your output: " << output_file << endl;
            std::cout << "-------------------------------" << endl;
            std::cout << "Expected output: " << expect_file << endl;
            std::cout << "-------------------------------" << endl;
            std::cout << "Structure of input file: " << endl;
            printFile("testcase/" + input_folder + "/readme.txt");
            break;
        }
        if (i == 499)
        {
            clearScreen();
            std::cout << "All test cases passed" << endl;
            std::string command = "rmdir /S /Q testcase\\" + input_folder + "\\output";
            system(command.c_str());
        }
    }
}

// Hàm main dùng để chạy chương trình
int main()
{
    while (true)
    {
        clearScreen();
        int choice;
        std::cout << "1. Check Sherlock's movement" << endl;
        std::cout << "2. Check Waison's movement" << endl;
        std::cout << "3. Check Sherlock, Watson, Criminal's movement. Not create robot" << endl;
        std::cout << "4. Check Configuration" << endl;
        std::cout << "5. Check Robot's movement" << endl;
        std::cout << "6. Check BaseBag" << endl;
        std::cout << "7. Check StudyPinkProgram" << endl;
        std::cout << "========================================" << endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        clearScreen();

        switch (choice)
        {
        case 1:
        {
            printResult("Sherlock_move");
            break;
        }
        case 2:
        {
            printResult("Watson_move");
            break;
        }
        case 3:
        {
            printResult("Criminal_move");
            break;
        }
        case 4:
        {
            printResult("Configuration");
            break;
        }
        case 5:
        {
            printResult("Robot_move");
            break;
        }
        case 6:
        {
            printResult("BaseBag");
            break;
        }
        case 7:
        {
            printResult("StudyPinkProgram");
            break;
        }
        }
        std::cout << "========================================" << endl;
        std::cout << "Press 0 to exit, 1 to continue: ";
        std::cin >> choice;
        if (choice == 0)
        {
            break;
        }
    }
    return 0;
}
