#pragma once

#include <QWidget>

namespace Ui {
class SystemCaptionButtons;
}

class SystemCaptionButtons : public QWidget
{
    Q_OBJECT

        public:
            explicit SystemCaptionButtons(QWidget *parent = nullptr);
            ~SystemCaptionButtons();
        signals:
            void minimizeButtonClicked();
            void miniPlayerButtonClicked();
            void closeButtonClicked();
         private:
            Ui::SystemCaptionButtons *ui;
};

