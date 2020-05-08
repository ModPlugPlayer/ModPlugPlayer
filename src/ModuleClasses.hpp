#ifndef MODULECLASSES_HPP
#define MODULECLASSES_HPP

#include <vector>
#include <map>

struct Row{
    int rowIndex;
    int rowGlobalIndex;
    int time = -1;
    int orderIndex = -1;
};

class Order{
public:
    Order(int orderIndex, int rowAmount, int beginningRowGlobalIndex);
    bool hasRow(int rowGlobalIndex);
    Row getRow(int rowIndex);
private:
    int orderIndex;
    int rowAmount;
    int beginningRowGlobalIndex;
    std::vector<Row> rows;
};

class Orders{
public:
    //void addOrder(Order order);
    void addOrder(int rowAmount);
    Order getOrder(int orderIndex);
private:
    std::vector<Order> orders;
    std::map<int,Row> rowByRowGlobalIndex;
};

struct TimeInfo {
    int globalRowIndex;
    int seconds;
};

#endif // MODULECLASSES_HPP
