/**
 * @file Tile.cpp
 * @brief Tile class implementation file
 * @author Parker Cranfield
*/

#include "Tile.hpp"

/**
 * @brief Tile constructor
*/
Tile::Tile() : m_isBomb(false), m_state(0), m_value(0) {
    // Nothing right now
}

/**
 * @brief Tile destructor
*/
Tile::~Tile() {
    // Nothing rn
}

/**
 * @brief Set the tile as a bomb
*/
void Tile::setBomb(bool bomb) {
    m_isBomb = bomb;
}


/**
 * @brief Set the state of the tile
 * @param state
*/
void Tile::setState(int state) {
    m_state = state;
}

/**
 * @brief Set the value of the tile
 * @param value
*/
void Tile::setValue(int value) {
    m_value = value;
}

/**
 * @brief Get if the tile is a bomb
 * @return bool
*/
bool Tile::getIsBomb() {
    return m_isBomb;
}


/**
 * @brief Get the state of the tile
 * @return int
*/
int Tile::getState() {
    return m_state;
}

/**
 * @brief Get the value of the tile
 * @return int
*/
int Tile::getValue() {
    return m_value;
}