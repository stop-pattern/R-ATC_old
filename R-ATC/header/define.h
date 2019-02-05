#pragma once

#define DECELERATION_PATTERN 22
#define DECELERATION_BRAKE 19.5	//常用P計算用減速定数
#define DECELARATION_EMR 25	//非常P計算用減速定数
#define ATC_MAX 120.0	//ATC最高速度[km/h]
#define NOTICE_TIME 1.5	//P接近表示時間[s]
#define PATTERN_BRAKE 5	//ATC緩和パターン段数

#define STOPLIMIT_UPDATE 
/*
(StopLimit != false && rand() % STOPLIMIT_UPDATE == 0) { panel[]; }	//←とかやる？
*/

#define SPEED_ATS_OFF 15	//ATS非保安(構内)時照査速度

//信号インデックス
const int SpeedLimit[256] = {0,0,0,0,0,0,0,0,0,0,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,0,0,125,130,135,140,145,150,155,160,165,170,175,180,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,25,45,55,65,75,90,100,110,120,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


enum ATC_status
{
	OFF = 000,

	R__ATC = 1,
	ATC__6 = 2,
	ATC__10 = 3,

	R__ATC_on = 10,
	R__ATC_receive = 12,
	R__ATC_backup = 14,

	ATC__6_on = 20,
	ATC__6_emergency = 21,

	ATC__10_on = 30,
	ATC__10_ORP = 31,
};


enum ATC_Beacon {
	//ATC-10
	ATC10_notice_f = 7,	//前方予告(強制)
	ATC10_ORP_extension = 12,	//ORP添線始点
	ATC10_notice_i = 31,	//前方予告(リアルタイム)
	ATC10_notice_f_2 = 46,	//前方予告(強制)
	ATC10_ORP_extension_2 = 47,	//ORP添線始点
	ATC10_notice_i_2 = 48,	//前方予告(リアルタイム)

	//R-ATC
	Status = 150,	//ATC状態管理
	SpeedUp,	//ATC最高速度変化(即時)
	SpeedDown,	//ATC最高速度変化(P制御)
	PlatformStart_1,	//ホーム区始端
	PlatformStart_2,	//ホーム区始端
	PlatformEnd_1,	//ホーム区終端
	PlatformEnd_2,	//ホーム区終端
	LocationCorrection,	//R-ATC距離補正
	PreTrainDistance_1,	//先行列車位置更新
	PreTrainDistance_2,	//先行列車位置更新
	PreTrainDistance_3,	//先行列車位置更新
	PreTrainDistance_4,	//先行列車位置更新
	PreTrainDistance_5,	//先行列車位置更新
	PreTrainTime_1,	//先行列車時刻更新
	PreTrainTime_2,	//先行列車時刻更新
	PreTrainTime_3,	//先行列車時刻更新
	PreTrainTime_4,	//先行列車時刻更新
	PreTrainTime_5,	//先行列車時刻更新
	Set2StepPattern,	//2段パターン設定


//端子台未定義
	//指定インデックスを02に
	ATC02_set_distance = 256,	//距離設定から02
	ATC02_set_time,	//指定時刻から02
	ATC02_end_time,	//指定時間[ms]経過後解除
	ATC02_end_distance,	//指定距離[m]走行後

};


enum ATC_Panel {
	ATCpower = 52,	//ATC電源
	ATC = 53,	//ATC
	ATCoff = 54,	//切
	ATCrelease = 55,	//ATC解放
	ATCemdrive = 56,	//ATC非常運転
	ATCemr = 57,	//ATC非常
	ATCbrake = 58,	//ATC常用

	ORP_P = 59,	//ORP現示
	ORP_10k = 60,	//ORP速度10k
	ATC_R = 63,	//ATC停止
	ATC_G = 62,	//ATC進行
	fw_notice = 63,	//前方予告
	ORP_1k = 64,	//ORP速度1k

	Rolling = 65,	//転動防止
	Limit_5 = 66,	//速度矢印5k
	Limit_1 = 67,	//速度矢印1k
	pattern = 68,	//P接近

	ATC02 = 70,	//02
	ATC01 = 71,	//01&矢印隠し
	RATC = 72,	//R-ATC
	IDmalfunction = 73,	//車上ID故障
	reduction = 74,	//P低減
	pattern_2 = 75,	//2段P
	StopLimit_10000 = 76,	//停止限界10000-1000
	StopLimit_100 = 77,	//停止限界100 - 10
	StopLimit_1 = 78,	//停止限界1 - 0.1

	info_0 = 82,	//開通情報000-099
	info_1 = 83,	//開通情報100-199
	info_2 = 84,	//開通情報200-299
	info_3 = 85,	//開通情報300-399
	info_4 = 86,	//開通情報400-499
	info_5 = 87,	//開通情報500-599
	info_6 = 88,	//開通情報600-699
	info_7 = 89,	//開通情報700-799
	info_8 = 90,	//開通情報800-799
	info_9 = 91,	//開通情報900-999

	here = 94,	//自列車

//端子台未定義
};

/*panel
ATACS{
	右上
		ATACS故障
		後退検知
	右下
		1系故障
		2系故障
	左上
		非常運転
		P低減
		切
		ATACS
		車上ID故障
	左下
		ATACS解放
		電源
		停通防止動作
		ATACS非常
		ATACS常用
}
*/


enum ATC_Sound {
	ATC_bell = 5,	//ATC単打ベル
	ATC10_notice = 9,	//前方予告
	ATC10_ORP = 18,	//ORPピープ
	ATC10_bell = 19,	//車警ベル
	ATS_trance = 26,	//ATS切換完了
	ATC_trance = 28,	//ATC切換完了
	RATC_bell = 50,	//R-ATCベル
};


enum ATC_Signal {
	ATC_02 = 9,
	ATC_01,
	ATC_05,
	ATC_10,
	ATC_15,
	ATC_20,
	ATC_25,
	ATC_30,
	ATC_35,
	ATC_40,
	ATC_45,
	ATC_50,
	ATC_55,
	ATC_60,
	ATC_65,
	ATC_70,
	ATC_75,
	ATC_80,
	ATC_85,
	ATC_90,
	ATC_95,
	ATC_100,
	ATC_105,
	ATC_110,
	ATC_115,
	ATC_120,
	ATC_ORP = 35,

	R_ATC_0 = 50,
	R_ATC_1,
	R_ATC_2,
	R_ATC_3,
	R_ATC_4,
	R_ATC_5,
	R_ATC_2P_1,
	R_ATC_2P_2,
	R_ATC_2P_3,
	R_ATC_強制開通,
	R_ATC_絶対停止 = 60,

	ATC_6_02 = 101,
	ATC_6_01,
	ATC_6_15,
	ATC_6_25,
	ATC_6_45,
	ATC_6_55,
	ATC_6_65,
	ATC_6_75,
	ATC_6_90,
	ATC_6_100,
	ATC_6_110,
	ATC_6_120 = 112,
};


enum ATC_Switch {
	非常運転 = 5,
};