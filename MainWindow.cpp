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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(this -> windowTitle().append(". QT version ").append(qVersion()));
    //
    /** button count */
    QObject::connect(ui->button_count_slider, &QSlider::valueChanged, this, [&](int32_t pos)
                   {
                       pie_menu->setButtonCount(pos);

                       for (int32_t i = 0; i < pos; i++)
                       {
                           pie_menu->setButtonIcon(i, ":/icons/image-line-icon.png");
                       }

                       pie_menu->update();
                       ui->button_count_label->setText(QString::number(pos));
                   });
    /** Base angle */
    QObject::connect(ui->base_angle_slider, &QSlider::valueChanged, this, [&](int32_t pos)
                     {
                         pie_menu->setBaseAngle(pos);
                         pie_menu->update();
                         ui->base_angle_label->setText(QString::number(pos));
                     });

    /** Stroke width */
    QObject::connect(ui->stroke_width_slider, &QSlider::valueChanged, this, [&](int32_t pos)
                     {
                         pie_menu->setStrokeWidth(pos);
                         pie_menu->update();
                         ui->stroke_width_slider->setValue(pos);
                         ui->stroke_width_label->setText(QString("%0 px").arg(QString::number(pos)));
                     });
    /** Close button radius */
    QObject::connect(ui->close_radius_slider, &QSlider::valueChanged, this, [&](int32_t pos)
                     {
                         pie_menu->setCloseButtonRadius(pos);
                         pie_menu->update();
                         ui->close_radius_slider->setValue(pos);
                         ui->close_button_radius_label->setText(QString("%0 px").arg(QString::number(pos)));
                     });
    /** Pin button radius */
    QObject::connect(ui->pin_radius_slider, &QSlider::valueChanged, this, [&](int32_t pos)
                     {
                         pie_menu->setPinButtonRadius(pos);
                         pie_menu->update();
                         ui->pin_radius_slider->setValue(pos);
                         ui->pin_button_radius_label->setText(QString("%0 px").arg(QString::number(pos)));
                     });
    /** Checkboxes */
    QObject::connect(ui->disable_button_checkbox, &QCheckBox::clicked, this, [&](bool value)
                     {
                         pie_menu->setButtonEnabled(0, !value);
                         pie_menu->update();
                     });
    /** About QT And Theme */
    QObject::connect(ui->aboutQt, SIGNAL(clicked()), qApp, SLOT(aboutQt()));
    initPieMenu();
}

void MainWindow::initPieMenu()
{
    pie_menu = new PieMenu(ui->centralwidget);
    pie_menu->setCloseButtonIcon(QIcon(":/icons/close-line-icon.png"));
    pie_menu->setPinButtonIcon(QIcon(":/icons/pushpin-icon.png"));

    emit this->ui->stroke_width_slider->valueChanged(5);
    emit this->ui->button_count_slider->valueChanged(4);

    for (int32_t i = 0; i < 4; i++)
    {
        pie_menu->setButtonIcon(i, ":/icons/image-line-icon.png");
    }

    QObject::connect(pie_menu, &PieMenu::buttonClicked, this, [&](uint8_t index)
    {
        ui->label->setText(QString("You are clicked button #%0").arg(index));
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        pie_menu->display();
    }
    else
    {
        pie_menu->hideIfNotPinned();
    }

    QMainWindow::mouseReleaseEvent(event);
}

void MainWindow::on_loadTheme_clicked()
{
    QFileDialog fd;
    fd.setOptions(QFileDialog::DontUseNativeDialog | QFileDialog::ReadOnly);
    fd.exec();

    QString filename = fd.selectedFiles().at(0);

    QFile file(filename);
    if (file.exists()) {

        file.open(QIODevice::ReadOnly|QIODevice::Text);

        QTextStream instream(& file);
        QString style;
        while (!instream.atEnd()){
            style = instream.readAll();
        }
        this->setStyleSheet(style);
    }
}

