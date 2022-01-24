#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 用于播放
    timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::play);
}

MainWindow::~MainWindow()
{
    this->stop();
    this->freePlayer();
    delete ui;
}

void Delay(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}


void MainWindow::on_btnPlay_clicked()
{
    if(!isPlaying)
    {
        this->initPlayer();
        timer->start(frame_rate);
        isPlaying = true;
    }
}

void MainWindow::initPlayer()
{
    //初始化编解码库
    av_register_all();//创建AVFormatContext对象，与码流相关的结构。

    pFormatCtx = avformat_alloc_context();
    //初始化pFormatCtx结构
    if (avformat_open_input(&pFormatCtx, filepath, NULL, NULL) != 0){
        printf("Couldn't open input stream.\n");
        return ;
    }
    //获取音视频流数据信息
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0){
        printf("Couldn't find stream information.\n");
        return ;
    }
    videoindex = -1;
    //nb_streams视音频流的个数，这里当查找到视频流时就中断了。
    for (i = 0; i < pFormatCtx->nb_streams; i++)
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            videoindex = i;
            break;
        }
    if (videoindex == -1){
        printf("Didn't find a video stream.\n");
        return ;
    }
    //获取视频流编码结构
    pCodecCtx = pFormatCtx->streams[videoindex]->codec;
    //查找解码器
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (pCodec == NULL){
        printf("Codec not found.\n");
        return ;
    }
    //用于初始化pCodecCtx结构
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0){
        printf("Could not open codec.\n");
        return ;
    }
    //创建帧结构，此函数仅分配基本结构空间，图像数据空间需通过av_malloc分配
    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();
    //创建动态内存,创建存储图像数据的空间
    //av_image_get_buffer_size获取一帧图像需要的大小
    out_buffer = (unsigned char *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1));
    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, out_buffer,
                         AV_PIX_FMT_RGB32, pCodecCtx->width, pCodecCtx->height, 1);

    packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    //Output Info-----------------------------
    printf("--------------- File Information ----------------\n");
    //此函数打印输入或输出的详细信息
    av_dump_format(pFormatCtx, 0, filepath, 0);
    printf("-------------------------------------------------\n");
    //初始化img_convert_ctx结构
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                                     pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

    // 设置显示大小
    int w = pCodecCtx->width;
    int h = pCodecCtx->height;
    int wgw = this->ui->centralwidget->width();
    int wgh = this->ui->centralwidget->height();
    double ar = wgw / w ;
    qDebug() << "区域大小："<< w << "," << h;
    this->ui->label->setScaledContents(true);
    this->ui->label->setGeometry(0,0,wgw,wgh * ar); // 为了方便显示全，这里显示缩小到60%

    // 获取帧频
    //    AVStream *stream=pFormatCtx->streams[packet->stream_index];
    //    frame_rate=stream->avg_frame_rate.num/stream->avg_frame_rate.den;//每秒多少帧

    //    qDebug() << "num,den" << stream->avg_frame_rate.num << "," << stream->avg_frame_rate.den << "   帧频："<< frame_rate;

    qDebug()<<"init player success.";
}

void MainWindow::freePlayer()
{
    sws_freeContext(img_convert_ctx);
    av_frame_free(&pFrameRGB);
    av_frame_free(&pFrame);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
}

void MainWindow::play()
{
    //av_read_frame读取一帧未解码的数据
    int ret = av_read_frame(pFormatCtx, packet);
    if (ret >= 0){
        //如果是视频数据
        if (packet->stream_index == videoindex){
            //解码一帧视频数据
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if (ret < 0){
                printf("Decode Error.\n");
                return ;
            }
            if (got_picture){
                sws_scale(img_convert_ctx, (const unsigned char* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                          pFrameRGB->data, pFrameRGB->linesize);

                QImage img((uchar*)pFrameRGB->data[0],pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);

                // 缩放自适应
                img.scaled(ui->label->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);//放缩图片，以固定大小显示
                // 显示图片到控件
                ui->label->setPixmap(QPixmap::fromImage(img));
                //                Delay(40);
            }
        }
        av_free_packet(packet);
    }
}

void MainWindow::stop()
{
    if(isPlaying){
        timer->stop();
    }
    isPlaying = false;
}
