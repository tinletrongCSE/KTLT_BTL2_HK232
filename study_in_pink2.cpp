#include "study_in_pink2.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Global variables
const Position Position::npos = Position(-1, -1); // not position

// Hàm hỗ trợ - Support functions
int GetDistance(Position pos1, Position pos2)
{
    int dis = abs(pos1.getRow() - pos2.getRow()) + abs(pos1.getCol() - pos2.getCol());
    return dis;
}

// 3.3 Vị trí - Position
Position::Position(const string &str_pos)
{
    stringstream ss(str_pos); // Turn the string into a stream.
    string item;

    getline(ss, item, '('); // remove '('

    getline(ss, item, ','); // get r
    this->r = stoi(item);
    getline(ss, item, ')'); // get c
    this->c = stoi(item);
}

// 3.2 Bản đồ - Map
Map::Map(int in_num_rows, int in_num_cols, int in_num_walls, Position *in_array_walls, int in_num_fake_walls, Position *in_array_fake_walls)
{
    this->num_rows = in_num_rows;
    this->num_cols = in_num_cols;
    this->map = new MapElement **[in_num_rows];
    for (int i = 0; i < in_num_rows; i++)
    {
        this->map[i] = new MapElement *[in_num_cols];
        for (int j = 0; j < in_num_cols; j++)
        {
            this->map[i][j] = new Path();
        }
    }

    for (int i = 0; i < in_num_walls; i++)
    {
        int row = in_array_walls[i].getRow();
        int col = in_array_walls[i].getCol();
        delete this->map[row][col];
        this->map[row][col] = new Wall();
    }

    for (int i = 0; i < in_num_fake_walls; i++)
    {
        int r = in_array_fake_walls[i].getRow();
        int c = in_array_fake_walls[i].getCol();
        int in_req_exp = (r * 257 + c * 139 + 89) % 900 + 1;
        delete this->map[r][c];
        this->map[r][c] = new FakeWall(in_req_exp);
    }
};

Map::~Map()
{
    for (int i = 0; i < this->num_rows; i++)
    {
        for (int j = 0; j < this->num_cols; j++)
        {
            delete this->map[i][j];
        }
        delete[] this->map[i];
    }
    delete[] this->map;
};

bool Map::isValid(const Position &pos, MovingObject *mv_obj) const
{
    bool isTrue = true;
    if (pos.getRow() < 0 || pos.getRow() >= this->num_rows || pos.getCol() < 0 || pos.getCol() >= this->num_cols)
    {
        return false;
    }
    else if (this->map[pos.getRow()][pos.getCol()]->getType() == WALL)
    {
        return false;
    }
    else if (this->map[pos.getRow()][pos.getCol()]->getType() == FAKE_WALL)
    {
        FakeWall *fakeWall = dynamic_cast<FakeWall *>(this->map[pos.getRow()][pos.getCol()]);
        if (mv_obj->getName() == "Watson")
        {
            Watson *watson = dynamic_cast<Watson *>(mv_obj);
            if (fakeWall->getReqExp() >= watson->getExp())
            {
                return false;
            }
        }
    }
    return true;
}

// 3.4 Đối tượng di chuyển - MovingObject
MovingObject::MovingObject(int index, const Position pos, Map *map, const string &name)
{
    this->index = index;
    this->pos = pos;
    this->map = map;
    this->name = name;
};

void MovingObject::move()
{
    Position next_pos = this->getNextPosition();
    if (!next_pos.isEqual(-1, -1))
    {
        this->pos = next_pos;
    }
    this->numsteps_now++;
}

// 3.5 Sherlock - Sherlock
Position Sherlock::getNextPosition()
{
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int temp_step = numsteps_now % this->moving_rule.length();

    if (this->moving_rule[temp_step] == 'U')
    {
        r--;
    }
    else if (this->moving_rule[temp_step] == 'D')
    {
        r++;
    }
    else if (this->moving_rule[temp_step] == 'L')
    {
        c--;
    }
    else if (this->moving_rule[temp_step] == 'R')
    {
        c++;
    }

    Position next_pos;

    if (this->map->isValid(Position(r, c), this))
    {
        next_pos.setRow(r);
        next_pos.setCol(c);
    }
    else
    {
        next_pos = Position::npos;
    }
    return next_pos;
}

void Sherlock::afterMove(ArrayMovingObject *arr_moving_objects, SherlockBag *sherlock_bag, WatsonBag *watson_bag)
{
    for (int i = 0; i < arr_moving_objects->getMovingObjectCount(); i++)
    {
        MovingObject *mv_obj = arr_moving_objects->get(i);
        if (mv_obj->getName() == "Watson" && mv_obj->getCurrentPosition().isEqual(this->getCurrentPosition()))
        {
            BaseItem *temp_sherItem = sherlock_bag->get(PASSING_CARD);
            BaseItem *temp_watsonItem = watson_bag->get(EXCEMPTION_CARD);
            if (temp_sherItem != NULL && temp_watsonItem != NULL)
            {
                while (temp_sherItem != NULL && temp_watsonItem != NULL)
                {
                    watson_bag->insert(temp_sherItem);
                    sherlock_bag->insert(temp_watsonItem);
                    temp_sherItem = sherlock_bag->get(PASSING_CARD);
                    temp_watsonItem = watson_bag->get(EXCEMPTION_CARD);
                }
            }
        }

        Robot *robot = dynamic_cast<Robot *>(mv_obj);
        if (robot != NULL && robot->getCurrentPosition().isEqual(this->getCurrentPosition()))
        {
            BaseItem *item = robot->getItem();
            switch (robot->getRobotType())
            {
            case C:
            {
                if (this->getExp() <= 500)
                    sherlock_bag->insert(item);
                break;
            }
            case S:
            {
                if (this->getExp() > 400)
                {
                    sherlock_bag->insert(item);
                }
                else if (sherlock_bag->get(EXCEMPTION_CARD) == nullptr)
                {
                    this->setExp(ceil(0.9 * this->getHp() - 0.0001));
                }
                break;
            }
            case W:
            {
                sherlock_bag->insert(item);
                break;
            }
            case SW:
            {
                if (this->getExp() > 300 && this->getHp() > 335)
                {
                    sherlock_bag->insert(item);
                }
                else if (sherlock_bag->get(EXCEMPTION_CARD) == nullptr)
                {
                    this->setHp(ceil(0.85 * this->getHp() - 0.0001));
                    this->setExp(ceil(0.85 * this->getExp() - 0.0001));
                }
                break;
            }
            }
            BaseItem *temp_item = sherlock_bag->get();
            if (temp_item != nullptr)
            {
                temp_item->use(this);
            }
        }
    }
}
// 3.6 Watson - Watson
Position Watson::getNextPosition()
{
    int r = this->pos.getRow();
    int c = this->pos.getCol();
    int temp_step = numsteps_now % this->moving_rule.length();

    if (this->moving_rule[temp_step] == 'U')
    {
        r--;
    }
    else if (this->moving_rule[temp_step] == 'D')
    {
        r++;
    }
    else if (this->moving_rule[temp_step] == 'L')
    {
        c--;
    }
    else if (this->moving_rule[temp_step] == 'R')
    {
        c++;
    }

    Position next_pos;

    if (this->map->isValid(Position(r, c), this))
    {
        next_pos.setRow(r);
        next_pos.setCol(c);
    }
    else
    {
        next_pos = Position::npos;
    }
    return next_pos;
}

// 3.7 Criminal - Criminal
Position Criminal::getNextPosition()
{
    int dis_U = -1;
    int dis_L = -1;
    int dis_D = -1;
    int dis_R = -1;

    int r = this->pos.getRow();
    int c = this->pos.getCol();

    if (this->map->isValid(Position(r - 1, c), this))
    {
        int dis_sherlock = GetDistance(Position(r - 1, c), this->sherlock->getCurrentPosition());
        int dis_watson = GetDistance(Position(r - 1, c), this->watson->getCurrentPosition());
        dis_U = dis_sherlock + dis_watson;
    }
    if (this->map->isValid(Position(r, c - 1), this))
    {
        int dis_sherlock = GetDistance(Position(r, c - 1), this->sherlock->getCurrentPosition());
        int dis_watson = GetDistance(Position(r, c - 1), this->watson->getCurrentPosition());
        dis_L = dis_sherlock + dis_watson;
    }
    if (this->map->isValid(Position(r + 1, c), this))
    {
        int dis_sherlock = GetDistance(Position(r + 1, c), this->sherlock->getCurrentPosition());
        int dis_watson = GetDistance(Position(r + 1, c), this->watson->getCurrentPosition());
        dis_D = dis_sherlock + dis_watson;
    }
    if (this->map->isValid(Position(r, c + 1), this))
    {
        int dis_sherlock = GetDistance(Position(r, c + 1), this->sherlock->getCurrentPosition());
        int dis_watson = GetDistance(Position(r, c + 1), this->watson->getCurrentPosition());
        dis_R = dis_sherlock + dis_watson;
    }

    int max_dis = max(max(dis_U, dis_L), max(dis_D, dis_R));

    if (max_dis == dis_U && dis_U != -1)
    {
        r--;
    }
    else if (max_dis == dis_L && dis_L != -1)
    {
        c--;
    }
    else if (max_dis == dis_D && dis_D != -1)
    {
        r++;
    }
    else if (max_dis == dis_R && dis_R != -1)
    {
        c++;
    }
    else
    {
        return Position::npos;
    }

    Position next_pos = Position(r, c);
    return next_pos;
}

void Watson::afterMove(ArrayMovingObject *arr_moving_objects, SherlockBag *sherlock_bag, WatsonBag *watson_bag)
{
    for (int i = 0; i < arr_moving_objects->getMovingObjectCount(); i++)
    {
        MovingObject *mv_obj = arr_moving_objects->get(i);
        Robot *robot = dynamic_cast<Robot *>(mv_obj);
        if (robot != NULL && robot->getCurrentPosition().isEqual(this->getCurrentPosition()))
        {
            BaseItem *item = robot->getItem();
            switch (robot->getRobotType())
            {
            case C:
            {
                watson_bag->insert(item);
                break;
            }
            case S:
                break;
            case W:
            {
                BaseItem *temp_item = watson_bag->get(PASSING_CARD);
                if (temp_item != NULL)
                {
                    temp_item->use(this, robot);
                    watson_bag->insert(item);
                }
                else if (this->getHp() > 350)
                {
                    watson_bag->insert(item);
                }
                else
                {
                    this->setHp(ceil(0.95 * this->getHp() - 0.0001));
                }
                break;
            }
            case SW:
            {
                BaseItem *temp_item = watson_bag->get(PASSING_CARD);
                if (temp_item != NULL)
                {
                    temp_item->use(this, robot);
                    watson_bag->insert(item);
                }
                else if (this->getHp() > 165 && this->getExp() > 600)
                {
                    watson_bag->insert(item);
                }
                else
                {
                    this->setHp(ceil(0.85 * this->getHp() - 0.0001));
                    this->setExp(ceil(0.85 * this->getExp() - 0.0001));
                }
                break;
            }
            }
            BaseItem *temp_item = watson_bag->get();
            if (temp_item != nullptr)
            {
                temp_item->use(this);
            }
        }
    }
}

void Criminal::createRobot(ArrayMovingObject *arr_moving_objects)
{
    if (!arr_moving_objects->isFull())
    {
        int dis_sherlock = GetDistance(this->prev_pos, this->sherlock->getCurrentPosition());
        int dis_watson = GetDistance(this->prev_pos, this->watson->getCurrentPosition());

        if (this->num_steps == 3)
        {
            RobotC *robotC = new RobotC(arr_moving_objects->getMovingObjectCount(), this->prev_pos, this->map, this);
            arr_moving_objects->add(robotC);
        }
        else if (dis_sherlock < dis_watson && this->num_steps % 3 == 0)
        {
            RobotS *robotS = new RobotS(arr_moving_objects->getMovingObjectCount(), this->prev_pos, this->map, this, this->sherlock);
            arr_moving_objects->add(robotS);
        }
        else if (dis_sherlock > dis_watson && this->num_steps % 3 == 0)
        {
            RobotW *robotW = new RobotW(arr_moving_objects->getMovingObjectCount(), this->prev_pos, this->map, this, this->watson);
            arr_moving_objects->add(robotW);
        }
        else if (this->num_steps % 3 == 0)
        {
            RobotSW *robotSW = new RobotSW(arr_moving_objects->getMovingObjectCount(), this->prev_pos, this->map, this, this->sherlock, this->watson);
            arr_moving_objects->add(robotSW);
        }
    }
}

void Criminal::move()
{
    Position next_pos = this->getNextPosition();
    if (!next_pos.isEqual(-1, -1))
    {
        this->num_steps++;
        this->prev_pos = this->pos;
        this->pos = next_pos;
    }
}

// 3.8 Mảng các đối tượng di chuyển - ArayMovingObject
ArrayMovingObject::ArrayMovingObject(int capacity)
{
    this->capacity = capacity;
    this->count = 0;
    this->arr_mv_objs = new MovingObject *[capacity];
    for (int i = 0; i < capacity; i++)
    {
        this->arr_mv_objs[i] = NULL;
    }
}

ArrayMovingObject::~ArrayMovingObject()
{
    if (this->arr_mv_objs != NULL)
        delete[] this->arr_mv_objs;
}

bool ArrayMovingObject::add(MovingObject *mv_obj)
{
    if (this->isFull())
    {
        return false;
    }
    else
    {
        this->arr_mv_objs[mv_obj->getIndex()] = mv_obj;
        this->count++;
        return true;
    }
}

string ArrayMovingObject::str() const
{
    string str = "ArrayMovingObject[count=" + to_string(this->count) + ";capacity=" + to_string(this->capacity);
    for (int i = 0; i < this->capacity; i++)
    {
        if (this->arr_mv_objs[i] == NULL)
            continue;
        str += ";" + this->arr_mv_objs[i]->str();
    }
    str += "]";
    return str;
}

// 3.9 Cấu hình cho chương trình - Configuration
Configuration::Configuration(const string &filepath)
{
    ifstream file(filepath);
    string line;
    // cout << "File path: " << filepath << endl;
    while (getline(file, line))
    {
        size_t found = line.find("=");

        if (found != string::npos)
        {
            string key = line.substr(0, found);

            if (key == "MAP_NUM_ROWS")
            {
                string attribute_value = line.substr(found + 1);
                this->map_num_rows = stoi(attribute_value);
            }
            else if (key == "MAP_NUM_COLS")
            {
                string attribute_value = line.substr(found + 1);
                this->map_num_cols = stoi(attribute_value);
            }
            else if (key == "MAX_NUM_MOVING_OBJECTS")
            {
                string attribute_value = line.substr(found + 1);
                this->max_num_moving_objects = stoi(attribute_value);
            }
            else if (key == "ARRAY_WALLS")
            {
                this->num_walls = 0;
                string attribute_value = line.substr(found + 1);
                for (char c : attribute_value)
                {
                    if (c == ';')
                    {
                        ++this->num_walls;
                    }
                }
                ++this->num_walls;
                this->arr_walls = new Position[this->num_walls];
                stringstream ss(attribute_value);
                string item;

                for (int i = 0; i < this->num_walls; i++)
                {
                    getline(ss, item, ';');
                    this->arr_walls[i] = Position(item);
                }
            }
            else if (key == "ARRAY_FAKE_WALLS")
            {
                this->num_fake_walls = 0;
                string attribute_value = line.substr(found + 1);
                for (char c : attribute_value)
                {
                    if (c == ';')
                    {
                        ++this->num_fake_walls;
                    }
                }
                ++this->num_fake_walls;
                this->arr_fake_walls = new Position[this->num_fake_walls];
                stringstream ss(attribute_value);
                string item;

                for (int i = 0; i < this->num_fake_walls; i++)
                {
                    getline(ss, item, ';');
                    this->arr_fake_walls[i] = Position(item);
                }
            }
            else if (key == "SHERLOCK_MOVING_RULE")
            {
                string attribute_value = line.substr(found + 1);
                this->sherlock_moving_rule = attribute_value;
            }
            else if (key == "SHERLOCK_INIT_POS")
            {
                string attribute_value = line.substr(found + 1);
                this->sherlock_init_pos = Position(attribute_value);
            }
            else if (key == "SHERLOCK_INIT_HP")
            {
                string attribute_value = line.substr(found + 1);
                this->sherlock_init_hp = stoi(attribute_value);
            }
            else if (key == "SHERLOCK_INIT_EXP")
            {
                string attribute_value = line.substr(found + 1);
                this->sherlock_init_exp = stoi(attribute_value);
            }
            else if (key == "WATSON_MOVING_RULE")
            {
                string attribute_value = line.substr(found + 1);
                this->watson_moving_rule = attribute_value;
            }
            else if (key == "WATSON_INIT_POS")
            {
                string attribute_value = line.substr(found + 1);
                this->watson_init_pos = Position(attribute_value);
            }
            else if (key == "WATSON_INIT_HP")
            {
                string attribute_value = line.substr(found + 1);
                this->watson_init_hp = stoi(attribute_value);
            }
            else if (key == "WATSON_INIT_EXP")
            {
                string attribute_value = line.substr(found + 1);
                this->watson_init_exp = stoi(attribute_value);
            }
            else if (key == "CRIMINAL_INIT_POS")
            {
                string attribute_value = line.substr(found + 1);
                this->criminal_init_pos = Position(attribute_value);
            }
            else if (key == "NUM_STEPS")
            {
                string attribute_value = line.substr(found + 1);
                this->num_steps = stoi(attribute_value);
            }
        }
    }
}

Configuration::~Configuration()
{
    delete[] this->arr_walls;
    delete[] this->arr_fake_walls;
}

string Configuration::str() const
{
    string str = "Configuration[\nMAP_NUM_ROWS=" + to_string(this->map_num_rows) + "\nMAP_NUM_COLS=" + to_string(this->map_num_cols) + "\nMAX_NUM_MOVING_OBJECTS=" + to_string(this->max_num_moving_objects) + "\nNUM_WALLS=" + to_string(this->num_walls) + "\nARRAY_WALLS=[";

    for (int i = 0; i < this->num_walls; i++)
    {
        if (i == this->num_walls - 1)
        {
            str += this->arr_walls[i].str();
        }
        else
        {
            str += this->arr_walls[i].str() + ";";
        }
    }

    str += "]\nNUM_FAKE_WALLS=" + to_string(this->num_fake_walls) + "\nARRAY_FAKE_WALLS=[";

    for (int i = 0; i < this->num_fake_walls; i++)
    {
        if (i == this->num_fake_walls - 1)
        {
            str += this->arr_fake_walls[i].str();
        }
        else
        {
            str += this->arr_fake_walls[i].str() + ";";
        }
    }

    str += "]\nSHERLOCK_MOVING_RULE=" + this->sherlock_moving_rule + "\nSHERLOCK_INIT_POS=" + this->sherlock_init_pos.str() + "\nSHERLOCK_INIT_HP=" + to_string(this->sherlock_init_hp) + "\nSHERLOCK_INIT_EXP=" + to_string(this->sherlock_init_exp) + "\nWATSON_MOVING_RULE=" + this->watson_moving_rule + "\nWATSON_INIT_POS=" + this->watson_init_pos.str() + "\nWATSON_INIT_HP=" + to_string(this->watson_init_hp) + "\nWATSON_INIT_EXP=" + to_string(this->watson_init_exp) + "\nCRIMINAL_INIT_POS=" + this->criminal_init_pos.str() + "\nNUM_STEPS=" + to_string(this->num_steps) + "\n]";

    return str;
}

// 3.10 Robot - Robot
Robot::Robot(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal) : MovingObject(index, init_pos, map)
{
    this->robot_type = robot_type;
    this->criminal = criminal;

    int p = this->pos.getRow() * this->pos.getCol();
    while (p >= 10)
    {
        int sum = 0;
        while (p > 0)
        {
            sum += p % 10;
            p /= 10;
        }
    }

    switch (p)
    {
    case 0:
    case 1:
    {
        this->item = new MagicBook();
        break;
    }
    case 2:
    case 3:
    {
        this->item = new EnergyDrink();
        break;
    }
    case 4:
    case 5:
    {
        this->item = new FirstAid();
        break;
    }
    case 6:
    case 7:
    {
        this->item = new ExcemptionCard();
        break;
    }
    case 8:
    case 9:
    {
        int t = (this->pos.getRow() * 11 + this->pos.getCol()) % 4;
        if (t == 0)
        {
            this->item = new PassingCard("RobotS");
        }
        else if (t == 1)
        {
            this->item = new PassingCard("RobotC");
        }
        else if (t == 2)
        {
            this->item = new PassingCard("RobotSW");
        }
        else if (t == 3)
        {
            this->item = new PassingCard("all");
        }
        break;
    }
    default:
        break;
    }
}

Robot::~Robot()
{
    delete this->item;
}

// RobotS
Position RobotS::getNextPosition()
{
    int dis_U = 999999999;
    int dis_L = 999999999;
    int dis_D = 999999999;
    int dis_R = 999999999;

    int r = this->pos.getRow();
    int c = this->pos.getCol();

    if (this->map->isValid(Position(r - 1, c), this))
    {
        dis_U = GetDistance(Position(r - 1, c), this->sherlock->getNextPosition());
    }
    if (this->map->isValid(Position(r, c - 1), this))
    {
        dis_L = GetDistance(Position(r, c - 1), this->sherlock->getNextPosition());
    }
    if (this->map->isValid(Position(r + 1, c), this))
    {
        dis_D = GetDistance(Position(r + 1, c), this->sherlock->getNextPosition());
    }
    if (this->map->isValid(Position(r, c + 1), this))
    {
        dis_R = GetDistance(Position(r, c + 1), this->sherlock->getNextPosition());
    }

    int min_dis = min(min(dis_U, dis_L), min(dis_D, dis_R));

    if (min_dis == dis_U && dis_U != 999999999)
    {
        r--;
    }
    else if (min_dis == dis_R && dis_R != 999999999)
    {
        c++;
    }
    else if (min_dis == dis_D && dis_D != 999999999)
    {
        r++;
    }
    else if (min_dis == dis_L && dis_L != 999999999)
    {
        c--;
    }
    else
    {
        return Position::npos;
    }

    Position next_pos = Position(r, c);
    return next_pos;
}

// RobotW
RobotW::RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson) : Robot(index, init_pos, map, W, criminal)
{
    this->watson = watson;
}

Position RobotW::getNextPosition()
{
    int dis_U = 999999999;
    int dis_L = 999999999;
    int dis_D = 999999999;
    int dis_R = 999999999;

    int r = this->pos.getRow();
    int c = this->pos.getCol();

    if (this->map->isValid(Position(r - 1, c), this))
    {
        dis_U = GetDistance(Position(r - 1, c), this->watson->getNextPosition());
    }
    if (this->map->isValid(Position(r, c - 1), this))
    {
        dis_L = GetDistance(Position(r, c - 1), this->watson->getNextPosition());
    }
    if (this->map->isValid(Position(r + 1, c), this))
    {
        dis_D = GetDistance(Position(r + 1, c), this->watson->getNextPosition());
    }
    if (this->map->isValid(Position(r, c + 1), this))
    {
        dis_R = GetDistance(Position(r, c + 1), this->watson->getNextPosition());
    }

    int min_dis = min(min(dis_U, dis_L), min(dis_D, dis_R));

    if (min_dis == dis_U && dis_U != 999999999)
    {
        r--;
    }
    else if (min_dis == dis_R && dis_R != 999999999)
    {
        c++;
    }
    else if (min_dis == dis_D && dis_D != 999999999)
    {
        r++;
    }
    else if (min_dis == dis_L && dis_L != 999999999)
    {
        c--;
    }
    else
    {
        return Position::npos;
    }

    Position next_pos = Position(r, c);
    return next_pos;
}

// RobotSW
Position RobotSW::getNextPosition()
{
    int dis_UU = 999999999;
    int dis_UR = 999999999;
    int dis_RR = 999999999;
    int dis_RD = 999999999;
    int dis_DD = 999999999;
    int dis_DL = 999999999;
    int dis_LL = 999999999;
    int dis_LU = 999999999;

    int r = this->pos.getRow();
    int c = this->pos.getCol();

    if (this->map->isValid(Position(r - 2, c), this))
    {
        int dis_sherlock = GetDistance(Position(r - 2, c), this->sherlock->getNextPosition());
        int dis_watson = GetDistance(Position(r - 2, c), this->watson->getNextPosition());
        dis_UU = dis_sherlock + dis_watson;
    }
    if (this->map->isValid(Position(r - 1, c + 1), this))
    {
        int dis_sherlock = GetDistance(Position(r - 1, c + 1), this->sherlock->getNextPosition());
        int dis_watson = GetDistance(Position(r - 1, c + 1), this->watson->getNextPosition());
        dis_UR = dis_sherlock + dis_watson;
    }
    if (this->map->isValid(Position(r, c + 2), this))
    {
        int dis_sherlock = GetDistance(Position(r, c + 2), this->sherlock->getNextPosition());
        int dis_watson = GetDistance(Position(r, c + 2), this->watson->getNextPosition());
        dis_RR = dis_sherlock + dis_watson;
    }
    if (this->map->isValid(Position(r + 1, c + 1), this))
    {
        int dis_sherlock = GetDistance(Position(r + 1, c + 1), this->sherlock->getNextPosition());
        int dis_watson = GetDistance(Position(r + 1, c + 1), this->watson->getNextPosition());
        dis_RD = dis_sherlock + dis_watson;
    }
    if (this->map->isValid(Position(r + 2, c), this))
    {
        int dis_sherlock = GetDistance(Position(r + 2, c), this->sherlock->getNextPosition());
        int dis_watson = GetDistance(Position(r + 2, c), this->watson->getNextPosition());
        dis_DD = dis_sherlock + dis_watson;
    }
    if (this->map->isValid(Position(r + 1, c - 1), this))
    {
        int dis_sherlock = GetDistance(Position(r + 1, c - 1), this->sherlock->getNextPosition());
        int dis_watson = GetDistance(Position(r + 1, c - 1), this->watson->getNextPosition());
        dis_DL = dis_sherlock + dis_watson;
    }
    if (this->map->isValid(Position(r, c - 2), this))
    {
        int dis_sherlock = GetDistance(Position(r, c - 2), this->sherlock->getNextPosition());
        int dis_watson = GetDistance(Position(r, c - 2), this->watson->getNextPosition());
        dis_LL = dis_sherlock + dis_watson;
    }
    if (this->map->isValid(Position(r - 1, c - 1), this))
    {
        int dis_sherlock = GetDistance(Position(r - 1, c - 1), this->sherlock->getNextPosition());
        int dis_watson = GetDistance(Position(r - 1, c - 1), this->watson->getNextPosition());
        dis_LU = dis_sherlock + dis_watson;
    }

    int min_dis = min(min(min(dis_UU, dis_UR), min(dis_RR, dis_RD)), min(min(dis_DD, dis_DL), min(dis_LL, dis_LU)));

    if (min_dis == dis_UU && dis_UU != 999999999)
    {
        r -= 2;
    }
    else if (min_dis == dis_UR && dis_UR != 999999999)
    {
        r--;
        c++;
    }
    else if (min_dis == dis_RR && dis_RR != 999999999)
    {
        c += 2;
    }
    else if (min_dis == dis_RD && dis_RD != 999999999)
    {
        r++;
        c++;
    }
    else if (min_dis == dis_DD && dis_DD != 999999999)
    {
        r += 2;
    }
    else if (min_dis == dis_DL && dis_DL != 999999999)
    {
        r++;
        c--;
    }
    else if (min_dis == dis_LL && dis_LL != 999999999)
    {
        c -= 2;
    }
    else if (min_dis == dis_LU && dis_LU != 999999999)
    {
        r--;
        c--;
    }
    else
    {
        return Position::npos;
    }

    Position next_pos = Position(r, c);
    return next_pos;
}

int RobotSW::getDistance() const
{
    int dis_sherlock = GetDistance(this->pos, this->sherlock->getCurrentPosition());
    int dis_watson = GetDistance(this->pos, this->watson->getCurrentPosition());
    int dis = dis_sherlock + dis_watson;
    return dis;
}

// 3.11 Vật phẩm - BaseItem

// MagicBook
bool MagicBook::canUse(Character *character, Robot *robot)
{
    if (character->getName() == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(character);
        if (sherlock && sherlock->getExp() <= 350)
        {
            return true;
        }
    }
    return false;
}

void MagicBook::use(Character *character, Robot *robot)
{
    if (this->canUse(character, robot))
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(character);
        sherlock->setExp(ceil(1.25 * sherlock->getExp() - 0.0001));
        if (sherlock->getExp() > 500)
        {
            sherlock->setExp(500);
        }
    }
}

// EnergyDrink
bool EnergyDrink::canUse(Character *character, Robot *robot)
{
    if (character->getName() == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(character);
        if (watson && watson->getHp() <= 100)
        {
            return true;
        }
    }
    else if (character->getName() == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(character);
        if (sherlock && sherlock->getHp() <= 100)
        {
            return true;
        }
    }
    return false;
}

void EnergyDrink::use(Character *character, Robot *robot)
{
    if (this->canUse(character, robot))
    {
        if (character->getName() == "Watson")
        {
            Watson *watson = dynamic_cast<Watson *>(character);
            watson->setHp(ceil(1.1 * watson->getHp() - 0.0001));
            if (watson->getHp() > 900)
            {
                watson->setHp(900);
            }
        }
        else if (character->getName() == "Sherlock")
        {
            Sherlock *sherlock = dynamic_cast<Sherlock *>(character);
            sherlock->setHp(ceil(1.1 * sherlock->getHp() - 0.0001));
            if (sherlock->getHp() > 900)
            {
                sherlock->setHp(900);
            }
        }
    }
}

// FirstAid
bool FirstAid::canUse(Character *character, Robot *robot)
{
    if (character->getName() == "Watson")
    {
        Watson *watson = dynamic_cast<Watson *>(character);
        if (watson && (watson->getHp() <= 100 || watson->getExp() <= 350))
        {
            return true;
        }
    }
    else if (character->getName() == "Sherlock")
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(character);
        if (sherlock && (sherlock->getHp() <= 100 || sherlock->getExp() <= 350))
        {
            return true;
        }
    }
    return false;
}

void FirstAid::use(Character *character, Robot *robot)
{
    if (this->canUse(character, robot))
    {
        if (character->getName() == "Watson")
        {
            Watson *watson = dynamic_cast<Watson *>(character);
            watson->setHp(ceil(1.5 * watson->getHp() - 0.0001));
            if (watson->getHp() > 900)
            {
                watson->setHp(900);
            }
            watson->setExp(ceil(1.25 * watson->getExp() - 0.0001));
            if (watson->getExp() > 500)
            {
                watson->setExp(500);
            }
        }
        else if (character->getName() == "Sherlock")
        {
            Sherlock *sherlock = dynamic_cast<Sherlock *>(character);
            sherlock->setHp(ceil(1.5 * sherlock->getHp() - 0.0001));
            if (sherlock->getHp() > 900)
            {
                sherlock->setHp(900);
            }
            sherlock->setExp(ceil(1.25 * sherlock->getExp() - 0.0001));
            if (sherlock->getExp() > 500)
            {
                sherlock->setExp(500);
            }
        }
    }
}

// ExcemptionCard
bool ExcemptionCard::canUse(Character *character, Robot *robot)
{
    if (character->getName() == "Sherlock" && robot != nullptr)
    {
        Sherlock *sherlock = dynamic_cast<Sherlock *>(character);
        if (sherlock && sherlock->getHp() % 2 == 1)
        {
            return true;
        }
    }
    return false;
}

void ExcemptionCard::use(Character *character, Robot *robot)
{
    // Your code here
}

// PassingCard
bool PassingCard::canUse(Character *character, Robot *robot)
{
    if (character->getName() == "Watson" && robot != nullptr)
    {
        Watson *watson = dynamic_cast<Watson *>(character);
        if (watson && watson->getHp() % 2 == 0)
        {
            return true;
        }
    }
    return false;
}

void PassingCard::use(Character *character, Robot *robot)
{
    if (this->canUse(character, robot))
    {
        bool condition1 = this->challenge == "RobotC" && robot->getRobotType() != C;
        bool condition2 = this->challenge == "RobotS" && robot->getRobotType() != S;
        bool condition3 = this->challenge == "RobotW" && robot->getRobotType() != W;
        bool condition4 = this->challenge == "RobotSW" && robot->getRobotType() != SW;
        bool condition = condition1 || condition2 || condition3 || condition4;

        if (condition)
        {
            Watson *watson = dynamic_cast<Watson *>(character);
            watson->setHp(watson->getHp() - 50);
            if (watson->getHp() < 0)
            {
                watson->setHp(0);
            }
        }
    }
}

// 3.12 Túi đồ - BaseBag
bool BaseBag::insert(BaseItem *item)
{
    if (this->count < this->capacity)
    {
        NodeItem *new_node = new NodeItem(item);
        new_node->next = this->head;
        this->head = new_node;
        this->count++;
        return true;
    }
    else
    {
        return false;
    }
}

string BaseBag::str() const
{
    string str = "BaseBag[count=" + to_string(this->count);
    NodeItem *current = this->head;
    while (current != nullptr)
    {
        str += ";" + current->item->getName();
        current = current->next;
    }
    str += "]";
    return str;
}

BaseItem *BaseBag::get()
{
    if (this->head == NULL)
        return NULL;

    // Nếu danh sách chỉ có một phần tử
    if (this->head->next == NULL)
    {
        if (this->head->item->canUse(this->obj))
        {
            BaseItem *temp = this->head->item;
            delete this->head;
            this->head = NULL;
            this->count--;
            return temp;
        }
        return NULL;
    }

    // Nếu phần tử đầu tiên có thể sử dụng
    if (this->head->item->canUse(this->obj))
    {
        BaseItem *res_item = this->head->item;
        NodeItem *temp = this->head;
        this->head = this->head->next;
        delete temp;
        this->count--;
        return res_item;
    }

    NodeItem *prev = NULL;
    NodeItem *current = this->head;

    // Tìm phần tử có thể sử dụng
    while (current != nullptr && !current->item->canUse(this->obj))
    {
        prev = current;
        current = current->next;
    }

    // Nếu không tìm thấy phần tử có thể sử dụng
    if (current == nullptr)
        return NULL;

    // Đảo chỗ phần tử có thể sử dụng với phần tử đầu tiên
    NodeItem *temp = this->head;
    this->head = current;
    if (prev)
        prev->next = temp;
    NodeItem *temp_next = current->next;
    current->next = temp->next;
    temp->next = temp_next;

    // Xóa phần tử đầu tiên (phần tử có thể sử dụng hiện tại)
    BaseItem *res_item = this->head->item;
    temp = this->head;
    this->head = this->head->next;
    delete temp;
    this->count--;

    return res_item;
}

BaseItem *BaseBag::get(ItemType itemType)
{
    string arr_name[] = {"MagicBook", "EnergyDrink", "FirstAid", "ExcemptionCard", "PassingCard"};

    if (this->head == NULL)
        return NULL;

    // Nếu danh sách chỉ có một phần tử
    if (this->head->next == NULL)
    {
        if (this->head->item->getName() == arr_name[itemType])
        {
            BaseItem *temp = this->head->item;
            delete this->head;
            this->head = NULL;
            this->count--;
            return temp;
        }
        return NULL;
    }

    // Nếu phần tử cần lấy là phần tử đầu tiên
    if (this->head->item->getName() == arr_name[itemType])
    {
        BaseItem *res_item = this->head->item;
        NodeItem *temp = this->head;
        this->head = this->head->next;
        delete temp;
        this->count--;
        return res_item;
    }

    NodeItem *prev = NULL;
    NodeItem *current = this->head;

    // Tìm phần tử cần lấy
    while (current != nullptr && current->item->getName() != arr_name[itemType])
    {
        prev = current;
        current = current->next;
    }

    // Nếu không tìm thấy phần tử cần lấy
    if (current == nullptr)
        return NULL;

    // Đảo chỗ phần tử cần lấy với phần tử đầu tiên
    NodeItem *temp = this->head;
    this->head = current;
    if (prev)
        prev->next = temp;
    NodeItem *temp_next = current->next;
    current->next = temp->next;
    temp->next = temp_next;

    // Xóa phần tử đầu tiên (phần tử cần lấy hiện tại)
    BaseItem *res_item = this->head->item;
    temp = this->head;
    this->head = this->head->next;
    delete temp;
    this->count--;

    return res_item;
}

BaseBag::~BaseBag()
{
    NodeItem *current = this->head;
    while (current != nullptr)
    {
        NodeItem *temp = current;
        current = current->next;
        delete temp;
    }
}

SherlockBag::SherlockBag(Sherlock *sherlock)
{
    this->obj = sherlock;
    this->head = NULL;
    this->count = 0;
    this->capacity = 13;
}

WatsonBag::WatsonBag(Watson *watson)
{
    this->obj = watson;
    this->head = NULL;
    this->count = 0;
    this->capacity = 15;
}

// 3.14 StudyInPink - StudyInPink
StudyPinkProgram::StudyPinkProgram(const string &config_file_path)
{
    this->config = new Configuration(config_file_path);
    this->map = new Map(config->getNumRows(), config->getNumCols(), config->getNumWalls(), config->getArrWalls(), config->getNumFakeWalls(), config->getArrFakeWalls());
    this->sherlock = new Sherlock(1, config->getSherlockMovingRule(), config->getSherlockInitPos(), map, config->getSherlockInitHp(), config->getSherlockInitExp());
    this->watson = new Watson(2, config->getWatsonMovingRule(), config->getWatsonInitPos(), map, config->getWatsonInitHp(), config->getWatsonInitExp());
    this->criminal = new Criminal(0, config->getCriminalInitPos(), map, sherlock, watson);
    this->arr_mv_objs = new ArrayMovingObject(config->getMaxNumMovingObjects());
    this->sherlock_bag = new SherlockBag(sherlock);
    this->watson_bag = new WatsonBag(watson);

    // cout << this->config->str() << endl;

    this->arr_mv_objs->add(sherlock);
    this->arr_mv_objs->add(watson);
    this->arr_mv_objs->add(criminal);
}

StudyPinkProgram::~StudyPinkProgram()
{
    delete this->config;
    delete this->map;
    delete this->sherlock;
    delete this->watson;
    delete this->criminal;
    delete this->arr_mv_objs;
    delete this->sherlock_bag;
    delete this->watson_bag;
}

bool StudyPinkProgram::isStop() const
{
    if (sherlock->getHp() <= 0 || watson->getHp() <= 0)
    {
        return true;
    }
    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()) || watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
    {
        return true;
    }

    RobotC *robotC = nullptr;
    for (int i = 0; i < arr_mv_objs->getMovingObjectCount(); i++)
    {
        robotC = dynamic_cast<RobotC *>(arr_mv_objs->get(i));
        if (robotC != nullptr)
        {
            if ((robotC->getCurrentPosition().isEqual(sherlock->getCurrentPosition()) && sherlock->getExp() > 500))
            {
                return true;
            }
        }
    }
    return false;
}

void StudyPinkProgram::printResult() const
{
    if (sherlock->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
    {
        cout << "Sherlock caught the criminal" << endl;
    }
    else if (watson->getCurrentPosition().isEqual(criminal->getCurrentPosition()))
    {
        cout << "Watson caught the criminal" << endl;
    }
    else
    {
        cout << "The criminal escaped" << endl;
    }
}

void StudyPinkProgram::printStep(int si) const
{
    cout << "Step: " << setw(4) << setfill('0') << si
         << "--"
         << sherlock->str() << "--|--" << watson->str() << "--|--" << criminal->str() << endl;
}

void StudyPinkProgram::run(bool verbose)
{
    for (int istep = 0; istep < config->getNumSteps(); ++istep)
    {
        for (int i = 0; i < arr_mv_objs->getMovingObjectCount(); ++i)
        {
            arr_mv_objs->get(i)->move();

            if (isStop())
            {
                printStep(istep);
                break;
            }

            if (verbose)
            {
                printStep(istep);
                // cout << "STEP" << istep << ":" << arr_mv_objs->str() << endl;
            }

            Sherlock *sherlock = dynamic_cast<Sherlock *>(arr_mv_objs->get(i));
            if (sherlock != nullptr)
            {
                sherlock->afterMove(arr_mv_objs, sherlock_bag, watson_bag);
            }
            Watson *watson = dynamic_cast<Watson *>(arr_mv_objs->get(i));
            if (watson != nullptr)
            {
                watson->afterMove(arr_mv_objs, sherlock_bag, watson_bag);
            }
            Criminal *criminal = dynamic_cast<Criminal *>(arr_mv_objs->get(i));
            if (criminal != nullptr)
            {
                criminal->createRobot(arr_mv_objs);
            }
        }
        // if (verbose)
        // {
        //     // printStep(istep);
        //     cout << "STEP" << istep << ":" << arr_mv_objs->str() << endl;
        // }
    }
    printResult();
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////