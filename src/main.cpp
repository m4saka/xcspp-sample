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

    // XCS hyperparameters
    xcspp::XCSParams params;
    params.n = 1200; // N (max number of classifiers)

    // Initialize experiment and environment
    xcspp::XCSExperimentHelper helper(settings, params);
    helper.constructEnvironments<xcspp::DatasetEnvironment>(xcspp::CSV::ReadDatasetFromFile(datasetInputFilename));

    // Run experiment (train & test)
    helper.runIteration(100000);

    // Save classifier.csv
    xcspp::XCS & xcs = helper.experiment(); // Get reference to the XCS instance in XCSExperimentHelper
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
