#include "controller.h"

Controller::Controller(Engine &engine, QObject *parent) : QObject(parent), m_engine(engine)
{
    QObject::connect(&engine, &Engine::tileStateChanged, this, &Controller::tileStateChanged);
    QObject::connect(&engine, &Engine::currentPlayerChanged, this, &Controller::currentPlayerChanged);
    QObject::connect(&engine, &Engine::roundStatusChanged, this, &Controller::roundStatusChanged);
    QObject::connect(&engine, &Engine::lineCompleted, this, &Controller::lineCompleted);
    QObject::connect(&engine, &Engine::drawsNumberChanged, this, &Controller::drawsNumberChanged);
    QObject::connect(&engine, &Engine::winsNumberChanged, this, &Controller::winsNumberChanged);
}

int Controller::boardSize() const
{
    return Engine::KBoardSize;
}

int Controller::currentPlayer() const
{
    return m_engine.getCurrentPlayer();
}

int Controller::roundStatus() const
{
    return m_engine.getRoundStatus();
}

int Controller::drawsNumber() const
{
    return m_engine.getDrawsNumberForCurrentPlayer();
}

int Controller::winsNumber() const
{
    return m_engine.getWinsNumberForCurrentPlayer();
}

void Controller::updateTileState(int index)
{
    m_engine.updateTileState(index);
}

int Controller::getTileType(int index) const
{
    return m_engine.getTileType(index);
}

void Controller::startNextRound()
{
    m_engine.startNextRound();
}
