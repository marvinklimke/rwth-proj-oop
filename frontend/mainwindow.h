/**
 * @file mainwindow.h
 * @brief Declaration of the MainWindow class.
 */

#pragma once

#include <QMainWindow>

#include "frontend.h"
#include "myimage.h"


namespace Ui
{
    class MainWindow;
}

/**
 * @brief The MainWindow class is in charge of handling the main application window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * @brief The MainWindow constructor.
     * @param parent The parent QObject instance.
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief The MainWindow destructor.
     * It notifies the backend to shutdown.
     */
    ~MainWindow();

public slots:

    /**
     * @brief The updateImageSlot gets triggered when a new pixelrow arrives to display relevant data on the gui.
     * @param image The new MyImage instance of the transmitted picture.
     * @param crc_flags The number of crc errors in the transmission of this pixelrow.
     * @param ber The bit-error-rate of this pixelrow.
     */
    void updateImageSlot(const MyImage &image, int crc_flags, double ber);

    /**
     * @brief The updateStatusSlot gets triggered when the backend is starting up or shutting down.
     * @param backend_present true for starting backend, false for backend shutting down.
     */
    void updateStatusSlot(bool backend_present);

private slots:

    /**
     * @brief Lets the user choose a file to load via QFileDialog.
     */
    void on_cmdLoad_clicked();

    /**
     * @brief Checks the user input, configures the backend and starts the transmission of the picture.
     */
    void on_cmdStart_clicked();

    /**
     * @brief Updates the sequential value in Frontend instance.
     * @param value The new sequential value.
     */
    void on_slSequential_valueChanged(int value);

private:

    /**
     * @brief The MainWindow instance in Ui namespace.
     */
    Ui::MainWindow *ui;

    /**
     * @brief The Frontend instance doing the background work and ipc handling.
     */
    Frontend* front;

};
