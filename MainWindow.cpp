/**
 * @file MainWindow.cpp
 * @author Simon Buchholz
 * @date 9 Jan 2023
 * @brief Demo program for the PieMenuQt widget
 *
 * The pie menu is fully customizable for arbitrary amounts of buttons,
 * disabled buttons, button icons and appearance parameters.
 * All button clicks are published as a Qt signal containing the
 * clicked button's index.
 *
 * When the pin/unpin button is activated, the pie menu can be pinned
 * to keep it open until it is manually or programmatically closed.
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QMouseEvent>
#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(this -> windowTitle().append(". QT version ").append(qVersion()));
    //
    /** button count */
    QObject::connect(ui->button_count_slider, &QSlider::valueChanged, this, [&](int32_t pos) {
                       pie_menu->setButtonCount(pos);

                       for (int32_t i = 0; i < pos; i++) {
                           pie_menu->setButtonIcon(i, ":/icons/image-line-icon.png");
                       }

                       pie_menu->update();
                       ui->button_count_label->setText(QString::number(pos));
                   });
    /** Base angle */
    QObject::connect(ui->base_angle_slider, &QSlider::valueChanged, this, [&](int32_t pos) {
                         pie_menu->setBaseAngle(pos);
                         pie_menu->update();
                         ui->base_angle_label->setText(QString::number(pos));
                     });

    /** Stroke width */
    QObject::connect(ui->stroke_width_slider, &QSlider::valueChanged, this, [&](int32_t pos) {
                         pie_menu->setStrokeWidth(pos);
                         pie_menu->update();
                         ui->stroke_width_slider->setValue(pos);
                         ui->stroke_width_label->setText(QString("%0 px").arg(QString::number(pos)));
                     });
    /** Close button radius */
    QObject::connect(ui->close_radius_slider, &QSlider::valueChanged, this, [&](int32_t pos) {
                         pie_menu->setCloseButtonRadius(pos);
                         pie_menu->update();
                         ui->close_radius_slider->setValue(pos);
                         ui->close_button_radius_label->setText(QString("%0 px").arg(QString::number(pos)));
                     });
    /** Pin button radius */
    QObject::connect(ui->pin_radius_slider, &QSlider::valueChanged, this, [&](int32_t pos) {
                         pie_menu->setPinButtonRadius(pos);
                         pie_menu->update();
                         ui->pin_radius_slider->setValue(pos);
                         ui->pin_button_radius_label->setText(QString("%0 px").arg(QString::number(pos)));
                     });
    /** Checkboxes */
    QObject::connect(ui->disable_button_checkbox, &QCheckBox::clicked, this, [&](bool value)                   {
                         pie_menu->setButtonEnabled(0, !value);
                         pie_menu->update();
                     });
    /** Aleternative */
    QObject::connect(ui->alternate_colors_checkbox, &QCheckBox::clicked, this, [&](bool value)  {
         pie_menu->setAlternateColors(value);
         pie_menu->update();
    });
    /** Close button as a regular button */
    QObject::connect(ui->closeButtonAsAction, &QCheckBox::clicked, this, [&](bool value)  {
        pie_menu->setCloseButtonAsRegularButton(value);
        pie_menu->update();
        ui->closeButtonAsAction->setChecked(value);
    });
    /** Show pin button */
    QObject::connect(ui->removePinButton, &QCheckBox::clicked, this, [&](bool value)  {
        pie_menu->setShowPinButton(!value);
        if(value) {
            pie_menu->setPinned(true);
        }
        pie_menu->update();
        ui->removePinButton->setChecked(value);
    });
    /** Pie radius */
    QObject::connect(ui->pieRadiusSlider, &QSlider::valueChanged, this, [&](int32_t pos) {
        pie_menu->setPieRadius(pos);
        ui->pieRadiusSlider->setValue(pos);
        ui->pieRadiusLabel->setText(QString("%0 px").arg(QString::number(pos)));
    });
    /** Move to center */
    QObject::connect(ui->centerButton, &QPushButton::clicked, this, [&](bool) {
        pie_menu->move(this->geometry().width() / 2 - pie_menu->geometry().width() / 2,
                       this->geometry().height() / 2 - pie_menu->geometry().height() / 2);
    });
    /** Load theme */
    QObject::connect(ui->loadTheme, &QPushButton::clicked, this, [&](bool) {
        QFileDialog fd;
        fd.setOptions(QFileDialog::DontUseNativeDialog | QFileDialog::ReadOnly);
        fd.exec();

        QString filename = fd.selectedFiles().at(0);
        qDebug()<< filename;
        loadTheme(QFile(filename));
    });
    /** About QT And Theme */
    QObject::connect(ui->aboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));

    initPieMenu();
    loadTheme(QFile(":/settings/black.qss"));
}

void MainWindow::initPieMenu()
{
    pie_menu = new PieMenu(ui->centralwidget);
    pie_menu->setCloseButtonIcon(QIcon(":/icons/close-line-icon.png"));
    pie_menu->setPinButtonIcon(QIcon(":/icons/pushpin-icon.png"));

    emit this->ui->stroke_width_slider->valueChanged(5);
    emit this->ui->button_count_slider->valueChanged(4);
    emit this->ui->pieRadiusSlider->valueChanged(50);
    emit this->ui->removePinButton->clicked(true);
    emit this->ui->closeButtonAsAction->clicked(true);

    for (int32_t i = 0; i < 4; i++) {
        pie_menu->setButtonIcon(i, ":/icons/image-line-icon.png");
    }

    QObject::connect(pie_menu, &PieMenu::buttonClicked, this, [&](uint8_t index) {
        ui->label->setText(QString("You are clicked button #%0").arg(index));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        pie_menu->display();
    }
    else {
        pie_menu->hideIfNotPinned();
    }

    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::loadTheme(QFile file)
{
    if (file.exists()) {
        file.open(QIODevice::ReadOnly | QIODevice::Text);

        QTextStream instream(& file);
        QString style;
        while (!instream.atEnd()) {
            style = instream.readAll();
        }
        this->setStyleSheet(style);
    }
}


