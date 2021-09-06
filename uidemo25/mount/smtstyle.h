#ifndef SMTSTYLE_H
#define SMTSTYLE_H
#include<QObject>
#include <QDebug>
#include <QComboBox>

#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long

#define s8  char
#define s16 short
#define s32 int
#define s64 long long//

#define MATH_PI 3.141592653589793

//#define SMT_VERSION1_3 0X13 //第一代控制板 CH340串口板
//#define SMT_VERSION2_1 0X21 //第二代控制板 232串口板
//#define SMT_VERSION2_2 0X22 //第二代控制板 网口板
//#define SMT_VERSION2_3 0X23 //第二代控制板 网口板
//#define SMT_VERSION2_5 0X25 //第二代控制板 网口板


//S曲线
#define SPEED_S_NUM   4     //曲线数

#define SPEED_NUM   4       //表格中列项数
#define SPEED_KHZ_XY    0   //XY最高频率
#define SPEED_TIME_XY   1   //XY加速时间
#define SPEED_KHZ_ZA    2   //ZA最高频率
#define SPEED_TIME_ZA   3   //ZA加速时间


#define A_BTN_UP    0 //方向按钮
#define A_BTN_DOWN  1
#define A_BTN_LEFT  2
#define A_BTN_RIGHT 3
#define A_BTN_ZERO  4


#define SMT_COLOR_SEND  0//发送时字体提示颜色
#define SMT_COLOR_READ  1//接收时字体提示颜色
#define SMT_LANG_CHINESE  0//中文
#define SMT_LANG_ENGLISH  1//英文

#define SMT_RUN_LEDSTOP  0	//运行灯 绿灯
#define SMT_RUN_LEDRUN  1	//运行灯 红灯

#define F_POINT 3//小数点显示位数

#define SMT_TIME_SEND 500    //ms定时发送参数
#define SMT_TIME 500    //ms定时器中断时间

#define SMT_RUN_CLOSE   0//停止运行
#define SMT_RUN_STOP  1//飞达测量，
#define SMT_RUN_AUTO    2//自动运行，
#define SMT_RUN_RETURN  3//返回原点
#define SMT_RUN_INIT    4//开机初始化状态，初始化成功则设置为CLOSE状态
#define SMT_RUN_ACCD    5//自动校正ACCD坐标
#define SMT_RUN_ALIB    6//自动换吸嘴
#define SMT_RUN_IAP     7//更新程序中

#define SMT_F0      500    //初始频率
#define SMT_FMAX    200000  //最高频率
#define SMT_FMIN    2000    //最高频率

#define SMT_X1MM    0
#define SMT_A1MM    1
#define SMT_Z1MM    2
#define SMT_P1MM    3

//LEN表示数据长度 需要加密 必须是加密位数的倍数
#define COM_LEN_LOAD 		1024//数据长度
#define COM_LEN_RUN 		26//数据长度
#define COM_LEN_SCURVE 		33//设置S曲线 32个数据 最后一个是保存endmsg参数
#define COM_LEN_SET  		50//设置上电参数
#define COM_LEN_IAP_DAT		2054//发送继续执行命令 包括
#define COM_LEN_IAP_DAT_NET	1030//发送继续执行命令 包括

#define COM_MSG_NOW             1	//实时位置
#define COM_MSG_GET             2	//校正
#define COM_MSG_RUN             3	//贴装
#define COM_MSG_HAND            4	//手动 全部的G
#define COM_MSG_STOP            5	//停止
#define COM_MSG_RETURN          6	//返回
#define COM_MSG_START           7   //START
#define COM_MSG_GO0             8	//自动复位
#define COM_MSG_RZ              9	//Z轴复位
#define COM_MSG_SET             10	//控制板设置
#define COM_MSG_TESTFLY         11	//测试飞行相机
#define COM_MSG_KEY             12	//按钮
#define COM_MSG_SCURVE          13	//S曲线设置
#define COM_MSG_POWEROUT        14	//电量低
#define COM_MSG_ACCD            15	//测量吸嘴和CCD的坐标
#define COM_MSG_PL              16	//抛料
#define COM_MSG_LOAD            17	//飞达矫正
#define COM_MSG_XY              18	//单独运行XY
#define COM_MSG_Z               19	//单独运行Z
#define COM_MSG_A               20	//单独运行A
#define COM_MSG_ZK              21	//单独真空电磁阀
#define COM_MSG_LED             22	//单独选择CCD
#define COM_MSG_C               23	//清除XY坐标
#define COM_MSG_RESET           24	//设备复位
#define COM_MSG_EN              25	//清除电机的报错
#define COM_MSG_F               26	//手动控制飞达
#define COM_MSG_O               27	//测量原点
#define COM_MSG_MXY             28	//测量XY范围---是用来测量原点，作为END_MSG
//以下为2017-8-14改版添加 控制板没有对应 执行代码
#define COM_MSG_CCDLIGHT        29	//灯光亮度
#define COM_MSG_RUN_LED         30	//运行指示
#define COM_MSG_RUN_BEEP        31	//报警
#define COM_MSG_ZD_PCB          32	//加紧PCB
#define COM_MSG_CQ              33	//吹气
#define COM_MSG_XZ              34  //读取24状态
#define COM_MSG_ZD_DB           35	//PCB挡板机构
#define COM_MSG_ALIB            36	//自动换吸嘴
#define COM_MSG_FLIGHT          37	//飞行对中
#define COM_MSG_FLIGHT_IMG      38	//读取照片
#define COM_MSG_ZL              39	//轮询Z轴
#define COM_MSG_OPEN_DOOR       40	//开门暂停
#define COM_MSG_RUN_TIME        243	//设置新的运行到期时间
#define COM_MSG_IO              244	//查看各个IO口的状态
//#define COM_MSG_OK              245	//收到回复
#define COM_MSG_ANAINZA			246	//重复发送一次数据 帖头用
#define COM_MSG_TIME            247	//测试网速
#define COM_MSG_ANAIN           248	//重复发送一次数据
#define COM_MSG_SETIP           249	//修改IP和网关
#define COM_MSG_IAP				250	//iap测试通信
#define COM_MSG_IAP_DAT			251	//iap数据
#define COM_MSG_TEA             252 //TEA
#define COM_MSG_TEST            253 //联机测试
#define COM_MSG_CLOSE           254 //不动作

//运行灯和报警
#define LED_S_RUN 1u	//S1运行
#define LED_S_BEEP 2u	//S3报警

//手动动作前先判断Z状态
#define XZ_ENDMSG_XY            0   //手动移动XY
#define XZ_ENDMSG_GET           1   //获取标定点
#define XZ_ENDMSG_ZD            2   //自动运行贴装时  首先判断Z位置
#define XZ_ENDMSG_BUTIE         3   //补贴状态下首先判断Z位置 从选中行运行也先判断Z位置
#define XZ_ENDMSG_O             4   //测量原点
#define XZ_ENDMSG_MXY           5   //测量限位
#define XZ_ENDMSG_STOP          6   //双击图像移动时
#define XZ_ENDMSG_PAOLIAO       7   //抛料
#define XZ_ENDMSG_PCBO          8   //运行到PCB原点
#define XZ_ENDMSG_MARKPIX1      9   //Mark测量偏心识别量 3不
#define XZ_ENDMSG_MARKPIX2      10
#define XZ_ENDMSG_MARKPIX3      11
#define XZ_ENDMSG_MARK1         12  //Mark1查找
#define XZ_ENDMSG_MARK2         13  //Mark2查找
#define XZ_ENDMSG_MM            14  //手动Mark测量
#define XZ_ENDMSG_A_AUTO        15  //A校正
#define XZ_ENDMSG_GO_A          16  //A_ccd距离
#define XZ_ENDMSG_GO_CCD1       17  //修改A_CCD1距离
#define XZ_ENDMSG_GO_ACCD1      18  //修改A_CCD1距离
#define XZ_ENDMSG_GO_ACCD2      19  //A_CCD2坐标
#define XZ_ENDMSG_GO_ACCD3      20  //A_CCD3坐标
#define XZ_ENDMSG_GO_ZL         21  //轮询Z轴
//#define XZ_ENDMSG_CHECK_CCD2    22  //校正CCD2坐标
//#define XZ_ENDMSG_CHECK_CCD3    23  //校正CCD3坐标
#define XZ_ENDMSG_GO_ALIB       24  //运行到吸嘴库坐标
#define XZ_ENDMSG_ALIB_AUTO     25  //自动更换吸嘴
#define XZ_ENDMSG_RETURN_STOP   26   //自动贴片后 固定停止位置
#define XZ_ENDMSG_GO_FEEDER     27   //运行到飞达
#define XZ_ENDMSG_CHECK_FEEDER  28   //校正飞达元件
#define XZ_ENDMSG_GO_FEEDER_A   29   //吸嘴对准飞达
#define XZ_ENDMSG_GO_CMD        30   //运行到坐标文件
#define XZ_ENDMSG_CMD_UP_DOWN   31   //上一行或者下一行
#define XZ_ENDMSG_A_FLIGHT      32   //飞行对中调试

#define COM_STOP_DATA0_XW    0//限位保护
#define COM_STOP_DATA0_JT    1//急停保护
#define COM_STOP_DATA0_ZT    2//暂停
#define COM_STOP_DATA1_X    1//X轴限位保护
#define COM_STOP_DATA1_Y    2//Y轴限位保护
//DATA[2-4]:X轴坐标，最高位=1为负数
//DATA[5-7]:Y轴坐标，最高位=1为负数
/********************************************/

/*******************************/
//格式：FE,16,COM_MSG_STEERING,DATA,CRH,CRL

#define COM_MIN  6
#define COM_MAX  256
#define COM_STX      0xFE
#define COM_ADD_STX  0x00
#define COM_ADD_LEN  0x01
#define COM_ADD_MSG  0x02
#define COM_ADD_RAN  0x03
#define COM_ADD_DAT  0x04
#define COM_ADD_MSG_END  COM_ADD_DAT+COM_LEN_RUN-1 //返回装台标志 DATA+19
#define COM_ADD_TEST_VERSION    10//测试联机 版本号在数据区的位置 从ADD_DATA开始

#endif // WANGSTYLE_H
