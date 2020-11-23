#include "engine.h"

Engine::Engine(QObject *parent) : QObject(parent)
{
    m_playerTileMapping.insert(PlayerO, Nought);
    m_playerTileMapping.insert(PlayerX, Cross);

    m_scoreMap.insert(PlayerO, std::move(Score()));
    m_scoreMap.insert(PlayerX, std::move(Score()));
    m_currentPlayer = EPlayerType::PlayerX;
    resetRoundParameters();
}

void Engine::resetRoundParameters()
{
    for (int tileIndex = 0; tileIndex < KNumberOfTiles; tileIndex++)
    {
        m_tileStates[tileIndex] = ETileState::Empty;
    }

    m_moveCounter = 0;
    m_roundStatus = NotFinished; 
}

void Engine::startNextRound()
{
    if (m_currentPlayer == EPlayerType::PlayerO) {
        m_currentPlayer = EPlayerType::PlayerX;
    } else {
        m_currentPlayer = EPlayerType::PlayerO;
    }

    resetRoundParameters();

    emit roundStatusChanged();
    emit winsNumberChanged();
    emit drawsNumberChanged();
    emit currentPlayerChanged();
}

void Engine::changePlayer()
{
    if (m_currentPlayer == EPlayerType::PlayerO)
    {
        m_currentPlayer = EPlayerType::PlayerX;
    }
    else
    {
        m_currentPlayer = EPlayerType::PlayerO;
    }

    emit currentPlayerChanged();
}

int Engine::getTileType(int index) const
{
    return m_tileStates[index];
}

int Engine::getCurrentPlayer() const
{
    return m_currentPlayer;
}

int Engine::getRoundStatus() const
{
    return m_roundStatus;
}

int Engine::getDrawsNumberForCurrentPlayer()
{
    return m_scoreMap[m_currentPlayer].draws();
}

int Engine::getWinsNumberForCurrentPlayer()
{
    return m_scoreMap[m_currentPlayer].wins();
}

void Engine::updateTileState(int index)
{
    m_moveCounter++;

    if (index >= 0 && index <= KNumberOfTiles)
    {
        switch (m_currentPlayer)
        {
        case EPlayerType::PlayerO:
            m_tileStates[index] = ETileState::Nought;
            break;

        case EPlayerType::PlayerX:
            m_tileStates[index] = ETileState::Cross;
            break;
        default:
            break;
        }

        emit tileStateChanged(index);
        processTileStateChange(index);
    }
}

void Engine::processTileStateChange(int index)
{
    checkForRoundCompletion(index);

    // Handle round status. In case it is not finished change player and notify about wins and draws numbers changes for the current player.
    if (m_roundStatus == ERoundStatus::NotFinished)
    {
        changePlayer();
        emit winsNumberChanged();
        emit drawsNumberChanged();
    }
    else
    {    
        if (m_roundStatus == ERoundStatus::FinishedWin)
        {
            // Update wins number for the current player.
            int currentWins = m_scoreMap[m_currentPlayer].wins();
            m_scoreMap[m_currentPlayer].setWins(currentWins + 1);
            emit winsNumberChanged();
        }
        else if (m_roundStatus == ERoundStatus::FinishedDraw)
        {
            // Update draws number for both players.
            auto it = m_scoreMap.begin();
            int currentDraws = 0;
            while (it != m_scoreMap.end())
            {
                currentDraws = it.value().draws();
                it.value().setDraws(currentDraws + 1);
                it++;
                emit drawsNumberChanged();
            }
        }

        emit roundStatusChanged();
    }
}

void Engine::checkForRoundCompletion(int lastIndex)
{
    if (checkForCompletedLines(lastIndex)) {
        // If any of the tiles lines has been completed round is finished with the win status of the current player.
        m_roundStatus = ERoundStatus::FinishedWin;
    }
    else
    {
        //If no line has been completed check for the number of moves. In case it reaches maximum number of moves that means there is a draw.
        // Otherwise the round is ongoing.
        if (m_moveCounter == KMaxTurnsNumber)
        {
            m_roundStatus = ERoundStatus::FinishedDraw;
        }
        else
        {
            m_roundStatus = ERoundStatus::NotFinished;
        }
    }
}

bool Engine::checkForCompletedLines(int tileIndex)
{
    QPoint tilePosition = indexToPoint(tileIndex);
    int linesCompleted = 0;

    if (checkRowForCompletion(tilePosition.y()))
    {
        emit lineCompleted(HorizontalLine, tilePosition.y());
        linesCompleted++;
    }
    if (checkCollumnForCompletion(tilePosition.x()))
    {
        emit lineCompleted(VerticalLine, tilePosition.x());
        linesCompleted++;
    }

    if (linesCompleted < 2 && isPointOnDownDiagonal(tilePosition) && checkDownDiagonalForCompletion())
    {
        emit lineCompleted(DownDiagonalLine, 0);
        linesCompleted++;
    }

    if (linesCompleted < 2 && isPointOnUpDiagonal(tileIndex) && checkUpDiagonalForCompletion())
    {
        emit lineCompleted(UpDiagonalLine, 0);
        linesCompleted++;
    }

    return linesCompleted != 0;
}

QPoint Engine::indexToPoint(int index)
{
    QPoint position;

    position.setX(index % KBoardSize);
    position.setY(index / KBoardSize);

    return position;
}

bool Engine::isPointOnUpDiagonal(int tileIndex)
{
    bool isOnDiagonal = false;
    int expectedIndex = 0;

    for (int i = 0; i < KBoardSize; i++)
    {
        expectedIndex = (i + 1) * (KBoardSize - 1);
        if (tileIndex == expectedIndex)
        {
            isOnDiagonal = true;
            break;
        }
    }

    return isOnDiagonal;
}

bool Engine::isPointOnDownDiagonal(QPoint point)
{
    return point.x() == point.y();
}

bool Engine::checkRowForCompletion(int rowIndex) const
{
    bool rowCompleted = true;

    int startIndex = rowIndex * KBoardSize;
    for (int i = startIndex ; i < startIndex + KBoardSize; i++)
    {
        if (m_tileStates[i] != m_playerTileMapping[m_currentPlayer])
        {
            rowCompleted = false;
            break;
        }
    }

    return rowCompleted;
}

bool Engine::checkCollumnForCompletion(int collumnIndex) const
{
    bool collumnCompleted = true;

    int startIndex = collumnIndex;
    for (int i = startIndex; i <= startIndex + KBoardSize * (KBoardSize - 1); i += KBoardSize)
    {
        if (m_tileStates[i] != m_playerTileMapping[m_currentPlayer])
        {
            collumnCompleted = false;
            break;
        }
    }

    return collumnCompleted;
}

bool Engine::checkDownDiagonalForCompletion() const
{
    bool diagonalCompleted = true;

    int index = 0;

    for (int i = 0; i < KBoardSize; i++)
    {
        if (m_tileStates[index] != m_playerTileMapping[m_currentPlayer])
        {
            diagonalCompleted = false;
            break;
        }
        index += KBoardSize + 1;
    }

    return diagonalCompleted;
}

bool Engine::checkUpDiagonalForCompletion() const
{
    bool diagonalCompleted = true;

    int index = KBoardSize - 1;

    for (int i = 0; i < KBoardSize; i++)
    {
        if (m_tileStates[index] != m_playerTileMapping[m_currentPlayer])
        {
            diagonalCompleted = false;
            break;
        }
        index += KBoardSize - 1;
    }

    return diagonalCompleted;
}
