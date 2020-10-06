#include <iostream>
#include <vector>
#include <xcspp/xcspp.hpp>

int main()
{
    // XCS hyperparameters
    xcspp::XCSParams params;
    params.n = 1200; // N (max number of classifiers)

    // Environment (i.e., the problem to solve)
    xcspp::DatasetEnvironment env(xcspp::CSV::ReadDatasetFromFile("dataset.csv"));

    // XCS classifier system
    xcspp::XCS xcs(env.availableActions(), params);

    // Train
    for (int i = 0; i < 50000; ++i)
    {
        // Get current situation from environment
        std::vector<int> situation = env.situation();

        // Select action by XCS
        int action = xcs.explore(situation);

        // Execute action and receive reward from environment
        double reward = env.executeAction(action);

        // Feedback the reward to XCS
        xcs.reward(reward);
    }

    // Test
    double rewardSum = 0.0;
    for (int i = 0; i < 50000; ++i)
    {
        // Get current situation from environment
        std::vector<int> situation = env.situation();

        // Select action by XCS
        int action = xcs.exploit(situation);

        // Execute action and receive reward from environment
        rewardSum += env.executeAction(action);
    }

    // Calculate average accuracy of test results
    double rewardAverage = rewardSum / 50000;

    std::cout << "=========================================" << std::endl;
    std::cout << "Classification Accuracy: " << (rewardAverage / 10.0) << '%' << std::endl;
    std::cout << "=========================================" << std::endl;

    // Show acquired rules
    std::cout << "Acquired Rules:" << std::endl;
    xcs.outputPopulationCSV(std::cout);

    return 0;
}
