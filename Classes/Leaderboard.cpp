#include "Leaderboard.h"
#include <algorithm> // �����㷨�⣬��������

// ���캯��
Leaderboard::Leaderboard(const std::string& filename) : filename(filename) {
    loadFromFile(); // �����ļ��е�����
}

// ��ӵ÷ֲ�����
void Leaderboard::addScore(const std::string& playerId, int score) {
    scores.emplace_back(playerId, score); // ����µ÷�
    sortScores(); // ����
    saveToFile(); // ���浽�ļ�
}

// ��ȡǰN���ĵ÷�
std::vector<PlayerScore> Leaderboard::getTopScores(unsigned int topN) const {
    std::vector<PlayerScore> topScores;
    for (size_t i = 0; i < std::min(scores.size(), static_cast<size_t>(topN)); ++i) {
        topScores.push_back(scores[i]);
    }
    return topScores; // ����ǰN����ҵĵ÷�
}

// ���ļ���������
void Leaderboard::loadFromFile() {
    std::ifstream file(filename); // ���ļ�
    std::string id;
    int score;
    while (file >> id >> score) {
        scores.emplace_back(id, score); // ��ȡ���ݲ���ӵ��÷�������
    }
    file.close(); // �ر��ļ�
}

// �������ݵ��ļ�
void Leaderboard::saveToFile() const {
    std::ofstream file(filename); // ���ļ�
    for (const auto& score : scores) {
        file << score.playerId << " " << score.score << std::endl; // д������
    }
    file.close(); // �ر��ļ�
}

// ����÷�
void Leaderboard::sortScores() {
    std::sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b) {
        return a.score > b.score; // ��������
        });
}