// achievement.hpp
#pragma once
#include <set>
#include <string>
#include <map>

class AchievementTracker
{
private:
    std::map<std::string, int> tieredAchievements;
    std::set<std::string> unlockedAchievements;

public:
    void unlock(const std::string &achievement);
    bool hasAchievement(const std::string &achievement) const;
    void displayAll() const;

    void unlockTiered(const std::string &baseName, int maxTier = 3);
    int getTier(const std::string &baseName) const;
};