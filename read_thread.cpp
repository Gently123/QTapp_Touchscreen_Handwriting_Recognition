#include "thread.h"
#include <QThread>
#include "hidapi.h"
#include <windows.h>
#include <QDebug>
bool ChangeToXYB(unsigned char *buf,int buflen,unsigned short *x,unsigned short *y, unsigned char*b,unsigned char *b1);
void WorkerThread::doWork() {
    int res;
            unsigned char buf[256];
            #define MAX_STR 255
            wchar_t wstr[MAX_STR];
            hid_device *handle;
            int i;
            struct hid_device_info *devs, *cur_dev;
            if (hid_init())
                     qDebug("hid_init() -1");
            else
                     qDebug("hid_init()1");
 //           devs = hid_enumerate(0x258a, 0x1016);
            devs = hid_enumerate(0x08f2, 0x6370);
            cur_dev = devs;
            hid_free_enumeration(devs);
            qDebug("hid_init() 2");
            // Set up the command buffer.
            memset(buf,0x00,sizeof(buf));
            buf[0] = 0x01;
            buf[1] = 0x81;
            // Open the device using the VID, PID,
            // and optionally the Serial number.
            ////handle = hid_open(0x4d8, 0x3f, L"12345");
//            handle = hid_open(0x258a, 0x1016, 0xd,1,NULL);
            handle = hid_open(0x08f2, 0x6370, 0xd,1,NULL);
            if (!handle) {
                    qDebug("unable to open device\n");
                    return ;
            }
            // Set the hid_read() function to be non-blocking.
            hid_set_nonblocking(handle, 0);
            // Read requested state. hid_read() has been set to be
            // non-blocking by the call to hid_set_nonblocking() above.
            // This loop demonstrates the non-blocking nature of hid_read().
            res = 0;

    while(true)
    {
//        for(int n=0;n<10;n++){
//    QThread::msleep(500); // 模拟长时间运行任务
        res = hid_read(handle, buf, sizeof(buf));
        if (res == 0)
        {
                qDebug("thread waiting...\n");
            //Sleep(1);
        }
        if (res < 0)
        {
                qDebug("thread Unable to read()\n");
                Sleep(1);
                continue;
        }
//       Sleep(1);
 //       printf("Data read:\n   ");
        // Print out the returned buffer.
//        for (i = 0; i < res; i++)
//                qDebug("%02x ", buf[i]);
//        qDebug("\n");

        unsigned short    x=0,y=0;
        unsigned char    b=0,b1=0;
       ChangeToXYB(buf,8,&x,&y,&b,&b1);
//       qDebug("thread x:%d,y:%d b:%d", x,y,b);

  //      if (buf[0]&&buf[1])
        {
 //           qDebug("abs(lastX-x) %d ,x:%d", abs(lastX-x),x);
 //           qDebug("abs(lastY-y) %d ,y:%d", abs(lastY-y),y);
           if((abs(lastX-x)>1||abs(lastY-y)>1))
             {
                emit resultReady(x,y,b,b1);

           }
           else
           {
               if(x==0&&y==0&&b==0)
               {

                   emit resultReady(x,y,b,b1);
               }

           }
           lastX=x;
           lastY=y;
        }
  //      else
   //         Sleep(1);
        memset(buf,0,256);
        }
    hid_close(handle);

    /* Free static HIDAPI objects. */
    hid_exit();



    return ;
    }
bool ChangeToXYB(unsigned char *buf,int buflen,unsigned short *x,unsigned short *y,unsigned char *b,unsigned char *b1)
{
        unsigned short  NowX;
        unsigned short  NowY;
        unsigned short  ButtonStatus;
        unsigned char  C,C1;

        buf[1]=buf[1] ^ 0x1a;
        buf[2]=buf[2] ^ 0x28;
        buf[3]=buf[3] ^ 0x10;
        buf[4]=buf[4] ^ 0x30;

        //	if (buf[5]==0x2a)//为了兼容phtIC
        //	buf[5]=0x22;
        //	if (buf[5]==0x2c)
        //	buf[5]=0x24;
        //	if (buf[5]==0x29)
        //	buf[5]=0x21;
        //	if (buf[5]==0x28)
        //	buf[5]=0x20;

        buf[5]=buf[5] ^ 0x28;
        buf[6]=buf[6] ^ 0x1a;


        C=buf[1];
        C1=buf[2];

        NowX=C1<<8 | C;

        C=buf[3];
        C1=buf[4];
        NowY=C1<<8 | C;

        C=buf[5];
        ButtonStatus=C;

        *x=NowX;
        *y=NowY;
        *b=(unsigned char)ButtonStatus;
        *b1=buf[6];

    return true;
}

