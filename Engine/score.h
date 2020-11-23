#ifndef SCORE_H
#define SCORE_H


class Score {
public:
    Score();
    int wins() const;
    void setWins(int wins);

    int draws() const;
    void setDraws(int draws);

private:
    int m_wins;
    int m_draws;
};

#endif // SCORE_H
