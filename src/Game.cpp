/*
   Rasende Roboter Solver, GPL Source Code

   This file is part of the Rasende Roboter Solver Source Code

   Rasende Roboter Solver Source Code is free software: you can
   redistribute it and/or modify it under the terms of the GNU General
   Public License as published by the Free Software Foundation, either
   version 3 of the License, or (at your option) any later version.

   Rasende Roboter Solver  Source Code is distributed in the hope that
   it will be useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See
   the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Rasende Roboter Solver Source Code. If not, see
   <http://www.gnu.org/licenses/>.
 */


/*===========================================================================*/
/*===============================[ Includes ]================================*/
/*===========================================================================*/
#include "Game.hpp"

#include <istream>
#include <ostream>
#include <iostream>
#include <stdexcept>

/*===========================================================================*/
/*==================================[ «» ]===================================*/
/*===========================================================================*/

void rr::Game::putVerticalWall(Line l, Column c)
{
    m_board.putVerticalWall(l,c);
}

void rr::Game::putHorizontalWall(Line l, Column c)
{
    m_board.putHorizontalWall(l,c);
}

void rr::Game::putRobot(Line l, Column c, char r)
{
    m_start_state.m_robots[Robot::char2robot(r)].put_at(l,c);
    std::cout << r << " found at " << int(l.v) << ", " << int(c.v) << "\n";
}

void rr::Game::setGoal(Line l, Column c)
{
    std::cout << "goal found at " << int(l.v) << ", " << int(c.v) << "\n";
    m_goal.set(l,c);
}

void rr::Game::checkCase(std::string const& str, Line l, Column c)
{
    const char ch = str[c*2+1];
    switch (ch)
    {
        case 'R':
        case 'B':
        case 'V':
        case 'G': this->putRobot(l,c,ch);
                  break;
        case '*': this->setGoal(l,c);
                  break;
        case ' ': break;
        default:
                  throw std::runtime_error(std::string()+"Invalid char `"+ch+"'");
    }
}

void rr::Game::readGameLine(std::string const& str, Line l)
{
    Column c=0;
    for (; c!=Board::nb_V_walls(); ++c)
    {
        {
            const char ch = str[c*2+2];
            switch (ch)
            {
                case '|': this->putVerticalWall(l,c);
                case ' ': break;
                default:
                          throw std::runtime_error(std::string()+"Invalid char `"+ch+"'");
            }
        }
        checkCase(str, l, c);
    }
    checkCase(str, l, c);
}

std::istream & operator>>(std::istream& is, rr::Game & g)
{
    std::string str;
    // 1- ignore first line (border)
    std::getline(is, str);

    // 2- read all following pair of lines
    for (rr::Line l=0; l!=rr::Board::nb_H_walls(); ++l)
    {
        // 2.1- read V walls
        std::getline(is, str);
        g.readGameLine(str,l);

        // 2.2- read H walls
        std::getline(is, str);
        for (rr::Column c=0; c!=rr::Board::nb_columns(); ++c) {
            const char ch = str[c*2+1];
            switch (ch)
            {
                case '-':
                    g.putHorizontalWall(l,c);
                case ' ':
                    break;
                default:
                    throw std::runtime_error(std::string()+"Invalid char `"+ch+"'");
            }
        }
    }

    // 3- last line
    std::getline(is, str);
    g.readGameLine(str,rr::Board::nb_H_walls());

    // ignore following line (border)
    return is;
}

std::ostream & operator<<(std::ostream& os, rr::Game const& g)
{
    return os << g.m_board;
}

