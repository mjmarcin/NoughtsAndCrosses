#include "score.h"

Score::Score()
{
    m_wins = 0;
    m_draws = 0;
}

int Score::wins() const
{
    return m_wins;
}

void Score::setWins(int wins)
{
    m_wins = wins;
}

int Score::draws() const
{
    return m_draws;
}

void Score::setDraws(int draws)
{
    m_draws = draws;
}
