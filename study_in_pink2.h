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

// 3.3 Vị trí - Position
class Position
{
private:
    int r, c;

public:
    friend class TestStudyPink;

    static const Position npos;

    Position() : r(0), c(0) {}
    Position(int in_r, int in_c) : r(in_r), c(in_c) {}
    Position(const string &str_pos);

    int getRow() const { return this->r; };
    int getCol() const { return this->c; };
    void setRow(int r) { this->r = r; };
    void setCol(int c) { this->c = c; };

    string str() const { return "(" + to_string(this->r) + "," + to_string(this->c) + ")"; }

    bool isEqual(int in_r, int in_c) const { return (this->r == in_r) && (this->c == in_c); };
    bool isEqual(const Position &pos) const { return isEqual(pos.r, pos.c); };
};

// 3.1 Thành phần bản đồ - MapElement
class MapElement
{
protected:
    ElementType type;

public:
    friend class TestStudyPink;

    MapElement(ElementType in_type) { this->type = in_type; };
    virtual ~MapElement(){};
    virtual ElementType getType() const { return this->type; };
};

class Path : public MapElement
{
public:
    friend class TestStudyPink;

    Path() : MapElement(PATH){};
};

class Wall : public MapElement
{
public:
    friend class TestStudyPink;

    Wall() : MapElement(WALL){};
};

class FakeWall : public MapElement
{
private:
    int req_exp;

public:
    friend class TestStudyPink;

    FakeWall(int in_req_exp) : MapElement(FAKE_WALL) { this->req_exp = in_req_exp; };
    int getReqExp() { return this->req_exp; };
};

// 3.2 Bản đồ - Map
class Map
{
private:
    int num_rows, num_cols;

public:
    friend class TestStudyPink;

    MapElement ***map;
    Map(int in_num_rows, int in_num_cols, int in_num_walls, Position *in_array_walls, int in_num_fake_walls, Position *in_array_fake_walls);
    ~Map();

    bool isValid(const Position &pos, MovingObject *mv_obj) const;
};

// 3.4 Đối tượng di chuyển - MovingObject
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

    MovingObject(int index, const Position pos, Map *map, const string &name = "");
    virtual ~MovingObject(){};
    int getIndex() const { return this->index; };
    Map *getMap() const { return this->map; };
    string getName() const { return this->name; };
    virtual Position getNextPosition() = 0;
    Position getCurrentPosition() const { return this->pos; };
    virtual void move();
    virtual string str() const = 0;
};

class Character : public MovingObject
{
public:
    friend class TestStudyPink;

    Character(int index, const Position pos, Map *map, const string &name = "") : MovingObject(index, pos, map, name){};
};

// 3.5 Đối tượng di chuyển - Robot
class Sherlock : public Character
{
private:
    string moving_rule;
    int hp;
    int exp;

public:
    friend class TestStudyPink;

    Sherlock(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Sherlock"), moving_rule(moving_rule), hp(init_hp), exp(init_exp){};
    int getHp() const { return this->hp; };
    int getExp() const { return this->exp; };
    void setHp(int hp) { this->hp = hp; };
    void setExp(int exp) { this->exp = exp; };
    Position getNextPosition() override;
    string str() const override { return "Sherlock[index=" + to_string(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]"; };
    void afterMove(ArrayMovingObject *arr_moving_objects, SherlockBag *sherlock_bag, WatsonBag *watson_bag);
};

// 3.6 Đối tượng di chuyển - Watson
class Watson : public Character
{
private:
    string moving_rule;
    int hp;
    int exp;

public:
    friend class TestStudyPink;

    Watson(int index, const string &moving_rule, const Position &init_pos, Map *map, int init_hp, int init_exp) : Character(index, init_pos, map, "Watson"), moving_rule(moving_rule), hp(init_hp), exp(init_exp){};
    int getHp() const { return this->hp; };
    int getExp() const { return this->exp; };
    void setHp(int hp) { this->hp = hp; };
    void setExp(int exp) { this->exp = exp; };
    Position getNextPosition() override;
    string str() const override { return "Watson[index=" + to_string(this->index) + ";pos=" + this->pos.str() + ";moving_rule=" + this->moving_rule + "]"; };
    void afterMove(ArrayMovingObject *arr_moving_objects, SherlockBag *sherlock_bag, WatsonBag *watson_bag);
};

// 3.7 Đối tượng di chuyển - Criminal
class Criminal : public Character
{
private:
    Sherlock *sherlock;
    Watson *watson;
    int num_steps = 0;
    Position prev_pos;

public:
    friend class TestStudyPink;

    Criminal(int index, const Position &init_pos, Map *map, Sherlock *sherlock, Watson *watson) : Character(index, init_pos, map, "Criminal"), sherlock(sherlock), watson(watson)
    {
        this->num_steps = 0;
    };
    Position getNextPosition() override;
    void createRobot(ArrayMovingObject *arr_moving_objects);
    void move() override;
    string str() const override { return "Criminal[index=" + to_string(this->index) + ";pos=" + this->pos.str() + "]"; };
    Position getPrevPosition() const { return this->prev_pos; };
};

// 3.8 Mảng đối tượng di chuyển - ArrayMovingObject
class ArrayMovingObject
{
public:
    MovingObject **arr_mv_objs;
    int count;
    int capacity;

public:
    friend class TestStudyPink;

    ArrayMovingObject(int capacity);
    ~ArrayMovingObject();
    bool isFull() const { return (this->count >= this->capacity); };
    bool add(MovingObject *mv_obj);
    int getMovingObjectCount() const { return this->count; };
    MovingObject *get(int index) const { return arr_mv_objs[index]; };
    string str() const;
};

// 3.9 Cấu hình cho chương trình - Configuration
class Configuration
{
private:
    int map_num_rows, map_num_cols = 0;
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

    Configuration(const string &filepath);
    ~Configuration();
    string str() const;
    int getNumRows() const { return this->map_num_rows; };
    int getNumCols() const { return this->map_num_cols; };
    int getMaxNumMovingObjects() const { return this->max_num_moving_objects; };
    int getNumWalls() const { return this->num_walls; };
    Position *getArrWalls() const { return this->arr_walls; };
    int getNumFakeWalls() const { return this->num_fake_walls; };
    Position *getArrFakeWalls() const { return this->arr_fake_walls; };
    string getSherlockMovingRule() const { return this->sherlock_moving_rule; };
    Position getSherlockInitPos() const { return this->sherlock_init_pos; };
    int getSherlockInitHp() const { return this->sherlock_init_hp; };
    int getSherlockInitExp() const { return this->sherlock_init_exp; };
    string getWatsonMovingRule() const { return this->watson_moving_rule; };
    Position getWatsonInitPos() const { return this->watson_init_pos; };
    int getWatsonInitHp() const { return this->watson_init_hp; };
    int getWatsonInitExp() const { return this->watson_init_exp; };
    Position getCriminalInitPos() const { return this->criminal_init_pos; };
    int getNumSteps() const { return this->num_steps; };
};

// 3.10 Robot - Robot
class Robot : public MovingObject
{
private:
    RobotType robot_type;
    BaseItem *item;

public:
    friend class TestStudyPink;

    Criminal *criminal;

    Robot(int index, const Position &init_pos, Map *map, RobotType robot_type, Criminal *criminal);
    ~Robot();
    RobotType getRobotType() { return this->robot_type; };
    BaseItem *getItem() { return this->item; };
};

class RobotC : public Robot
{
public:
    friend class TestStudyPink;

    RobotC(int index, const Position &init_pos, Map *map, Criminal *criminal) : Robot(index, init_pos, map, C, criminal){};
    Position getNextPosition() override { return this->criminal->getPrevPosition(); };
    int getDistance(Character *character) const { return GetDistance(this->pos, character->getCurrentPosition()); };
    string str() const override { return "Robot[pos=" + this->pos.str() + ";type=C" + ";dist=]"; };
};

class RobotS : public Robot
{
public:
    friend class TestStudyPink;

    Sherlock *sherlock;

    RobotS(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock) : Robot(index, init_pos, map, S, criminal), sherlock(sherlock){};
    Position getNextPosition() override;
    int getDistance() const { return GetDistance(this->pos, this->sherlock->getCurrentPosition()); };
    string str() const override { return "Robot[pos=" + this->pos.str() + ";type=S" + ";dist=" + to_string(this->getDistance()) + "]"; };
};

class RobotW : public Robot
{
public:
    friend class TestStudyPink;

    Watson *watson;

    RobotW(int index, const Position &init_pos, Map *map, Criminal *criminal, Watson *watson);
    Position getNextPosition() override;
    int getDistance() const { return GetDistance(this->pos, this->watson->getCurrentPosition()); };
    string str() const override { return "Robot[pos=" + this->pos.str() + ";type=W" + ";dist=" + to_string(this->getDistance()) + "]"; };
};

class RobotSW : public Robot
{
public:
    friend class TestStudyPink;

    Sherlock *sherlock;
    Watson *watson;

    RobotSW(int index, const Position &init_pos, Map *map, Criminal *criminal, Sherlock *sherlock, Watson *watson) : Robot(index, init_pos, map, SW, criminal), sherlock(sherlock), watson(watson){};
    Position getNextPosition() override;
    int getDistance() const;
    string str() const override { return "Robot[pos=" + this->pos.str() + ";type=SW" + ";dist=" + to_string(this->getDistance()) + "]"; };
};

// 3.11 Vật phẩm - BaseItem
class BaseItem
{
public:
    friend class TestStudyPink;

    string name;
    BaseItem(const string &name) : name(name){};
    virtual bool canUse(Character *character, Robot *robot = NULL) = 0;
    virtual void use(Character *character, Robot *robot = NULL) = 0;
    string getName() const { return this->name; };
};

class MagicBook : public BaseItem
{
public:
    friend class TestStudyPink;

    MagicBook() : BaseItem("MagicBook"){};
    bool canUse(Character *character, Robot *robot = NULL) override;
    void use(Character *character, Robot *robot = NULL) override;
};

class EnergyDrink : public BaseItem
{
public:
    friend class TestStudyPink;

    EnergyDrink() : BaseItem("EnergyDrink"){};
    bool canUse(Character *character, Robot *robot = NULL) override;
    void use(Character *character, Robot *robot = NULL) override;
};

class FirstAid : public BaseItem
{
public:
    friend class TestStudyPink;

    FirstAid() : BaseItem("FirstAid"){};
    bool canUse(Character *character, Robot *robot = NULL) override;
    void use(Character *character, Robot *robot = NULL) override;
};

class ExcemptionCard : public BaseItem
{
public:
    friend class TestStudyPink;

    ExcemptionCard() : BaseItem("ExcemptionCard"){};
    bool canUse(Character *character, Robot *robot = NULL) override;
    void use(Character *character, Robot *robot = NULL) override;
};

class PassingCard : public BaseItem
{
public:
    friend class TestStudyPink;

    string challenge;
    PassingCard(const string &challenge) : BaseItem("PassingCard"), challenge(challenge){};
    bool canUse(Character *character, Robot *robot = NULL) override;
    void use(Character *character, Robot *robot = NULL) override;
};

// 3.12 Túi đồ - BaseBag
struct NodeItem
{
    BaseItem *item;
    NodeItem *next;
    NodeItem(BaseItem *item, NodeItem *next = NULL) : item(item), next(next){};
};

class BaseBag
{
protected:
    int capacity;
    int count = 0;
    Character *obj;
    NodeItem *head = NULL;

public:
    friend class TestStudyPink;

    BaseBag(){};
    ~BaseBag();
    virtual bool insert(BaseItem *item);
    virtual BaseItem *get();
    virtual BaseItem *get(ItemType itemType);
    virtual string str() const;
};

class SherlockBag : public BaseBag
{
public:
    friend class TestStudyPink;

    SherlockBag(Sherlock *sherlock);
};

class WatsonBag : public BaseBag
{
public:
    friend class TestStudyPink;

    WatsonBag(Watson *watson);
};

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
