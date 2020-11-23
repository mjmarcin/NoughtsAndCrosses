#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QStringList>

#include "Engine/engine.h"


/*!
 * \brief The Controller class providing communication between the c++ backend (game engine) and QML frontend (the QML view).
 */
class Controller : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int currentPlayer READ currentPlayer NOTIFY currentPlayerChanged)
    Q_PROPERTY(int roundStatus READ roundStatus NOTIFY roundStatusChanged)
    Q_PROPERTY(int drawsNumber READ drawsNumber NOTIFY drawsNumberChanged)
    Q_PROPERTY(int winsNumber READ winsNumber NOTIFY winsNumberChanged)

signals:
    /*!
     * \brief tileStateChanged Signal emitted when tile state changes.
     * \param tileIndex Index of the tile which state has changed.
     */
    void tileStateChanged(int tileIndex);

    /*!
     * \brief currentPlayerChanged Signal emitted when current player has been changed.
     */
    void currentPlayerChanged();

    /*!
     * \brief roundStatusChanged Signal emitted when round status has changes.
     */
    void roundStatusChanged();

    /*!
     * \brief lineCompleted Signal emitted when full line has been filled with noughts or crosses.
     * \param lineType Type of the line representing one of horizontal, vertical or diagonal lines.
     * \param index Position of the line. For diagonal lines this parameter is always 0.
     */
    void lineCompleted(int lineType, int index);

    /*!
     * \brief drawsNumberChanged Signal emitted to notify about current player's number of draws change.
     */
    void drawsNumberChanged();

    /*!
     * \brief winsNumberChanged Signal emitted to notify about current player's number of wins change.
     */
    void winsNumberChanged();

public:
    /*!
     * \brief Controller
     * \param engine Game engine.
     * \param parent Parent QObject instance.
     */
    Controller(Engine& engine, QObject* parent = nullptr);

    /*!
     * \brief boardSize Getter method returning game board size.
     * \return Game board size.
     */
    Q_INVOKABLE int boardSize() const;

    /*!
     * \brief updateTileState Method for updating tile state in the game engine.
     * \param index Index of the tile which state is to be updated.
     */
    Q_INVOKABLE void updateTileState(int index);

    /*!
     * \brief getTileType Method returns the type of the board's tile.
     * \param index Index of the tile to return the type for.
     * \return Type of the tile
     */
    Q_INVOKABLE int getTileType(int index) const;

    /*!
     * \brief startNextRound Method for starting the next round in the game engine.
     */
    Q_INVOKABLE void startNextRound();

private:
    Engine& m_engine; /*!< Reference to game engine. */

    /*!
     * \brief currentPlayer Method for retrieving current player from game engine.
     * \return  Current player type.
     */
    int currentPlayer() const;

    /*!
     * \brief roundStatus Method for retrieving current round status from game engine.
     * \return Current round status.
     */
    int roundStatus() const;

    /*!
     * \brief drawsNumber Method for retrieving current number of draws for current player from game engine.
     * \return Number of draws for current player.
     */
    int drawsNumber() const;

    /*!
     * \brief winsNumber Method for retrieving current number of wins for current player from game engine.
     * \return Number of wins for current player.
     */
    int winsNumber() const;
};

#endif // CONTROLLER_H
