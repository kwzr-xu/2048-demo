#include "Leaderboard.h"
#include <algorithm> // 包含算法库，用于排序

// 构造函数
Leaderboard::Leaderboard(const std::string& filename) : filename(filename) {
    loadFromFile(); // 加载文件中的数据
}

// 添加得分并保存
void Leaderboard::addScore(const std::string& playerId, int score) {
    scores.emplace_back(playerId, score); // 添加新得分
    sortScores(); // 排序
    saveToFile(); // 保存到文件
}

// 获取前N名的得分
std::vector<PlayerScore> Leaderboard::getTopScores(unsigned int topN) const {
    std::vector<PlayerScore> topScores;
    for (size_t i = 0; i < std::min(scores.size(), static_cast<size_t>(topN)); ++i) {
        topScores.push_back(scores[i]);
    }
    return topScores; // 返回前N名玩家的得分
}

// 从文件加载数据
void Leaderboard::loadFromFile() {
    std::ifstream file(filename); // 打开文件
    std::string id;
    int score;
    while (file >> id >> score) {
        scores.emplace_back(id, score); // 读取数据并添加到得分向量中
    }
    file.close(); // 关闭文件
}

// 保存数据到文件
void Leaderboard::saveToFile() const {
    std::ofstream file(filename); // 打开文件
    for (const auto& score : scores) {
        file << score.playerId << " " << score.score << std::endl; // 写入数据
    }
    file.close(); // 关闭文件
}

// 排序得分
void Leaderboard::sortScores() {
    std::sort(scores.begin(), scores.end(), [](const PlayerScore& a, const PlayerScore& b) {
        return a.score > b.score; // 降序排列
        });
}