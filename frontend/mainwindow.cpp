/**
 * @file mainwindow.cpp
 * @brief Definition of the MainWindow class' functions.
 */

#include <QFileDialog>
#include <QMessageBox>

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    front(new Frontend(this))
{
    ui->setupUi(this);
    ui->cmdStart->setEnabled(false);

    front->run();

    connect(front, SIGNAL(updateGUI(MyImage,int,double)), this, SLOT(updateImageSlot(MyImage,int,double)));
    connect(front, SIGNAL(updateStatus(bool)), this, SLOT(updateStatusSlot(bool)));
}

MainWindow::~MainWindow()
{
    emit front->frontendStatus(false);

    delete front;
    delete ui;
}

void MainWindow::updateImageSlot(const MyImage &image, int crc_flags, double ber)
{
    ui->plProcessed->setPixmap(QPixmap::fromImage(image));
    ui->lcdCrcFlag->display(crc_flags);
    ui->lcdBer->display(100 * ber);
}

void MainWindow::updateStatusSlot(bool backend_present)
{
    if(backend_present) {
        ui->lblBackendStatus->setText("Backend is present.");
        ui->cmdStart->setEnabled(true);
    } else {
        ui->lblBackendStatus->setText("Backend is not present.");
        ui->cmdStart->setEnabled(false);
    }
}

void MainWindow::on_cmdLoad_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open Picture File", "", "Image Files (*.jpg *.jpeg *.png);;All Files (*)");

    QPixmap pix(filename);
    ui->plOriginal->setPixmap(pix);

    ui->plOriginal->setFixedWidth(pix.width());
    ui->plOriginal->setFixedHeight(pix.height());
    ui->plProcessed->setFixedWidth(pix.width());
    ui->plProcessed->setFixedHeight(pix.height());

    front->loadOriginal(filename);
}

void MainWindow::on_cmdStart_clicked()
{
    bool ok = false;
    int crc = ui->cbCrcPoly->currentText().split(" ")[0].toInt(&ok,16);

    if(ok) {
        do {
            front->setPolyAppend(crc & 1);
            crc >>= 1;
        } while(crc > 0);
    }

    front->setEccLength(ui->sbEcc->value());
    front->setMapping(ui->cbMapping->currentIndex());
    front->setSnr(ui->sbSNR->value());

    if(ok) {
        front->startTransmission();
    } else {
        QMessageBox::critical(this, "Error while parsing CRC polynomal", "The chosen CRC polynomal could not be parsed. Please check your input and make sure to use hex-notation.", QMessageBox::Ok);
    }
}

void MainWindow::on_slSequential_valueChanged(int value)
{
    front->setSequential(value);
}
