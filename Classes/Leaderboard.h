#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>
#include <vector>
#include <fstream>//处理文件输入和输出流

// 定义玩家得分的结构体
struct PlayerScore {
    std::string playerId; // 玩家ID
    int score; // 玩家得分

    // 构造函数，初始化玩家ID和得分
    PlayerScore(const std::string& id, int sc) : playerId(id), score(sc) {}
};

// 排行榜类
class Leaderboard {
public:
    // 构造函数，传入文件名用于数据持久化
    Leaderboard(const std::string& filename);

    // 添加得分
    void addScore(const std::string& playerId, int score);

    // 获取前N名的得分
    std::vector<PlayerScore> getTopScores(unsigned int topN) const;

    // 从文件加载排行榜数据
    void loadFromFile();

    // 将排行榜数据保存到文件
    void saveToFile() const;

private:
    std::string filename; // 文件名
    std::vector<PlayerScore> scores; // 存储所有玩家得分的向量

    // 对得分进行排序
    void sortScores();
};

#endif // LEADERBOARD_H