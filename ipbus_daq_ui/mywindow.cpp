#include "mywindow.h"
#include "ui_mywindow.h"
#include "gva.h"

#include <iostream>

u_int32_t* m_buffer;

//bool test=true;
MYWindow::MYWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MYWindow)
{
	ui->setupUi(this);
	this->setWindowTitle(QString("display"));
	//resize(900,400);
	connect(ui->btn_start,SIGNAL(clicked()),this,SLOT(onbtnStart()));
	connect(ui->btn_config,SIGNAL(clicked()),this,SLOT(onbtnConfig()));
	connect(ui->btn_stop,SIGNAL(clicked()),this,SLOT(onbtnStop()));
	connect(ui->btn_connect,SIGNAL(clicked()),this,SLOT(onbtnConnect()));
	connect(ui->btn_disconnect,SIGNAL(clicked()),this,SLOT(onbtnDisconnect()));

	connect(ui->radio_save,SIGNAL(clicked()),this,SLOT(onradioSave()));
	connect(ui->radio_display,SIGNAL(clicked()),this,SLOT(onradioDisplay()));
	connect(ui->btn_startdisplay,SIGNAL(clicked()),this,SLOT(onbtnStartdisplay()));
	connect(ui->btn_stopdisplay,SIGNAL(clicked()),this,SLOT(onbtnStopdisplay()));
	
	qRegisterMetaType<QVariant>("QVariant");   

	mt=new mythread();
	connect(mt,SIGNAL(drawsignal(int)),this,SLOT(ondrawsignal(int)),Qt::QueuedConnection);
	connect(this,SIGNAL(sig_stopdisplay()),mt,SLOT(onsigstopdisplay()),Qt::QueuedConnection);
    
	
	l=new log_recv();
    connect(l,SIGNAL(log_display(QVariant)),this,SLOT(onlogsignal(/*AF_DATA_REQUEST(*/QVariant)),Qt::QueuedConnection);
    connect(this,SIGNAL(sig_stop()),l,SLOT(onsigstop()),Qt::QueuedConnection);    // notify log thread stop

    ui->verticalScrollBar->setMaximum(ui->verticalScrollBar->maximum()+1);
    ui->verticalScrollBar->setValue(ui->verticalScrollBar->maximum()+1);

	m_connected=false;
	m_runned=false;
	m_stopped=false;
	m_configured=false;
        m_loged =false;

	m_display =false;
	m_save =false;
	m_stopdisplay =false;
	m_startdisplay = false;

	ui->btn_connect->setEnabled(true);
	ui->btn_start->setEnabled(false);
	ui->btn_config->setEnabled(false);
	ui->btn_stop->setEnabled(false);
	ui->btn_disconnect->setEnabled(false);

	ui->radio_save->setEnabled(false);
	ui->radio_display->setEnabled(false);
	ui->btn_stopdisplay->setEnabled(false);
	ui->btn_startdisplay->setEnabled(false);
	m_buffer = new u_int32_t[1024*6];
	first=0;
        l->start();
	m_loged =true;
}


MYWindow::~MYWindow()
{
	delete ui;
	delete m_buffer;
}

bool MYWindow::sendcmd(CMD cmd)
{
	if(send(m_sock_cmd,&cmd,sizeof(cmd),0)!=sizeof(cmd))
	{
		//log
		std::cout<<"send error"<<cmd<<std::endl;
		return false;
	}
	return true;
}

void MYWindow::onbtnConfig()
{
	if(m_connected==true && m_configured==false)
	{
		CMD cmd=CONFIG;
		sendcmd(cmd);
		std::cout<<"send config"<<"cmd="<<cmd<<std::endl;
		m_configured=true;
	}
	ui->btn_config->setEnabled(false);
	ui->btn_start->setEnabled(true);
}

void MYWindow::onbtnStart()
{
	if(m_configured==true && m_runned==false)
	{
		CMD cmd=START;
		sendcmd(cmd);
		std::cout<<"send start"<<"cmd="<<cmd<<std::endl;
		m_runned=true;
	}
	ui->btn_start->setEnabled(false);
	ui->btn_stop->setEnabled(true);
	if(m_display)
	{
		ui->btn_startdisplay->setEnabled(true);
	}
}

void MYWindow::onbtnStop()
{
	if(m_runned==true && m_stopped==false)
	{
		std::cout<<"enter stop"<<std::endl;
		CMD cmd= STOP;
		sendcmd(cmd);
		std::cout<<"send stop"<<"cmd="<<cmd<<std::endl;
		m_stopped=true;
	}
	if(!m_stopdisplay)
	{
		//mt->terminate();
		//mt->wait();
		emit sig_stopdisplay();  //in case stop_display buttom not clicked  
	}
	emit sig_stop();
	ui->btn_stop->setEnabled(false);
	ui->btn_disconnect->setEnabled(true);

}
void MYWindow::onbtnDisconnect()
{
	if(m_disconnected==false && m_stopped==true)
	{
		CMD cmd= DISCONNECT;
		sendcmd(cmd);
		std::cout<<"send disconnect"<<"cmd="<<cmd<<std::endl;
		m_disconnected=true;
	}
	ui->btn_disconnect->setEnabled(false);
	::close(m_sock_cmd);
	if(!m_stopdisplay)
	{
		emit sig_stopdisplay();
	}
}

void MYWindow::onbtnConnect()
{
	struct sockaddr_in addr_server,addr_client;
	addr_server.sin_family= AF_INET;
	addr_server.sin_port= htons(12345);
	addr_server.sin_addr.s_addr=inet_addr("192.168.37.12");

	addr_client.sin_family=AF_INET;
	addr_client.sin_port=htons(5600);
	addr_client.sin_addr.s_addr=inet_addr("192.168.37.12");

	m_sock_cmd=socket(AF_INET,SOCK_STREAM,0);

	if(bind(m_sock_cmd,(struct sockaddr*)&addr_client,sizeof(struct sockaddr))<0)
	{
		std::cout<<"bind error"<<std::endl;
	}
	if(::connect(m_sock_cmd,(struct sockaddr*)&addr_server,sizeof(struct sockaddr))<0)
	{
		//log
		std::cout<<"connect error"<<std::endl;
	}
	else
	{
		m_connected=true;
	}
	ui->btn_connect->setEnabled(false);
	ui->btn_config->setEnabled(true);
	ui->radio_save->setEnabled(true);
	ui->radio_display->setEnabled(true);
}

void MYWindow::onradioSave()
{
	if(m_connected =true && m_runned == false )
	{
		CMD cmd = SAVE;
		sendcmd(cmd);
		std::cout<<"send save option" <<"cmd="<<cmd<<std::endl;
	}
	ui->radio_save->setEnabled(false);
	m_save=true;
}
void MYWindow::onradioDisplay()
{
	if(m_connected = true && m_runned ==false)
	{
		CMD cmd = DISPLAY;
		sendcmd(cmd);
		std::cout<<"send display option" <<"cmd="<<cmd<<std::endl;
	}
	else
	{
		std::cout<<"cmd error"<<std::endl;
		//log
	}
	ui->radio_display->setEnabled(false);
	m_display=true;
}
void MYWindow::onbtnStartdisplay()
{
	if(m_runned==true && m_startdisplay ==false)
	{
		mt->start();
		m_startdisplay=true;
		CMD cmd=START_DISPLAY;
		sendcmd(cmd);
		ui->btn_startdisplay->setEnabled(false);
		ui->btn_stopdisplay->setEnabled(true);
		std::cout<<"send start display"<<"cmd="<<cmd<<std::endl;
	}    
}

void MYWindow::onbtnStopdisplay()
{
	if(m_runned == true && m_stopdisplay==false)
	{
		CMD cmd=STOP_DISPLAY;
		sendcmd(cmd);
		std::cout<<"send stopdisplay"<<"cmd="<<cmd<<std::endl;
	}
	m_stopdisplay =true;
	ui->btn_stopdisplay->setEnabled(false);
	emit sig_stopdisplay();
	//mt->terminate();
	//mt->wait();

}


void MYWindow::draw()
{
	QVector<double> xs,ys;
	for(int i=0;i<length;i++)
	{
		xs.append(i);
		ys.append(*(m_buffer+i));
	}
	//QwtPointArrayData * const data = new QwtPointArrayData(xs, ys);
	//QwtPlotCurve* curve=new QwtPlotCurve("hh");
	double time[10]={1,2,3,4,5,6,7,8,9,10};
	double val[10]={3,6,6,1,2,9,11,2,7,2};

	if(first==1)
	{
		std::cout<<"here!!!!!!!!!!!!"<<std::endl;
		for(int i=0;i<10;i++)
		{
			val[i] =5;
		}
	}
	//curve->setData(data);
	//curve->setStyle(QwtPlotCurve::Lines);
	//curve->attach(ui->qwtPlot);
	for(int i=0;i<10;i++)
	{
		std::cout<<val[i]<<" !";
	}
	curve->setSamples(time,val,10);
	ui->qwtPlot->setAutoReplot(true);
	ui->qwtPlot->replot();
	//ui->qwtPlot->setAutoReplot(true);
}

//double ti[10]={1,2,3,4,5,6,7,8,9,10};
//double val[10]={3,6,6,1,2,9,11,2,7,2};

void MYWindow::ondrawsignal(int length)
{

	double X[length],Y[length];
	ui->qwtPlot->detachItems();
	curve=new QwtPlotCurve("hh");
	curve->attach(ui->qwtPlot);
        for(int i=0;i<length;i++)
	{
		X[i]=i;
		Y[i]=*(m_buffer+i);
	}
	Y[1]=1450000000;
        if(first> 30) 
	{
		Y[1]= 8000000000;
	}
	//std::cout<<std::hex<<Y[0]<<" "<<Y[1]<<" "<<Y[2]<<" "<<Y[3]<<std::endl;
        /*
	if(first<1000)
	{
		for(int i=0;i<10;i++)
		{
			val[i] =5;
		}
	}
	if(first>=1000 && first <2000)
	{
		for(int i=0;i<5;i++)
		{
			val[i] =2;
		}
		for(int i=5;i<10;i++)
		{
			val[i]=i;
		}
	}
	if(first>=2000 )
	{
		for(int i=0;i<10;i++)
		{
			val[i] =40;
		}
	}
	*/
	curve->setSamples(X,Y,length);
	ui->qwtPlot->replot();

	first++;
}

void MYWindow::onlogsignal(/*AF_DATA_REQUEST(*/QVariant DataVar)
{
	loginfo log;
    log=DataVar.value<loginfo>();
    int level=log.level;
    QTreeWidgetItem* item = new QTreeWidgetItem(ui->treeWidget);
    switch (level)
    {
        case 0:
            item->setBackgroundColor(1,QColor(33,225,51));
            item->setText(1,QString("INFO"));
            break;
        case 1:
            item->setBackgroundColor(1,QColor(207, 219, 97));
            item->setText(1,QString("WARNNING"));
            break;
        case 2:
            item->setBackgroundColor(1,QColor(180, 30, 40));
            item->setText(1,QString("ERROR"));
            break;
        case 3:
            item->setBackgroundColor(1,QColor(231, 30, 40));
            item->setText(1,"FATAL");
            break;
    }
    /*
    QString curTime = QDateTime::currentDateTime().toString("hh:mm:ss dd/MM/yyyy") ;
    __item->setText(2, QString(stat)) ;
    __item->setText(0, QDateTime::currentDateTime().toString("hh:mm:ss dd/MM/yyyy")) ;
    list_mrs->addTopLevelItem (__item) ;
    std::cout << curTime.toStdString() << " " << stat.toStdString() << std::endl ;
     */
    //item->setText(0,QString(log.logtime));
    item->setText(2,QString(log.log));
    ui->treeWidget->addTopLevelItem (item) ;
}



