#include "consoleprogressbar.h"

#include <QTextStream>

#include <cmath>

ConsoleProgressBar::ConsoleProgressBar(QTextStream &out, QObject *parent) :
    QObject(parent),
    m_out(out),
    m_minimum(0),
    m_maximum(0),
    m_progress(0)
{

}

void ConsoleProgressBar::setMinimum(int minimum)
{
    m_minimum = minimum;
}

void ConsoleProgressBar::setMaximum(int maximum)
{
    m_maximum = maximum;
}

void ConsoleProgressBar::setRange(int minimum, int maximum)
{
    m_minimum = minimum;
    m_maximum = maximum;
}

void ConsoleProgressBar::setValue(int progress)
{
    m_progress = progress;

    Display();
}

void ConsoleProgressBar::Display() const
{
    if (m_minimum < m_maximum)
    {
        float progress = float(m_progress - m_minimum)/(m_maximum - m_minimum);

        // Display the progress bar
        if (BAR_WIDTH > 0)
        {
            m_out << "[";
            int pos = int(std::round(BAR_WIDTH * progress));
            for (int i = 0; i < BAR_WIDTH; ++i) {
                if (i < pos) {
                    m_out << "=";
                } else if (i == pos) {
                    m_out << ">";
                } else {
                    m_out << " ";
                }
            }
            m_out << "] ";
        }

        m_out << int(100.0 * progress) << " %\r";
        m_out.flush();
    }
}
