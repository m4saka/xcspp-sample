#include <iostream>
#include <vector>
#include <xcspp/xcspp.hpp>

int main()
{
    // Input Filenames
    const std::string datasetInputFilename = "dataset.csv";
    const std::string newDataInputFilename = "new_data.csv";

    // Output Filenames
    const std::string classifierOutputFilename = "classifier.csv";
    const std::string estimationResultOutputFilename = "estimation_result.csv";

    // XCS hyperparameters
    xcspp::XCSParams params;
    params.n = 1200; // N (max number of classifiers)

    // Environment (i.e., the problem to solve)
    xcspp::DatasetEnvironment env(xcspp::CSV::ReadDatasetFromFile(datasetInputFilename));
    std::cout << '\'' << datasetInputFilename << "' has been loaded." << std::endl;

    // XCS classifier system
    xcspp::XCS xcs(env.availableActions(), params);

    // Train
    std::cout << "Training..." << std::endl;
    for (int i = 0; i < 100000; ++i)
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
    std::cout << "Testing..." << std::endl;
    {
        double rewardSum = 0.0;
        for (int i = 0; i < 100000; ++i)
        {
            // Get current situation from environment
            std::vector<int> situation = env.situation();

            // Select action by XCS
            int action = xcs.exploit(situation);

            // Execute action and receive reward from environment
            rewardSum += env.executeAction(action);
        }

        // Calculate average accuracy of test results
        double rewardAverage = rewardSum / 100000;

        std::cout << "=========================================================" << std::endl;
        std::cout << "Test Classification Accuracy (100,000 times): " << (rewardAverage / 10.0) << '%' << std::endl;
        std::cout << "=========================================================" << std::endl;
    }

    // Save classifier.csv
    xcs.savePopulationCSVFile("classifier.csv");
    std::cout << "Acquired ruleset has been saved to 'classifier.csv'." << std::endl;

    // Estimation
    {
        auto situations = xcspp::CSV::LoadCSVFile<int>(newDataInputFilename);
        std::cout << '\'' << newDataInputFilename << "' has been loaded." << std::endl;

        // Choose the best action for each situation
        for (auto & situation : situations)
        {
            int action = xcs.exploit(situation);
            situation.push_back(action);
        }

        // Save CSV file
        xcspp::CSV::SaveCSVFile(estimationResultOutputFilename, situations);

        std::cout << "Estimation result has been saved to '" << estimationResultOutputFilename << "'." << std::endl;
    }

    return 0;
}
