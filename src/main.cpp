#include <QApplication>

#include "mainwindow.h"
#include "consoleapplication.h"

enum class CommandLineParseResult
{
    OkGui,
    OkCmd,
    Error
};

CommandLineParseResult parseCommandLine(const QApplication &app, QCommandLineParser &parser, ConsoleApplicationParameters *parameters);

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Parse command line arguments
    QCommandLineParser parser;
    parser.setApplicationDescription("Search for near-duplicates in large image collections");
    ConsoleApplicationParameters parameters;
    CommandLineParseResult action = parseCommandLine(app, parser, &parameters);

    if (action == CommandLineParseResult::OkGui)
    {
        MainWindow gui_app;
        gui_app.show();

        return app.exec();
    }
    else if (action == CommandLineParseResult::OkCmd)
    {
        ConsoleApplication console_app(parameters);
        QObject::connect(&console_app, &ConsoleApplication::finished, &app, &QApplication::exit);

        console_app.exec();
        return app.exec();
    }
    else if (action == CommandLineParseResult::Error)
    {
        qWarning() << "Error while parsing the command line arguments.";
        parser.showHelp();
    }

    return 1;
}

CommandLineParseResult parseCommandLine(const QApplication &app, QCommandLineParser &parser, ConsoleApplicationParameters *parameters)
{
    const QCommandLineOption helpOption = parser.addHelpOption();
    parser.addPositionalArgument("directories", QCoreApplication::translate("main", "A list of directories in which the application will search for image duplicates."), "[dirs...]");

    // A boolean option to launch the application in a graphical interface
    const QCommandLineOption guiOption(QStringList() << "g" << "gui",
                                       QCoreApplication::translate("main", "Launch the application in a graphical interface."));
    parser.addOption(guiOption);

    // A boolean option to include subfolders
    const QCommandLineOption recursiveOption(QStringList() << "r" << "recursive",
                                             QCoreApplication::translate("main", "Explore directories recursively."));
    parser.addOption(recursiveOption);

    // A boolean option to show progress
    QCommandLineOption showProgressOption(QStringList() << "p" << "progress",
                                          QCoreApplication::translate("main", "Show progress during execution."));
    parser.addOption(showProgressOption);

    // An option to set the output file
    const QCommandLineOption outputOption(QStringList() << "o" << "output",
                                          QCoreApplication::translate("main", "Output JSON file."),
                                          QCoreApplication::translate("main", "file"));
    parser.addOption(outputOption);

    // Process the actual command line arguments given by the user
    parser.process(app);
    const QStringList positionalArguments = parser.positionalArguments();

    // (GUI option) or (no arguments at all), launch the GUI application
    if (parser.isSet(guiOption)
    || (positionalArguments.isEmpty() && !parser.isSet(recursiveOption) && !parser.isSet(showProgressOption) && !parser.isSet(outputOption)))
    {
        return CommandLineParseResult::OkGui;
    }
    // At least a positional argument and an output file, launch the cmd application
    else if (positionalArguments.size() >= 1 && parser.isSet(outputOption))
    {
        parameters->directories.append(positionalArguments);
        parameters->exploreSubfolders = parser.isSet(recursiveOption);
        parameters->outputFile = parser.value(outputOption);
        parameters->progressBar = parser.isSet(showProgressOption);
        return CommandLineParseResult::OkCmd;
    }

    return CommandLineParseResult::Error;
}
