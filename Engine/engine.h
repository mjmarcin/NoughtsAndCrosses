#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QPoint>
#include <QMap>
#include "score.h"

/// Namespace with enum types used both on C++ and QML sides.
namespace Enums {
Q_NAMESPACE

/*!
 * \brief The EPlayerType enum Player types corresponding to noughts and crosses.
 */
enum EPlayerType {
    PlayerO,
    PlayerX
};

/*!
 * \brief The ETileState enum Tile states.
 */
enum ETileState {
    Nought,
    Cross,
    Empty
};

/*!
 * \brief The ERoundStatus enum Enum representing possible round statuses.
 */
enum ERoundStatus {
    FinishedWin,
    FinishedDraw,
    NotFinished
};

/*!
 * \brief The ELineType enum Enum representing types of the crossing lines indicating full tiles line completion.
 */
enum ELineType {
    HorizontalLine,
    VerticalLine,
    DownDiagonalLine,
    UpDiagonalLine
};



Q_ENUM_NS(ETileState);
Q_ENUM_NS(ERoundStatus);
Q_ENUM_NS(EPlayerType);
Q_ENUM_NS(ELineType);

}

using namespace Enums;

/*!
 * \brief The Engine class Class representing Naughts and Crosses game engine.
 *
 * The engine class stores and updates tiles states. It is also responsible for checking status of the game rounds completion.
 * Once the game round has finished the engine sends notification by emitting appropriate signals.
 * The round is finished when line (horizontal, vertical or diagonal) of tiles is fully filled with noughts or crosses states.
 */
class Engine : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief Engine Constructor.
     * \param parent Parent QObject.
     */
    Engine(QObject* parent = nullptr);


    static const int KBoardSize = 3; /*!< Constant value representing game board size. */
    static const int KNumberOfTiles = KBoardSize * KBoardSize; /*!< Total number of tiles on the game board. */
    static const int KMaxTurnsNumber = KNumberOfTiles; /*!< Number of possible player turns during one round. */

    /*!
     * \brief updateTileState Method updates given tile state and check game round for completion.
     * \param index Tile index to update state for.
     */
    void updateTileState(int index);

    /*!
     * \brief getTileType Method returns the type of the given tile.
     * \param index Tile index to return state for.
     * \return State of the tile.
     */
    int getTileType(int index) const;

    /*!
     * \brief getCurrentPlayer Method returns current player type.
     * \return Current player type.
     */
    int getCurrentPlayer() const;

    /*!
     * \brief getRoundStatus Method returns current round status.
     * \return Current round status.
     */
    int getRoundStatus() const;

    /*!
     * \brief getDrawsNumberForCurrentPlayer Method returns number of draws for current player.
     * \return Number of draws.
     */
    int getDrawsNumberForCurrentPlayer();

    /*!
     * \brief getWinsNumberForCurrentPlayerMethod returns number of wins for current player.
     * \return Number of wins.
     */
    int getWinsNumberForCurrentPlayer();

    /*!
     * \brief startNextRound Methods resets the engine state before starting the next round.
     */
    void startNextRound();

signals:
    /*!
     * \brief tileStateChanged Signal indicating that tile state has been changed.
     * \param index Tile index.
     */
    void tileStateChanged(int index);

    /*!
     * \brief currentPlayerChanged Signal indicatin that current player has changed.
     */
    void currentPlayerChanged();

    /*!
     * \brief lineCompleted Signal indicating that line of tiles on the game board has been completed with tiles of the same type.
     * \param lineType Type of the line. (Horizontal, Vertical or Diagonal)
     * \param index Index indicating row or column of the completed line of tiles.
     */
    void lineCompleted(int lineType, int index);

    /*!
     * \brief roundStatusChanged Signal indicating that round status has changed.
     */
    void roundStatusChanged();

    /*!
     * \brief drawsNumberChanged Signal indicating that number of draws of current player has been updated.
     */
    void drawsNumberChanged();

    /*!
     * \brief winsNumberChanged Signal indicating that number of wins of the current player has been updated.
     */
    void winsNumberChanged();

private:
    EPlayerType m_currentPlayer; /*!< Type of the current player. */
    ETileState m_tileStates[KNumberOfTiles]; /*!< Array of the tiles states. */
    ERoundStatus m_roundStatus; /*!< Status of the current round. */
    QMap<EPlayerType, ETileState> m_playerTileMapping; /*!< Mapping between player type and corresponding tile type */
    QMap<EPlayerType, Score> m_scoreMap; /*!< Map storing scores for each player. */
    int m_moveCounter; /*!< Counter of compleded turns (tiles states changes) in the current round. */


    /*!
     * \brief resetRoundParameters Method for resetting round parameters: tiles states, turns counter, round status.
     */
    void resetRoundParameters();

    /*!
     * \brief processTileStateChange This method is called every time after tile state change.
     * In case round is not completed it causes current player to be changed.
     * Otherwise it updates the player scores accordingly and emits appropriate signals.
     * \param index Tile index which has been recently updated.
     */
    void processTileStateChange(int index);

    /*!
     * \brief checkForRoundCompletion Method checks for round completion and updates the round status accordingly.
     * \param lastIndex Tile index which has been recently updated.
     */
    void checkForRoundCompletion(int lastIndex);

    /*!
     * \brief indexToPoint Method for convering tile index to xy positions on the game board.
     * \param index Index of tile to be converted.
     * \return Tile position on the board.
     */
    QPoint indexToPoint(int index);

    /*!
     * \brief isPointOnUpDiagonal Method checks if index belongs to diagonal line starting at the top left corner of the game board.
     * \param tileIndex Index of the tile to be checked.
     * \return True if tile belongs to diagonal, False otherwise.
     */
    bool isPointOnUpDiagonal(int tileIndex);

    /*!
     * \brief isPointOnDownDiagonal Method checks if tile belongs to diagonal line starting at bottom left corner of the game board.
     * \param point Coordinates of the tile to be checked.
     * \return True if tile belongs to diagonal, False otherwise.
     */
    bool isPointOnDownDiagonal(QPoint point);

    /*!
     * \brief checkForCompletedLines Method check is any of the lines (horizontal, vertical or diagonal) has been filled in with tiles of the same type.
     * \param tileIndex Index of the last tile for which status has been changed.
     * \return True is any of the lines has been completed, False otherwise.
     */
    bool checkForCompletedLines(int tileIndex);

    /*!
     * \brief checkRowForCompletion Method checks if the row (horizontal line) has been completed.
     * \param rowIndex Index of the row (horizontal line) to be checked.
     * \return  True if the row has been completed, False otherwise.
     */
    bool checkRowForCompletion(int rowIndex) const;

    /*!
     * \brief checkCollumnForCompletion Method checks if the column (vertical line) has been completed.
     * \param collumnIndex Index of the column (vertical line) to be checked.
     * \return True if the column has been completed, False otherwise.
     */
    bool checkCollumnForCompletion(int collumnIndex) const;

    /*!
     * \brief checkDownDiagonalForCompletion Method checks for the completion of the diagonal line starting at the top left corner of the game board.
     * \return True if the diagonal line has been completed, False otherwise.
     */
    bool checkDownDiagonalForCompletion() const;

    /*!
     * \brief checkUpDiagonalForCompletion Method checks for the completion of the diagonal line starting at the bottom left corner of the game board.
     * \return True if the diagonal line has been completed, False otherwise.
     */
    bool checkUpDiagonalForCompletion() const;

    /*!
     * \brief changePlayer Method changes the current player.
     */
    void changePlayer();
};

#endif // ENGINE_H
