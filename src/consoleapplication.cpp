#include "consoleapplication.h"
#include "imageduplicategroups.h"
#include "imageduplicategroupsjsonexporter.h"

ConsoleApplication::ConsoleApplication(const QStringList &directories,
                                       bool exploreSubfolders,
                                       const QString &outputFile,
                                       bool progressBar,
                                       QObject *parent) :
    QObject(parent),
    m_out(stdout, QIODevice::WriteOnly),
    m_duplicateFinder(new ImageDuplicateFinder(this)),
    m_progressBar(new ConsoleProgressBar(m_out, this)),
    m_directories(directories),
    m_exploreSubfolders(exploreSubfolders),
    m_outputFile(outputFile)
{
    if (progressBar)
    {
        QObject::connect(m_duplicateFinder, &ImageDuplicateFinder::progressTextChanged, this, &ConsoleApplication::DisplayProgressLabelText);
        QObject::connect(m_duplicateFinder, &ImageDuplicateFinder::progressRangeChanged, m_progressBar, &ConsoleProgressBar::setRange);
        QObject::connect(m_duplicateFinder, &ImageDuplicateFinder::progressValueChanged, m_progressBar, &ConsoleProgressBar::setValue);
    }

    QObject::connect(m_duplicateFinder, &ImageDuplicateFinder::finished, this, &ConsoleApplication::ImageDuplicateFinderFinished);
}

ConsoleApplication::ConsoleApplication(const ConsoleApplicationParameters &parameters,
                                       QObject *parent) :
    ConsoleApplication(parameters.directories,
                       parameters.exploreSubfolders,
                       parameters.outputFile,
                       parameters.progressBar,
                       parent)
{

}

void ConsoleApplication::exec()
{
    m_duplicateFinder->start(m_directories, m_exploreSubfolders);
}

void ConsoleApplication::DisplayProgressLabelText(const QString &text)
{
    m_out << "\n" << text << "\n";
}

void ConsoleApplication::ImageDuplicateFinderFinished()
{
    // returnCode is either 0 if the output is successfully saved, or 1 if a problem occurred
    int returnCode = 1;

    const ImageDuplicateGroups& groups = m_duplicateFinder->duplicateGroups();

    // Export the duplicates in a JSON file
    ImageDuplicateGroupsJsonExporter exporter(&groups);
    bool success = exporter.save(m_outputFile);
    m_duplicateFinder->reset();

    if (success)
    {
        returnCode = 0;
    }

    emit finished(returnCode);
}
