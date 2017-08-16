#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QMainWindow>

#include "data_display.h"
#include <qwt_plot_curve.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_point_data.h>
#include "gva.h"
#include "log_recv.h"

namespace Ui {
class MYWindow;
}

enum CMD{CONNECT,CONFIG,START,STOP,DISCONNECT,SAVE,DISPLAY,START_DISPLAY,STOP_DISPLAY};
class MYWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MYWindow(QWidget *parent = 0);
    ~MYWindow();
    bool sendcmd(CMD cmd);
    int recvn(int sock,u_int32_t* m_buffer,size_t size);
    void draw();
signals:
    void sig_stopdisplay();
    void sig_stop();

private slots:
    void onbtnConnect();
    void onbtnStart();
    void onbtnStop();
    void onbtnConfig();
    void onbtnDisconnect();

    void onbtnStartdisplay();
    void onbtnStopdisplay();
    void onradioSave();
    void onradioDisplay();

    void ondrawsignal(int len);
    void onlogsignal(/*AF_DATA_REQUEST(*/QVariant);

private:
    Ui::MYWindow *ui;
    int m_sock_cmd;
    //int m_sock_data;

    mythread* mt;
    log_recv* l;
    //u_int32_t* m_buffer;
    bool m_connected;
    bool m_runned;
    bool m_configured;
    bool m_stopped;
    bool m_disconnected;
    bool m_loged;

    bool m_startdisplay;
    bool m_stopdisplay;
    bool m_save ;
    bool m_display ;
    int length;   //words
    int first;
    QwtPlotCurve* curve;
};

#endif // MYWINDOW_H
