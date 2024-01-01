#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <vector>
#include <fstream>//�����ļ�����������

// ������ҵ÷ֵĽṹ��
struct PlayerScore {
    std::string playerId; // ���ID
    int score; // ��ҵ÷�

    // ���캯������ʼ�����ID�͵÷�
    PlayerScore(const std::string& id, int sc) : playerId(id), score(sc) {}
};

// ���а���
class Leaderboard {
public:
    // ���캯���������ļ����������ݳ־û�
    Leaderboard(const std::string& filename);

    // ��ӵ÷�
    void addScore(const std::string& playerId, int score);

    // ��ȡǰN���ĵ÷�
    std::vector<PlayerScore> getTopScores(unsigned int topN) const;

    // ���ļ��������а�����
    void loadFromFile();

    // �����а����ݱ��浽�ļ�
    void saveToFile() const;

private:
    std::string filename; // �ļ���
    std::vector<PlayerScore> scores; // �洢������ҵ÷ֵ�����

    // �Ե÷ֽ�������
    void sortScores();
};

#endif // LEADERBOARD_H