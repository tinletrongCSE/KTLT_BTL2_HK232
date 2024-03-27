/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 1
 * Programming Fundamentals Spring 2023
 * Author: Vu Van Tien
 * Date: 02.02.2023
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_STUDY_IN_PINK_2_H_
#define _H_STUDY_IN_PINK_2_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Position;

class MapElement;
class Path;
class Wall;
class FakeWall;

class Map;

class MovingObject;
class Character;
class Sherlock;
class Watson;
class Criminal;

class ArrayMovingObject;

class Configuration;

class RobotS;
class RobotW;
class RobotSW;
class RobotC;

class BaseItem;
class MagicBook;
class EnergyDrink;
class FirstAid;
class ExemptionCard;
class PassingCard;

class BaseBag;
class SherlockBag;
class WatsonBag;

class StudyPinkProgram;
class TestStudyPink;

enum ItemType
{
    MAGIC_BOOK,
    ENERGY_DRINK,
    FIRST_AID,
    EXCEMPTION_CARD,
    PASSING_CARD
};
enum ElementType
{
    PATH,
    WALL,
    FAKE_WALL
};
enum RobotType
{
    C = 0,
    S,
    W,
    SW
};

// Hàm hỗ trợ - Support functions
int GetDistance(Position pos1, Position pos2);

/**
 * CLASS POSITION
 **/
class Position
{
private:
    int r, c;

public:
    friend class TestStudyPink;
    static const Position npos;

    // Constructor
    // Position() : r(0), c(0) {}
    Position(int in_r = 0, int in_c = 0) : r(in_r), c(in_c) {}
    Position(const string &str_pos);

    // Getter & Setter
    int getRow() const { return r; }
    int getCol() const { return c; }
    void setRow(int r) { this->r = r; }
    void setCol(int c) { this->c = c; }

    // Tạo chuỗi string Position
    string str() const
    {
        return "(" + to_string(r) + "," + to_string(c) + ")";
    }

    // So sánh vị trí
    bool isEqual(int in_r, int in_c) const
    {
        return (r == in_r) && (c == in_c);
    }

    bool isEqual(const Position &pos) const
    {
        return isEqual(pos.r, pos.c);
    }
};

/**
 * CLASS MAP ELEMENT
 * CLASS PATH
 * CLASS WALL
 * CLASS FAKE WALL
 */
class MapElement
{
protected:
    ElementType type;

public:
    friend class TestStudyPink;

    // Constructor & Destructor
    MapElement(ElementType in_type) : type(in_type) {}
    virtual ~MapElement() {}

    // Kiểm tra loại phần tử
    virtual ElementType getType() const { return type; }
};

// PATH
class Path : public MapElement
{
public:
    friend class TestStudyPink;

    Path() : MapElement(PATH) {}
};

// WALL
class Wall : public MapElement
{
public:
    friend class TestStudyPink;

    Wall() : MapElement(WALL) {}
};

// FAKE WALL
class FakeWall : public MapElement
{
private:
    int req_exp;

public:
    friend class TestStudyPink;

    FakeWall(int in_req_exp) : MapElement(FAKE_WALL), req_exp(in_req_exp) {}

    // Lấy số exp cần để vượt qua
    int getReqExp() const { return req_exp; }
};

/**
 * CLASS MAP
 */
class Map
{
private:
    int num_rows, num_cols;
    MapElement ***map;

public:
    friend class TestStudyPink;

    // Constructor & Destructor
    Map(int in_num_rows, int in_num_cols, int in_num_walls, Position *in_array_walls, int in_num_fake_walls, Position *in_array_fake_walls);
    ~Map();

    // Kiểm tra vị trí có hợp lệ với MovingObject không
    bool isValid(const Position &pos, MovingObject *mv_obj) const;
};

/**
 * CLASS MOVING OBJECT
 * CLASS CHARACTER
 * CLASS SHERLOCK
 * CLASS WATSON
 * CLASS CRIMINAL
 */

// MOVING OBJECT
class MovingObject
{
protected:
    int index;
    Position pos;
    Map *map;
    string name;
    int numsteps_now = 0;

public:
    friend class TestStudyPink;

    // Constructor & Destructor
    MovingObject(int index, const Position pos, Map *map, const string &name = "")
        : index(index), pos(pos), map(map), name(name){};
    virtual ~MovingObject() {}

    // Getter
    int getIndex() const { return index; }
    Map *getMap() const { return map; }
    string getName() const { return name; }
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const { return pos; }

    // Di chuyển
    virtual void move();

    // Tạo chuỗi string MovingObject
    virtual string str() const = 0;
};

// CHARACTER
class Character : public MovingObject
{
public:
    friend class TestStudyPink;

    // Constructor
    Character(int index, const Position pos, Map *map, const string &name = "")
        : MovingObject(index, pos, map, name) {}
};

// SHERLOCK
class Sherlock : public Character
{
private:
    string moving_rule;
    int hp;
    int exp;

public:
    friend class TestStudyPink;

    // Constructor
    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
        : Character(index, init_pos, map, "Sherlock"), moving_rule(moving_rule), hp(init_hp), exp(init_exp) {}

    // Getter & Setter
    int getHp() const { return hp; }
    int getExp() const { return exp; }
    void setHp(int hp) { this->hp = hp; }
    void setExp(int exp) { this->exp = exp; }
    void setPos(Position pos) { this->pos = pos; }

    // Lấy vị trí tiếp theo
    Position getNextPosition() override;

    // Tạo chuỗi string Sherlock
    string str() const override
    {
        return "Sherlock[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
    }

    // Hành động sau khi di chuyển
    void afterMove(ArrayMovingObject *arr_moving_objects, SherlockBag *sherlock_bag, WatsonBag *watson_bag);
};

// WATSON
class Watson : public Character
{
private:
    string moving_rule;
    int hp;
    int exp;

public:
    friend class TestStudyPink;

    // Constructor
    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp)
        : Character(index, init_pos, map, "Watson"), moving_rule(moving_rule), hp(init_hp), exp(init_exp) {}

    // Getter & Setter
    int getHp() const { return hp; }
    int getExp() const { return exp; }
    void setHp(int hp) { this->hp = hp; }
    void setExp(int exp) { this->exp = exp; }

    // Lấy vị trí tiếp theo
    Position getNextPosition() override;

    // Tạo chuỗi string Watson
    string str() const override
    {
        return "Watson[index=" + to_string(index) + ";pos=" + pos.str() + ";moving_rule=" + moving_rule + "]";
    }

    // Hành động sau khi di chuyển
    void afterMove(ArrayMovingObject *arr_moving_objects, SherlockBag *sherlock_bag, WatsonBag *watson_bag);
};

// CRIMINAL
class Criminal : public Character
{
private:
    Sherlock *sherlock;
    Watson *watson;
    int num_steps = 0;
    Position prev_pos;

public:
    friend class TestStudyPink;

    // Constructor
    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson)
        : Character(index, init_pos, map, "Criminal"), sherlock(sherlock), watson(watson), num_steps(0) {}

    // Lấy vị trí tiếp theo
    Position getNextPosition() override;

    // Tạo robot
    void createRobot(ArrayMovingObject *arr_moving_objects);

    // Di chuyển
    void move() override;

    // Tạo chuỗi string Criminal
    string str() const override
    {
        return "Criminal[index=" + to_string(index) + ";pos=" + pos.str() + "]";
    }

    // Lấy vị trí liền trước đó
    Position getPrevPosition() const { return prev_pos; }
};

/**
 * CLASS ARRAY MOVING OBJECT
 */
class ArrayMovingObject
{
public:
    MovingObject **arr_mv_objs;
    int count;
    int capacity;

public:
    friend class TestStudyPink;

    // Constructor & Destructor
    ArrayMovingObject(int capacity)
        : capacity(capacity), count(0), arr_mv_objs(new MovingObject *[capacity]()){};

    ~ArrayMovingObject()
    {
        delete[] arr_mv_objs;
    }

    // Kiểm tra mảng đầy
    bool isFull() const { return (count >= capacity); }

    // Thêm MovingObject vào mảng
    bool add(MovingObject *mv_obj);

    // Lấy số lượng MovingObject
    int getMovingObjectCount() const { return count; }

    // Lấy MovingObject theo index
    MovingObject *get(int index) const { return arr_mv_objs[index]; }

    // Lấy MovingObject theo vị trí
    string str() const;
};

/**
 * CLASS CONFIGURATION
 */
class Configuration
{
private:
    int map_num_rows;
    int map_num_cols = 0;
    int max_num_moving_objects = 0;
    int num_walls = 0;
    Position *arr_walls = NULL;
    int num_fake_walls = 0;
    Position *arr_fake_walls = NULL;
    string sherlock_moving_rule = "";
    Position sherlock_init_pos = Position(-1, -1);
    int sherlock_init_hp = 0;
    int sherlock_init_exp = 0;
    string watson_moving_rule = "";
    Position watson_init_pos = Position(-1, -1);
    int watson_init_hp = 0;
    int watson_init_exp = 0;
    Position criminal_init_pos = Position(-1, -1);
    int num_steps;

public:
    friend class TestStudyPink;

    // Constructor & Destructor
    Configuration(const string &filepath);
    ~Configuration();
    string str() const;

    // Getter
    int getNumRows() const { return this->map_num_rows; }
    int getNumCols() const { return this->map_num_cols; }
    int getMaxNumMovingObjects() const { return this->max_num_moving_objects; }
    int getNumWalls() const { return this->num_walls; }
    Position *getArrWalls() const { return this->arr_walls; }
    int getNumFakeWalls() const { return this->num_fake_walls; }
    Position *getArrFakeWalls() const { return this->arr_fake_walls; }
    string getSherlockMovingRule() const { return this->sherlock_moving_rule; }
    Position getSherlockInitPos() const { return this->sherlock_init_pos; }
    int getSherlockInitHp() const { return this->sherlock_init_hp; }
    int getSherlockInitExp() const { return this->sherlock_init_exp; }
    string getWatsonMovingRule() const { return this->watson_moving_rule; }
    Position getWatsonInitPos() const { return this->watson_init_pos; }
    int getWatsonInitHp() const { return this->watson_init_hp; }
    int getWatsonInitExp() const { return this->watson_init_exp; }
    Position getCriminalInitPos() const { return this->criminal_init_pos; }
    int getNumSteps() const { return this->num_steps; }
};

/**
 * CLASS ROBOT
 * CLASS ROBOT C
 * CLASS ROBOT S
 * CLASS ROBOT W
 * CLASS ROBOT SW
 */

// ROBOT
class Robot : public MovingObject
{
private:
    RobotType robot_type;
    BaseItem *item;

public:
    friend class TestStudyPink;

    Criminal *criminal;

    // Constructor & Destructor
    Robot(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal);
    ~Robot();

    // Getter
    RobotType getRobotType() const { return this->robot_type; }
    BaseItem *getItem() const { return this->item; }
};

// ROBOT C
class RobotC : public Robot
{
public:
    friend class TestStudyPink;

    // Constructor
    RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal)
        : Robot(index, init_pos, map, C, criminal) {}

    // Lấy vị trí tiếp theo
    Position getNextPosition() override
    {
        return this->criminal->getPrevPosition();
    }

    // Lấy khoảng cách
    int getDistance(Character *character) const
    {
        return GetDistance(this->pos, character->getCurrentPosition());
    }

    // Tạo chuỗi string RobotC
    string str() const override
    {
        return "Robot[pos=" + this->pos.str() + ";type=C" + ";dist=]";
    }
};

// ROBOT S
class RobotS : public Robot
{
public:
    friend class TestStudyPink;

    Sherlock *sherlock;

    // Constructor
    RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock)
        : Robot(index, init_pos, map, S, criminal), sherlock(sherlock) {}

    // Lấy vị trí tiếp theo
    Position getNextPosition() override;

    // Lấy khoảng cách
    int getDistance() const
    {
        return GetDistance(this->pos, this->sherlock->getCurrentPosition());
    }

    // Tạo chuỗi string RobotS
    string str() const override
    {
        return "Robot[pos=" + this->pos.str() + ";type=S" + ";dist=" + to_string(this->getDistance()) + "]";
    }
};

// ROBOT W
class RobotW : public Robot
{
public:
    friend class TestStudyPink;

    Watson *watson;

    // Constructor
    RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson);

    // Lấy vị trí tiếp theo
    Position getNextPosition() override;

    // Lấy khoảng cách
    int getDistance() const
    {
        return GetDistance(this->pos, this->watson->getCurrentPosition());
    }

    // Tạo chuỗi string RobotW
    string str() const override
    {
        return "Robot[pos=" + this->pos.str() + ";type=W" + ";dist=" + to_string(this->getDistance()) + "]";
    }
};

// ROBOT SW
class RobotSW : public Robot
{
public:
    friend class TestStudyPink;

    Sherlock *sherlock;
    Watson *watson;

    // Constructor
    RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson)
        : Robot(index, init_pos, map, SW, criminal), sherlock(sherlock), watson(watson) {}

    // Lấy vị trí tiếp theo
    Position getNextPosition() override;

    // Lấy khoảng cách
    int getDistance() const;

    // Tạo chuỗi string RobotSW
    string str() const override
    {
        return "Robot[pos=" + this->pos.str() + ";type=SW" + ";dist=" + to_string(this->getDistance()) + "]";
    }
};

/**
 * CLASS BASE ITEM
 * CLASS MAGIC BOOK
 * CLASS ENERGY DRINK
 * CLASS FIRST AID
 * CLASS EXCEMPTION CARD
 * CLASS PASSING CARD
 */

// BASE ITEM
class BaseItem
{
public:
    friend class TestStudyPink;

    string name;

    // Constructor
    BaseItem(const string &name) : name(name) {}

    virtual bool canUse(Character *character, Robot *robot = NULL) = 0;
    virtual void use(Character *character, Robot *robot = NULL) = 0;

    // Lấy tên Item
    string getName() const { return this->name; }
};

// MAGIC BOOK
class MagicBook : public BaseItem
{
public:
    friend class TestStudyPink;

    // Constructor
    MagicBook() : BaseItem("MagicBook") {}

    bool canUse(Character *character, Robot *robot = NULL) override;
    void use(Character *character, Robot *robot = NULL) override;
};

// ENERGY DRINK
class EnergyDrink : public BaseItem
{
public:
    friend class TestStudyPink;

    // Constructor
    EnergyDrink() : BaseItem("EnergyDrink") {}

    bool canUse(Character *character, Robot *robot = NULL) override;
    void use(Character *character, Robot *robot = NULL) override;
};

// FIRST AID
class FirstAid : public BaseItem
{
public:
    friend class TestStudyPink;

    // Constructor
    FirstAid() : BaseItem("FirstAid") {}

    bool canUse(Character *character, Robot *robot = NULL) override;
    void use(Character *character, Robot *robot = NULL) override;
};

// EXCEMPTION CARD
class ExcemptionCard : public BaseItem
{
public:
    friend class TestStudyPink;

    // Constructor
    ExcemptionCard() : BaseItem("ExcemptionCard") {}

    bool canUse(Character *character, Robot *robot = NULL) override;
    void use(Character *character, Robot *robot = NULL) override;
};

// PASSING CARD
class PassingCard : public BaseItem
{
public:
    friend class TestStudyPink;

    string challenge;

    // Constructor
    PassingCard(const string &challenge) : BaseItem("PassingCard"), challenge(challenge) {}

    bool canUse(Character *character, Robot *robot = NULL) override;
    void use(Character *character, Robot *robot = NULL) override;
};

/**
 * CLASS BASE BAG
 * CLASS SHERLOCK BAG
 * CLASS WATSON BAG
 */

// NODE ITEM
struct NodeItem
{
    BaseItem *item;
    NodeItem *next;

    NodeItem(BaseItem *item, NodeItem *next = NULL) : item(item), next(next) {}
};

// BASE BAG
class BaseBag
{
protected:
    int capacity;
    int count = 0;
    Character *obj;
    NodeItem *head = NULL;

public:
    friend class TestStudyPink;

    // Constructor & Destructor
    BaseBag() {}
    ~BaseBag();

    // Thêm Item vào Bag
    virtual bool insert(BaseItem *item);

    virtual BaseItem *get();
    virtual BaseItem *get(ItemType itemType);

    // Tạo chuỗi string Bag
    virtual string str() const;
};

// SHERLOCK BAG
class SherlockBag : public BaseBag
{
public:
    friend class TestStudyPink;

    // Constructor
    SherlockBag(Sherlock *sherlock);
};

// WATSON BAG
class WatsonBag : public BaseBag
{
public:
    friend class TestStudyPink;

    // Constructor
    WatsonBag(Watson *watson);
};

/**
 * CLASS STUDY PINK PROGRAM
 */
class StudyPinkProgram
{
private:
    Configuration *config;
    Sherlock *sherlock;
    Watson *watson;
    Criminal *criminal;
    Map *map;
    ArrayMovingObject *arr_mv_objs;
    SherlockBag *sherlock_bag;
    WatsonBag *watson_bag;

public:
    friend class TestStudyPink;

    // Constructor & Destructor
    StudyPinkProgram(const string &config_file_path);

    bool isStop() const;
    void printResult() const;
    void printStep(int si) const;
    void run(bool verbose);

    ~StudyPinkProgram();
};

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
#endif // _H_STUDY_IN_PINK_2_H_
