#ifndef CONSOLEPROGRESSBAR_H
#define CONSOLEPROGRESSBAR_H

#include <QObject>
#include <QTextStream>

class ConsoleProgressBar : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleProgressBar(QTextStream &out, QObject *parent = nullptr);

    static const int BAR_WIDTH = 64;

public slots:
    /**
     * @brief Set the minimum value representable by the progress bar.
     * @param minimum The minimum value.
     */
    void setMinimum(int minimum);

    /**
     * @brief Set the maximum value representable by the progress bar.
     * @param maximum The maximum value.
     */
    void setMaximum(int maximum);

    /**
     * @brief Set the range of values representable by the progress bar.
     * @param minimum The minimum value.
     * @param maximum The maximum value.
     */
    void setRange(int minimum, int maximum);

    /**
     * @brief Set the current amount of progress made.
     * @param progress The current amount of progress made.
     */
    void setValue(int progress);

private:
    /**
     * @brief Display the progress bar.
     */
    void Display() const;

    QTextStream &m_out;

    int m_minimum;
    int m_maximum;
    int m_progress;
};

#endif // CONSOLEPROGRESSBAR_H
