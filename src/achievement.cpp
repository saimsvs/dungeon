
#include "../headers/achievement.hpp"
#include <iostream>

void AchievementTracker::unlock(const std::string &achievement)
{
    unlockedAchievements.insert(achievement);
    std::cout << "★ Achievement Unlocked: " << achievement << "!\n";
}

bool AchievementTracker::hasAchievement(const std::string &achievement) const
{
    return unlockedAchievements.find(achievement) != unlockedAchievements.end();
}

void AchievementTracker::displayAll() const
{
    std::cout << "\n=== ACHIEVEMENTS ===\n";

    for (const auto &a : unlockedAchievements)
        std::cout << "► " << a << "\n";

    for (const auto &[name, tier] : tieredAchievements)
    {
        std::cout << "► " << name << " ";
        for (int i = 0; i < tier && i < 10; i++)
        {
            std::cout << "★";
        }
        std::cout << "\n";
    }
}
void AchievementTracker::unlockTiered(const std::string &baseName, int maxTier)
{
    int &currentTier = tieredAchievements[baseName];
    if (currentTier < maxTier)
    {
        currentTier++;
        std::cout << "★ " << baseName << " "
                  << (currentTier == 1 ? "I" : currentTier == 2 ? "II"
                                           : currentTier == 3   ? "III"
                                                                : std::to_string(currentTier))
                  << " Unlocked!\n";
    }
}

int AchievementTracker::getTier(const std::string &baseName) const
{
    auto it = tieredAchievements.find(baseName);
    return (it != tieredAchievements.end()) ? it->second : 0;
}
