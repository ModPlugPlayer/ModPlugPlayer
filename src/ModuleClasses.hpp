/*
ModuleClasses class declarations of ModPlug Player
Copyright (C) 2020 Volkan Orhan

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef MODULECLASSES_HPP
#define MODULECLASSES_HPP

#include <vector>
#include <map>

struct Row{
    int rowIndex;
    int rowGlobalIndex;
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
