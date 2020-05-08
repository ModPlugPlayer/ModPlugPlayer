#ifndef HIDETITLEBAR_H
#define HIDETITLEBAR_H

#include <QWindow>

class HideTitleBar : public QObject
{
    Q_OBJECT
public:
    HideTitleBar();

public slots:
    void hideTitleBar(QWindow* window);
};

#endif // HIDETITLEBAR_H
