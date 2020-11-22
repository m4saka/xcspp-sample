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

    // Experimental settings
    xcspp::ExperimentSettings settings;
    settings.outputSummaryToStdout = true;
    
    // Initialize helper
    xcspp::ExperimentHelper helper(settings);

    // XCS hyperparameters
    xcspp::XCSParams params;
    params.n = 1200; // N (max number of classifiers)

    // Construct Enviroments
    const auto & trainEnv = helper.constructExplorationEnvironment<xcspp::DatasetEnvironment>(xcspp::CSV::ReadDatasetFromFile(datasetInputFilename));
    const auto & testEnv = helper.constructExploitationEnvironment<xcspp::DatasetEnvironment>(xcspp::CSV::ReadDatasetFromFile(datasetInputFilename));

    // Construct XCS and get reference to it
    auto & xcs = helper.constructClassifierSystem<xcspp::XCS>(trainEnv.availableActions(), params);

    // Run experiment (train & test)
    helper.runIteration(100000);

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
