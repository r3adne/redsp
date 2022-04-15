#include <juce_core/juce_core.h>

int main(int argc, char** argv)
{
  juce::ConsoleApplication app;
  juce::UnitTestRunner runner;
  juce::int64 seed = 0;
  app.addHelpCommand("--help|-h", "use", true);
  app.addCommand({"--seed|-s", "Sets the random seed for running", "Sets the random seed for running", "", [&seed](const juce::ArgumentList& args){ seed = args.getValueForOption("--seed|-s").getLargeIntValue(); }});
  app.addCommand({"--all|-a", "Runs all tests", "Runs all tests", "",[&runner, seed](const auto&){ runner.runAllTests(seed); } } );
  app.addCommand({"--category|-c", "runs all tests in the given category", "", "", [](const juce::ArgumentList& args){} });
  return app.findAndRunCommand(argc, argv);
}