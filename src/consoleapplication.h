#ifndef CONSOLEAPPLICATION_H
#define CONSOLEAPPLICATION_H

#include <QObject>
#include <QTextStream>

#include "imageduplicatefinder.h"
#include "consoleprogressbar.h"

struct ConsoleApplicationParameters
{
    QStringList directories;
    bool exploreSubfolders;
    QString outputFile;
    bool progressBar;
};

class ConsoleApplication : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleApplication(const QStringList &directories,
                                bool exploreSubfolders,
                                const QString &outputFile,
                                bool progressBar = true,
                                QObject *parent = nullptr);

    explicit ConsoleApplication(const ConsoleApplicationParameters &parameters,
                                QObject *parent = nullptr);

signals:
    /**
     * @brief This signal is emitted when the application has finished.
     */
    void finished(int returnCode);

public slots:
    /**
     * @brief Execute the application.
     */
    void exec();

private slots:
    /**
     * @brief Display the progress text label of ImageDuplicateFinder.
     * @param text The progress text
     */
    void DisplayProgressLabelText(const QString &text);

    /**
     * @brief Called when the execution of ImageDuplicateFinder has finished.
     */
    void ImageDuplicateFinderFinished();

private:
    QTextStream m_out;

    ImageDuplicateFinder *m_duplicateFinder;
    ConsoleProgressBar *m_progressBar;

    QStringList m_directories;
    bool m_exploreSubfolders;
    QString m_outputFile;
};

#endif // CONSOLEAPPLICATION_H
