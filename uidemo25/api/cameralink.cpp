#include "cameralink.h"
#include <QDebug>

CMagDevice m_magDevLeft;
CMagDevice m_magDevRight;

void CALLBACK NewFrameLeft(UINT intChannelIndex, int intCameraTemperature,
                       DWORD dwFFCCounterdown, DWORD dwCamState,
                       DWORD dwStreamType, void * pUser) {
    Q_UNUSED(intChannelIndex);
    Q_UNUSED(dwFFCCounterdown);
    Q_UNUSED(dwStreamType);
    VideoWidget *pVideoPanel = (VideoWidget *)pUser;
    pVideoPanel->newFrameEmit(intCameraTemperature, (void *)(&m_magDevLeft));
}
void CALLBACK NewFrameRight(UINT intChannelIndex, int intCameraTemperature,
                       DWORD dwFFCCounterdown, DWORD dwCamState,
                       DWORD dwStreamType, void * pUser) {
    Q_UNUSED(intChannelIndex);
    Q_UNUSED(dwFFCCounterdown);
    Q_UNUSED(dwStreamType);
    VideoWidget *pVideoPanel = (VideoWidget *)pUser;
    pVideoPanel->newFrameEmit(intCameraTemperature, (void *)(&m_magDevRight));
}

void CALLBACK CBSerical(UINT intChannelIndex, void* pData, UINT intDataLen, void* pUserData) {
    Q_UNUSED(intChannelIndex);
    Q_UNUSED(pUserData);
    unsigned char* cmd = (unsigned char*)pData;
    for (UINT i = 0; i < intDataLen; ++i) {
        printf("0x%02x ", cmd[i]);
    }
    //qDebug() << std::endl;
}
cameraLink *cameraLink::GetInstance()
{
    static cameraLink instance;
    return &instance;
}

cameraLink::cameraLink()
{
    QLOG_INFO()<<"cameraLink is init";
    m_magSer.EnableAutoReConnect(TRUE);
    linkCamera();
}

bool cameraLink::linkCamera()
{
    QLOG_INFO()<<"linkCamera";
    if (!m_magDevLeft.IsInitialized())
    {
        QLOG_INFO()<<"m_magDevLeft.IsInitialized() fail";
        return false;
    }
    //ROOM1 = Room1, 200.120.72.166, 6060,200.120.72.167, 3777, 200.120.72.168, xtkj66666,200.120.72.170
    if (!m_magDevLeft.LinkCamera("200.120.72.167"))
    {
        QLOG_INFO()<<"m_magDevLeft.LinkCamera() fail";
        return false;
    }
    if (!m_magDevRight.IsInitialized())
    {
        QLOG_INFO()<<"m_magDevRight.IsInitialized() fail";
        return false;
    }
    //ROOM1 = Room1, 200.120.72.166, 6060,200.120.72.167, 3777, 200.120.72.168, xtkj66666,200.120.72.170
    if (!m_magDevRight.LinkCamera("200.120.72.168"))
    {
        QLOG_INFO()<<"m_magDevRight.LinkCamera() fail";
        return false;
    }

    return true;
}
void cameraLink::refresh() {
    if (m_magSer.EnumCameras()) {
        Sleep(200);

        int n = m_magSer.GetTerminalCount();
        QLOG_INFO()<<"refreshCamera" << n;
        if (n > 0) {
            m_vTermList.resize(2);

            n = m_magSer.GetTerminalList(
                &m_vTermList[0], m_vTermList.size() * sizeof(struct_TerminalList));
        }
        else {
            QLOG_INFO() << "Enum cameras failed\n";
        }
    }
}

void cameraLink::link()
{
    QLOG_INFO()<<"linkCamera";
    if (m_vTermList.size() !=2 ) {
        return;
    }

    m_magDevLeft.DisLinkCamera();
    if (!m_magDevLeft.LinkCamera(m_vTermList[0].intTerminalIp)) {
        QLOG_INFO() << "left link to camrera " << m_vTermList[0].charTerminalName
                  << " failed\n";
        return;
    }

    if (!m_magDevLeft.SetSerialCallBack(CBSerical, nullptr)) {
        QLOG_INFO() << "left set serical callback failed\n";
    }

    m_magDevRight.DisLinkCamera();
    if (!m_magDevRight.LinkCamera(m_vTermList[1].intTerminalIp)) {
        QLOG_INFO() << "right link to camrera " << m_vTermList[1].charTerminalName
                  << " failed\n";
        return;
    }

    if (!m_magDevRight.SetSerialCallBack(CBSerical, nullptr)) {
        QLOG_INFO() << "right set serical callback failed\n";
    }
}

bool cameraLink::play(VideoWidget * w1, int c)
{
    QLOG_INFO()<<"playCamera";
//    if (m_vTermList.size() != 2)
//    {
//        return false;
//    }
    if(c == 0)
    {
        const struct_CamInfo *camInfo = m_magDevLeft.GetCamInfo();
        OutputPara paraOut = {camInfo->intFPAWidth,   camInfo->intFPAHeight,
                              camInfo->intVideoWidth, camInfo->intVideoHeight,
                              16,                     camInfo->intVideoHeight};
        DWORD streamType = STREAM_TEMPERATURE;
         //DWORD streamType =  STREAM_VIDEO;
        if (!m_magDevLeft.StartProcessImage(&paraOut, NewFrameLeft, streamType,
                                   (void *)(w1)))
        {
            QLOG_INFO() << "left play failed\n";
            return false;
        }
        m_magDevLeft.SetColorPalette(IronBow);
        return true;
    }
    else if(c == 1)
    {
        const struct_CamInfo *camInfo = m_magDevRight.GetCamInfo();
        OutputPara paraOut = {camInfo->intFPAWidth,   camInfo->intFPAHeight,
                              camInfo->intVideoWidth, camInfo->intVideoHeight,
                              16,                     camInfo->intVideoHeight};
        DWORD streamType = STREAM_TEMPERATURE;
         //DWORD streamType =  STREAM_VIDEO;
        if (!m_magDevRight.StartProcessImage(&paraOut, NewFrameRight, streamType,
                                   (void *)(w1)))
        {
            QLOG_INFO() << "right play failed\n";
            return false;
        }
        m_magDevRight.SetColorPalette(IronBow);
        return true;
    }
}

void cameraLink::stop()
{
    if (m_vTermList.size() !=2 ) {
        return;
    }
    m_magDevLeft.StopProcessImage();
    m_magDevRight.StopProcessImage();
}


